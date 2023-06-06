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
1. Fix character double echo bug
2. Set `termios` to prevent capture of control characters and allow them to pass through to the emulated input device
3. Implement a series of characters that pause emulation to allow inspection of the machine, exiting to the monitor, and exiting the emulator
4. Scale emulated cpu speed by adjusting the number of cycles run per loop
5. Setup a cross-compiling environment and build some programming language to add to the uClinux image
6. Emulate some type of storage device so everything doesn't have to live in the "ROM" image
7. "Upgrade" the emulated CPU to a 68030 and try out images that require an MMU

# Example session
(note wierd double echo bug)
```
ed@MacBook-Air:~/git/68katy-musashi $ make && ./sim rom-images/monitor-plus-linux-pcb.bin 
make: Nothing to be done for `all'.
Boot from RESET vector

zBug(ROM) for 68Katy (press ? for help)

084000>jjump to address 084000>000033000000

uClinux/MC68000
Flat model support (C) 1998,1999 Kenneth Albanowski, D. Jeff Dionne
MC68000 68 Katy support by Big Mess o' Wires, Steve Chamberlin
Calibrating delay loop.. ok - 40.03 BogoMIPS
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
/> llss  bbiinn
advent4
expand
init
ledblink
sh
vi
/> aaddvveenntt44


                              Welcome to 

                       Colossal Cave Adventure!


               Original development by Willie Crowther.
                  Major features added by Don Woods.
                 Conversion to BDS C by J. R. Jaeger.
                Unix standardization by Jerry D. Pohl.
            Port to QNX 4 and bug fixes by James Lummel.

Would you like instructions?

>nnoo

You are inside a building, a well house for a large spring.
There are some keys on the ground here.
There is a shiny brass lamp nearby.
There is tasty food here.
There is a bottle of water here.
>ttaakkee  kkeeyyss

ok.
>qquuiitt

Do you really want to quit now?
>yyeess

ok.
Treasures:          0
Survival:           30
Score:              32
pid 6: failed 65280
/> ^[ed@MacBook-Air:~/git/68katy-musashi $ 
```

# Similar projects
- https://github.com/alexwinston/Jaty68k
- https://github.com/demoze/py68k