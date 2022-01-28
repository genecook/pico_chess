pico_chess
==========

A port of [SeaChess](https://github.com/genecook/SeaChess) code
to raspberry pi pico. No optimizations to code
for use on pico, thus not terribly fast. Min-max algorithm only, default
of three levels. Five levels works too, but is slow.

Debugged using serial port (minicom) but since having implemented
a corresponding GUI (see [PicoGui](https://github.com/genecook/pico_gui).

Notes to myself on debugging pico code via picoprobe, openocd, gdb
------------------------------------------------------------------
To see which usb device is active:

   lsblk
   
To start picoprobe (assuming pico is 'mapped' to /dev/sda1):

   sudo mount /dev/sda1 /mnt/pico
   sudo cp ~/Desktop/pico/picoprobe/build/picoprobe.uf2 /mnt/pico
   
To start minicom (for communication with pico over serial):

   sudo minicom -D /dev/ttyACM0 -b 115200

To stop minicom:

   Ctrl-A, Z then X

For the following steps make sure both pico usb's are plugged in, at least when first starting openocd...

To start ocd:

   cd ~/Desktop/pico/openocd
   sudo src/openocd -f tcl/interface/picoprobe.cfg -f tcl/target/rp2040.cfg -s tcl

To compile with debug symbols (using cmake version 3.16.3):

   1. Add the following line below the 'project' line in the CMakeLists.txt file
   
      set(CMAKE_BUILD_TYPE Debug)

   2. Use:

      cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER_ID:STRING=GNU ..
      
   3. Make as usual 

To run gdb:

   gdb-multiarch 
   target remote localhost:3333
   load pico_chess.elf
   file pico_chess.elf  ### gdb-multiarch doesn't automatically load debug symbols
   monitor reset init
   continue

To load/run new program image (from already running gdb session):

   ctrl-C                   ### to interrupt gdb
   load hello_world.elf     ### load (presumably updated) elf image into memory
   continue                 ### resume (start) execution
   
To stop gdb:
   Ctrl-C
   quit

   
   
