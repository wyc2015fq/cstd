# gdb：加载共享库的符号表 solib-search-path - xqhrs232的专栏 - CSDN博客
2016年10月18日 23:19:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4322
原文地址::[http://blog.chinaunix.net/uid-29065085-id-4703912.html](http://blog.chinaunix.net/uid-29065085-id-4703912.html)
相关文章
1、set solib-search-path command----[http://visualgdb.com/gdbreference/commands/set_solib-search-path](http://visualgdb.com/gdbreference/commands/set_solib-search-path)
# 原文网址：[http://visualgdb.com/gdbreference/commands/set_solib-search-path](http://visualgdb.com/gdbreference/commands/set_solib-search-path)
# set solib-search-path command
Specifies directories where GDB will search for shared libraries with symbols. This option is useful when debugging with gdbserver.
#### Syntax
set solib-search-path [Directories]
show solib-search-path
#### Parameters
Directories Specifies the list of directories separated by colon (:) on Linux or semicolon (;) on Windows where GDB will look for shared libraries when searching for symbols.
#### Typical use
This command is useful when debugging remote programs via gdbserver. If the shared library path on the remote computer and the GDB computer is different, GDB won't automatically find the local copy of the library and load its symbols unless the directory containing
 it is specified in set solib-search-path.
E.g. if you have copied /home/testuser/libtest/libTest.so on the computer with GDB to /tmp/libTest.so on the computer with GDBServer, you will need to specify set solib-search-path /home/testuser/libtest in
 order to get the symbols loaded.
#### Default value
The default value for the solib-search-path variable is "." that corresponds to the working directory of GDB (directory where GDB was launched unless changed using the cd command).
#### Examples
In this example we will debug a simple shared library with gdbserver:
#include
int func()
{
    printf("In func()\n");
return 0;
}
We will use a simple program to test our library:
#include
int func();
int main()
{
    printf("In main()\n");
    func();
return 0;
}
First, we build the application and the library and deploy it to another machine:
cd /home/testuser/libtest
g++ -ggdb -fPIC -shared lib.cpp -o libTest.so
g++ -ggdb main.cpp libTest.so -o testApp -Wl,--rpath='$ORIGIN'
scp testApp libTest.so deploy_machine:/tmp
Then we run gdbserver on the deploy_machine machine:
cd /tmp
gdbserver :2000 testApp
Finally we run GDB from a different directory (otherwise it will still find libTest.so):
cd /
gdb /home/testuser/libtest/testApp
Now we will try to set a breakpoint inside libTest.so (that will fail as /tmp/libTest.so is not initially loaded). Note how specifying set solib-search-path allows GDB load the symbols and set
 a breakpoint:
(gdb) target remote deploy_machine:2000
Remote debugging using deploy_machine:2000
Reading symbols from /lib/ld-linux.so.2...
Reading symbols from /usr/lib/debug/lib/i386-linux-gnu/ld-2.15.so...done.
done.
Loaded symbols for /lib/ld-linux.so.2
(gdb) break main
Breakpoint 1 at 0x80484ed: file main.cpp, line 7.
(gdb) continue
Continuing.
Breakpoint 1, main () at main.cpp:7
7 printf("In main()\n");
(gdb) break test
Function "test" not defined.
Make breakpoint pending on future shared library load? (y or [n]) n
(gdb) info sharedlibrary 
warning: Could not load shared library symbols for /tmp/libTest.so.
Do you need "set solib-search-path" or "set sysroot"?
From To Syms Read Shared Object Library
0x0019b820 0x001b3b9f Yes /lib/ld-linux.so.2
No /tmp/libTest.so
0x004bcf10 0x005f15cc No /lib/i386-linux-gnu/libc.so.6
(gdb) set solib-search-path /home/testuser/libtest
Reading symbols from /home/testuser/libtest/libTest.so...done.
Loaded symbols for /home/testuser/libtest/libTest.so
Reading symbols from /lib/i386-linux-gnu/libc.so.6...
Reading symbols from /usr/lib/debug/lib/i386-linux-gnu/libc-2.15.so...done.
done.
Loaded symbols for /lib/i386-linux-gnu/libc.so.6
(gdb) info sharedlibrary 
From To Syms Read Shared Object Library
0x0019b820 0x001b3b9f Yes /lib/ld-linux.so.2
0x00f893a0 0x00f894c8 Yes /home/testuser/libtest/libTest.so
0x004bcf10 0x005f15cc Yes /lib/i386-linux-gnu/libc.so.6
(gdb) break func
Breakpoint 2 at 0xf8946e: file lib.cpp, line 5.
(gdb) continue
Continuing.
Breakpoint 2, func () at lib.cpp:5
5 printf("In func()\n");
(gdb) backtrace 
#0 func () at lib.cpp:5
#1 0x080484fe in main () at main.cpp:8
#### Compatibility with VisualGDB
VisualGDB automatically configures search paths using the set solib-search-path command when you debug your Linux or Android projects with Visual Studio. Nonetheless you can issue the command
 manually as well using the GDB Session window.
