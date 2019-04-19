# Debug Native c/c++ Application for Android(Step by Step) - xqhrs232的专栏 - CSDN博客
2012年05月29日 13:35:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：918标签：[android																[application																[character																[tcp																[tree																[shell](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=tree&t=blog)](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)](https://so.csdn.net/so/search/s.do?q=character&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.cppblog.com/luonjtu/archive/2009/02/19/74310.html](http://www.cppblog.com/luonjtu/archive/2009/02/19/74310.html)
 调试Android上的c/c++程序一直是个难题，以前我经常靠输出log来解决问题，对于稍复杂一些的工程，这几乎是个不可能完成的任务，尤其有些错误，在wincewindows下都没事，只在android上出现，就更难找了。后来看了些资料，知道可以用gdbserver来调试，今天决定必须把这个先弄清楚，不然以后干活效率实在是太低了，找了很多网站，终于成功了。这里把整个过程整理一下，以备以后查阅。
    1. 准备gdbserver。
    android 1.0 代码刚开放到时候，里面并没有带gdbserver，有些强人就自己编译了gdbserver来使用。不过现在好了，android的新源码里已经包含了gdbserver，就在prebuilt目录下。如果想在android 1.0里使用，可以到如下地址下载：[http://android.git.kernel.org/?p=platform/prebuilt.git;a=tree](http://android.git.kernel.org/?p=platform/prebuilt.git;a=tree)。gdbserver的二进制文件就在android-arm/gdbserver/gdbserver，我们只需要把gdbserver这个可执行文件放到模拟器上即可。
    准备把它放在/system/bin，这样它就在默认的PATH里了。启动模拟器后:
    adb push gdbserver /system/bin
    如果提示文件系统不可写的话，执行一下“adb remount”，因为默认是用只读模式挂载的。
    2. 编译程序。
    默认情况下，android的编译系统在编译程序时已经使用了“-g”选项，即已经生成了调试信息。但是在生成最终的文件时，是经过strip的，去除了所有到调试信息。所以最终我们到调试目标要使用strip之前的文件。用make showcommands查看详细的命令信息，下面是其中某次的输出：
target Executable: libomstts (out/target/product/generic/obj/EXECUTABLES/libomstts_intermediates/LINKED/libomstts)
prebuilt/linux-x86/toolchain/arm-eabi-4.2.1/bin/arm-eabi-g++ -nostdlib -Bdynamic -Wl,-T,build/core/armelf.x -Wl,-dynamic-linker,/system/bin/linker -Wl,--gc-sections -Wl,-z,nocopyreloc -o out/target/product/generic/obj/EXECUTABLES/libomstts_intermediates/LINKED/libomstts
 -Lout/target/product/generic/obj/lib -Wl,-rpath-link=out/target/product/generic/obj/lib -llog -lcutils -lutils -landroid_runtime -lnativehelper -lstdc++ -lz -lc -lstdc++ -lm  out/target/product/generic/obj/lib/crtbegin_dynamic.o         out/target/product/generic/obj/EXECUTABLES/libomstts_intermediates/src/tts.o
 out/target/product/generic/obj/EXECUTABLES/libomstts_intermediates/src/TTSWrapper.o out/target/product/generic/obj/lib/crtend_android.o
target Non-prelinked: libomstts (out/target/product/generic/symbols/system/bin/libomstts)
out/host/linux-x86/bin/acp -fpt out/target/product/generic/obj/EXECUTABLES/libomstts_intermediates/LINKED/libomstts out/target/product/generic/symbols/system/bin/libomstts
target Strip: libomstts (out/target/product/generic/obj/EXECUTABLES/libomstts_intermediates/libomstts)
out/host/linux-x86/bin/soslim --strip --shady --quiet out/target/product/generic/symbols/system/bin/libomstts --outfile out/target/product/generic/obj/EXECUTABLES/libomstts_intermediates/libomstts
Install: out/target/product/generic/system/bin/libomstts
out/host/linux-x86/bin/acp -fpt out/target/product/generic/obj/EXECUTABLES/libomstts_intermediates/libomstts out/target/product/generic/system/bin/libomstts
    生成的可执行文件是libomstts，可以看到，初次链接的目标文件是“out/target/product/generic/obj/EXECUTABLES/libomstts_intermediates/LINKED/libomstts”，然后拷贝到“out/target/product/generic/symbols/system/bin/libomstts”，strip后的文件是“out/target/product/generic/obj/EXECUTABLES/libomstts_intermediates/libomstts”和“out/target/product/generic/system/bin/libomstts”。调试只能使用前两个文件。
    把带调试信息到可执行文件放到模拟器上，我用到是“out/target/product/generic/symbols/system/bin/libomstts”:
    adb push out/target/product/generic/symbols/system/bin/libomstts /system/bin
    3. 启动调试器
    首先在模拟器上启动gdbserver:
    adb shell
    进入模拟器的控制台后
    gdbserver 10.0.2.2:1234 /system/bin/libomstts
    10.0.2.2是模拟器的默认ip地址，让gdbserver在模拟器上监听1234端口。如果启动成功会显示以下信息:
Process /system/bin/libomstts created; pid = 1025
Listening on port 1234
    为来让gdb能连接到模拟器上到gdbserver，必须进行数据转发：
telnet localhost 5554
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
Android Console: type 'help' for a list of commands
OK
redir add tcp:1234:1234
OK
exit
    上面的telnet localhost 5554，redir add tcp:1234:1234，exit是自己输入的命令，其他的是输出信息。5554是模拟器控制台的监听端口，这些命令是将所有到localhost:1234的数据转发到模拟器的1234端口。
    最后在本机启动gdb:
    arm-eabi-gdb out/target/product/generic/symbols/system/bin/libomstts
    arm-eabi-gdb是android自带的toolchain里的，注意后面的可执行文件是strip之前的。
    gdb启动后，在gdb里输入命令连接gdbserver：
    target remote localhost:1234
    连接到gdbserver成功后，就可以使用所有的gdb调试命令啦
    现在的这个gdbserver还不能调试动态链接库，只能先编译成可执行文件调试。
