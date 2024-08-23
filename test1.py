###############################################################################
# (c) 2020 Xantrex Canada ULC. All rights reserved.
# All trademarks are owned or licensed by Xantrex Canada ULC,
# its subsidiaries or affiliated companies.
#
# FILE NAME:  Rvc2Mqtt.py
#
# PURPOSE:
#     This is program to receive RV-C messages received on the CAN bus
#     and publish them to an MQTT broker for other MQTT clients to receive
#     and handle
#
###############################################################################

###############################################################################
#                               Includes
###############################################################################

import AsyncRvc
import _fastrvc as _rvc
import json
import asyncio
import signal
import time
import gmqtt
import re
import random
import traceback
import sys
import os
import logging
import logging.config
import yaml
from pathlib import Path
from fnmatch import fnmatch
import DiagMsgHandler
from XnetParams import MODEL_MAP # PARAM_MAP, CMD_MAP
from BmsFlags2Alerts import BMS_FLAG_LOOKUP
from argparse import ArgumentParser

###############################################################################
#                             Constant Data
###############################################################################

# Prefix for all topics used by the Xantrex network system
TOPIC_PREFIX = 'xnet'

# Default source address for self-identification as a control panel
# Other options we could have chosen include:
# - RVC_eDFLT_SRC_ADDR_SERVICE_TOOL
# - RVC_eDFLT_SRC_ADDR_NETWORK_BRIDGE
MY_DFLT_SRC_ADDR = _rvc.RVC_eDFLT_SRC_ADDR_CONTROL_PANEL

# Mapping from device function in addrclaim to function in topics
FUNC_MAP = {'Inverter Charger' : 'invchg',
            'Charger' : 'scc',
            'Inverter' : 'inv',
            'Battery Monitor' : 'battmon',
            'Battery Management System' : 'bms',
            'Service Tool' : 'service'}


SUBSCRIPTION_MATCHER = re.compile(r'^([^/]+)/(.*)$')

# PGN mnemonic gets munged for PmAssocSts to include the association type and instance
ASSOC_MATCHER = re.compile(r"^PmAssocSts(\w+)(\d)$")

# Using subscriptions of form <func><inst>/<param>,
# we want to map to <devcon>/<pgn>/<qualifier>/<signal>
# Create an interest tree on each device connection?

CHGEN_TO_CHGCMDSTS = {'On' : 'Enable Charger',
                      'Off': 'Disable'}

# For DefaultBatteryTemperature, we want to present options
# Cold (10 deg C), Warm (25 deg C) and Hot (40 deg C)
# where the RV-C parameter just takes values in deg C.
DEFBATTTEMP_TO_CMD = {'Cold (10 deg C)' : '10 deg C',
                      'Warm (25 deg C)' : '25 deg C',
                      'Hot (40 deg C)' : '40 deg C'}

STS_TO_DEFBATTTEMP = {'10 deg C' : 'Cold (10 deg C)',
                      '25 deg C' : 'Warm (25 deg C)',
                      '40 deg C' : 'Hot (40 deg C)'}

# For InvFaultRecovery, we want to present options Auto/Manual
# instead of the On/Off used in RV-C
STS_TO_INVFAULTRECOVERY = {'On' : 'Auto',
                           'Off' : 'Manual'}

INVFAULTRECOVERY_TO_CMD = {'Auto' : 'On',
                           'Manual' : 'Off'}

# Polling interval for messages that are not periodically broadcast.
# We want this polling period to be longer than any of the
# broadcast periods so that we don't request broadcast messages unnecessarily.
REREQ_TIMEOUT = 5.0

# A lock to prevent requests for product identification from happening all
# at once.  This uses up lots of bandwidth so responses may be lost if
# the stack is handling multi-frame messages from a dozen devices at a time.
IDENT_LOCK = asyncio.Lock()

# Regular expression for getting a setting, which could be a floating point number
# with units
SETTING_MATCHER = re.compile(r"^\s*(-?\d+\.?\d*)")

# Models that use a BattSts message for two-part instance instead of the
# usual one-part instance from InstSts
ZRPM_BMS_MODELS = ['0884-0310-12', '0884-0410-12', '0884-0205-24', '0884-0100-51',
                   '0884-0310-12-01', '0884-0410-12-01']

# Models of SRNE MPPT Solar Charge Controllers
SRNE_MPPT_MODELS = ['MC4860N15']

# Timeout for holding on to being the spokes-device for a given battery bank
DC_PRIORITY_TIMEOUT = 6 # 6 seconds

# Check ZeroRPM proprietary parameters every five seconds
PPN_CYCLE_SKIPS = 5

# Timeouts and retry counts for waiting for response from other CAN devices
RSP_DEFAULT_TIMEOUT = 1.0
PPN_CMD_TIMEOUT = 0.5
PPN_CMD_MAX_TRIES = 4
PPN_REQ_TIMEOUT = 0.5
PPN_REQ_MAX_TRIES = 3

# Where we can find the serial number in the file system
SERIALNUM_PATH = '/etc/xgw_serialnum'

# Path to the model number we report in our product identification
MODEL_PATH = '/etc/xgw_model'

###############################################################################
#                             Global Variables
###############################################################################

# An override of the transmit timers.  We would not normally transmit address
# claim, but we do it for the legacy devices out there.
# Eventually, we'll have to get smart about this and know when we can
# get away without not transmitting address claim.
TxTimers = [(_rvc.RVC_PGN_ISO_ADDR_CLAIM, _rvc.PGN_ADDR_GLOBAL, 100),
            (_rvc.RVC_PGN_DIAG_MSG1,      _rvc.PGN_ADDR_GLOBAL, 500)]

# Logging setup, overridden by config file logging.yaml
Logger = None

###############################################################################
#                           Function Definitions
###############################################################################

# Setup logging according to a yaml configuration file
def fnSetupLogging(default_level="INFO"):
    global Logger
    
    default_path = str(Path(__file__).resolve().parent / "logging.yaml")
    path_to_config = os.getenv("LOG_CFG", default_path)

    if os.path.exists(path_to_config):
        with open(path_to_config, "rt") as fp:
            try:
                config = yaml.safe_load(fp.read())
                logging.config.dictConfig(config)
            except Exception as e:
                print(e)
                print("Error in logging configuration. Using default configs")
                logging.basicConfig(level=default_level)
    else:
        logging.basicConfig(level=default_level)
        print("Failed to load configuration file. Using default configs")

    Logger = logging.getLogger('Rvc2Mqtt')

# Convert On/Off enum for ChgEn to enum used in ChgCmd.Sts
def fnChgEn2ChgCmdSts(param_value):
    return CHGEN_TO_CHGCMDSTS[param_value]

# Convert On received for ChgStartEqlz parameter to 'Start Equalization'
# for ChgCmd.Sts.  All other values recieved get translated to 'Data Not Available'
def fnStartEqlz2ChgCmdSts(param_value):
    if param_value == 'On':
        return 'Start Equalization'
    else:
        return 'Data Not Available'

def fnSts2DefBattTemp(msg):
    sts_value = msg.fnGetValue('DefBattTemp')
    return STS_TO_DEFBATTTEMP.get(sts_value, sts_value)

def fnDefBattTemp2Cmd(param_value):
    return DEFBATTTEMP_TO_CMD.get(param_value, '25 deg C')

# Convert SccSts.OpState so that the 'Do Not Charge'
# value in the RV-C database becomes 'Not Charging'
def fnSccSts2ChgOpState(msg):
    sts_value = msg.fnGetValue('OpState')
    if sts_value == 'Do Not Charge':
        sts_value = 'Not Charging'
    elif sts_value == 'Undefined Source Decides':
        sts_value = 'Disabled'

    return sts_value

