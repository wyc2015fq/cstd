# GCC -R -L - Koma Hub - CSDN博客
2019年02月10日 18:26:06[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：87
```bash
cc test.c -L/home/linden -R/home/linden -lfruit
```
The -L/home/linden -R/home/linden options tell the linker in which directories to look for libraries at **linktime **and at **runtime**, respectively.
The compiler option -Lpathname is used to tell the linker a list of other directories in which to search for libraries that have been specified with the -l option. There are a couple of environment variables, **LD_LIBRARY_PATH **and **LD_RUN_PATH**, that can also be used to provide this information. Using these environment variables is now officially frowned on, for reasons of security, performance, and build/execute independence. Use the -Lpathname -Rpathname options at linktime instead.
|#include Filename|Library Pathname|Compiler Option to Use|
|----|----|----|
|<math.h>|/usr/lib/libm.so|-lm|
|<math.h>|/usr/lib/libm.a|-dn -lm|
|<stdio.h>|/usr/lib/libc.so|linked in automatically|
|"/usr/openwin/include/X11.h"|/usr/openwin/lib/libX11.so|-L/usr/openwin/lib			-lX11|
|<thread.h>|/usr/lib/libthread.so|-lthread|
|<curses.h>|/usr/ccs/lib/libcurses.a|-lcurses|
|<sys/socket.h>|/usr/lib/libsocket.so|-lsocket|
