#!/bin/sh
# A program to build the RVC to MQTT daemon from scratch from inside
# a Raspberry Pi docker image

if [ ! -d "./venv" ]; then
    python3 -m venv venv
    echo  "export SCONS_ENABLE_VIRTUALENV=1" >> venv/bin/activate
    echo  "export NUITKA_CACHE_DIR=/nonexistent/" >> venv/bin/activate
    . venv/bin/activate
    pip install --upgrade pip
    pip install -r requirements.txt
    pip install *.whl
else
    . venv/bin/activate
fi

scons Deb