# Use InvSts.Sts as a readback for the InvCmd.InvEn command for
# the FXC Pro's InverterEnable parameter.
# The inverter is disabled if InvSts.Sts is 'Disabled' and assumed
# to be enabled for any other value.
def fnInvSts2InvEnDis(msg):
    sts_value = msg.fnGetValue('Sts')
    if sts_value == 'Disabled':
        return 'Off'
    else:
        return 'On'

# The SccArraySts message publishes array voltage and current,
# but not power.  We'll give a power value by multiplying the two
def fnPvArrayPower(msg):
    voltage = msg.fnGetFloatValue('ArrayV')
    current = msg.fnGetFloatValue('ArrayI')
    power = voltage * current

    return f"{power:.0f} W"

# Convert long name of device function to a short name for topics
def fnXnetFunc(rvcfunc):
    if rvcfunc in FUNC_MAP:
        return FUNC_MAP[rvcfunc]

    # If we can't find an alternate name, at least remove the spaces
    return rvcfunc.replace(' ', '').lower()

# Check if a qualifier passes for a given message
def fnQualPasses(msg, qual):
    for qualtup in qual:
        sig, testval = qualtup
        val = str(msg.fnGetValue(sig))
        if testval != val:
            # Failed one of the clauses
            return False

    # No clauses indicated this wasn't our message
    return True

# What is the combined operating state of an inverter/charger?
def fnInvChgOpState(opmode, invstate, chgstate):
    opstate = 'unknown'
    if opmode in ['Safe', 'Hibernate', 'Power Save', 'Diagnostic', 'Remote Power Off']:
        # We'll call all of the above states as 'Safe' because there is no power flow
        opstate = 'Safe'
    elif chgstate in ['Bulk', 'Absorption', 'Float', 'Overcharge',
                      'Equalize', 'Constant Voltage Constant Current']:
        opstate = 'Charging'
    elif invstate == 'Invert':
        # We'll say 'Inverting' to be consistent with 'Charging'
        opstate = 'Inverting'
    else:
        # Possible other states are 'Disabled', 'AC Passthru', 'APS Only',
        # 'Load Sense' and 'Waiting To Invert'.
        # The values are from RV-C enum 9404 in PgDatabase.py
        opstate = invstate

    return opstate

# What is the simplified operating state of an inverter?
def fnInvOpState(invstate):
    if invstate == 'Invert':
        # We'll say 'Inverting' to be consistent with 'Charging'
        opstate = 'Inverting'
    else:
        # Possible other states are 'Disabled', 'AC Passthru', 'APS Only',
        # 'Load Sense' and 'Waiting To Invert'.
        # The values are from RV-C enum 9404 in PgDatabase.py
        opstate = invstate

    return opstate

def fnInvChgSts4OutACVolt2Enum(msg):
    sts_value = msg.fnGetValue("OutACVolt")
    
    # Default to just pass the value through
    param_value = sts_value

    match = SETTING_MATCHER.match(sts_value)
    if match:
        float_value = float(match.group(1))

        # Legal values are 108, 110 and 120
        if float_value < 109.0:
            param_value = "108 V"
        elif float_value > 115.0:
            param_value = "120 V"
        else:
            param_value = "110 V"

    return param_value

# Convert On/Off from PmFXCCCfgSts.OvrFltRecEnDis to Auto/Manual for xnet parameter
def fnSts2InvFaultRecovery(msg):
    sts_value = msg.fnGetValue('OvrFltRecEnDis')
    return STS_TO_INVFAULTRECOVERY.get(sts_value, sts_value)

# Convert Auto/Manual from InvFaultRecovery to On/Off on PmFXCCCfgCmd.OverFltRecEnDis
def fnInvFaultRecovery2Cmd(param_value):
    return INVFAULTRECOVERY_TO_CMD.get(param_value, 'Unknown')

# Convert '0 h' from PwrSaveTime to Off for xnet parameter
def fnSts2PwrSaveTime(msg):
    sts_value = msg.fnGetValue('PwrSvHr')
    if sts_value.startswith('0'):
        sts_value = 'Off'

    return sts_value

# Convert 'Off' from PwrSaveTime to '0 h'.  All other values get passed as-is
def fnPwrSaveTime2Cmd(param_value):
    if param_value == 'Off':
        param_value = '0 h'

    return param_value

# Convert battery type 'Custom Li-Ion For CC/CV' to just 'Custom'
def fnSts2BatteryType(msg):
    sts_value = msg.fnGetValue('BattType')
    if sts_value == 'Custom Li-Ion For CC/CV':
        sts_value = 'Custom'

    return sts_value

# Convert battery type 'Custom' back to RV-C database's 'Custom Li-Ion For CC/CV'
def fnBatteryType2Cmd(param_value):
    if param_value == 'Custom':
        param_value = 'Custom Li-Ion For CC/CV'

    return param_value

# Translate SccSts.OpState into On/Off for ChargerEnable control
def fnSts2ChargerEnable(msg):
    sts_value = msg.fnGetValue('OpState')
    if sts_value == 'Undefined Source Decides':
        endis = 'Off'
    else:
        endis = 'On'

    return endis

# Translate On/Off from ChargerEnable control into SccCmd.Sts
def fnChargerEnable2Cmd(param_value):
    if param_value == 'On':
        cmd_value = 'Enable Charger'
    else:
        cmd_value = 'Disable'

    return cmd_value

# Translate SccCfgSts4.TempCompConst to a negative integer in mV/degC
def fnSts2TempComp(msg):
    int_value = -1 * msg.fnGetRawValue('TempCompConst')
    sts_value = f"{int_value} mV/degC"

    return sts_value

# Translate negative TempCompConst setting to a positive integer
def fnTempComp2Cmd(param_value):
    cmd_value = param_value.replace('-', '')

    return cmd_value


###############################################################################
#                           Class Definitions
###############################################################################

# The base device manager that can be either a device with active communication,
# or a virtual device
class clBaseDeviceManager(object):
    def __init__(self, parent):
        self.parent = parent
        self.client = parent.client
        self.xb = self.parent.xb

        # Fill out what we know (just about nothing)
        self.func = 'unk'
        self.inst = None
        self.inst2 = None
        self.name_fmt = "{func}{inst}"
        self.name = self.func + '?'
        self.subscriptions = {}
        self.ppn_subscriptions = {}
        self.bpc_serial_num = ""

        # Default handling of messages
        self.param_map = {}
        self.cmd_map = {}
        self.fault_lookup = []
        self.warning_lookup = []

        # No alerts detected yet
        self.alerts = {'faults' : {}, 'warnings' : {}}

    # Get a dictionary of what pgn/signals are of interest from a device
    def fnUpdateSubscriptions(self, active_subscriptions):
        self.subscriptions = {}
        self.ppn_subscriptions = {}
        for topic in active_subscriptions:
            match = SUBSCRIPTION_MATCHER.match(topic)
            if match:
                topic_dev = match.group(1)
                topic_param = match.group(2)
                if fnmatch(self.name, topic_dev):
                    if topic_param in self.param_map:
                        # Match the topic to how it maps from a field of an RV-C message
                        pgn, sig, qual, translator = self.param_map[topic_param]

                        # We can't subscribe to parameters from ProdIdent, because that
                        # is a multi-frame message and we don't want to poll that regularly
                        if pgn == 'ProdIdent':
                            Logger.warning(f"Ignoring subscription to {topic_dev}/{topic_param}")
                            continue

                        # Convert the name of a translator function into the function
                        if translator is None:
                            fnTranslate = None
                        else:
                            fnTranslate = eval(translator)

                        if pgn == 'PmPpnReadRsp':
                            # ZeroRPM proprietary parameter
                            self.ppn_subscriptions[sig] = (fnTranslate, topic_param)

                        else:
                            # Regular RV-C message
                            subtup = (sig, qual, fnTranslate, topic_param)
                            if pgn not in self.subscriptions:
                                self.subscriptions[pgn] = []

                            # Make sure there are no dupes, because there may
                            # be duplicates in the master active_subscriptions list
                            if subtup not in self.subscriptions[pgn]:
                                self.subscriptions[pgn].append(subtup)

