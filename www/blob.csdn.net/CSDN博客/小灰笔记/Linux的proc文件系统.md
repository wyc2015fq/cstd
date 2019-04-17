# Linux的proc文件系统 - 小灰笔记 - CSDN博客





2017年08月20日 01:30:40[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：164








       最初我是想看一下Win10内置Ubuntu中的I/O信息，但是发现根本找不到相应的文件。好奇中查看了一下整个/proc目录，看了一下似乎这是一个系统信息状态的信息汇集地。

       查看文档得知，/proc其实是一个特殊的文件系统，是一种虚拟文件系统。看着像是文件，也能够读取到其中的信息，但是有时候会发现这些文件占用的大小是0。此外，文件的时间戳似乎也一直随着时间而改变。

       查看了一下Win10中的Linux子系统信息如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/11_算法/01_TAOCP$ ls -al /proc/

total 4

dr-xr-xr-x 1 rootroot 0 Aug 19 18:48 .

drwxr-xr-x 2 rootroot 0 Jan  1  1970 ..

dr-xr-xr-x 1 rootroot 0 Aug 20 00:57 1

dr-xr-xr-x 1 greygrey 0 Aug 20 01:20 143

dr-xr-xr-x 1 greygrey 0 Aug 20 00:57 2

-r--r--r-- 1 rootroot 0 Aug 19 18:48 cmdline

-r--r--r-- 1 rootroot 0 Aug 19 18:48 cpuinfo

-r--r--r-- 1 rootroot 0 Aug 19 18:48 filesystems

-r--r--r-- 1 rootroot 0 Aug 19 18:48 interrupts

-r--r--r-- 1 rootroot 0 Aug 19 18:48 loadavg

-r--r--r-- 1 rootroot 0 Aug 19 18:48 meminfo

lrwxrwxrwx 1 rootroot 0 Aug 19 18:48 mounts -> self/mounts

lrwxrwxrwx 1 rootroot 0 Aug 19 18:48 net -> self/net

lrwxrwxrwx 1 rootroot 0 Aug 19 18:48 self -> 143

-r--r--r-- 1 rootroot 0 Aug 19 18:48 stat

dr-xr-xr-x 1 rootroot 0 Aug 19 18:48 sys

-r--r--r-- 1 rootroot 0 Aug 19 18:48 uptime

-r--r--r-- 1 rootroot 0 Aug 19 18:48 version

       里面的信息很多，但是其实是少了不少信息的。可能微软搞了这个东西纯粹是想让人用一下bash，其他的东西压根儿也就没有给考虑到。

       差不多每个文件我都查看了一下，大致的信息如下：

       Cmdline存储的信息应该是跟系统启动相关的；

       Cpuinfo则是CPU的信息，对每一个CPU的核都进行了信息汇总，汇总的比较详细，一个内核的信息示范如下：

processor       : 7

vendor_id       : GenuineIntel

cpu family      : 6

model           : 94

model name      :Intel(R) Core(TM) i7-6820HQ CPU @ 2.70GHz

stepping        : 3

microcode       : 0xffffffff

cpu MHz         : 2701.000

cache size      : 256 KB

physical id     : 0

siblings        : 8

core id         : 3

cpu cores       : 4

apicid          : 0

initialapicid  : 0

fpu             : yes

fpu_exception   : yes

cpuid level     : 6

wp              : yes

flags           : fpu vme de pse tsc msr pae mce cx8apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss httm pbe syscall nx pdpe1gb rdtscp lm pni pclmulqdq dtes64 monitor ds_cpl vmx smxest tm2 ssse3 fma cx16 xtpr pdcm pcid sse4_1 sse4_2
 x2apic movbe popcnttsc_deadline_timer aes xsave osxsave avx f16c rdrand

bogomips        : 5402.00

clflush size    : 64

cache_alignment :64

address sizes   : 36 bits physical, 48 bits virtual

power management:

       其他几个我能够稍微熟悉又又多少能够看懂的有：

       Filesystem：系统文件信息，其实有一些是硬件信息；

       Memeinfo：内存的信息

       Version：内核以及系统的版本

       Win10内置的版本信息如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/11_算法/01_TAOCP$ cat /proc/version

Linux version3.4.0-Microsoft (Microsoft@Microsoft.com) (gcc version 4.7 (GCC) ) #1 SMPPREEMPT Wed Dec 31 14:42:53 PST 2014

       应该是使用了3.4.0内核的Ubuntu2014吧？



