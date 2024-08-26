#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "cpp_redis" for configuration "Release"
set_property(TARGET cpp_redis APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(cpp_redis PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/arm-linux-gnueabihf/libcpp_redis.so.4.4.0"
  IMPORTED_SONAME_RELEASE "libcpp_redis.so.4.4.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS cpp_redis )
list(APPEND _IMPORT_CHECK_FILES_FOR_cpp_redis "${_IMPORT_PREFIX}/lib/arm-linux-gnueabihf/libcpp_redis.so.4.4.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