# The object that oversees a single device and keeps track of its communications
class clDeviceManager(clBaseDeviceManager):
    def __init__(self, parent, addr):
        clBaseDeviceManager.__init__(self, parent)
        self.addr = addr

        # Information about the last command, just in case it gets NAK'd
        self.cmd_pgn = None
        self.cmd_param = None
        self.cmd_value = None

        # Event and mnemonic for communicating between the message receiving task
        # and the request polling task
        self.rvc_rsp_mnem = None
        self.rvc_rsp_evt = asyncio.Event()

        # Create a connection to this device
        self.devcon = self.xb.fnGetSimpleConnection(addr)

        # Create a queue for the message polling task to write any
        # ZeroRPM proprietary parameters
        self.ppn_write_queue = asyncio.Queue()

        # Start the message task, which will in turn start the polling task
        self.msgtask = asyncio.ensure_future(self.fnMsgTask(parent.active_subscriptions))

    # Shut down communication with the device
    async def fnClose(self):
        if hasattr(self, 'polltask'):
            self.polltask.cancel()
            try:
                await self.polltask
            except asyncio.CancelledError:
                Logger.debug("Poll task for node %d is cancelled" % self.addr)

        self.msgtask.cancel()
        try:
            await self.msgtask
        except asyncio.CancelledError:
            Logger.debug("Message task for node %d is cancelled" % self.addr)

        self.devcon.fnClose()

    # Request and receive a response to a request.  Retry if necessary
    async def fnFirmRequestByMnem(self, pgn_mnem, timeout=10.0, retries=4):
        for retries in range(retries):
            self.devcon.fnSendRequestByMnem(pgn_mnem)
            start = time.perf_counter()

            # It can take a while for the expected message to come in,
            # especially for multi-frame messages while there is lots of other
            # traffic on the CAN bus
            while (time.perf_counter() - start) < timeout:
                try:
                    msg = await asyncio.wait_for(self.devcon.fnGetMsg(), timeout=2.0)
                    if isinstance(msg, AsyncRvc.clMessage):
                        if msg.fnGetMnem() == pgn_mnem:
                            return msg

                        # If this isn't the message we're waiting for then
                        # maybe the next message is

                except asyncio.TimeoutError:
                    # No reply received to our request.  Try again
                    pass

            Logger.warning("Node %d retry %d for %s" % (self.addr, retries, pgn_mnem))

        # If we get this far then we've run out of retries
        # and a reply ain't coming
        return None

    # A task monitoring messages from a device connection
    async def fnMsgTask(self, active_subscriptions={}):
        # Get the product identification first
        is_identified = False
        async with IDENT_LOCK:
            try:
                reply = await self.fnFirmRequestByMnem('ProdIdent')
                if isinstance(reply, AsyncRvc.clMessage):
                    if reply.fnGetMnem() == 'ProdIdent':
                        self.fnHandleProdIdent(reply)

                    else:
                        raise Exception(f"Could not get ProdIdent from node {self.addr}, got {reply.fnGetMnem()} instead")

                else:
                    raise Exception(f"Could not get ProdIdent from node {self.addr} (no reply)")

            except Exception as exc:
                traceback.print_exc()
                return

        # Now get the device instance.  Most use InstSts, but the ZeroRPM has two instances, as seen in any BattSts msg
        # So, are we dealing with a ZeroRPM BMS or something normal?
        if self.model in ZRPM_BMS_MODELS:
            self.name_fmt = "bank{inst}batt{inst2}"
            inst_msg = 'BattSts6'
            handler = self.fnHandleBattSts

        elif self.model in SRNE_MPPT_MODELS:
            # The MPPTs don't support InstSts, so we get inst from SccSts
            inst_msg = 'SccSts'
            handler = self.fnHandleSccSts
            
        else:
            inst_msg = 'InstSts'
            handler = self.fnHandleInstSts


        reply = await self.fnFirmRequestByMnem(inst_msg)
        if isinstance(reply, AsyncRvc.clMessage):
            if reply.fnGetMnem() == inst_msg:
                handler(reply)
            else:
                Logger.warning(f"Could not get {inst_msg} from node {self.addr}, got {reply.fnGetMnem()} instead")
                self.devcon.fnClose()
                return

        else:
            Logger.warning(f"Could not get {inst_msg} from node {self.addr} (no reply)")
            self.devcon.fnClose()
            return

        # Now that we know the device function/instance,
        # what information is subscribed to?
        self.fnUpdateSubscriptions(active_subscriptions)

        self.msgtimes = {}
        self.devstate = {}
        self.polltask = asyncio.ensure_future(self.fnPollTask())

        # Now that we know who we're dealing with, we'll go into a message receiving loop
        while True:
            try:
                await self.fnMsgPoll()

            except asyncio.CancelledError:
                raise

            except:
                traceback.print_exc()

    # Receive and process a single message from a device connection
    async def fnMsgPoll(self):
        msg = await self.devcon.fnGetMsg()
        if isinstance(msg, AsyncRvc.clMessage):
            pgn = msg.fnGetMnem()

            # Time stamp the message so that we can see what messages have to be requested
            self.msgtimes[pgn] = time.perf_counter()

            # Now what do we do with this newly received message?
            if pgn == 'ProdIdent':
                self.fnHandleProdIdent(msg)
            elif pgn == 'InstSts':
                self.fnHandleInstSts(msg)
            elif pgn == 'SccSts':
                # For MPPT charge controllers, use SccSts to determine instance
                self.fnHandleSccSts(msg)
            elif pgn == 'BattSts6':
                # Use BattSts message to determine ZeroRPM BMS's bank and battery instance
                self.fnHandleBattSts(msg)

                # Also use BattSts6 to handle standard alerts related to the BMS
                self.fnHandleBmsFlags(msg)

            elif pgn in ['DCSrcSts1', 'DCSrcSts2', 'DCSrcSts3', 'DCSrcSts4', 'DcSrcSts11']:
                self.fnHandleDCSrcSts(msg)

            elif pgn == 'PmAssocSts':
                # We need to add the association type and instance to the mnemonic
                # for subscription lookup
                assoc_type = msg.fnGetValue('AssocType')
                assoc_inst = msg.fnGetValue('AssocInst')
                pgn = f"{pgn}{assoc_type}{assoc_inst}"

            elif pgn in ['BattSts6', 'DcSrcSts6', 'PmLithionicsSts']:
                # The Lithionics BMS uses its own status message and the standard
                # DcSrcSts6 message to report faults and warnings.
                # The ZeroRPM BMS uses BattSts6 for the same thing
                self.fnHandleBmsFlags(msg)

            elif pgn == 'PmPpnReadRsp':
                # This is the ZeroRPM battery responding to a PmPpnReadCmd message
                # for a subscribed proprietary parameter
                self.fnHandlePmPpnReadRsp(msg)

            elif pgn == 'PmPpnNakRsp':
                # The ZeroRPM battery didn't like a PPN command message
                Logger.info("PmPpnNakRsp received")

            elif pgn == 'IsoAck':
                # This IsoAck is a response to a command we sent earlier.  If it is a NAK
                # then we will want to publish the failure
                acknak = msg.fnGetValue('CtrlByte')
                if acknak != 'Ack':
                    self.fnHandleNak(msg)

            # Is this PGN of interest to us?
            if pgn in self.subscriptions:
                for sig, qual, fnTranslate, param in self.subscriptions[pgn]:
                    if fnQualPasses(msg, qual):
                        if pgn == 'DiagMsg1':
                            # Special handling for this one because we keep and return a
                            # list of active faults and warnings
                            DiagMsgHandler.fnUpdateAlerts(self, msg)

                        else:
                            # Publish the signal value
                            if callable(fnTranslate):
                                value = fnTranslate(msg)
                            else:
                                value = msg.fnGetValue(sig)
                            self.client.publish(f"{TOPIC_PREFIX}/sts/{self.name}/{param}", str(value))

                            # Save the state information if it is relevant to creating an overall state
                            if param in ['OpMode', 'InvOpState', 'ChgOpState']:
                                self.devstate[param] = str(value)

            # Are we waiting for this message in the polling task?
            if not self.rvc_rsp_evt.is_set():
                if pgn in [self.rvc_rsp_mnem, 'IsoAck', 'PmPpnNakRsp']:
                    self.rvc_rsp_evt.set()

    # Update a device connection with InstSts PGN and send appropriate topics
    def fnHandleInstSts(self, msg):
        # Are we changing instance here?
        newinst = msg.fnGetRawValue('BaseInst')
        if newinst != self.inst:
            # Handle this with knowledge of all other device managers
            # so that we can deal with duplicate instances
            self.parent.fnHandleInstChange(self, newinst)

    # Update a device connection with a BattSts<n>, which has a bank instance and battery instance
    def fnHandleBattSts(self, msg):
        bankinst = msg.fnGetRawValue('DcInst')
        battinst = msg.fnGetRawValue('BattInst')
        if (bankinst != self.inst) or (battinst != self.inst2):
            # Handle this with knowledge of all other device managers
            # so that we can deal with duplicate instances
            self.parent.fnHandleInstChange(self, bankinst, newinst2=battinst)

    # Update a device connection with SccSts PGN
    def fnHandleSccSts(self, msg):
        # Are we changing instance here?
        newinst = msg.fnGetRawValue('Inst')
        if newinst != self.inst:
            # Handle this with knowledge of all other device managers
            # so that we can deal with duplicate instances
            self.parent.fnHandleInstChange(self, newinst)

    # Handle any DCSrcSts message, publishing as a message from the bank<n> device
    def fnHandleDCSrcSts(self, msg):
        self.parent.fnHandleDCSrcSts(msg)

    # This is the ZeroRPM battery responding to a PmPpnReadCmd message
    # for a subscribed proprietary parameter
    def fnHandlePmPpnReadRsp(self, msg):
        param_mnem = msg.fnGetValue("ParamId")
        value = msg.fnGetValue("Value")

        if param_mnem in self.ppn_subscriptions:
            fnTranslate, mqtt_param = self.ppn_subscriptions[param_mnem]
            if callable(fnTranslate):
                value = fnTranslate(msg)

            self.client.publish(f"{TOPIC_PREFIX}/sts/{self.name}/{mqtt_param}", str(value))

        # Is this for the BPC serial number, which comes in two parts?
        elif param_mnem == 'BpcSerialNum1':
            self.bpc_serial_num = str(value)

        elif param_mnem == 'BpcSerialNum2':
            self.bpc_serial_num += str(value)
            self.client.publish(f"{TOPIC_PREFIX}/sts/{self.name}/BpcSerialNum",
                                self.bpc_serial_num)

    # Change a device name and update online/offline status
    def fnChangeName(self, newinst, newinst2=None, dupe=False):
        # Set the new name
        newname = self.name_fmt.format(func=self.func,
                                       inst=newinst,
                                       inst2=newinst2)
        if dupe:
            newname += f"_node{self.addr}"

        # Nothing to do if the newly calculated name is the same as the old name
        if newname == self.name:
            return

        # This device under the old name is now offline
        if self.inst is not None and self.func != 'unk':
            self.client.publish(f"{TOPIC_PREFIX}/sts/{self.name}/status", 'offline', retain=True)

        # Change to the new name and publish its online status
        self.inst = newinst
        self.inst2 = newinst2
        self.name = newname
        self.client.publish(f"{TOPIC_PREFIX}/sts/{self.name}/status", 'online', retain=True)
        self.client.publish(f"{TOPIC_PREFIX}/sts/{self.name}/network", 'rvc', retain=True)

        # Also publish the product information if it exists
        self.fnPublishProdIdent()

    def fnPublishProdIdent(self):
        # Retain this info because it generally only gets received
        # when an device connection is made
        for attr, param in [('make', 'Manufacturer'),
                            ('model', 'Model'),
                            ('serial', 'SerialNumber')]:
            if hasattr(self, attr):
                topic = f'{TOPIC_PREFIX}/sts/{self.name}/{param}'
                self.client.publish(topic, getattr(self, attr), retain=True)

    # Update a device connection with ProdIdent PGN and send appropriate topics
    def fnHandleProdIdent(self, msg):
        # We'll need to keep track of make and model for each device
        # in order to support model-specific features and differences
        # such as in fault and warning reporting
        self.make = msg.fnGetValue('Make')
        self.model = msg.fnGetValue('Model')
        self.serial = msg.fnGetValue('Serial')
        Logger.info(f"{self.make} {self.model} detected at node {self.addr}")

        # How we map PGN fields to/from topics depends on the model name
        if self.model in MODEL_MAP:
            self.func, self.param_map, self.cmd_map, self.fault_lookup, self.warning_lookup = MODEL_MAP[self.model]
        else:
            raise Exception(f"Model {self.model} is not supported")


    # Handle a message from the BMS that would indicate fault or warning flags
    def fnHandleBmsFlags(self, msg):
        # Are we talking BattSts6, DcSrcSts6 or PmLithionicsSts?
        mnem = msg.fnGetMnem()
        # Logger.debug(f"fnHandleBmsFlags received {mnem}")
        try:
            fieldlist = BMS_FLAG_LOOKUP[mnem]
        except KeyError:
            Logger.warning(f"{mnem} doesn't have alert flags")
            return

        now = time.perf_counter()
        for signal, code, condition, severity in fieldlist:
            flag = msg.fnGetValue(signal)
            if flag in ['On', 'Disconnected']:
                # Logger.debug(f"Alert: {mnem}.{signal} is {flag} -> {condition}")
                self.alerts[severity][code] = (now, condition)
            elif flag in ['Off', 'Connected']:
                try:
                    del(self.alerts[severity][code])
                except KeyError:
                    pass

            else:
                Logger.debug(f"{mnem}.{signal} is {flag}")

    # Handle a Nak or Dny to a previous command
    def fnHandleNak(self, msg):
        acknak = msg.fnGetValue('CtrlByte')
        nakd_pgn = msg.fnGetValue('ParmGrpNum')
        if nakd_pgn == self.cmd_pgn:
            # This is the NAK for the last command we sent
            topic = f'{TOPIC_PREFIX}/sts/{self.name}/{acknak}'
            payload = json.dumps({'param' : self.cmd_param,
                                  'value' : self.cmd_value,
                                  'reason' : msg.fnGetValue('GroupFunctionValue')})
            self.client.publish(topic, payload)
            # Logger.debug(f"Published topic {topic} : {payload}")

        else:
            Logger.warning(f"Got {acknak} for PGN {nakd_pgn} but last command was {self.cmd_pgn}")


    # Convert a diagnostic message to alerts
    def fnGetActiveAlerts(self):
        # We're returning a dictionary of lists of dictionaries
        active_alerts = {'faults' : [], 'warnings' : []}
        for key in self.alerts.keys():
            alert_dict = self.alerts[key]
            alert_list = []
            for alert_code in alert_dict.keys():
                alert_string = alert_dict[alert_code][1]
                new_alert = {'code' : alert_code,
                             'desc' : alert_string}
                alert_list.append(new_alert)
            active_alerts[key] = alert_list

        return active_alerts

    # A task dedicated to a connected device that creates a combined operating state
    # topic from other state topics
    async def fnPollTask(self):
        # We won't check for ZeroRPM proprietary parameters every cycle.
        # More like every 5 seconds
        ppn_cycle_count = PPN_CYCLE_SKIPS

        while True:
            try:
                # Re-calculate and publish the combined inverter/charger operating state
                if self.func == 'invchg':
                    await self.fnPollInvChgOpState()

                elif self.func == 'inv':
                    self.fnPollInvOpState()

                # Are there any non-broadcast messages that we're listening for that need
                # to be requested?
                await self.fnPollSubscriptions()

                # Are there any ZeroRPM proprietary parameters that need to be requested?
                if self.ppn_subscriptions:
                    ppn_cycle_count -= 1
                    if ppn_cycle_count <= 0:
                        ppn_cycle_count = PPN_CYCLE_SKIPS

                        await self.fnPollPpnSubscriptions()

                # Are there any ZeroRPM proprietary parameters that need to be written?
                if not self.ppn_write_queue.empty():
                    await self.fnPollPpnWrites()

                # While we're at it, update the alerts.  If there are multiple alerts
                # then we don't want to be sending out several duplicate messages per
                # second, so we do it here in a controlled, constant flow way.
                alerts = self.fnGetActiveAlerts()
                topic = f'{TOPIC_PREFIX}/sts/{self.name}/Alerts'
                self.client.publish(topic, json.dumps(alerts))

                # Run once per second
                await asyncio.sleep(1.0)
                #1.0

            except asyncio.CancelledError:
                raise

            except:
                traceback.print_exc()


    # Calculate and publish an inverter/charger's overall operating state
    async def fnPollInvChgOpState(self):
        if self.model.startswith('FEX_RVC'):
            self.devcon.fnSendRequestByMnem('PmInvSts')
            try:
                await self.fnWaitForResponse("PmInvSts")
            except asyncio.TimeoutError:
                Logger.warning("Timeout after requesting PmInvSts")

        try:
            # Not all inverter/chargers transmit OpMode, so we'll assume Operating if not defined
            opstate = fnInvChgOpState(self.devstate.get('OpMode', 'Operating'),
                                      self.devstate['InvOpState'],
                                      self.devstate['ChgOpState'])
        except KeyError:
            pass
        else:
            self.client.publish(f"{TOPIC_PREFIX}/sts/{self.name}/OpState", opstate)

    # Calculate and publish an inverter's overall operating state
    def fnPollInvOpState(self):
        try:
            opstate = fnInvOpState(self.devstate['InvOpState'])
        except KeyError:
            # Inverter InvOpState may not be received yet
            pass
        else:
            self.client.publish(f"{TOPIC_PREFIX}/sts/{self.name}/OpState", opstate)

    # Poll for any non-broadcast PGNs that we're listening for but need to be requested
    async def fnPollSubscriptions(self):
        for pgn_mnem in self.subscriptions:
            if pgn_mnem not in self.msgtimes or (time.perf_counter() - self.msgtimes[pgn_mnem]) > REREQ_TIMEOUT:
                if pgn_mnem is None:
                    continue  # Skip if the mnemonic is None
                # AssocSts request needs special handling
                if pgn_mnem.startswith('PmAssocSts'):
                    # Extract the association type and instance from the mnemonic
                    match = ASSOC_MATCHER.match(pgn_mnem)
                    if match:
                        assoc_type = match.group(1)
                        assoc_inst = int(match.group(2))

                        self.devcon.fnRequestPmAssocSts(assoc_type, assoc_inst)

                        # Await a response
                        try:
                            await self.fnWaitForResponse(pgn_mnem)
                        except asyncio.TimeoutError:
                            Logger.warning(f"Timeout after requesting {pgn_mnem}")

                else:
                    # Just a simple ISO request or a parameterless PmReq
                    # Logger.debug(f"Requesting {pgn_mnem}")
                    self.devcon.fnSendRequestByMnem(pgn_mnem)

                    # Await a response
                    try:
                        await self.fnWaitForResponse(pgn_mnem)
                    except asyncio.TimeoutError:
                        Logger.warning(f"Timeout after requesting {pgn_mnem}")

    # Poll for any ZeroRPM proprietary prescription numbers from subscriptions
    async def fnPollPpnSubscriptions(self):
        # An open session is not required for reading proprietary parameters

        # Send requests for all subscribed PPN parameters
        for param_mnem in self.ppn_subscriptions:
            if param_mnem == 'BpcSerialNum':
                # BPC serial number comes in two parts
                await self.fnPpnRequest('BpcSerialNum1')
                await self.fnPpnRequest('BpcSerialNum2')

            else:
                # Normal single-part parameter
                await self.fnPpnRequest(param_mnem)

    # Update writeable ZeroRPM PPNs with queued up changes
    async def fnPollPpnWrites(self):
        # Open a session
        await self.fnPpnSessionCmd('On')

        # Send write messages for all queued PPN parameters
        while not self.ppn_write_queue.empty():
            param_mnem, param_value = self.ppn_write_queue.get_nowait()
            await self.fnPpnWriteCmd(param_mnem, param_value)

        # Close the session
        await self.fnPpnSessionCmd('Off')

    # Make a PPN read request.  This usually takes two tries on a ZeroRPM battery
    async def fnPpnRequest(self, param_mnem, timeout=PPN_REQ_TIMEOUT, max_tries=PPN_REQ_MAX_TRIES):
        tries = 0
        while tries < max_tries:
            self.devcon.fnSendPpnReadCmd(param_mnem)
            try:
                await self.fnWaitForResponse("PmPpnReadRsp", timeout=timeout)
            except asyncio.TimeoutError:
                tries += 1
            else:
                # Got a reply before the timeout
                break

        if tries >= max_tries:
            Logger.warning(f"No response to PPN request for {param_mnem}")

    # Make a PPN session request.  This may take a few requests before getting a reply
    async def fnPpnSessionCmd(self, session_state, timeout=PPN_CMD_TIMEOUT, max_tries=PPN_CMD_MAX_TRIES):
        tries = 0
        while tries < max_tries:
            self.devcon.fnSendPpnSessionCmd(session_state)
            try:
                await self.fnWaitForResponse("PmPpnSessionRsp", timeout=timeout)
            except asyncio.TimeoutError:
                tries += 1
            else:
                # Got a reply before timeout
                break

        if tries >= max_tries:
            Logger.warning(f"No response to request to set PPN session to {session_state}")

    # Make a PPN write request.  As with other requests, this may take multiple tries
    async def fnPpnWriteCmd(self, param_mnem, param_value, timeout=PPN_CMD_TIMEOUT, max_tries=PPN_CMD_MAX_TRIES):
        tries = 0
        while tries < max_tries:
            self.devcon.fnSendPpnWriteCmd(param_mnem, param_value)
            try:
                await self.fnWaitForResponse("PmPpnWriteRsp", timeout=timeout)
            except asyncio.TimeoutError:
                tries += 1
            else:
                # Got a reply before timeout
                break

        if tries >= max_tries:
            Logger.warning(f"No response to write request for PPN {param_mnem}")

    # Wait for a response to a request to the connected device.
    # There is a timeout if no reply
    async def fnWaitForResponse(self, pgn_mnem, timeout=RSP_DEFAULT_TIMEOUT):
        # We are now waiting for an event from the message polling task
        self.rvc_rsp_mnem = pgn_mnem
        self.rvc_rsp_evt.clear()
        await asyncio.wait_for(self.rvc_rsp_evt.wait(), timeout=timeout)


