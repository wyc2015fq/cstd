# Android下用gdb远程调试 - xqhrs232的专栏 - CSDN博客
2012年05月03日 14:06:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：980
原文地址::[http://www.2cto.com/kf/201203/124820.html](http://www.2cto.com/kf/201203/124820.html)
Host: ubuntu
Target: Android ICS
1. 将gdbserver和gdbclient分别放入target和host。两者都可在android源代码的prebuilt目录下找到。
2. 在Target上启动gdbserver，在adb shell中执行
$ /path/to/gdbserver :1111--attach PID
其中PID为想要debug程序的进程号，可以通过ps得到。1111为端口号，可以自定义。
3. 在Host上启动gdbclient，首先将target上的库pull到host上（如/home/jzj/debug/gdb）
$cd /home/jzj/debug/gdb
$adb pull /system/lib
$adb pull /system/bin/app_process
然后写启动脚本init.gdb（这里假设程序要用到的库放在/home/jzj/debug/gdb/下，android的symbol放在/home/jzj/debug/ics/symbols/system/lib/下，android的[源码](http://www.2cto.com/ym)放在/home/jzj/workspace/ics下）
file /home/jzj/debug/gdb/app_process
set solib-absolute-prefix /home/jzj/debug/ics/symbols/
set solib-search-path /home/jzj/debug/ics/symbols/system/lib/:/home/jzj/debug/gdb
dir /home/jzj/workspace/ics
shell adb forward tcp:1111 tcp:1111
target remote :1111
最后执行
$ gdb -x init.gdb
就可以启动gdb了。一切正常的话这时候就可以用gdb的基本功能，如设断点或者查看内存了。这时有些so库的符号表可能还没载入，可以执行
(gdb) shared
载入这些符号表。
最后附个关于gdb设置脚本中两个变量的官方解释，一般solib-absolute-prefix先于solib-search-path查找。
solib-absolute-prefix ：
If this variable is set, path will be used as a prefix for any absolute shared library paths; many runtime loaders store the absolute paths to the shared library in the target program's memory. If you use `solib-absolute-prefix' to find shared libraries, they
 need to be laid out in the same way that they are on the target, with e.g. a `/usr/lib' hierarchy under path. You can set the default value of `solib-absolute-prefix' by using the configure-time `--with-sysroot' option.
solib-search-path ：
If this variable is set, path is a colon-separated list of directories to search for shared libraries. `solib-search-path' is used after `solib-absolute-prefix' fails to locate the library, or if the path to the library is relative instead of absolute. If you
 want to use `solib-search-path' instead of `solib-absolute-prefix', be sure to set `solib-absolute-prefix' to a nonexistant directory to prevent GDB from finding your host's libraries.
摘自  金卓军的博客@CSDN
