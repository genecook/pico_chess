to start picoprobe:

   sudo mount /dev/sda1 /mnt/pico
   sudo cp ~/Desktop/pico/picoprobe/build/picoprobe.uf2 /mnt/pico
   
to start minicom:

   sudo minicom -D /dev/ttyACM0 -b 115200

to stop minicom:

   Ctrl-A, Z then X
 
to start ocd:

   cd ~/Desktop/pico/openocd
   sudo src/openocd -f tcl/interface/picoprobe.cfg -f tcl/target/rp2040.cfg -s tcl

to run gdb:

   gdb-multiarch hello_world.elf
   monitor reset init
   continue

to stop gdb:
   Ctrl-C
   quit

   
   