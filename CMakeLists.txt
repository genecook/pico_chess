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

include_directories(${CMAKE_SOURCE_DIR})

add_executable(pico_chess main.C pico_stream_player.C
               board.C move.C pieces.C
               bishop.C king.C knight.C queen.C rook.C pawn.C
	       moves_tree.C engine.C
              )

# Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(pico_chess pico_stdlib)

# enable uart output, disable usb output
pico_enable_stdio_usb(pico_chess 0)
pico_enable_stdio_uart(pico_chess 1)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(pico_chess)

# add url via pico_set_program_url
pico_set_program_url(pico_chess "www.plinth.com/foo")


