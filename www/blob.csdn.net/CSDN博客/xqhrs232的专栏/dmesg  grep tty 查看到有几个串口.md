# dmesg | grep tty*查看到有几个串口 - xqhrs232的专栏 - CSDN博客
2016年03月21日 23:24:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1858
原文地址::[http://blog.sina.com.cn/s/blog_475cb6780100rrl1.html](http://blog.sina.com.cn/s/blog_475cb6780100rrl1.html)
相关文章
1、[linux dmesg命令参数及用法详解(linux显示开机信息命令)](http://blog.csdn.net/zhongyhc/article/details/8909905)----[http://blog.csdn.net/zhongyhc/article/details/8909905](http://blog.csdn.net/zhongyhc/article/details/8909905)
dmesg | grep tty* 可以查看到有几个串口，
执行效果如图:
[root@localhost ~]# dmesg | grep tty*
Kernel command line: ro root=LABEL=/1 rhgb quiet console=tty1 console=ttyS1,1920
0n8
console [tty1] enabled
console [ttyS1] enabled
Setting up standard PCI resources
Error attaching device data
Error attaching device data
Error attaching device data
Error attaching device data
PCI: Setting latency timer of device 0000:00:1c.0 to 64
PCI: Setting latency timer of device 0000:00:1e.0 to 64
PCI: Setting latency timer of device 0000:00:1c.0 to 64
serial8250: ttyS0 at I/O 0x3f8 (irq = 4) is a 16550A
serial8250: ttyS1 at I/O 0x2f8 (irq = 3) is a 16550A
serial8250: ttyS2 at I/O 0x3e8 (irq = 11) is a 16550A
serial8250: ttyS3 at I/O 0x2e8 (irq = 10) is a 16550A
00:06: ttyS0 at I/O 0x3f8 (irq = 4) is a 16550A
00:07: ttyS1 at I/O 0x2f8 (irq = 3) is a 16550A
00:09: ttyS2 at I/O 0x3e8 (irq = 11) is a 16550A
00:0a: ttyS3 at I/O 0x2e8 (irq = 10) is a 16550A
input: Macintosh mouse button emulation as /class/input/input0
hash
 matches device tty46
PCI: Setting latency timer of device 0000:00:1d.7 to 64
PCI: Setting latency timer of device 0000:00:1d.0 to 64
PCI: Setting latency timer of device 0000:00:1d.1 to 64
PCI: Setting latency timer of device 0000:00:1d.2 to 64
PCI: Setting latency timer of device 0000:00:1d.3 to 64
PCI: Setting latency timer of device 0000:00:1f.1 to 64
PCI: Setting latency timer of device 0000:00:1f.2 to 64
sd 2:0:0:0: [sda] Attached SCSI disk
SELinux: Setting
 up existing superblocks.
SELinux: initialized (dev sda1, type ext3), uses xattr
sd 2:0:0:0: Attached scsi generic sg0 type 0
PCI: Setting latency timer of device 0000:01:00.0 to 64
This program comes with ABSOLUTELY NO WARRANTY; for details, please see <http:/
/www.gnu.org/licenses/>.
This is free software, and you are welcome to redistribute it under certain con
ditions; see <[url]http://www.gnu.org/licenses/[/url]>.
input: Power Button (FF) as /class/input/input1
ACPI: Power Button (FF) [PWRF]
input: Power Button (CM) as /class/input/input2
ACPI: Power Button (CM) [PWRB]
PCI: Setting latency timer of device 0000:00:1b.0 to 64
[root@localhost ~]#
