# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/catkin_ws/cpp_redis/tacopie

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/catkin_ws/cpp_redis/tacopie/build

# Include any dependencies generated for this target.
include CMakeFiles/tacopie.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tacopie.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tacopie.dir/flags.make

CMakeFiles/tacopie.dir/sources/network/io_service.cpp.o: CMakeFiles/tacopie.dir/flags.make
CMakeFiles/tacopie.dir/sources/network/io_service.cpp.o: ../sources/network/io_service.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/catkin_ws/cpp_redis/tacopie/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tacopie.dir/sources/network/io_service.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tacopie.dir/sources/network/io_service.cpp.o -c /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/io_service.cpp

CMakeFiles/tacopie.dir/sources/network/io_service.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tacopie.dir/sources/network/io_service.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/io_service.cpp > CMakeFiles/tacopie.dir/sources/network/io_service.cpp.i

CMakeFiles/tacopie.dir/sources/network/io_service.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tacopie.dir/sources/network/io_service.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/io_service.cpp -o CMakeFiles/tacopie.dir/sources/network/io_service.cpp.s

CMakeFiles/tacopie.dir/sources/network/tcp_client.cpp.o: CMakeFiles/tacopie.dir/flags.make
CMakeFiles/tacopie.dir/sources/network/tcp_client.cpp.o: ../sources/network/tcp_client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/catkin_ws/cpp_redis/tacopie/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/tacopie.dir/sources/network/tcp_client.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tacopie.dir/sources/network/tcp_client.cpp.o -c /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/tcp_client.cpp

CMakeFiles/tacopie.dir/sources/network/tcp_client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tacopie.dir/sources/network/tcp_client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/tcp_client.cpp > CMakeFiles/tacopie.dir/sources/network/tcp_client.cpp.i

CMakeFiles/tacopie.dir/sources/network/tcp_client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tacopie.dir/sources/network/tcp_client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/tcp_client.cpp -o CMakeFiles/tacopie.dir/sources/network/tcp_client.cpp.s

CMakeFiles/tacopie.dir/sources/network/tcp_server.cpp.o: CMakeFiles/tacopie.dir/flags.make
CMakeFiles/tacopie.dir/sources/network/tcp_server.cpp.o: ../sources/network/tcp_server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/catkin_ws/cpp_redis/tacopie/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/tacopie.dir/sources/network/tcp_server.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tacopie.dir/sources/network/tcp_server.cpp.o -c /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/tcp_server.cpp

CMakeFiles/tacopie.dir/sources/network/tcp_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tacopie.dir/sources/network/tcp_server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/tcp_server.cpp > CMakeFiles/tacopie.dir/sources/network/tcp_server.cpp.i

CMakeFiles/tacopie.dir/sources/network/tcp_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tacopie.dir/sources/network/tcp_server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/tcp_server.cpp -o CMakeFiles/tacopie.dir/sources/network/tcp_server.cpp.s

CMakeFiles/tacopie.dir/sources/network/common/tcp_socket.cpp.o: CMakeFiles/tacopie.dir/flags.make
CMakeFiles/tacopie.dir/sources/network/common/tcp_socket.cpp.o: ../sources/network/common/tcp_socket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/catkin_ws/cpp_redis/tacopie/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/tacopie.dir/sources/network/common/tcp_socket.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tacopie.dir/sources/network/common/tcp_socket.cpp.o -c /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/common/tcp_socket.cpp

CMakeFiles/tacopie.dir/sources/network/common/tcp_socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tacopie.dir/sources/network/common/tcp_socket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/common/tcp_socket.cpp > CMakeFiles/tacopie.dir/sources/network/common/tcp_socket.cpp.i

CMakeFiles/tacopie.dir/sources/network/common/tcp_socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tacopie.dir/sources/network/common/tcp_socket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/common/tcp_socket.cpp -o CMakeFiles/tacopie.dir/sources/network/common/tcp_socket.cpp.s

