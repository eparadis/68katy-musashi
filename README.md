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
4. ~~Setup a cross-compiling environment and build some programming language to add to the uClinux image~~
5. Use "three tilde" escape sequence to pause emulation and open a menu to allow things like inspection of the machine, jumping to the monitor, and exiting the emulator.
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

# Setting up a a development toolchain
This isn't a cut-and-paste level of detail of what you need to do, but more of a guide of the steps I figured out over a few days. If you have questions, feel free to [check the issues or open a new one](https://github.com/eparadis/68katy-musashi/issues).


## Gather all the things you'll need
1. The original `m68k-elf-tools-20030314` cross compiling toolchain is available [here on the Wayback Machine](https://web.archive.org/web/20161028125832/www.uclinux.org/pub/uClinux/m68k-elf-tools/m68k-elf-tools-20030314.sh)
2. The toolchain only work on 32bit x86 machines, so you'll need to setup a 32bit x86 VM.
    1. I used QEMU `qemu-system-i386`. At time of writing, this is broken on M1/M2 Macs, so I had to use an Intel Mac.
    2. I used [this Debian 12 NetInst ISO image](https://cdimage.debian.org/debian-cd/current/i386/iso-cd/debian-12.0.0-i386-netinst.iso) and just set up my own new VM.
    3. Be prepared to `apt install` a lot of packages, most notably `build-essential`.
3. Get Steve Chamberlin's (the person who created 68Katy) modified uClinux distribution. This has his changes to support the memory map and whatnot of the 68Katy. Here's [a link to it on the Wayback Machine](https://web.archive.org/web/20201130011541/https://www.bigmessowires.com/68katy-pcb-files/uClinux-20040218-pcb.tar.gz).

## Set everything up
1. Install the m68k toolchain. It is a self-executing archive but if you have trouble there are instructions [on the 68Katy project page](https://www.bigmessowires.com/2015/01/14/make-your-own-katy/). You should end up with some `m68k-*` stuff in `/usr/local/bin/` amongst other places
2. Extract the uClinux distribution with `tar zxvf uClinux-20040218-pcb.tar.gz`.

## Build the original images to prove everything works
1. `cd uClinux-20040218-dist` and run `make dep`. Normally you'd start with `make config` or `make xconfig` or one of it's varients, but Steve's archive already has the configuration setup correctly and it's too easy to mess it up, so don't bother with that yet.
2. Build your image with `make`
3. copy the uClinux image somewhere convenient. It will be named `images/68Katy.rom`.

## Assemble the binary image
1. The emulator (or the real thing if you're building for that) needs the monitor binary with the uClinux image appended at `0x003000`. You can do using the following technique:
```
dd conv=notrunc bs=1 \
    if=~/68katy-musashi/rom-images/monitor.bin \
    of=newrom.bin \
    seek=0

dd conv=notrunc bs=1 \
    if=~/uClinux-20040218-dist/images/68Katy.rom \
    of=newrom.bin \
    seek=$((0x3000))
```

## Run the image on the emulator
1. Change directory to this project and build it
```
cd ~/68katy-musashi
make
```
2. Load your new image
```
$ ./sim wherevever/you/left/newrom.bin
Boot from RESET vector

zBug(ROM) for 68Katy (press ? for help)

084000>
```
3. Press `j` for `jump to address` and then type the address `003000`
```
zBug(ROM) for 68Katy (press ? for help)

084000>jump to address 084000>003000

uClinux/MC68000
Flat model support (C) 1998,1999 Kenneth Albanowski, D. Jeff Dionne
MC68000 68 Katy support by Big Mess o' Wires, Steve Chamberlin
Calibrating delay loop.. ok - 40.85 BogoMIPS
...
```

## Add your own program to the image
1. Now would be a good time to make sure everything is committed in the git repo you've used to keep track of all this. You've set this all up in a git repo, haven't you?
2. The instructions to add a "user" binary to the uClinux build are available at `Documentation/Adding-User-Apps-HOWTO`. I'll quickly summarize the minimum work here.
3. Create a directory in `user`. You can use Steve's `ledblink` as a template.
```
cd user
cp -r ledblink helloworld
cd helloworld
rm ledblink ledblink.o       # clean up
mv ledblink.c helloworld.c
vim Makefile                 # fix the names in the Makefile
vim helloworld.c             # actually add your code to do whatever you like
                             # It's standard C development stuff
```
4. Add an entry to `user/Makefile` like shown. It's around line 142. Look for `ledblink` as an example.

```
dir_$(CONFIG_USER_HELLOWORLD)        += helloworld
```

5. Add an entry in `config/config.in` like shown. It's around line 512. Look for `ledblink` again.

```
bool 'helloworld'                    'CONFIG_USER_HELLOWORLD'
```

6. Reconfigure uClinux with `make config` or `make xconfig`. Don't change anything other than to enable ("Y") the `CONFIG_USER_HELLOWORLD` setting.

7. Now you can go back up to `Build the original images to prove everything works` and follow the instructions from there.
```
make dep
make
dd conv=notrunc bs=1 if=monitor.bin of=newrom.bin seek=0
dd conv=notrunc bs=1 if=images/68Katy.rom of=newrom.bin seek=$((0x3000))
cd ~/68katy-musashi
make      # if you haven't recently
./sim wherever/newrom.bin
```

## Iterate!
You should be able to edit your `helloworld.c` and simply run `make dep` and `make` without having to reconfigure uClinux again. Reassemble the new ROM image with the monitor using `dd` and launch the emulator to test.

