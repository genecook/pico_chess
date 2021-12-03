pico_chess
==========

A port of [SeaChess](https://github.com/genecook/SeaChess) code
to raspberry pi pico. No optimizations to code
for use on pico, thus not terribly fast. Min-max algorithm only, default
of three levels. Five levels works too, but is slow.

At some point I'll run performance tests on the original code in SeaChess,
then do some optimizations on the pico code as a result.


Notes to myself on debugging pico code via picoprobe, openocd, gdb
------------------------------------------------------------------
To start picoprobe:

   sudo mount /dev/sda1 /mnt/pico
   sudo cp ~/Desktop/pico/picoprobe/build/picoprobe.uf2 /mnt/pico
   
To start minicom:

   sudo minicom -D /dev/ttyACM0 -b 115200

To stop minicom:

   Ctrl-A, Z then X
 
To start ocd (so far, can only load and flash a new elf using the -c option):

   cd ~/Desktop/pico/openocd
   sudo src/openocd -f tcl/interface/picoprobe.cfg -f tcl/target/rp2040.cfg -s tcl -c "program hello_world.elf"

To compile with debug symbols (using cmake version 3.16.3):

   1. Add the following line below the 'project' line in the CMakeLists.txt file
   
      set(CMAKE_BUILD_TYPE Debug)

   2. Use:

      cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER_ID:STRING=GNU ..
      
   3. Make as usual 

To run gdb:

   gdb-multiarch hello_world.elf
   target remote localhost:3333
   file hello_world.elf           ### gdb-multiarch doesn't automatically load debug symbols
   monitor reset init
   continue

To load/run new program image (from already running gdb session):

   ctrl-C                   ### to interrupt gdb
   load hello_world.elf     ### load (presumably updated) elf image into memory
   continue                 ### resume (start) execution
   
To stop gdb:
   Ctrl-C
   quit

   
   