CMakeFiles/tacopie.dir/sources/utils/error.cpp.o: CMakeFiles/tacopie.dir/flags.make
CMakeFiles/tacopie.dir/sources/utils/error.cpp.o: ../sources/utils/error.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/catkin_ws/cpp_redis/tacopie/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/tacopie.dir/sources/utils/error.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tacopie.dir/sources/utils/error.cpp.o -c /home/pi/catkin_ws/cpp_redis/tacopie/sources/utils/error.cpp

CMakeFiles/tacopie.dir/sources/utils/error.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tacopie.dir/sources/utils/error.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/catkin_ws/cpp_redis/tacopie/sources/utils/error.cpp > CMakeFiles/tacopie.dir/sources/utils/error.cpp.i

CMakeFiles/tacopie.dir/sources/utils/error.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tacopie.dir/sources/utils/error.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/catkin_ws/cpp_redis/tacopie/sources/utils/error.cpp -o CMakeFiles/tacopie.dir/sources/utils/error.cpp.s

CMakeFiles/tacopie.dir/sources/utils/logger.cpp.o: CMakeFiles/tacopie.dir/flags.make
CMakeFiles/tacopie.dir/sources/utils/logger.cpp.o: ../sources/utils/logger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/catkin_ws/cpp_redis/tacopie/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/tacopie.dir/sources/utils/logger.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tacopie.dir/sources/utils/logger.cpp.o -c /home/pi/catkin_ws/cpp_redis/tacopie/sources/utils/logger.cpp

CMakeFiles/tacopie.dir/sources/utils/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tacopie.dir/sources/utils/logger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/catkin_ws/cpp_redis/tacopie/sources/utils/logger.cpp > CMakeFiles/tacopie.dir/sources/utils/logger.cpp.i

CMakeFiles/tacopie.dir/sources/utils/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tacopie.dir/sources/utils/logger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/catkin_ws/cpp_redis/tacopie/sources/utils/logger.cpp -o CMakeFiles/tacopie.dir/sources/utils/logger.cpp.s

CMakeFiles/tacopie.dir/sources/utils/thread_pool.cpp.o: CMakeFiles/tacopie.dir/flags.make
CMakeFiles/tacopie.dir/sources/utils/thread_pool.cpp.o: ../sources/utils/thread_pool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/catkin_ws/cpp_redis/tacopie/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/tacopie.dir/sources/utils/thread_pool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tacopie.dir/sources/utils/thread_pool.cpp.o -c /home/pi/catkin_ws/cpp_redis/tacopie/sources/utils/thread_pool.cpp

CMakeFiles/tacopie.dir/sources/utils/thread_pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tacopie.dir/sources/utils/thread_pool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/catkin_ws/cpp_redis/tacopie/sources/utils/thread_pool.cpp > CMakeFiles/tacopie.dir/sources/utils/thread_pool.cpp.i

CMakeFiles/tacopie.dir/sources/utils/thread_pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tacopie.dir/sources/utils/thread_pool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/catkin_ws/cpp_redis/tacopie/sources/utils/thread_pool.cpp -o CMakeFiles/tacopie.dir/sources/utils/thread_pool.cpp.s

CMakeFiles/tacopie.dir/sources/network/unix/unix_self_pipe.cpp.o: CMakeFiles/tacopie.dir/flags.make
CMakeFiles/tacopie.dir/sources/network/unix/unix_self_pipe.cpp.o: ../sources/network/unix/unix_self_pipe.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/catkin_ws/cpp_redis/tacopie/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/tacopie.dir/sources/network/unix/unix_self_pipe.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tacopie.dir/sources/network/unix/unix_self_pipe.cpp.o -c /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/unix/unix_self_pipe.cpp

CMakeFiles/tacopie.dir/sources/network/unix/unix_self_pipe.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tacopie.dir/sources/network/unix/unix_self_pipe.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/unix/unix_self_pipe.cpp > CMakeFiles/tacopie.dir/sources/network/unix/unix_self_pipe.cpp.i

CMakeFiles/tacopie.dir/sources/network/unix/unix_self_pipe.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tacopie.dir/sources/network/unix/unix_self_pipe.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/unix/unix_self_pipe.cpp -o CMakeFiles/tacopie.dir/sources/network/unix/unix_self_pipe.cpp.s

