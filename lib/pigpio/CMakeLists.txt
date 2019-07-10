# ------------------------------------------------------------------------------
# Build matrix-hal library
# ------------------------------------------------------------------------------

add_compile_options(
        -pthread
)

set(LIB_NAME pigpio)

set(${LIB_NAME}_SRC_FILES
        pigpio/pigpio.c
        pigpio/command.c
        )

add_library(${LIB_NAME} STATIC ${${LIB_NAME}_SRC_FILES})

target_include_directories(${LIB_NAME} PRIVATE pigpio)

target_link_libraries(${LIB_NAME} rt)