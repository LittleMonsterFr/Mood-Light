#-------------------------------------------------------------------------------
# Cmake toolchain file specifying the compiler to use.
# See README.md for how to install the compiler.
#-------------------------------------------------------------------------------

# Define our host system
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_SYSTEM_VERSION 1)

SET(rpi-tools-dir ${CMAKE_SOURCE_DIR}/rpi-tools)

# Define the C cross compiler location
SET(CMAKE_C_COMPILER  ${rpi-tools-dir}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc)

# Define the CXX cross compiler location
SET(CMAKE_CXX_COMPILER  ${rpi-tools-dir}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-g++)

# Define the sysroot path for the RaspberryPi distribution in our tools folder
SET(CMAKE_FIND_ROOT_PATH ${rpi-tools-dir}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/arm-linux-gnueabihf/sysroot/)

# Only use binaries from the host and not from the toolchain sysroot
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search for libraries and headers in the target directories only
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)

SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)