CMakeFiles/tacopie.dir/sources/network/unix/unix_tcp_socket.cpp.o: CMakeFiles/tacopie.dir/flags.make
CMakeFiles/tacopie.dir/sources/network/unix/unix_tcp_socket.cpp.o: ../sources/network/unix/unix_tcp_socket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/catkin_ws/cpp_redis/tacopie/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/tacopie.dir/sources/network/unix/unix_tcp_socket.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tacopie.dir/sources/network/unix/unix_tcp_socket.cpp.o -c /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/unix/unix_tcp_socket.cpp

CMakeFiles/tacopie.dir/sources/network/unix/unix_tcp_socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tacopie.dir/sources/network/unix/unix_tcp_socket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/unix/unix_tcp_socket.cpp > CMakeFiles/tacopie.dir/sources/network/unix/unix_tcp_socket.cpp.i

CMakeFiles/tacopie.dir/sources/network/unix/unix_tcp_socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tacopie.dir/sources/network/unix/unix_tcp_socket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/catkin_ws/cpp_redis/tacopie/sources/network/unix/unix_tcp_socket.cpp -o CMakeFiles/tacopie.dir/sources/network/unix/unix_tcp_socket.cpp.s

# Object files for target tacopie
tacopie_OBJECTS = \
"CMakeFiles/tacopie.dir/sources/network/io_service.cpp.o" \
"CMakeFiles/tacopie.dir/sources/network/tcp_client.cpp.o" \
"CMakeFiles/tacopie.dir/sources/network/tcp_server.cpp.o" \
"CMakeFiles/tacopie.dir/sources/network/common/tcp_socket.cpp.o" \
"CMakeFiles/tacopie.dir/sources/utils/error.cpp.o" \
"CMakeFiles/tacopie.dir/sources/utils/logger.cpp.o" \
"CMakeFiles/tacopie.dir/sources/utils/thread_pool.cpp.o" \
"CMakeFiles/tacopie.dir/sources/network/unix/unix_self_pipe.cpp.o" \
"CMakeFiles/tacopie.dir/sources/network/unix/unix_tcp_socket.cpp.o"

# External object files for target tacopie
tacopie_EXTERNAL_OBJECTS =

lib/libtacopie.a: CMakeFiles/tacopie.dir/sources/network/io_service.cpp.o
lib/libtacopie.a: CMakeFiles/tacopie.dir/sources/network/tcp_client.cpp.o
lib/libtacopie.a: CMakeFiles/tacopie.dir/sources/network/tcp_server.cpp.o
lib/libtacopie.a: CMakeFiles/tacopie.dir/sources/network/common/tcp_socket.cpp.o
lib/libtacopie.a: CMakeFiles/tacopie.dir/sources/utils/error.cpp.o
lib/libtacopie.a: CMakeFiles/tacopie.dir/sources/utils/logger.cpp.o
lib/libtacopie.a: CMakeFiles/tacopie.dir/sources/utils/thread_pool.cpp.o
lib/libtacopie.a: CMakeFiles/tacopie.dir/sources/network/unix/unix_self_pipe.cpp.o
lib/libtacopie.a: CMakeFiles/tacopie.dir/sources/network/unix/unix_tcp_socket.cpp.o
lib/libtacopie.a: CMakeFiles/tacopie.dir/build.make
lib/libtacopie.a: CMakeFiles/tacopie.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/catkin_ws/cpp_redis/tacopie/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX static library lib/libtacopie.a"
	$(CMAKE_COMMAND) -P CMakeFiles/tacopie.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tacopie.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tacopie.dir/build: lib/libtacopie.a

.PHONY : CMakeFiles/tacopie.dir/build

CMakeFiles/tacopie.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tacopie.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tacopie.dir/clean

CMakeFiles/tacopie.dir/depend:
	cd /home/pi/catkin_ws/cpp_redis/tacopie/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/catkin_ws/cpp_redis/tacopie /home/pi/catkin_ws/cpp_redis/tacopie /home/pi/catkin_ws/cpp_redis/tacopie/build /home/pi/catkin_ws/cpp_redis/tacopie/build /home/pi/catkin_ws/cpp_redis/tacopie/build/CMakeFiles/tacopie.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tacopie.dir/depend
