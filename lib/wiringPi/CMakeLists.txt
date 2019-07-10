# ------------------------------------------------------------------------------
# Build wiringPi library
# ------------------------------------------------------------------------------

set(LIB_NAME wiringPi)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

set(${LIB_NAME}_SRC_FILES
        wiringPi/wiringPi/softTone.c
        wiringPi/wiringPi/softServo.c
        wiringPi/wiringPi/max31855.c
        wiringPi/wiringPi/mcp3004.c
        wiringPi/wiringPi/bmp180.c
        wiringPi/wiringPi/ds18b20.c
        wiringPi/wiringPi/wiringShift.c
        wiringPi/wiringPi/drcNet.c
        wiringPi/wiringPi/sn3218.c
        wiringPi/wiringPi/sr595.c
        wiringPi/wiringPi/ads1115.c
        wiringPi/wiringPi/wiringSerial.c
        wiringPi/wiringPi/wiringPiI2C.c
        wiringPi/wiringPi/wiringPi.c
        wiringPi/wiringPi/mcp23016.c
        wiringPi/wiringPi/piHiPri.c
        wiringPi/wiringPi/pseudoPins.c
        wiringPi/wiringPi/mcp23008.c
        wiringPi/wiringPi/rht03.c
        wiringPi/wiringPi/pcf8591.c
        wiringPi/wiringPi/wpiExtensions.c
        wiringPi/wiringPi/wiringPiSPI.c
        wiringPi/wiringPi/mcp23s17.c
        wiringPi/wiringPi/mcp23s08.c
        wiringPi/wiringPi/mcp3422.c
        wiringPi/wiringPi/mcp3002.c
        wiringPi/wiringPi/htu21d.c
        wiringPi/wiringPi/pcf8574.c
        wiringPi/wiringPi/piThread.c
        wiringPi/wiringPi/max5322.c
        wiringPi/wiringPi/softPwm.c
        wiringPi/wiringPi/mcp4802.c
        wiringPi/wiringPi/drcSerial.c
        wiringPi/wiringPi/mcp23017.c
        )

add_library(${LIB_NAME} STATIC ${${LIB_NAME}_SRC_FILES})

target_include_directories (${LIB_NAME} PRIVATE wiringPi/wiringPi)

target_link_libraries(${LIB_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT} crypt m rt)
