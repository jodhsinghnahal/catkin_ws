#g++ -std=c++17 -pthread -Wno-psabi -o my_program rvccc.cpp -g -D_32BIT_ARCHITECTURE="" -DXT_FAR="" -DXT_LITTLE_ENDIAN="" -DNO_APOX="" -DNO_CANCARDX="" -DNO_CANUSB="" -DNO_XANBR="" -DDEVICE_CLAIM_PGN=RVC_PGN_DIAG_MSG1 -ICode/Pkgs/Protocols/XanBus/Inc -ICode/Inc -ICode/Pkgs/Protocols/XanBus/Common/Pkgs/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Windows/WIN32/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Windows/WIN32/Pkgs/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Linux/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Linux/Pkgs/Inc -ICode/Pkgs/Protocols/XanBus/Devices/PROTOCOL_RVC/Inc librvc.so -lmosquitto
g++ --sysroot=/home/jodh/sysroot/rpi-sysroot -march=armv6 -mfpu=vfp -mfloat-abi=hard -marm -std=c++17 -pthread -Wno-psabi -o my_program rvccc.cpp -g -D_32BIT_ARCHITECTURE="" -DXT_FAR="" -DXT_LITTLE_ENDIAN="" -DNO_APOX="" -DNO_CANCARDX="" -DNO_CANUSB="" -DNO_XANBR="" -DDEVICE_CLAIM_PGN=RVC_PGN_DIAG_MSG1 -ICode/Pkgs/Protocols/XanBus/Inc -ICode/Inc -ICode/Pkgs/Protocols/XanBus/Common/Pkgs/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Windows/WIN32/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Windows/WIN32/Pkgs/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Linux/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Linux/Pkgs/Inc -ICode/Pkgs/Protocols/XanBus/Devices/PROTOCOL_RVC/Inc -lstdc++ -lpthread -lm  -ldl -lc librvc.so

# arm-linux-gnueabihf-g++ -std=c++17 -pthread -Wno-psabi -o my_program rvccc.cpp -g -D_32BIT_ARCHITECTURE="" -DXT_FAR="" -DXT_LITTLE_ENDIAN="" -DNO_APOX="" -DNO_CANCARDX="" -DNO_CANUSB="" -DNO_XANBR="" -DDEVICE_CLAIM_PGN=RVC_PGN_DIAG_MSG1 -ICode/Pkgs/Protocols/XanBus/Inc -ICode/Inc -ICode/Pkgs/Protocols/XanBus/Common/Pkgs/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Windows/WIN32/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Windows/WIN32/Pkgs/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Linux/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Linux/Pkgs/Inc -ICode/Pkgs/Protocols/XanBus/Devices/PROTOCOL_RVC/Inc --sysroot=/path/to/sysroot -L/path/to/sysroot/usr/lib -L/path/to/sysroot/lib librvc.so -lmosquitto

# arm-linux-gnueabihf-g++ -std=c++17 -pthread -Wno-psabi -o my_program rvccc.cpp -g -D_32BIT_ARCHITECTURE="" -DXT_FAR="" -DXT_LITTLE_ENDIAN="" -DNO_APOX="" -DNO_CANCARDX="" -DNO_CANUSB="" -DNO_XANBR="" -DDEVICE_CLAIM_PGN=RVC_PGN_DIAG_MSG1 -ICode/Pkgs/Protocols/XanBus/Inc -ICode/Inc -ICode/Pkgs/Protocols/XanBus/Common/Pkgs/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Windows/WIN32/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Windows/WIN32/Pkgs/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Linux/Inc -ICode/Pkgs/Protocols/XanBus/Targets/Linux/Pkgs/Inc -ICode/Pkgs/Protocols/XanBus/Devices/PROTOCOL_RVC/Inc librvc.so -lmosquitto
