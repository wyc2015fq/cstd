# Android native crash log分析 - xqhrs232的专栏 - CSDN博客
2011年04月22日 11:23:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4781标签：[android																[crash																[python																[signal																[file																[timer](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=crash&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://wiki.impjq.net/doku.php?id=android:android-jni:android-ndk-stacktrace-analyzer](http://wiki.impjq.net/doku.php?id=android:android-jni:android-ndk-stacktrace-analyzer)
找到两种方法一种是使用google 的[stack工具](http://source.android.com/porting/debugging_native.html)，一种是使用[Analyze
 android-ndk stack trace](http://code.google.com/p/android-ndk-stacktrace-analyzer/). 
参考： 
- 
[http://code.google.com/p/android-ndk-stacktrace-analyzer/wiki/Usage](http://code.google.com/p/android-ndk-stacktrace-analyzer/wiki/Usage)
- 
[http://source.android.com/porting/debugging_native.html](http://source.android.com/porting/debugging_native.html)
stack下载： 
- 
[http://bootloader.wdfiles.com/local--files/linux%3Aandroid%3Acrashlog/stack](http://bootloader.wdfiles.com/local--files/linux%3Aandroid%3Acrashlog/stack)
android-ndk-stacktrace-analyzer用法 
android-ndk/android-ndk-1.6_r1/build/prebuilt/linux-x86/arm-eabi-4.2.1/bin/arm-eabi-objdump -S mylib.so > mylib.asm
python parse_stack.py mylib.asm logcat.txt
举例: 
D:/stack>python parse_stack.py libslpi.asm logcat.txt
0x000b7e80:            pjsua_acc_unregister + 0x0068
D:/stack>ls
libslpi.asm  libslpi.so  logcat.txt  parse_stack.py  stack.py
stack用法: 
python stack.py logcat.txt
说明:stack看起来是google内部使用的工具，在windows上跑不起来，需要在Linux环境下，还有使用的时候会提示so文件的位置不存在，按照它的要求，建立相应的目录，并放入so文件就可以了。举例:
root@impjq:~/stack# python stack.py logcat.txt
make: build/core/envsetup.mk: No such file or directory
make: *** No rule to make target `build/core/envsetup.mk'.
Searching for native crashes in logcat.txt
Reading symbols from
pid: 4538, tid: 4553  >>> com.arcsoft.ddtui <<<
signal 11 (SIGSEGV), fault addr deadbaad
  r0 00000000  r1 afd14679  r2 00000027  r3 00000070
  r4 afd42328  r5 00000000  r6 00000000  r7 00000cb0
  r8 80fac890  r9 485d8d8c  10 485d8ce4  fp 0000033c
  ip 00001770  sp 485d8880  lr deadbaad  pc afd11cd0
arm-eabi-addr2line: '/system/lib/libc.so': No such file
arm-eabi-addr2line: '/system/lib/libc.so': No such file
arm-eabi-addr2line: '/system/lib/libc.so': No such file
arm-eabi-addr2line: '/system/lib/libc.so': No such file
arm-eabi-addr2line: '/system/lib/libc.so': No such file
arm-eabi-addr2line: '/system/lib/libc.so': No such file
arm-eabi-addr2line: '/system/lib/libc.so': No such file
arm-eabi-addr2line: '/system/lib/libc.so': No such file
arm-eabi-addr2line: '/system/lib/libc.so': No such file
arm-eabi-addr2line: '/system/lib/libc.so': No such file
arm-eabi-addr2line: '/system/lib/libc.so': No such file
arm-eabi-addr2line: '/system/lib/libc.so': No such file
arm-eabi-addr2line: '/system/lib/libc.so': No such file
Stack Trace:
  ADDR      FUNCTION              FILE:LINE
  00011cd0  (unknown)             (unknown)
  00018fac  (unknown)             (unknown)
  000b7e80  pjsua_acc_unregister  ??:0
Stack Data:
  ADDR      VALUE     FILE:LINE/FUNCTION
  485d8840  00000340
  485d8844  afd146a9  (unknown)
                      (unknown)
  485d8848  afd42498  (unknown)
                      (unknown)
  485d884c  afd43bb4  (unknown)
                      (unknown)
  485d8850  00000000
  485d8854  afd156c3  (unknown)
                      (unknown)
  485d8858  afd14679  (unknown)
                      (unknown)
  485d885c  afd14679  (unknown)
                      (unknown)
  485d8860  00000070
  485d8864  afd42328  (unknown)
                      (unknown)
  485d8868  00000000
  485d886c  485d8894
  485d8870  00000cb0
  485d8874  afd1491b  (unknown)
                      (unknown)
  485d8878  df002777
  485d887c  e3a070ad
  485d8880  485d8df4
  485d8884  80fad540
  485d8888  00000cb0
  485d888c  afd14c81  (unknown)
                      (unknown)
  485d8890  afd4254c  (unknown)
                      (unknown)
  485d8894  fffffbdf
  485d8898  00000002
  485d889c  485d8df4
  485d88a0  80fad540
  485d88a4  afd18fb1  (unknown)
                      (unknown)
  485d88a8  00000946
  485d88ac  00000946
  485d88b0  80f5b018  ??:0
                      ??
  485d88b4  80eb7e84  ??:0
                      pjsua_acc_unregister
指定路径: 
root@impjq:~/stack# python stack.py  --symbols-dir=. logcat.txt
这样会提示某些lib不存在 
root@impjq:~/stack# python stack.py  --symbols-dir=. logcat.txt
make: build/core/envsetup.mk: No such file or directory
make: *** No rule to make target `build/core/envsetup.mk'.  Stop.
Searching for native crashes in logcat.txt
Reading symbols from .
pid: 4538, tid: 4553  >>> com.arcsoft.ddtui <<<
signal 11 (SIGSEGV), fault addr deadbaad
  r0 00000000  r1 afd14679  r2 00000027  r3 00000070
  r4 afd42328  r5 00000000  r6 00000000  r7 00000cb0
  r8 80fac890  r9 485d8d8c  10 485d8ce4  fp 0000033c
  ip 00001770  sp 485d8880  lr deadbaad  pc afd11cd0
arm-eabi-addr2line: './system/lib/libc.so': No such file
arm-eabi-addr2line: './system/lib/libc.so': No such file
arm-eabi-addr2line: './data/data/com.arcsoft.ddtui/lib/libslpi.so': No such file
arm-eabi-addr2line: './system/lib/libc.so': No such file
arm-eabi-addr2line: './system/lib/libc.so': No such file
arm-eabi-addr2line: './system/lib/libc.so': No such file
arm-eabi-addr2line: './system/lib/libc.so': No such file
arm-eabi-addr2line: './system/lib/libc.so': No such file
arm-eabi-addr2line: './system/lib/libc.so': No such file
arm-eabi-addr2line: './system/lib/libc.so': No such file
arm-eabi-addr2line: './system/lib/libc.so': No such file
arm-eabi-addr2line: './system/lib/libc.so': No such file
arm-eabi-addr2line: './system/lib/libc.so': No such file
arm-eabi-addr2line: './system/lib/libc.so': No such file
arm-eabi-addr2line: './data/data/com.arcsoft.ddtui/lib/libslpi.so': No such file
arm-eabi-addr2line: './data/data/com.arcsoft.ddtui/lib/libslpi.so': No such file
按照它的提示，建立相应的目录就可以了，其中libc.so我是直接从ndk里拷贝过来的，这样正常的运行结果如下： 
root@impjq:~/stack# python stack.py  --symbols-dir=. logcat.txt
make: build/core/envsetup.mk: No such file or directory
make: *** No rule to make target `build/core/envsetup.mk'.  Stop.
Searching for native crashes in logcat.txt
Reading symbols from .
pid: 4538, tid: 4553  >>> com.arcsoft.ddtui <<<
signal 11 (SIGSEGV), fault addr deadbaad
  r0 00000000  r1 afd14679  r2 00000027  r3 00000070
  r4 afd42328  r5 00000000  r6 00000000  r7 00000cb0
  r8 80fac890  r9 485d8d8c  10 485d8ce4  fp 0000033c
  ip 00001770  sp 485d8880  lr deadbaad  pc afd11cd0
Stack Trace:
  ADDR      FUNCTION              FILE:LINE
  00011cd0  pclose                /usr/local/google/home/digit/android/main/cupc
ake/android/bionic/libc/unistd/popen.c:146
  00018fac  strtod                /opt/digit/android/main/cupcake/android/bionic
/libc/stdlib/strtod.c:1327
  000b7e80  pjsua_acc_unregister  ??:0
Stack Data:
  ADDR      VALUE     FILE:LINE/FUNCTION
  485d8840  00000340
  485d8844  afd146a9  /usr/local/google/home/digit/android/main/cupcake/android/
bionic/libc/stdio/setvbuf.c:84
                      setvbuf
  485d8848  afd42498  libgcc2.c:0
                      __timer_table
  485d884c  afd43bb4  libgcc2.c:0
                      __timer_table
  485d8850  00000000
  485d8854  afd156c3  /usr/local/google/home/digit/android/main/cupcake/android/
bionic/libc/stdio/vfprintf.c:396
                      vfprintf
  485d8858  afd14679  /usr/local/google/home/digit/android/main/cupcake/android/
bionic/libc/stdio/setvbuf.c:67
                      setvbuf
  485d885c  afd14679  /usr/local/google/home/digit/android/main/cupcake/android/
bionic/libc/stdio/setvbuf.c:67
                      setvbuf
  485d8860  00000070
  485d8864  afd42328  libgcc2.c:0
                      __timer_table
  485d8868  00000000
  485d886c  485d8894
  485d8870  00000cb0
  485d8874  afd1491b  /usr/local/google/home/digit/android/main/cupcake/android/
bionic/libc/stdio/tempnam.c:50
                      tempnam
  485d8878  df002777
  485d887c  e3a070ad
  485d8880  485d8df4
  485d8884  80fad540
  485d8888  00000cb0
  485d888c  afd14c81  /usr/local/google/home/digit/android/main/cupcake/android/
bionic/libc/stdio/ungetc.c:139
                      ungetc
  485d8890  afd4254c  libgcc2.c:0
                      __timer_table
  485d8894  fffffbdf
  485d8898  00000002
  485d889c  485d8df4
  485d88a0  80fad540
  485d88a4  afd18fb1  /opt/digit/android/main/cupcake/android/bionic/libc/stdlib
/strtod.c:1327
                      strtod
  485d88a8  00000946
  485d88ac  00000946
  485d88b0  80f5b018  ??:0
                      ??
  485d88b4  80eb7e84  ??:0
                      pjsua_acc_unregister

