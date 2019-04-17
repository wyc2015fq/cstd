# 在32位的ubuntu12.04 上编译android 2.3.4的错误及解决方法 - V__KING__的专栏 - CSDN博客





2013年10月24日 16:11:47[v__king__](https://me.csdn.net/V__KING__)阅读数：1200标签：[ubuntu																[linux																[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[android](https://blog.csdn.net/V__KING__/article/category/1704545)





1.问题：

vk@vk:~/android-2.3.4_r1$ make

============================================

PLATFORM_VERSION_CODENAME=REL

PLATFORM_VERSION=2.3.4 

TARGET_PRODUCT=generic

TARGET_BUILD_VARIANT=eng

TARGET_SIMULATOR=false 

TARGET_BUILD_TYPE=release 

TARGET_BUILD_APPS= 

TARGET_ARCH=arm

HOST_ARCH=x86 

HOST_OS=linux

 HOST_BUILD_TYPE=release

 BUILD_ID=GRJ22



============================================

 find: `frameworks/base/frameworks/base/docs/html': 没有那个文件或目录

 find: `out/target/common/docs/gen': 没有那个文件或目录

 find: `frameworks/base/frameworks/base/docs/html': 没有那个文件或目录

 find: `out/target/common/docs/gen': 没有那个文件或目录

 find: `frameworks/base/frameworks/base/docs/html': 没有那个文件或目录

 find: `out/target/common/docs/gen': 没有那个文件或目录

 find: `frameworks/base/frameworks/base/docs/html': 没有那个文件或目录

 find: `out/target/common/docs/gen': 没有那个文件或目录

 find: `frameworks/base/frameworks/base/docs/html': 没有那个文件或目录

 find: `out/target/common/docs/gen': 没有那个文件或目录

 Install: out/host/linux-x86/framework/apicheck.jar

 Install: out/host/linux-x86/framework/clearsilver.jar

 Install: out/host/linux-x86/framework/droiddoc.jar

 target Generated: libclearsilver-jni <= out/host/common/obj/JAVA_LIBRARIES/clearsilver_intermediates/javalib.jar

 /bin/bash: javah: 未找到命令

 make: *** [out/host/linux-x86/obj/SHARED_LIBRARIES/libclearsilver-jni_intermediates/org_clearsilver_HDF.h] 错误 127



解决：

sudo update-alternatives --install /usr/bin/javah javah /usr/lib/jvm/jdk1.6.0_45/bin/javah 1

update-alternatives --config javah



2.问题：

frameworks/base/libs/utils/RefBase.cpp: 在成员函数‘void android::RefBase::weakref_type::trackMe(bool, bool)’中:

frameworks/base/libs/utils/RefBase.cpp:483:67: 错误： 将‘const  android::RefBase::weakref_impl’作为‘void  android::RefBase::weakref_impl::trackMe(bool, bool)’的‘this’实参时丢弃了类型限定  [-fpermissive]

make: *** [out/host/linux-x86/obj/STATIC_LIBRARIES/libutils_intermediates/RefBase.o] 错误 1

原因：Ubuntu 11.10的gcc 4.6版本太高了，android编译需要gcc 4.4

解决办法：更换gcc、g++版本4.6为4.4

sudo apt-get install gcc-4.4

sudo apt-get install g++-4.4

                         sudo rm -rf /usr/bin/gcc /usr/bin/g++

                         sudo ln -s /usr/bin/gcc-4.4 /usr/bin/gcc

                         sudo ln -s /usr/bin/g++-4.4 /usr/bin/g++



3. 问题：

g++: 错误：–fpermissive：没有那个文件或目录

make: *** [out/host/linux-x86/obj/STATIC_LIBRARIES/libutils_intermediates/RefBase.o] 错误1

解决办法：sudo  apt-get install g++-4.4-multilib](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)




