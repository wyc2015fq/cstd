
# Android kernel Crash后，定位出错点的方法 - 嵌入式Linux - CSDN博客

2014年12月29日 16:54:25[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：699


1. 将/prebuild/gcc/linux-x86/arm/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-gdb拷贝到/usr/local/bin下

2. 进入out/target/product/工程名xxx/obj/KERNEL_OBJ 目录，找到文件vmlinux

3. 执行arm-linux-androideabi-gdb vmlinux，进入gdb调试界面

4. 输入list*函数名+行号，会显示出错对应的文件和行号


