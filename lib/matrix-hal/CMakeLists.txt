# ------------------------------------------------------------------------------
# Build matrix-hal library
# -----------------------------------------------------------------------------

add_compile_options(
        -std=gnu++11
        -Wno-unused-result
)

include_directories(
        ${CMAKE_SOURCE_DIR}/lib/fftw3/include
        ${CMAKE_SOURCE_DIR}/lib/wiringPi/wiringPi/wiringPi
)

set(LIB_NAME matrix-hal)

set(${LIB_NAME}_SRC_FILES
        matrix-creator-hal/cpp/driver/audio_output.cpp
        matrix-creator-hal/cpp/driver/bus_direct.cpp
        matrix-creator-hal/cpp/driver/bus_kernel.cpp
        matrix-creator-hal/cpp/driver/cross_correlation.cpp
        matrix-creator-hal/cpp/driver/direction_of_arrival.cpp
        matrix-creator-hal/cpp/driver/dummy_sensor.cpp
        matrix-creator-hal/cpp/driver/everloop.cpp
        matrix-creator-hal/cpp/driver/gpio_control.cpp
        matrix-creator-hal/cpp/driver/humidity_sensor.cpp
        matrix-creator-hal/cpp/driver/imu_sensor.cpp
        matrix-creator-hal/cpp/driver/matrix_driver.cpp
        matrix-creator-hal/cpp/driver/matrixio_bus.cpp
        matrix-creator-hal/cpp/driver/mcu_firmware.cpp
        matrix-creator-hal/cpp/driver/microphone_array.cpp
        matrix-creator-hal/cpp/driver/microphone_core.cpp
        matrix-creator-hal/cpp/driver/pressure_sensor.cpp
        matrix-creator-hal/cpp/driver/uart_control.cpp
        matrix-creator-hal/cpp/driver/uv_sensor.cpp
        matrix-creator-hal/cpp/driver/zwave_gpio.cpp
        )

add_library(${LIB_NAME} STATIC ${${LIB_NAME}_SRC_FILES})

target_link_libraries(${LIB_NAME} ${CMAKE_SOURCE_DIR}/lib/fftw3/lib/libfftw3f.a wiringPi gflags)

target_include_directories(${LIB_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/matrix-creator-hal
        )

set(DEMOS demos)

# Add all the demo source files
set(${DEMOS}_SRC_FILES
        matrix-creator-hal/demos/arc_demo.cpp
        matrix-creator-hal/demos/audio_output.cpp
        matrix-creator-hal/demos/compass_demo.cpp
        matrix-creator-hal/demos/direction_of_arrival_demo_direct.cpp
        matrix-creator-hal/demos/everloop_demo.cpp
        matrix-creator-hal/demos/firmware_info.cpp
        matrix-creator-hal/demos/gpio_demo.cpp
        matrix-creator-hal/demos/ipaddress_demo.cpp
        matrix-creator-hal/demos/ir_demo.cpp
        matrix-creator-hal/demos/micarray_pipes_direct.cpp
        matrix-creator-hal/demos/micarray_recorder_direct.cpp
        matrix-creator-hal/demos/mic_demo_direct.cpp
        matrix-creator-hal/demos/mic_demo_fir_direct.cpp
        matrix-creator-hal/demos/mic_energy_direct.cpp
        matrix-creator-hal/demos/pwm_demo.cpp
        matrix-creator-hal/demos/sensors_demo.cpp
        matrix-creator-hal/demos/sensors_status.cpp
        matrix-creator-hal/demos/servo_demo.cpp
        matrix-creator-hal/demos/uart_demo.cpp
        matrix-creator-hal/demos/uv_demo.cpp
        )

if (DEBUG)
    foreach(demo_source_file ${${DEMOS}_SRC_FILES})
        get_filename_component(demo_name ${demo_source_file} NAME_WE)

        add_executable(${demo_name} ${demo_source_file})

        target_link_libraries(${demo_name} ${LIB_NAME})

        # Specify the folder in which put the executable
        set_target_properties(${demo_name} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/hal-demos"
                )
    endforeach(demo_source_file ${${DEMOS}_SRC_FILES})
endif()