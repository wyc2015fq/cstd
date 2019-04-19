# how could I intercept linux sys calls? - xqhrs232的专栏 - CSDN博客
2016年10月25日 16:49:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：573
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://stackoverflow.com/questions/69859/how-could-i-intercept-linux-sys-calls](http://stackoverflow.com/questions/69859/how-could-i-intercept-linux-sys-calls)

Why can't you / don't want to use the LD_PRELOAD trick?
Example code here:
```
/*
 * File: soft_atimes.c
 * Author: D.J. Capelis
 *
 * Compile:
 * gcc -fPIC -c -o soft_atimes.o soft_atimes.c
 * gcc -shared -o soft_atimes.so soft_atimes.o -ldl
 *
 * Use:
 * LD_PRELOAD="./soft_atimes.so" command
 *
 * Copyright 2007 Regents of the University of California
 */
#define _GNU_SOURCE
#include <dlfcn.h>
#define _FCNTL_H
#include <bits/fcntl.h>
extern int errorno;
int (*_open)(const char * pathname, int flags, ...);
int (*_open64)(const char * pathname, int flags, ...);
int open(const char * pathname, int flags, mode_t mode)
{
    _open = (int (*)(const char * pathname, int flags, ...)) dlsym(RTLD_NEXT, "open");
    if(flags & O_CREAT)
        return _open(pathname, flags | O_NOATIME, mode);
    else
        return _open(pathname, flags | O_NOATIME, 0);
}
int open64(const char * pathname, int flags, mode_t mode)
{
    _open64 = (int (*)(const char * pathname, int flags, ...)) dlsym(RTLD_NEXT, "open64");
    if(flags & O_CREAT)
        return _open64(pathname, flags | O_NOATIME, mode);
    else
        return _open64(pathname, flags | O_NOATIME, 0);
}
```
From what I understand... it is pretty much the LD_PRELOAD trick or a kernel module. There's not a whole lot of middle ground unless you want to run it under an emulator which can trap out to your function or do code re-writing on the actual binary to trap
 out to your function.
Assuming you can't modify the program and can't (or don't want to) modify the kernel, the LD_PRELOAD approach is the best one, assuming your application is fairly standard and isn't actually one that's maliciously trying to get past your interception. (In which
 case you will need one of the other techniques.)
