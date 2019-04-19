# LKMs：Loadable Kernel Modules - Koma Hub - CSDN博客
2019年02月28日 21:46:05[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：23
个人分类：[Linux driver](https://blog.csdn.net/Rong_Toa/article/category/8645170)
# Loadable Kernel Modules
- ● LKMs (Loadable Kernel Modules)
- ● Pre-compiled binary pieces
- ● Each piece is called “module”
- ● Can be loaded at runtime
- ● Extend the functionality of the system
- ● Enforce modularity
- ○ Easy to develop, debug and maintain
- ○ No need to rebuild the kernel
- ● Can save memory (load only the necessary)
# What are LKMs used for
- ● Everything that is not required in the core
- ● 6 main categories
- ○ Device drivers
- ○ File system drivers
- ■ Implementation of a specific file system
- ○ System calls
- ○ Network stack
- ■ Interprets a network protocol
- ○ TTY line disciplines
- ○ Executable interpreters for the supported formats
# Character Device Driver
- ● Read or Write a byte at a time
- ● Accessed by a stream of bytes
- ● Usually permit only sequential access
- ● Implement: open, close, read, write
- ● Similar to regular files
- ● Examples:
- ○ /dev/console
- ○ /dev/ttyS0
# Block Device Driver
- ● Read or Write block-size multiples
- ● Permit random access
- ● Accessed in the /dev/
- ● File systems can be mount on top
- ● Handle I/O operations
- ● Differ with the char module in the way the manage data inside the kernel
- ● Different interface to the kernel than char modules
# Network Drivers
- ● Handle any network transaction made
- ● Transfer packets of data
- ● Independent of a specific protocol
- ● Reception and Transmission instead of Read/Write
- ● Usually the interface is a hardware device but it can also be software like the loopback
- ○ loopback is used to communicate with the servers that run in the same node, debugging etc.
- ● They are not mapped to the file system; they are identified by a name
> 
Praktikum Kernel Programming
University of Hamburg
Scientific Computing
Winter semester 2014/2015
