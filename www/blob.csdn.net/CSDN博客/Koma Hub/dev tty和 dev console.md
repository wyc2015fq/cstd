# /dev/tty和/dev/console - Koma Hub - CSDN博客
2019年01月31日 21:25:46[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：54
个人分类：[Linux kernel																[Linux driver																[Linux](https://blog.csdn.net/Rong_Toa/article/category/7156254)](https://blog.csdn.net/Rong_Toa/article/category/8645170)](https://blog.csdn.net/Rong_Toa/article/category/7348890)
**目录**
[1.串行端口终端（/dev/ttySn）](#1.%E4%B8%B2%E8%A1%8C%E7%AB%AF%E5%8F%A3%E7%BB%88%E7%AB%AF%EF%BC%88%2Fdev%2FttySn%EF%BC%89)
[2.伪终端（/dev/pty/）](#2.%E4%BC%AA%E7%BB%88%E7%AB%AF%EF%BC%88%2Fdev%2Fpty%2F%EF%BC%89)
[3.控制终端（/dev/tty）](#3.%E6%8E%A7%E5%88%B6%E7%BB%88%E7%AB%AF%EF%BC%88%2Fdev%2Ftty%EF%BC%89%C2%A0)
[4.控制台终端（/dev/ttyn, /dev/console）](#4.%E6%8E%A7%E5%88%B6%E5%8F%B0%E7%BB%88%E7%AB%AF%EF%BC%88%2Fdev%2Fttyn%2C%20%2Fdev%2Fconsole%EF%BC%89%C2%A0)
[5.其它类型](#5.%E5%85%B6%E5%AE%83%E7%B1%BB%E5%9E%8B%C2%A0)
（1）**/dev/console**是系统控制终端，系统的错误信息什么的都输出到这里。 
（2）**/dev/tty**（n或者Sn）是进程控制台，如果在进程里打开一个这样的文件且该文件不是其他进程的控制终端时，那么该文件就是这个进程的控制终端。那么我们进程printf一个数据就是输出到这里.
（3）在本机上，用户可以使用alt+f1~fn切换控制台，看起来感觉存在多个屏幕，这种虚拟控制台对应tty1~n 但实际上机器只有一个屏幕，也就是我们看到的这个屏幕，对应console。 可以理解为console指向激活的那个tty（详细来说不是，tty0才是激活的那个tty，准确地说是激活的那个tty才将输出显示到console）.历史上，console指主机本身的屏幕键盘，而tty指用电缆链接的其它位置的控制台(仅包含屏幕和键盘)
```cpp
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int   main()
{
    int   fd;
    fd   =   open( "/dev/tty ",   O_WRONLY   |   O_NOCTTY);
    write(fd,   "hello   tty\n ",   10);
    write(fileno(stdout), "hello stdout", 12);
        return   0;
}
```
(4)**终端是一种字符型设备**，它有多种类型，通常使用tty来简称各种类型的终端设备。**tty是 Teletype的缩写**。Teletype是最早出现的一种终端设备，很象电传打字机（或者说就是），是由Teletype公司生产的。设备名放在特殊文件目录/dev/下，终端特殊设备文件一般有以下几种：
## 1.串行端口终端（/dev/ttySn）
串行端口终端（Serial Port Terminal）是使用计算机串行端口连接的终端设备。计算机把每个串行端口都看作是一个字符设备。有段时间这些串行端口设备通常被称为**终端设备**，因为那时它的最大用途就是用来连接终端。这些串行端口所对应的设备名称是/dev/tts/0（或/dev /ttyS0）、/dev/tts/1（或/dev/ttyS1）等，设备号分别是（4,0）、（4,1）等，分别对应于DOS系统下的COM1、 COM2等。
**若要向一个端口发送数据，可以在命令行上把标准输出重定向到这些特殊文件名上即可**。例如，在命令行提示符下键入：echo test > /dev/ttyS1会把单词”test”发送到连接在ttyS1（COM2）端口的设备上。 
## 2.伪终端（/dev/pty/）
伪终端（Pseudo Terminal）是成对的逻辑终端设备，例如/dev/ptyp3和/dev/ttyp3（或着在设备文件系统中分别是 /dev/pty/m3和/dev/pty/s3）。
它们与实际物理设备并不直接相关。如果一个程序把ttyp3看作是一个串行端口设备，则它对该端口的读/写操作会反映在该逻辑终端设备对的另一个上面（ttyp3）, 而ttyp3则是另一个程序用于读写操作的逻辑设备。这样，两个程序就可以通过这种逻辑设备进行互相交流，而其中一个使用ttyp3的程序则认为自己正在与一个串行端口进行通信。这很象是逻辑设备对之间的管道操作。 
对于ttyp3（s3），任何设计成使用一个串行端口设备的程序都可以使用该逻辑设备。但对于使用ptyp3的程序，则需要专门设计来使用ptyp3（m3）逻辑设备。 **例如**，如果某人在网上使用**telnet**程序连接到你的计算机上，则telnet程序就可能会开始连接到设备ptyp2（m2）上（一个伪终端端口上）。此时一个getty程序就应该运行在对应的ttyp2（s2）端口上。当telnet从远端获取了一个字符时，该字符就会通过m2、s2传递给 getty程序，而getty程序就会通过s2、m2和telnet程序往网络上返回”login:”字符串信息。这样，登录程序与telnet程序就通过“伪终端”进行通信。通过使用适当的软件，就可以把两个甚至多个伪终端设备连接到同一个物理串行端口上。 
## 3.控制终端（/dev/tty） 
如果当前进程有控制终端（Controlling Terminal）的话，那么/dev/tty就是当前进程的控制终端的设备特殊文件。可以使用命令”ps –ax”来查看进程与哪个控制终端相连。对于你登录的shell，/dev/tty就是你使用的终端，设备号是（5,0）。使用命令 ”tty”可以查看它具体对应哪个实际终端设备。/dev/tty有些类似于到实际所使用终端设备的一个联接。 
```
$ ps -a
      PID    PPID    PGID     WINPID   TTY         UID    STIME COMMAND
     3640     864    3640       8700  pty0      197609 21:23:20 /usr/bin/ps
      864    1256     864      13172  pty0      197609 20:38:24 /usr/bin/bash
     1256       1    1256       1256  ?         197609 20:38:23 /usr/bin/mintty
$ ls /dev/
clipboard  console  full     ptmx    scd0  sda2  sda5  sr0      stdout@  windows
conin      dsp      mqueue/  pty0    sda   sda3  sda6  stderr@  tty      zero
conout     fd@      null     random  sda1  sda4  shm/  stdin@   urandom
```
## 4.控制台终端（/dev/ttyn, /dev/console） 
在UNIX系统中，计算机显示器通常被称为控制台终端（Console）。它仿真了类型为Linux的一种终端（TERM=Linux），并且有一些设备特殊文件与之相关联：tty0、tty1、tty2等。当你在控制台上登录时，使用的是tty1。使用Alt+[F1—F6]组合键时，我们就可以切换到tty2、tty3等上面去。tty1 –tty6等称为虚拟终端，而tty0则是当前所使用虚拟终端的一个别名，系统所产生的信息会发送到该终端上。
因此不管当前正在使用哪个虚拟终端，系统信息都会发送到控制台终端上。 你可以登录到不同的虚拟终端上去，因而可以让系统同时有几个不同的会话期存在。只有系统或超级用户root可以向/dev/tty0进行写操作， 
## 5.其它类型 
还针对很多不同的字符设备存在有很多其它种类的终端设备特殊文件。例如针对ISDN设备的/dev/ttyIn终端设备等。这里不再赘述。
[https://www.cnblogs.com/yxmx/articles/1612627.html](https://www.cnblogs.com/yxmx/articles/1612627.html)
