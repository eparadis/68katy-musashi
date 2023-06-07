# 68katy + Musashi
a 68katy emulator using Musashi

# Features
- Runs the original `monitor+uClinux` image from BMoW

# "Features"/Bugs
- Emulation speed is not throttled in any way
- Output device seems to work okay
- Input device is weird, but works enough to poke around

# 68katy
https://www.bigmessowires.com/68-katy/

## Memory map
```
PCB 68 KATY
00000 - 77FFF : ROM
78000 - 79FFF : serial in
7A000 - 7BFFF : serial out
7C000 - 7CFFF : serial status RDF
7D000 - 7DFFF : serial status TXE
7E000 - 7FFFF : LED register
80000 - FFFFF : RAM
```

# TODO
1. ~~Fix character double echo bug~~
2. ~~Set `signal` to prevent capture of control characters and allow them to pass through to the emulated input device. CTRL-D and CTRL-H work, amongst others. But CTRL-C is caught by the outer terminal.~~
3. ~~Scale emulated cpu speed by adjusting the number of cycles run per loop~~
4. Use "three tilde" escape sequence to pause emulation and open a menu to allow things like inspection of the machine, jumping to the monitor, and exiting the emulator.
5. Setup a cross-compiling environment and build some programming language to add to the uClinux image
6. Emulate some type of storage device so everything doesn't have to live in the boot image
7. "Upgrade" the emulated CPU to a 68030 and try out images that require an MMU

# Example session
```
ed@MacBook-Air:~/git/68katy-musashi $ make && ./sim rom-images/monitor-plus-linux-pcb.bin
gcc -Wall -Wextra -pedantic   -c -o sim.o sim.c
gcc -Wall -Wextra -pedantic   -c -o cpu_read.o cpu_read.c
gcc -Wall -Wextra -pedantic   -c -o cpu_write.o cpu_write.c
gcc -Wall -Wextra -pedantic   -c -o nmi.o nmi.c
gcc -Wall -Wextra -pedantic   -c -o input_device.o input_device.c
gcc -Wall -Wextra -pedantic   -c -o output_device.o output_device.c
gcc -Wall -Wextra -pedantic   -c -o timer.o timer.c
gcc -Wall -Wextra -pedantic   -c -o irqs.o irqs.c
gcc -Wall -Wextra -pedantic   -c -o osd_linux.o osd_linux.c
gcc -Wall -Wextra -pedantic   -c -o m68kcpu.o m68kcpu.c
gcc -Wall -Wextra -pedantic   -c -o osd_linux.o osd_linux.c
<------ a bunch of warnings in Musashi's m68kfpu.c omitted ------>
gcc -Wall -Wextra -pedantic   -c -o m68kdasm.o m68kdasm.c
gcc -Wall -Wextra -pedantic   -c -o m68kops.o m68kops.c
gcc -o sim sim.o cpu_read.o cpu_write.o nmi.o input_device.o output_device.o timer.o irqs.o osd_linux.o m68kcpu.o m68kdasm.o softfloat/softfloat.o m68kops.o -Wall -Wextra -pedantic -lm
Boot from RESET vector

zBug(ROM) for 68Katy (press ? for help)

084000>jump to address 084000>003000

uClinux/MC68000
Flat model support (C) 1998,1999 Kenneth Albanowski, D. Jeff Dionne
MC68000 68 Katy support by Big Mess o' Wires, Steve Chamberlin
Calibrating delay loop.. ok - 40.85 BogoMIPS
Memory available: 388k/495k RAM, 240k/480k ROM (240k kernel code, 104k data)
Swansea University Computer Society NET3.035 for Linux 2.0
NET3: Unix domain sockets 0.13 for Linux NET3.035.
uClinux version 2.0.39.uc2 (ubuntu@ubuntu-VirtualBox) (gcc version 2.95.3 20010315 (release)(ColdFire patches - 20010318 from http://fiddes.net/coldfire/)(uClinux XIP and shared lib patches from http://www.snapgear.com/)) 154 Tue Jan 13 12:06:52 PST 2015
FTDI FT245 driver version 1.01 by Steve Chamberlin
ttyS0 at 0x00078000 (irq = 2) is a FTDI FT245
Ramdisk driver initialized : 16 ramdisks of 4096K size
Blkmem copyright 1998,1999 D. Jeff Dionne
Blkmem copyright 1998 Kenneth Albanowski
Blkmem 1 disk images:
0: 48530-7792F (RO)
VFS: Mounted root (romfs filesystem) readonly.
Shell invoked to run file: /etc/rc
Command: /bin/expand /etc/ramfs.img /dev/ram0
Command: mount -t proc proc /proc
Command: mount -t ext2 /dev/ram0 /var
Command: mkdir /var/tmp
Command: mkdir /var/log
Command: mkdir /var/run
Command: mkdir /var/lock
Command: cat /etc/motd
Welcome to
          ____ _  _
         /  __| ||_|                 
    _   _| |  | | _ ____  _   _  _  _ 
   | | | | |  | || |  _ \| | | |\ \/ /
   | |_| | |__| || | | | | |_| |/    \
   |  ___\____|_||_|_| |_|\____|\_/\_/
   | |
   |_|

For further information check:
http://www.uclinux.org/

Command: 
Execution Finished, Exiting

Sash command shell (version 1.1.1)
/> ls bin
advent4
expand
init
ledblink
sh
vi
/> advent4

                              Welcome to 

                       Colossal Cave Adventure!


               Original development by Willie Crowther.
                  Major features added by Don Woods.
                 Conversion to BDS C by J. R. Jaeger.
                Unix standardization by Jerry D. Pohl.
            Port to QNX 4 and bug fixes by James Lummel.

Would you like instructions?

>no
You are inside a building, a well house for a large spring.
There are some keys on the ground here.
There is a shiny brass lamp nearby.
There is tasty food here.
There is a bottle of water here.
>quit
Do you really want to quit now?
>yes
ok.
Treasures:          0
Survival:           30
Score:              32
pid 6: failed 65280
/> # pressing ESC to quit
/> ed@MacBook-Air:~/git/68katy-musashi $ 
```

# Similar projects
- https://github.com/alexwinston/Jaty68k
- https://github.com/demoze/py68k