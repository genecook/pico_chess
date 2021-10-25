cmake_minimum_required(VERSION 3.12)

set(CMAKE_MODULE_PATH ~/Desktop/pico/pico-sdk ~/Desktop/pico/pico-examples)

# Pull in SDK (must be before project)
include(pico_sdk_import)

project(pico_chess C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

include(example_auto_set_url)

add_executable(hello_world
        main.C
        )

# Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(hello_world pico_stdlib pico_multicore)

# enable uart output, disable usb output
pico_enable_stdio_usb(hello_world 0)
pico_enable_stdio_uart(hello_world 1)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_world)

# add url via pico_set_program_url
pico_set_program_url(hello_world "www.plinth.com/foo")


