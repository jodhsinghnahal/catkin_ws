###############################################################################
# (c) 2020 Xantrex Canada ULC. All rights reserved.
# All trademarks are owned or licensed by Xantrex Canada ULC,
# its subsidiaries or affiliated companies.
#
# FILE NAME:  ParamListGen.py
#
# PURPOSE:
#     This is program to generate the parameter list used by Rvc2Mqtt.py.
#     It does this by reading the parameter database and generating
#     Python code into auto-generated XnetParams.py
#
###############################################################################

###############################################################################
#                               Includes
###############################################################################

import sqlite3
import time
import re
from PgDesc import clXbDatabase

###############################################################################
#                             Constant Data
###############################################################################

# Database file name
DB_FILE = 'xnet_params.db'


CODEFILE_NAME = 'XnetParams.py'

FILE_HEADER = """###############################################################################
# (c) {} Xantrex Canada ULC. All rights reserved.
# All trademarks are owned or licensed by Xantrex Canada ULC,
# its subsidiaries or affiliated companies.
#
# FILE NAME:  {}
#
# PURPOSE:
#     This auto-generated file contains the RV-C PGN.Signallookups for
#     both the status and commandable parameters used in the RV-C to
#     MQTT bridge.  Each supported model gets its own lookup tables.
#
###############################################################################

###############################################################################
#                             Constant Data
###############################################################################

"""

###############################################################################
#                           Function Definitions
###############################################################################

# Get a qualification as a list of tuples
def fnQualList(db, param_id, model_id):
    qual_list = []
    cursor = db.cursor()
    cursor.execute("""SELECT signal, value FROM rvc_qual
                      WHERE param_id = ? and model_id = ?
                      ORDER BY signal DESC;""",
                   (param_id, model_id))
    while True:
        try:
            sig, val = cursor.fetchone()
        except:
            break

        qual_list.append((sig, val))

    cursor.close()

    return qual_list

# Convert a model name into a capitalized identifier
def fnModelIdentifier(model_name):
    model_ident = model_name.upper()
    model_ident = re.sub('[- ]', '_', model_ident)
    model_ident = re.sub(r'^(\d)', r'_\1', model_ident)

    return model_ident

# Convert a string for a Fault Mode Indicator into its number
def fnReverseInterpretFmi(xbdb, fmi_name):
    enumdict = xbdb.enumlookup[9419]
    for fmi, test_name in enumdict.items():
        if test_name == fmi_name:
            return fmi

    else:
        raise Exception(f"No FMI code for {fmi_name}")

# Create lookup tables for a model's alerts
def fnModelAlerts(db, fp, model_id, model_type, model_ident, alert_type):
    fp.write(f"# Mapping of SPN/FMI to {alert_type} code\n")
    fp.write(f"{model_ident}_{alert_type.upper()}_LOOKUP = [\n")

    # What kind of device is this? (DSA needed to interpret SPN into components)
    if model_type == 'scc':
        dsa = 141
    elif model_type.startswith('inv'):
        dsa = 66
    else:
        dsa = 255

    xbdb = clXbDatabase()

    alerts = []
    cursor = db.cursor()
    cursor.execute("""select rvc_alert_mapping.spn, rvc_alert_mapping.fmi,
                      rvc_alert_mapping.code, alerts.condition
                      from rvc_alert_mapping inner join alerts
                      on rvc_alert_mapping.model_id = alerts.model_id
                      and rvc_alert_mapping.code = alerts.code
                      where rvc_alert_mapping.model_id = ?
                      and rvc_alert_mapping.alert_type = ?;""",
                   (model_id, alert_type))
    while True:
        try:
            spn_name, fmi_name, code, condition = cursor.fetchone()
        except:
            break

        # Break down the SPN name into its three numeric parts
        spn_msb, spn_isb, spn_lsb = xbdb.fnReverseInterpretSpn(dsa, spn_name)

        # Convert the FMI name into its numeric equivalent
        fmi = fnReverseInterpretFmi(xbdb, fmi_name)

        alerts.append((spn_msb, spn_isb, spn_lsb, fmi, code, condition))

    cursor.close()

    alerts.sort()
    for spn_msb, spn_isb, spn_lsb, fmi, code, condition in alerts:
        if spn_msb != 0 and spn_isb == 0:
            spn_isb = None

        fp.write(f"    ({spn_msb}, {spn_isb}, {spn_lsb}, {fmi}, {code}, '{condition}'),\n")


    fp.write("    ]\n\n")


