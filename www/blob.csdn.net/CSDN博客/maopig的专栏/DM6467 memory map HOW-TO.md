# DM6467 memory map HOW-TO - maopig的专栏 - CSDN博客
2012年01月08日 17:57:14[maopig](https://me.csdn.net/maopig)阅读数：1904标签：[vector																[application																[allocation																[insert																[video																[reference](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=allocation&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=vector&t=blog)
个人分类：[DM368](https://blog.csdn.net/maopig/article/category/919673)
1.       First, you should calculate your memory map of your board. For example, we use 128M DDR2 memory, and our Address Range is as below(this step is very important):
Address Range                   Size         Description
0x80000000 - 0x847FFFFF 72 MB    Linux
0x84800000 - 0x867FFFFF 32 MB    CMEM
0x86800000 - 0x877FFFFF 16 MB    DDRALGHEAP
0x87800000 - 0x87EFFFFF 7 MB      DDR2 (BIOS, Codecs, Applications)
0x87F00000 - 0x87F0007F 128B      DSPLINK (RESET)
0x87F00080 - 0x87FFFFFF        1MB-128B      DSPLINK (MEM)
From the Address Range, we know that the Linux use the 72M, so we should set the uboot’s parameter to 72M, as step 2.
2.       Second, reboot the board, and set the bootargs like this:
setenv nfshost 192.168.0.111;setenv rootpath /home/colin/workdir/filesys;
setenv bootcmd 'dhcp;bootm';
setenv bootargs console=ttyS0,115200n8 noinitrd rw ip=dhcp root=/dev/nfs nfsroot=192.168.0.111:/home/colin/workdir/filesys,nolock mem=**72M**;
setenv serverip 192.168.0.111;
setenv bootfile uImage_6467_evm.dat;
3.       Third, when the system is ready, we should execute the shell script: loadmodules.sh, but we should modify it first, like this:
# insert cmemk, tell it to occupy physical 64MB-104MB and create enough
# contiguous buffers for the worst case requirements of the demos.
insmod cmemk.ko phys_start=0x84800000 phys_end=0x86800000 pools=9x3133440,2x1434240,2x663552,10x60000
# insert dsplinkk, tell it that DSP's DDR is at physical 250MB-254MB
insmod dsplinkk.ko
# alter dma queue mapping for better visual performance
if [ -f mapdmaq-hd ]
then
    ./mapdmaq-hd
fi
# make /dev/dsplink
rm -f /dev/dsplink
mknod /dev/dsplink c `awk "\\$2==\"dsplink\" {print \\$1}" /proc/devices` 0
because in our digiplayer, we allocate 7 buffers for video and audio respectively, so 9x3133440 is for video decode, 10x60000 is for audio decode.
**3133440 = 1920*(1080+8)*1.5, so the buffer is big enough for video with 1080i to decode.**
4.       Now, we should rebuild the decodeCombo.x64P, change directory to PlayerDepend, and find the directory servers/decode, modify the file decodeCombo.tcf:
var mem_ext = [
    // will use for cmem: base: 0x84800000 (72MB offset), size: 0x2000000(32MB)
{
    comment:    "DDRALGHEAP: off-chip memory for dynamic algmem allocation",
    name:       "DDRALGHEAP",
    base:       0x86800000,   // 104MB offset
    len:        0x01000000,   // 16MB
    space:      "code/data"
},
{
    comment:    "DDR2: off-chip memory for code and data",
    name:       "DDR2",
    base:       0x87800000,   // 120MB offset
    len:        0x00700000,   // 7MB size
    space:      "code/data"
},
{
    comment:    "RESET_VECTOR: off-chip memory for the reset vector table",
    name:       "RESET_VECTOR",
    base:       0x87F00000, // 127MB
    len:        0x00000080, // 128B
    space:      "code/data"
},
{
    comment:    "DSPLINK: off-chip memory for DSPLINK code and data",
    name:       "DSPLINKMEM",
    base:       0x87F00080,   // 127MB
    len:        0x000FFF80,   //   1M-128B
    space:      "code/data"
},
];
**Attention: the memory map is calculated through step 1.**
**And then, recompile the decodeCombo.x64P using command make.**
5.       modify the Application *.cfg file, add the following lines:
var osalGlobal = xdc.useModule('ti.sdo.ce.osal.Global');
osalGlobal.armDspLinkConfig = {
    memTable: [
        ["DDRALGHEAP",   {addr: 0x86800000, size: 0x01000000, type: "other"}],
        ["DDR2",         {addr: 0x87800000, size: 0x00700000, type: "main" }],
        ["RESET_VECTOR", {addr: 0x87F00000, size: 0x00000080, type: "reset"}],
        ["DSPLINKMEM",   {addr: 0x87F00080, size: 0x000FFF80, type: "link" }],
 ],
};
**Attention: this memory map is calculated by step 1, and you must be the same as decodeCombo.tcf.**
6.       Test:
1)      ./loadmodules.sh
2)      Copy the decodeCombo.x64P and application to the execution directory and run the application.
**Good Luck!**
7.       Reference
1)      [http://wiki.davincidsp.com/index.php?title=Changing_the_DVEVM_memory_map](http://wiki.davincidsp.com/index.php?title=Changing_the_DVEVM_memory_map)
2)      [http://wiki.davincidsp.com/index.php/Codec_Engine_Examples_Memory_Map](http://wiki.davincidsp.com/index.php/Codec_Engine_Examples_Memory_Map)
3)      Mastering the art of Memory Map Configuration for DaVinci-Based Systems, SPRAAQ6
            
