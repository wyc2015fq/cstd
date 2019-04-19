# 将自己的程序永久写入Android的/system/bin的方法 - xqhrs232的专栏 - CSDN博客
2011年03月16日 14:23:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1981
原文地址::[http://hi.baidu.com/mcu99/blog/item/954a511085280509203f2e3a.html](http://hi.baidu.com/mcu99/blog/item/954a511085280509203f2e3a.html)
Android模拟器运行之后，/system目录即为只读属性。如果想把自己的程序传到该目录下运行，就会发现无法成功。当然，使用adb remount命令可以暂时去除它的只读限制，可以向里面传文件，但是一旦重启Android模拟器，再次使用adb shell进入模拟器终端时就会发现，自己传进去的文件在重启之后被清除掉了。
当然，你可以把文件传到/data文件夹下，这个文件夹不用remount就可写，而且重启模拟器之后自己的文件也不会被清空。但是如果我想做一个较为底层的程序，或者开机启动的程序，每次开机在/data下启动总是显得怪怪的。最好能让自己编写的程序也能享受到Android自带的那些程序的地位，每次运行在system/bin目录下。这可以通过把自己的程序代码加在Android源码中，然后重新make的方法实现。
首先根据你工程的性质，在Android源码的相应位置建立一个文件夹。比如我的这个程序如果和硬件有关的话，可以在hardware下建立一个叫做my_hardware的文件夹，然后把自己的程序源码放在里面，比如叫hard.c。
再给这个程序写一个makefile文件，好让make的时候可以自动找到你的程序并对它进行编译。起名字叫做Android.mk，这个名字不能随便起，否则make不认识。把这个Android.mk和hard.c都放在my_hardware下面。
# Android.mk文件内容举例
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= /
    hard.c
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE:= myhard
include $(BUILD_EXECUTABLE)
做好这些之后，回到总的Android源码目录下，然后执行make，如果你之前已经make过了，那么这个过程会很快，几分钟吧。因为和上一次make相比，源码改动很少，只是加了1个文件夹和2个文件而已。如果你这是第一次make，那么会比较慢，或许大约要1－2个小时，速度应该也和机器配置有关。
make成功之后，运行emulator模拟器，用adb shell进入模拟器终端，cd /system/bin，就可以找到你的程序了（注意名字是myhard，不是hard，也不是my_hardware，这里产生的程序名字由上面程序中红色字体的那一行控制）。而且再次启动之后也不会丢失。