# Create lookup tables for a specific model that supports RV-C
def fnModelSupport(db, fp, model_id, model_type, model_name, model_friendly):
    fp.write(f"# --- Support for {model_friendly} ---")
    fp.write("\n\n")
    model_ident = fnModelIdentifier(model_name)

    # Generate the status parameters
    fp.write("# Mapping from device parameter names to Pgn/Signal/Qualifier\n")
    fp.write(f"{model_ident}_PARAM_MAP")
    fp.write(" = {\n")

    cursor = db.cursor()
    cursor.execute("""select param.id, param.name, rvc_support.sts_pgn, rvc_support.sts_sig,
                      rvc_support.from_sts_trans_func
                      from param inner join rvc_support
                      on param.id = rvc_support.param_id
                      where rvc_support.model_id = ?;""",
                    (model_id,))
    while True:
        try:
            param_id, param_name, pgn_mnem, sig_mnem, from_sts_trans_func = cursor.fetchone()

        except:
            break

        if param_name:
            if pgn_mnem is not None:
                pgn_mnem = f"'{pgn_mnem}'"
            else:
                pgn_mnem = 'None'

            if sig_mnem is not None:
                sig_mnem = f"'{sig_mnem}'"
            else:
                sig_mnem = 'None'

            qlist = fnQualList(db, param_id, model_id)

            if from_sts_trans_func:
                from_sts_trans_func = f"'{from_sts_trans_func}'"
            else:
                from_sts_trans_func = 'None'

            fp.write(f"    '{param_name}' : ({pgn_mnem}, {sig_mnem}, {qlist}, {from_sts_trans_func}),")
            fp.write("\n")

    fp.write("    }\n\n")
    cursor.close()

    # Generate the writeable parameters
    fp.write("# Mapping from commandable parameter to PGN and signal\n")
    fp.write(f"{model_ident}_CMD_MAP")
    fp.write(" = {\n")

    cursor = db.cursor()
    cursor.execute("""select param.id, param.name, rvc_support.cmd_pgn, rvc_support.cmd_sig,
                      rvc_support.to_cmd_trans_func
                      from param inner join rvc_support
                      on param.id = rvc_support.param_id
                      where cmd_pgn is not null
                      and rvc_support.model_id = ?;""",
                    (model_id,))

    while True:
        try:
            param_id, param_name, cmd_pgn, cmd_sig, to_cmd_trans_func = cursor.fetchone()
        except:
            break

        qlist = fnQualList(db, param_id, model_id)

        if to_cmd_trans_func:
            to_cmd_trans_func = f"'{to_cmd_trans_func}'"
        else:
            to_cmd_trans_func = 'None'

        fp.write(f"    '{param_name}' : ('{cmd_pgn}', '{cmd_sig}', {qlist}, {to_cmd_trans_func}),")
        fp.write("\n")

    fp.write("    }\n\n")

    cursor.close()

    # Generate the SPN/FMI to fault and warning lookups
    fnModelAlerts(db, fp, model_id, model_type, model_ident, 'fault')
    fnModelAlerts(db, fp, model_id, model_type, model_ident, 'warning')

# The main program starts here
def fnMain():
    # Create a connection to the parameter database
    db = sqlite3.Connection(DB_FILE)

    # Open the generated code file for writing
    with open(CODEFILE_NAME, 'w') as fp:
        # Write the file header
        year = time.localtime().tm_year
        fp.write(FILE_HEADER.format(year, CODEFILE_NAME))

        # Go through each model that supports RV-C and create mappings for each
        model_names = []
        cursor = db.cursor()
        cursor.execute("""select distinct model.id, model.devtype,
                          model.name, model.friendly_name
                          from model inner join rvc_support
                          on model.id = rvc_support.model_id
                          order by model.id""")
        while True:
            try:
                model_id, model_type, model_name, model_friendly = cursor.fetchone()

            except:
                break

            model_names.append((model_name, model_type))
            fnModelSupport(db, fp, model_id, model_type, model_name, model_friendly)

        cursor.close()

        # Now create a lookup table for it all, keyed on the model name from ProdInfoSts
        fp.write("\n# Mapping of support for each model\n")
        fp.write("MODEL_MAP = {\n")
        for model_name, model_type in model_names:
            model_ident = fnModelIdentifier(model_name)
            fp.write(f"    '{model_name}' : ('{model_type}', {model_ident}_PARAM_MAP, {model_ident}_CMD_MAP,\n")
            fp.write(f"                      {model_ident}_FAULT_LOOKUP, {model_ident}_WARNING_LOOKUP),\n")
        fp.write("    }\n")




if __name__ == '__main__':
    fnMain()

