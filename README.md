

# Rvc2Mqtt - RV-C to MQTT bridge


## Purpose

This is a program to act as a proxy on the RV-C network, acting as a
control panel but publishing any received messages to the MQTT server
and subscribing to any commands. This allows clients of the MQTT server
to act as virtual control panels to the connected RV-C network.


## Dependencies

This script runs on Python version 3.5 or higher. It needs the following
packages installed:


### Dependencies for Development

-   cffi (For building C code of PyRvc stack)
-   et-xmlfile
-   openpyxl (For reading spreadsheet data into sqlite database)
-   pkg-resources (For building PyRvc)
-   pycparser (For building C code of PyRvc stack)
-   jdcal (Probably a dependency of openpyxl)
-   scons (For updating the database and generating code)


### Dependencies for Runtime

-   PyRvc (using AsyncRvc variant)
-   gmqtt MQTT library
-   future (used by PyRvc)

This program can run either under a venv that uses the traditional Python interpreter or
PyPy.


## Setting up the Virtual Environment

Create the virtual environment, load it up with pip-installable packages
and then do the customized install of PyRvc.

    python3 -m venv venv
    echo "export SCONS_ENABLE_VIRTUALENV=1" >> venv/bin/activate
    . venv/bin/activate
    pip install -r requirements.txt
    cd ~/xantrex_lib_xanbus/Software/Testing/Python/PyXanBus
    python setup.py install


## Building

If there are any updates to the spreadsheets in the docs/ directory or
changes to the database generation scripts then the database will have
to be regenerated with MakeParamDatabase.py and MakeAlertTable.py.

Do not change the database directly, as those changes will be lost
when the database is regenerated via the above scripts.

If the database is changed, then the auto-generated code will also
need to be regenerated via ParamListGen.py.

This is the automatic method with scons:

    scons

This is the by-hand method:

    python MakeParamDatabase.py
    python MakeAlertTable.py
    python ParamListGen.py


## Running

To run call Rvc2Mqtt.py under the Python virtual environment created above.
There are no parameters.

    ./venv/bin/python Rvc2Mqtt.py


## Published topics

All data items are published to the MQTT broker with topics in the
format of xnet/sts/<devname>/<Param>. The payload is
usually a simple text string with the value. If a numeric value has
engineering units, the units follow the number.


## Subscriptions

Rvc2Mqtt is a CPU-intensive program and therefore needs to only process
the incoming CAN data that is needed by the MQTT clients. Unfortunately,
MQTT does not give the capability to inform publishers of what data that
subscribers are listening for. The only work-around is for subscribers
to publish what they want to receive and for the Rvc2Mqtt bridge to
subscribe to subscription topics.


### Subscribing

The Rvc2Mqtt bridge subscribes to all topics of the form
xnet/sub/<devname>. The expected topic is a JSON-formatted list
of parameters that are subscribed to. Wildcards are not allowed.


### Unsubscribing

Once a client no longer needs information from a topic, the subscription
can be cancelled. The un-subscription topic name is similar to
subscription: xnet/unsub/<devname>. Again, the payload can be a
JSON-formatted list of parameter names. Additionally, using 'null' as
the payload acts as a wildcard and will unsubscribe all topics from the
named device.

Note: In order to support multiple clients, subscriptions are
cumulative. That is, if two clients subscribe to BattVolts, and then one
unsubscribes from BattVolts, the Rvc2Mqtt bridge will still publish
BattVolts. It will stop only once all clients have unsubscribed.

Also note: Using the wildcard to unsubscribe, will wipe out all
publishing of topics from a device, regardless of how many other clients
are still subscribed.

Third note: Unsubscribing will not prevent basic information from being
published for any and all connected devices. This includes:

-   status
-   OpState
-   OpMode
-   InvOpState
-   ChgOpState
-   Alerts
-   Model
-   Manufacturer
-   SerialNumber (Where available)


## Commands

Device commands and configuration changes can be made by publishing new
values to topics of the form
xnet/cmd/<devname>/<Param>. The payload is usually a
text string representing the new value. The Rvc2Mqtt bridge will
translate the value string into the appropriate RV-C PGN with the
appropriate fields set.

If the setting command is successful then the device will reply with an
IsoAck(Ack) and provide an updated Configuration Status PGN.
If the setting fails for some reason, then the device will reply with
an IsoAck(Nak).  In this case, the RV-C to MQTT bridge will publish
the NAK as an MQTT topic. The topic is of the form
xnet/sts/<devname>/Nak.
The payload of the message is a JSON object with the rejected topic
name, the value that was to be set, and a reason string.
The reason will usually be "Invalid Data"

Here is an example Nak payload:

    {"param": "CustomFloatVoltage", "value": "55.0 V", "reason": "Data Invalid"}

