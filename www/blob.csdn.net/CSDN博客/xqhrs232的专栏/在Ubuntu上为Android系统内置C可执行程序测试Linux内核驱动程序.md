# 在Ubuntu上为Android系统内置C可执行程序测试Linux内核驱动程序 - xqhrs232的专栏 - CSDN博客
2012年03月05日 09:32:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：449标签：[linux内核																[android																[ubuntu																[c																[测试																[frameworks](https://so.csdn.net/so/search/s.do?q=frameworks&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/luoshengyang/article/details/6571210](http://blog.csdn.net/luoshengyang/article/details/6571210)
 在前一篇文章中，我们介绍了如何在Ubuntu上为Android系统编写Linux内核驱动程序。在这个名为hello的Linux内核驱动程序中，创建三个不同的文件节点来供用户空间访问，分别是传统的设备文件/dev/hello、proc系统文件/proc/hello和devfs系统属性文件/sys/class/hello/hello/val。进一步，还通过cat命令来直接访问/proc/hello和/sys/class/hello/hello/val文件来，以验证驱动程序的正确性。在这一篇文章里，我们将通过自己编写的C可执行程序来访问设备文件/dev/hello。可能读者会觉得奇怪，怎么能在Android系统中用C语言来编写应用程序呢？Android系统上的应用程序不都是Java应用程序吗？其实是可以的，读者不妨用adb
 shell命令连上Android模拟器，在/system/bin目录下可以看到很多C可执行程序，如cat命令。今天，我们就来学习一下怎么在Android系统中添加用C语言编写的可执行程序吧。
      一. 参照[在Ubuntu上为Android系统编写Linux内核驱动程序](http://blog.csdn.net/luoshengyang/article/details/6568411)一文，准备好Linux驱动程序。使用Android模拟器加载包含这个Linux驱动程序的内核文件，并且使用adb shell命令连接上模拟，验证在/dev目录中存在设备文件hello。
      二. 进入到Android源代码工程的external目录，创建hello目录:
**USER-NAME@MACHINE-NAME:~/Android$ cd external**
**      USER-NAME@MACHINE-NAME:~/Android/external$ mkdir hello**
**      三. **在hello目录中新建hello.c文件：
**[cpp]**[view plain](http://blog.csdn.net/luoshengyang/article/details/6571210#)[copy](http://blog.csdn.net/luoshengyang/article/details/6571210#)[print](http://blog.csdn.net/luoshengyang/article/details/6571210#)[?](http://blog.csdn.net/luoshengyang/article/details/6571210#)
- #include <stdio.h> 
- #include <stdlib.h> 
- #include <fcntl.h> 
- #define DEVICE_NAME "/dev/hello" 
- int main(int argc, char** argv)  
- {  
- int fd = -1;  
- int val = 0;  
-     fd = open(DEVICE_NAME, O_RDWR);  
- if(fd == -1) {  
-         printf("Failed to open device %s./n", DEVICE_NAME);  
- return -1;  
-     }  
- 
-     printf("Read original value:/n");  
-     read(fd, &val, sizeof(val));  
-     printf("%d./n/n", val);  
-     val = 5;  
-     printf("Write value %d to %s./n/n", val, DEVICE_NAME);  
-         write(fd, &val, sizeof(val));  
- 
-     printf("Read the value again:/n");  
-         read(fd, &val, sizeof(val));  
-         printf("%d./n/n", val);  
-     close(fd);  
- return 0;  
- }  
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#define DEVICE_NAME "/dev/hello"
int main(int argc, char** argv)
{
	int fd = -1;
	int val = 0;
	fd = open(DEVICE_NAME, O_RDWR);
	if(fd == -1) {
		printf("Failed to open device %s./n", DEVICE_NAME);
		return -1;
	}
	
	printf("Read original value:/n");
	read(fd, &val, sizeof(val));
	printf("%d./n/n", val);
	val = 5;
	printf("Write value %d to %s./n/n", val, DEVICE_NAME);
        write(fd, &val, sizeof(val));
	
	printf("Read the value again:/n");
        read(fd, &val, sizeof(val));
        printf("%d./n/n", val);
	close(fd);
	return 0;
}
这个程序的作用中，打开/dev/hello文件，然后先读出/dev/hello文件中的值，接着写入值5到/dev/hello中去，最后再次读出/dev/hello文件中的值，看看是否是我们刚才写入的值5。从/dev/hello文件读写的值实际上就是我们虚拟的硬件的寄存器val的值。
四. 在hello目录中新建Android.mk文件：
**      LOCAL_PATH := $(call my-dir)**
**      include $(CLEAR_VARS)**
**      LOCAL_MODULE_TAGS := optional**
**      LOCAL_MODULE := hello**
**      LOCAL_SRC_FILES := $(call all-subdir-c-files)**
**      include $(BUILD_EXECUTABLE)**
注意，BUILD_EXECUTABLE表示我们要编译的是可执行程序。
五. 参照[如何单独编译Android源代码中的模块](http://blog.csdn.net/luoshengyang/article/details/6566662)一文，使用mmm命令进行编译：
**USER-NAME@MACHINE-NAME:~/Android$ mmm ./external/hello**
编译成功后，就可以在out/target/product/gerneric/system/bin目录下，看到可执行文件hello了。
      六. 重新打包Android系统文件system.img：
**USER-NAME@MACHINE-NAME:~/Android$ make snod**
这样，重新打包后的system.img文件就包含刚才编译好的hello可执行文件了。
      七. 运行模拟器，使用/system/bin/hello可执行程序来访问Linux内核驱动程序：
**USER-NAME@MACHINE-NAME:~/Android$ emulator -kernel ./kernel/common/arch/arm/boot/zImage &**
**      USER-NAME@MACHINE-NAME:~/Android$ adb shell**
**      root@android:/ # cd system/bin**
**      root@android:/system/bin # ./hello**
**      Read the original value:**
**      0.**
**      Write value 5 to /dev/hello.**
**      Read the value again:**
**      5.**
看到这个结果，就说我们编写的C可执行程序可以访问我们编写的Linux内核驱动程序了。
      介绍完了如何使用C语言编写的可执行程序来访问我们的Linux内核驱动程序，读者可能会问，能不能在Android的Application Frameworks提供Java接口来访问Linux内核驱动程序呢？可以的，接下来的几篇文章中，我们将介绍如何在Android的Application Frameworks中，增加Java接口来访问Linux内核驱动程序，敬请期待。
