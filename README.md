to start picoprobe:

   sudo mount /dev/sda1 /mnt/pico
   sudo cp ~/Desktop/pico/picoprobe/build/picoprobe.uf2 /mnt/pico
   
to start minicom:

   sudo minicom -D /dev/ttyACM0 -b 115200

to stop minicom:

   Ctrl-A, Z then X
 
to start ocd (so far, can only load and flash a new elf using the -c option):

   cd ~/Desktop/pico/openocd
   sudo src/openocd -f tcl/interface/picoprobe.cfg -f tcl/target/rp2040.cfg -s tcl -c "program hello_world.elf"

to run gdb:

   gdb-multiarch hello_world.elf
   target remote localhost:3333
   monitor reset init
   continue

to load/run (but NOT flash) new program image (from already running gdb session):

   ctrl-C                   ### to interrupt gdb
   load hello_world.elf     ### load (presumably updated) elf image into memory
   continue                 ### resume (start) execution
   
to stop gdb:
   Ctrl-C
   quit

   
   