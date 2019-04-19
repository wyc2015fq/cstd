# ls -l字符设备 - Koma Hub - CSDN博客
2019年02月27日 21:03:52[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：12
个人分类：[Linux driver](https://blog.csdn.net/Rong_Toa/article/category/8645170)
字符设备通过文件系统中的名子来存取. 那些名子称为文件系统的特殊文件, 或者设备文件, 或者文件系统的简单结点; 惯例上它们位于 /dev 目录. 字符驱动的特殊文件由使用ls -l 的输出的第一列的"c"标识. 块设备也出现在 /dev 中, 但是它们由"b"标识. 
```
ls -l
crw-rw-rw- 1 root root 1, 3 Apr 11 2002 null
crw------- 1 root root 10, 1 Apr 11 2002 psaux
crw------- 1 root root 4, 1 Oct 28 03:04 tty1
crw-rw-rw- 1 root tty 4, 64 Apr 11 2002 ttys0
crw-rw---- 1 root uucp 4, 65 Apr 11 2002 ttyS1
crw--w---- 1 vcsa tty 7, 1 Apr 11 2002 vcs1
crw--w---- 1 vcsa tty 7,129 Apr 11 2002 vcsa1
crw-rw-rw- 1 root root 1, 5 Apr 11 2002 zero
```