# A virtual device manager that doesn't have its own CAN connection, such as a battery bank
class clVirtualDeviceManager(clBaseDeviceManager):
    def __init__(self, parent, model, name, inst):
        clBaseDeviceManager.__init__(self, parent)

        # Set the message to topic mapping according to the model given
        try:
            self.func, self.param_map, self.cmd_map, self.fault_lookup, self.warning_lookup = MODEL_MAP[model]
        except KeyError:
            raise Exception(f"No mapping of {model} for virtual {name}")

        self.name = name
        self.inst = inst

    # Handle a status message and publish topics according to subscriptions
    def fnHandleStsMsg(self, msg):
        # Is this PGN of interest to us?
        pgn = msg.fnGetMnem()
        if pgn in self.subscriptions:
            for sig, qual, fnTranslate, param in self.subscriptions[pgn]:
                if fnQualPasses(msg, qual):
                    # Publish the signal value
                    if callable(fnTranslate):
                        value = fnTranslate(msg)
                    else:
                        value = msg.fnGetValue(sig)
                    self.client.publish(f"{TOPIC_PREFIX}/sts/{self.name}/{param}", str(value))

# The overall object that oversees all of the event monitor and device
# monitoring tasks
class clBridgeApp(object):
    def __init__(self):
        # Event triggered by SIGTERM or SIGINT
        self.stop_evt = asyncio.Event()

        # Event triggered by MQTT client connect
        self.connect_evt = asyncio.Event()


        # Connect to the RV-C network and monitor for devices coming and going.
        self.devmgrs = {}  # keyed by CAN address

        # Virtual devices (like battery banks) keyed by device name
        self.vdevs = {}

        self.active_subscriptions = ['*/status',
                                     'inv*/OpState',
                                     'scc*/OpState',
                                     'invchg*/OpMode',
                                     'inv*/InvOpState',
                                     'invchg*/ChgOpState',
                                     '*/Alerts',
                                     '*batt*/HiVoltLimitSts',
                                     # 'invchg1/BattVolts',
                                     # 'invchg1/AcOut1Line1RmsVolts',
                                     # 'invchg1/AcOut1Line1RmsCurrent',
                                     # 'invchg1/AcIn1Line1RmsVolts',
        ]

        # The priority list for devices broadcasting DCSrcSts* messages.
        # The device sending with the highest device priority gets to publish
        # under the bank<n> device name.  This is usually the BMS.
        # The dictionary below is indexed on bank number.  The data for each
        # bank is another dictionary.  This inner dictionary is keyed on
        # priority (DCSrcSts*.DevPri) and the value is an iteger, which is a count-down timer.
        # The highest priority with a non-zero timer is the winner.
        self.bank_priority_timeouts = {}

    # The main task that calls the other tasks
    async def fnMainTask(self):
        # Before starting the RV-C bus connection, get into the internals
        # and override the transmit timer list.
        _rvc.fnOverrideTxTimeouts(TxTimers)
        _rvc.fnEnablePeriodReports(True)

        # Read the serial number for use in the ProdIdent PGN as well as the unique
        # number in IsoAddrClaim
        serialnum = '0' * 14
        try:
            with open(SERIALNUM_PATH, 'r') as fp:
                serialnum = fp.read().strip()

        except OSError:
            Logger.error(f"Could not read {SERIALNUM_PATH} for serial number")

        # Read the model number from a file
        modelnum = ''
        try:
            with open(MODEL_PATH, 'r') as fp:
                modelnum = fp.read().strip()

        except OSError:
            Logger.error(f"Could not read {MODEL_PATH} for XGW model number")

        # Override the default CAN address
        _rvc.MyNetAddr = MY_DFLT_SRC_ADDR

        # Use the bus connection as a context manager so that
        # it and its task get closed automatically on exit
        async with AsyncRvc.clBusConnection(uniquenum=None) as xb:
            # Create an event monitor right away, so that it detects devices
            # that are already on the network
            self.xb = xb
            self.monitor = xb.fnGetEventMonitor()

            # Set up send function for DiagMsg1 to be sent by periodic timer
            diagmsg1_obj = xb.fnMakeMsg('DiagMsg1', 1)
            for field, val in [('OpStsProdOn', 1),
                               ('OpStsProdActive', 1),
                               ('OpStsYel', 0),
                               ('OpStsRed', 0),
                               ('ProdId', MY_DFLT_SRC_ADDR)]:
                diagmsg1_obj.fnSetRawValue(field, val)

            def fnSendDiagMsg1(solicited, dest_addr,
                               pgn_mnem="DiagMsg1",
                               msgobj=diagmsg1_obj,
                               buscon=xb):
                try:
                    buscon.fnSendMsg(msgobj, dest_addr)

                except:
                    Logger.error("Error sending DiagMsg1")

            xb.fnAddPgnSender("DiagMsg1", fnSendDiagMsg1)

            # Set up send function for ProdIdent to be sent on request
            # (It's ProdIdent that needs the serial number)
            ident_str = f'Xantrex*{modelnum}*{serialnum}*'
            Logger.info(f"Our ProdIdent is {ident_str}")

            prodident_obj = xb.fnMakeMsg('ProdIdent', len(ident_str))
            for i, c in enumerate(ident_str):
                rawval = c.encode('utf-8')
                prodident_obj.fnSetRawValue('Data', rawval, tupindex=i)

            def fnSendIdent(solicited, dest_addr,
                            pgn_mnem='ProdIdent',
                            msgobj=prodident_obj,
                            buscon=xb):
                try:
                    buscon.fnSendMsg(msgobj, dest_addr)

                except:
                    Logger.error("Error sending ProdIdent")

            xb.fnAddPgnSender('ProdIdent', fnSendIdent)

            # Create an MQTT client and connect it
            will_message = gmqtt.Message(f"{TOPIC_PREFIX}/rvc/status", 'offline',
                                         retain=True,
                                         will_delay_interval=10)
            self.client = gmqtt.Client('RV-C',
                                       will_message=will_message)
            self.client.on_connect = self.fnConnectCB
            self.client.on_message = self.fnMessageCB

            await self.client.connect('localhost')

            # Run the event processing loop as a task once the connection
            # to the MQTT broker is fully made.
            # The event task will spawn and cancel CAN message tasks as
            # devices appear and disappear on the network
            await self.connect_evt.wait()
            evt_task = asyncio.ensure_future(self.fnEventTask())

            # Run another task to monitor timeouts, such as battery bank authorities
            # timing out
            timeout_task = asyncio.ensure_future(self.fnTimeoutTask())

            # Keep running until the user or somebody else kills this task
            await self.stop_evt.wait()

            # Cancel the event task, which will in turn cancel all of its active device tasks
            evt_task.cancel()
            try:
                await evt_task
            except asyncio.CancelledError:
                Logger.debug("Event task was cancelled")

            # Cancel the timeout task
            timeout_task.cancel()
            try:
                await timeout_task
            except asyncio.CancelledError:
                Logger.debug("Timeout task was cancelled")

            # Reason code 4 means disconnect with will message
            await self.client.disconnect(reason_code=4, reason_string="Exiting program")

    # Handle exiting the program when ^C is pressed
    def ask_exit(self, *args):
        self.stop_evt.set()

    # Handler for MQTT connect event
    def fnConnectCB(self, client, flags, rc, properties):
        Logger.debug(f"Broker connected with result code {rc}")

        # Publish that we've gone online
        client.publish(f"{TOPIC_PREFIX}/sts/rvc/status", 'online', retain=True)

        # Subscribe to any commands coming from outside
        # TODO: We should validate who can send what commands?
        client.subscribe('xnet/cmd/#')

        # Subscribe to any subscriptions and unsubscriptions from outside
        client.subscribe('xnet/sub/#')
        client.subscribe('xnet/unsub/#')

        # Unblock the RV-C bus event monitor task from starting
        self.connect_evt.set()

    # Message handling callback
    def fnMessageCB(self, client, topic, payload, qos, properties):
        Logger.info(f"Received {topic}: {payload}")

        # The topic is in the form xnet/cmd/<devname>/<param> for commands
        # xnet/sub/<devname> for subscriptions
        # xnet/unsub/<devname> for unsubscriptions

        # We'll need the device function and instance to find the device
        # connection that we need
        parts = topic.split('/')
        cmd = parts[1]
        devname = parts[2]
        if cmd == 'cmd':
            if len(parts) == 4:
                param = parts[3]
                self.fnHandleMqttCmd(devname, param, payload.decode())

        elif cmd == 'sub':
            self.fnHandleMqttSub(devname, payload)

        elif cmd == 'unsub':
            self.fnHandleMqttUnsub(devname, payload)

        else:
            # No action required
            Logger.warning(f"{cmd} is not a valid command")

    # Handle a command meant to be passed along to a device from an MQTT client
    def fnHandleMqttCmd(self, devname, param, param_value):
        devmgr = None
        for dm in self.devmgrs.values():
            print("hi")
            if dm.name == devname:
                devmgr = dm
                break

        if devmgr is None:
            Logger.warning(f"No connection to {devname}")
            return

        # Look up the PGN mnemonic and signal for the parameter name
        if param not in devmgr.cmd_map:
            Logger.error(f"No Pgn.Signal for {param}")
            return

        # Create a blank RV-C message according to the parameter and field
        pgn_mnem, sig_mnem, qual, translator = devmgr.cmd_map[param]
        if translator is None:
            sig_value = param_value
        else:
            fnTranslate = eval(translator)
            if not callable(fnTranslate):
                Logger.error(f"Translator for {param} needs to be a function")
                return

            sig_value = fnTranslate(param_value)

        # Is this on a weird PGN for proprietary parameters or a regular one?
        if pgn_mnem == 'PmPpnWriteCmd':
            # Weird
            devmgr.ppn_write_queue.put_nowait((sig_mnem, sig_value))

        else:
            # Handle a normal configuration command message
            msg = devmgr.devcon.fnMakeMsg(pgn_mnem)
            try:
                msg.fnSetValue('Inst', devmgr.inst)
            except AsyncRvc.XbMsgError:
                # No Inst signal for this message
                pass

            # Use the qualifiers to identify what parameter we're actually setting
            # when multiple messages of this PGN are possible from the device
            for qualtup in qual:
                sig, val = qualtup
                msg.fnSetValue(sig, val)

            msg.fnSetValue(sig_mnem, sig_value)
            # Logger.debug("Sending %s" % msg)

            # Save the particulars of this command in case it gets NAK'd
            devmgr.cmd_pgn = msg.fnGetPgn()
            devmgr.cmd_param = param
            devmgr.cmd_value = param_value

            # Send the message on the RV-C bus
            devmgr.devcon.fnSend(msg)

    # An MQTT client out there wants to subscribe to parameters from a device
    def fnHandleMqttSub(self, devname, payload):
        # The payload is a JSON list of parameters
        try:
            params = json.loads(payload)
        except json.JSONDecodeError:
            Logger.warning(f"Invalid json list: {payload}")
            return

        # Add to the updated list of subscribed parameters
        for param in params:
            if param not in ['Alerts', 'OpState']:
                self.active_subscriptions.append(f'{devname}/{param}')

        # If devname is connected, update the device task's subscription list
        self.fnUpdateDeviceSubscriptions(devname)


    # An MQTT client out there wants to unsubscribe from a device's parameters
    def fnHandleMqttUnsub(self, devname, payload):
        params = json.loads(payload)
        try:
            params = json.loads(payload)
        except json.JSONDecodeError:
            Logger.warning(f"Invalid json list: {payload}")
            return

        if params is None:
            # Unsubscribe all parameters for this device
            for subscription in self.active_subscriptions:
                if subscription.startswith(devname + '/'):
                    self.active_subscriptions.remove(subscription)

        elif type(params) == list:
            # Unsubscribe the specific parameters in the list
            for param in params:
                subscription = f'{devname}/{param}'
                if subscription in self.active_subscriptions:
                    self.active_subscriptions.remove(subscription)

        self.fnUpdateDeviceSubscriptions(devname)


    # Update a device's subscription list
    def fnUpdateDeviceSubscriptions(self, devname):
        # If devname is connected, update the device task's subscription list
        devmgr = None
        for dm in self.devmgrs.values():
            if dm.name == devname:
                devmgr = dm
                break

        if devmgr is not None:
            devmgr.fnUpdateSubscriptions(self.active_subscriptions)

        # Also check virtual devices like battery banks
        if devname in self.vdevs:
            self.vdevs[devname].fnUpdateSubscriptions(self.active_subscriptions)

    # The task that polls for events
    async def fnEventTask(self):
        try:
            while True:
                # Handle the next network event
                await self.fnEventPoll()

                # Add a bit of dead time between handling network events
                await asyncio.sleep(0.2)
                #0.2

        except asyncio.CancelledError:
            Logger.info("CancelledError within fnEventTask")
            # Cancel all sub-tasks and their device connections before exiting
            for devmgr in self.devmgrs.values():
                self.client.publish(f'{TOPIC_PREFIX}/sts/{devmgr.name}/status', 'offline', retain=True)
                await devmgr.fnClose()
                Logger.debug("devtask for node %d was cancelled" % devmgr.addr)

            self.client.publish(f'{TOPIC_PREFIX}/sts/rvc/status', 'offline', retain=True)

            raise

    # Process CAN network events
    async def fnEventPoll(self):
        evt = await self.monitor.fnGetEvent(blocking=True)
        if isinstance(evt, AsyncRvc.clNewNodeEvt):
            devmgr = clDeviceManager(self, evt.newaddr)
            self.devmgrs[evt.newaddr] = devmgr

            Logger.info(f"New node found at address {evt.newaddr}")

        elif isinstance(evt, AsyncRvc.clChangeNodeEvt):
            # The node has changed address but is the same identity
            Logger.info(f"Node {evt.oldaddr} changed to {evt.newaddr}")
            old_devmgr = self.devmgrs[evt.oldaddr]
            await old_devmgr.fnClose()
            new_devmgr = clDeviceManager(self, evt.newaddr)
            new_devmgr.func = old_devmgr.func
            new_devmgr.inst = old_devmgr.inst
            new_devmgr.inst2 = old_devmgr.inst2
            new_devmgr.name = old_devmgr.name
            new_devmgr.subscriptions = old_devmgr.subscriptions
            self.devmgrs[evt.newaddr] = new_devmgr

            del self.devmgrs[evt.oldaddr]

        elif isinstance(evt, AsyncRvc.clBumpNodeEvt) or isinstance(evt, AsyncRvc.clNodeTimeoutEvt):
            # Publish the demise of the node
            old_devmgr = self.devmgrs[evt.oldaddr]
            await old_devmgr.fnClose()
            self.client.publish(f"{TOPIC_PREFIX}/sts/{old_devmgr.name}/status", 'offline', retain=True)
            Logger.info(f"{old_devmgr.name} has gone offline")

            # Is this disappeared node a duplicate instance of another?
            if re.search(r'_node\d+$', old_devmgr.name):
                # Update duplicate instances may have disappeared.
                # If so, remove their name's _node<n> extension.
                remaining_dupes = []
                for dm in self.devmgrs.values():
                    if dm is not old_devmgr:
                        if ((dm.func == old_devmgr.func)
                            and (dm.inst == old_devmgr.inst)
                            and (dm.inst2 == old_devmgr.inst2)):
                            remaining_dupes.append(dm)

                if len(remaining_dupes) == 1:
                    # No longer a duplicate, but the instance(s) remain the same
                    dm = remaining_dupes[0]
                    dm.fnChangeName(newinst=dm.inst, newinst2=dm.inst2, dupe=False)

            del self.devmgrs[evt.oldaddr]

            # Update that list for MQTT.
            self.fnPublishInstAlerts()

        elif evt is None:
            pass

        else:
            Logger.error("Unexpected event class")

    # Handle timeouts that can take place over seconds
    async def fnTimeoutTask(self):
        while True:
            self.fnUpdateDcPriority()
            await asyncio.sleep(1.0)
            #1.0


    # Handle instance change of one device, changing names of other
    # devices as necessary in order to prevent duplicate names
    def fnHandleInstChange(self, devmgr, newinst, newinst2=None):
        # Are there any duplicate instances of this device type?
        dupes = False
        old_dupes = []
        for dm in self.devmgrs.values():
            if dm is not devmgr:
                if dm.func == devmgr.func:
                    if (dm.inst == newinst) and (dm.inst2 == newinst2):
                        dupes = True
                        dm.fnChangeName(newinst=dm.inst, newinst2=dm.inst2, dupe=True)

                    if devmgr.inst is not None:
                        if (dm.inst == devmgr.inst) and (dm.inst2 == devmgr.inst2):
                            old_dupes.append(dm)

        devmgr.fnChangeName(newinst, newinst2=newinst2, dupe=dupes)

        # Are there any other devices device type that are no longer duplicate?
        if len(old_dupes) == 1:
            for dm in old_dupes:
                dm.fnChangeName(dm.inst, newinst2=dm.inst2, dupe=dupes)

        # Update duplicate instances may have appeared or disappeared.
        # Update that list for MQTT.
        self.fnPublishInstAlerts()

    # Update alerts topic about duplicate instances (duplicates could either appear or disappear)
    def fnPublishInstAlerts(self):
        # Scan for any duplicate devicetype/instance combinations.
        devdict = {}
        dupe_found = False
        for dm in self.devmgrs.values():
            base_name = re.sub(r'_node\d+', '', dm.name)
            if base_name not in devdict:
                devdict[base_name] = 1
            else:
                dupe_found = True
                break

        # Start with an empty xnet/sts/rvc/Alerts dictionary and add to it as duplicates are detected
        alerts = {'faults' : [], 'warnings' : []}
        if dupe_found:
            # Duplicate instances are all faults with code 1.
            # The user can figure out the specifics from the device list.
            alerts['faults'].append({'code' : 1,
                                     'desc' : "Duplicate device instances found"})

        self.client.publish(f"{TOPIC_PREFIX}/sts/rvc/Alerts", json.dumps(alerts), retain=True)

    # Handle a DCSrcSts message that might be of interest to a battery bank virtual device
    def fnHandleDCSrcSts(self, msg):
        # All DCSrcSts* messages start with Inst an DevPri fields.
        # We want to use the info that comes from the most authorative source
        bankinst = msg.fnGetRawValue('Inst')
        priority = msg.fnGetRawValue('DevPri')

        # Check the priority against other devices that may be sending DCSrcSts messages.
        # If a device with higher priority is broadcasting then we don't process.
        # Otherwise, we publish under the bank<n> device name
        if self.fnCheckDcPriority(bankinst, priority):
            # Pass the message to the appropriate virtual device
            # and let its subscriptions determine how the message is processed
            devname = f"bank{bankinst}"
            vdev = self.vdevs[devname]
            vdev.fnHandleStsMsg(msg)

    # Check the priority of a DCSrcSts* message.  The device with the highest priority
    # (Usually the BMS) gets to publish data on the battery bank
    def fnCheckDcPriority(self, bankinst, priority):
        # The priority we're checking is the highest priority unless proven otherwise
        best = True
        found = False

        # Ignore anything with priority 255 (No Data), lest it be the
        # considered the highest priority source on battery bank data
        if priority == 255:
            return False

        try:
            # Is there already an active timer for this priority on this bank?
            bankdict = self.bank_priority_timeouts[bankinst]
            if priority in bankdict and bankdict[priority] > 0:
                found = True

        except KeyError:
            bankdict = {}
            self.bank_priority_timeouts[bankinst] = bankdict


        # Update the timer
        bankdict[priority] = DC_PRIORITY_TIMEOUT

        # Is there an active timer with higher priority?
        for other_priority, other_timer in bankdict.items():
            if other_priority > priority and other_timer > 0:
                best = False
                break

        # Announce bank coming online if this is our first detection of it
        if best and not found:
            devname = f"bank{bankinst}"

            # Create a new battery bank with the same parameter mapping as a
            # ZeroRPM BMS, to publish subscribed info as bank<n>
            vdev =clVirtualDeviceManager(self,
                                         ZRPM_BMS_MODELS[0],
                                         devname,
                                         bankinst)
            vdev.fnUpdateSubscriptions(self.active_subscriptions)
            self.vdevs[devname] = vdev
            self.client.publish(f"{TOPIC_PREFIX}/sts/{devname}/status", "online", retain=True)

        return best

    # Decrement the timers on the DCSrcSts battery bank priority list.
    # Call this every second by the timeout task
    def fnUpdateDcPriority(self):
        # Decrement all non-zero timers
        for bankinst, bankdict in self.bank_priority_timeouts.items():
            bank_alive = False
            bank_updated = False
            for priority, timer in bankdict.items():
                if timer > 0:
                    bankdict[priority] -= 1
                    bank_updated = True

                    if bankdict[priority] > 0:
                        bank_alive = True

            if bank_updated and not bank_alive:
                # Data for this battery bank stopped coming in a few seconds ago
                devname = f"bank{bankinst}"
                self.client.publish(f"{TOPIC_PREFIX}/sts/{devname}/status", "offline", retain=True)
                del(self.vdevs[devname])



# Print out the version info for this program
def fnShowVersionInfo():
    import VersionInfo
    
    print(f"Rvc2Mqtt Version {VersionInfo.VERSION}, Built {VersionInfo.BUILD_DATE}")
    if not (VersionInfo.REPO_CLEAN and VersionInfo.MASTER_SYNCED):
        print("Unofficial developer build")


if __name__ == '__main__':
    # Get command-line parameters just in case the user wants to get the version
    parser = ArgumentParser(description="RV-C to MQTT Bridge")
    parser.add_argument("--version",
                        dest="version",
                        help="Get version information and exit",
                        default=False,
                        action='store_true')
    args = parser.parse_args()

    if args.version:
        # Print the version info and quit
        fnShowVersionInfo()

    else:
        fnSetupLogging()

        app = clBridgeApp()

        loop = asyncio.get_event_loop()

        loop.add_signal_handler(signal.SIGINT, app.ask_exit)
        loop.add_signal_handler(signal.SIGTERM, app.ask_exit)

        loop.run_until_complete(app.fnMainTask())
