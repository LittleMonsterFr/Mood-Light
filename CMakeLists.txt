#-------------------------------------------------------------------------------
# Main CMakeList file handling the build of the project
#-------------------------------------------------------------------------------

# Specify the minimal CMake version supported
cmake_minimum_required(VERSION 3.0.0)

# Name of the project
set(PROJECT_NAME Mood-Light)

# Set the name of the project and (optionaly) the type of the project
project(${PROJECT_NAME} C CXX)

# Tell CMake to recurse in these folders
add_subdirectory(lib/wiringPi)
add_subdirectory(lib/matrix-hal)

# For future control of the light with Infra Red
# add_subdirectory(lib/pigpio)

# The Matrix Hal use the namespace "google" to use gflags functions
set(GFLAGS_NAMESPACE "google")
add_subdirectory(lib/gflags)

# Binaries we want to build to then send on the RaspberryPi
set(EXECS
        src/bin/mood-light.cpp
        )

# Sources
set(sources
        src/ringBuffer.cpp
        src/mySemaphore.cpp
        src/patterns/rainbowWheelPattern.cpp
        src/patterns/emptyPattern.cpp
        src/patternHandler.cpp
        src/patterns/errorPattern.cpp
        src/patterns/hsvPattern.cpp
        src/patterns/sineRainbowPattern.cpp
        src/patterns/randomPattern.cpp
        src/patterns/decodeFailedPattern.cpp
        src/patterns/wavePattern.cpp
        src/patterns/uniformPattern.cpp
        src/helpers.cpp
        src/patterns/fastLedRainbowPattern.cpp
        src/patterns/transitionPattern.cpp
        src/patterns/whiteLightPattern.cpp
        src/patterns/duetPattern.cpp
        src/patterns/candlePattern.cpp
        )

# Flags to be used only with the sources written by us (not coming from a 3rd party lib)
set(flags
        -Wall
        -Werror
        -Wextra
        -pedantic
        -std=gnu++11
        -Wdouble-promotion
        -Wno-unknown-pragmas
        -Wno-unused-parameter
        -Wno-comment
        -Wno-unused-result
        )

if(DEBUG)
    add_compile_options(-O0 -ggdb3)
else(DEBUG)
    add_compile_options(-O2)
endif(DEBUG)

# Create an `object library` which represent object files
add_library(objs OBJECT ${sources})

# Add some includes only for the object library
target_include_directories(objs PRIVATE include lib/boost/)

# Add the compile flags only for the object library
target_compile_options(objs PRIVATE ${flags})

foreach(exec ${EXECS})
    get_filename_component(exec_name ${exec} NAME_WE)

    add_executable(${exec_name} ${exec} $<TARGET_OBJECTS:objs>)

    target_compile_options(${exec_name} PRIVATE ${flags})

    # Specify the folder in which to put the executable
    set_target_properties(${exec_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            )

    # Add needed include directories
    target_include_directories(${exec_name} PRIVATE
            lib/matrix-hal/matrix-creator-hal/cpp/driver
#            lib/pigpio/pigpio
            lib/chirp/include
            include
            )

    target_link_libraries(${exec_name}
            matrix-hal
#            pigpio
            ${CMAKE_SOURCE_DIR}/lib/chirp/lib/libchirp-connect_raspbian-arm.a
            )

endforeach(exec ${EXECS})
