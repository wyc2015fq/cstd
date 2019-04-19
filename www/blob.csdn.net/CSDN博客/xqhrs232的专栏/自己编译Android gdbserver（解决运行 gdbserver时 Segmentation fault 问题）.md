# 自己编译Android gdbserver（解决运行 gdbserver时 Segmentation fault 问题） - xqhrs232的专栏 - CSDN博客
2012年04月24日 22:23:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：991标签：[android																[build																[linker																[object																[header																[file](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=header&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=linker&t=blog)](https://so.csdn.net/so/search/s.do?q=build&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/yuleslie/article/details/7226767](http://blog.csdn.net/yuleslie/article/details/7226767)

一、引言
        使用Android 预装 gdbserver 调试 动态库时出现，Segmentation fault错误。不得不使用现有编译链重新编译一份gdbserver。多谢[Android
 - How-to Rebuild gdbserver](http://www.omappedia.org/wiki/Android_-_How-to_Rebuild_gdbserver)这篇文章，参考这篇文章我的解决过程如下。
        这里要强调一点：**下面编译sysroot时sysroot的目标路径，和编译gdbserver时指定的sysroot路径，一定要使用[绝对路径]**。我编译过程中，曾因未使用绝对路径浪费不少精力。
二、开发环境
Ubuntu 11.04 
编译通过的Android 2.3 源码
三、解决过程
**1、因已经有了Android 2.3源码，省去下载源码并编译的漫长过程。**
#设置Android源码目录为环境变量：$ANDROID_SRC
simba@simba-Vostro-3400:~$ export ANDROID_SRC=/home/simba/neptune/android_2.3
**2、下载gdb源码，并打补丁**
simba@simba-Vostro-3400:~$ mkdir gdb_build
simba@simba-Vostro-3400:~/neptune/gdb_src$ git clone https://android.googlesource.com/toolchain/gdb.git
#坐等下载完毕，然后...
#将以下保存成.patch文件
**[plain]**[view
 plain](http://blog.csdn.net/yuleslie/article/details/7226767#)[copy](http://blog.csdn.net/yuleslie/article/details/7226767#)
- <span style="font-size:13px;"> diff --git a/gdb-7.1.x/gdb/gdbserver/Makefile.in b/gdb-7.1.x/gdb/gdbserver/Makefile.in  
-  index 5bf82e2..488bfb6 100644  
-  --- a/gdb-7.1.x/gdb/gdbserver/Makefile.in  
-  +++ b/gdb-7.1.x/gdb/gdbserver/Makefile.in  
-  @@ -176,13 +176,13 @@ clean-info:  
- 
-   gdbserver$(EXEEXT): $(OBS) ${ADD_DEPS} ${CDEPS}  
-     rm -f gdbserver$(EXEEXT)  
-  -  ${CC-LD} $(INTERNAL_CFLAGS) $(INTERNAL_LDFLAGS) -o gdbserver$(EXEEXT) $(OBS) \  
-  -    $(GDBSERVER_LIBS) $(XM_CLIBS)  
-  +  ${CC-LD} $(INTERNAL_CFLAGS) -o gdbserver$(EXEEXT) $(OBS) \  
-  +    $(GDBSERVER_LIBS) $(XM_CLIBS) $(INTERNAL_LDFLAGS)  
- 
-   gdbreplay$(EXEEXT): $(GDBREPLAY_OBS)  
-     rm -f gdbreplay$(EXEEXT)  
-  -  ${CC-LD} $(INTERNAL_CFLAGS) $(INTERNAL_LDFLAGS) -o gdbreplay$(EXEEXT) $(GDBREPLAY_OBS) \  
-  -    $(XM_CLIBS)  
-  +  ${CC-LD} $(INTERNAL_CFLAGS) -o gdbreplay$(EXEEXT) $(GDBREPLAY_OBS) \  
-  +    $(XM_CLIBS) $(INTERNAL_LDFLAGS)  
- 
-   # Put the proper machine-specific files first, so M-. on a machine  
-   # specific routine gets the one for the correct machine.  
-  diff --git a/gdb-7.1.x/gdb/gdbserver/linux-arm-low.c b/gdb-7.1.x/gdb/gdbserver/linux-arm-low.c  
-  index 54668f8..7a78cce 100644  
-  --- a/gdb-7.1.x/gdb/gdbserver/linux-arm-low.c  
-  +++ b/gdb-7.1.x/gdb/gdbserver/linux-arm-low.c  
-  @@ -43,10 +43,7 @@ void init_registers_arm_with_neon (void);  
-   # define PTRACE_SETWMMXREGS 19  
-   #endif  
- 
-  -#ifndef PTRACE_GETVFPREGS  
-  -# define PTRACE_GETVFPREGS 27  
-   # define PTRACE_SETVFPREGS 28  
-  -#endif  
- 
-   static unsigned long arm_hwcap;</span>  
#打补丁
simba@simba-Vostro-3400:~/gdb_build$ cd gdb/
simba@simba-Vostro-3400:~/gdb_build/gdb$ patch -p1 <./gdb_patch.patch
**3、下载build源码**
simba@simba-Vostro-3400:~/gdb_build$ git clone https://android.googlesource.com/toolchain/build.git
#坐等下载完毕，然后...
#编译 sysroot
simba@simba-Vostro-3400:~/neptune/build_src$ cd build/
simba@simba-Vostro-3400:~/gdb_build/build$  ./build-sysroot.sh  $ANDROID_SRC/out/target/product/imx51_bbg/**$HOME/gdb_build/sysroot**
#注意此处sysroot的目标目录一定要使用**绝对路径**，否则sysroot不能正常生成，之后编译gdbserver时会报错。
**4.修改编译脚本**
#备份并修改build-gdbserver.sh
 simba@simba-Vostro-3400:~/gdb_build/build$ cd ..
simba@simba-Vostro-3400:~/gdb_build$ cp $ANDROID_SRC/ndk/build/tools/build-gdbserver.sh $ANDROID_SRC/ndk/build/tools/build-gdbserver.sh_back
simba@simba-Vostro-3400:~/gdb_build$ gedit $ANDROID_SRC/ndk/build/tools/build-gdbserver.sh
#以下红字部分为添加内容，目的是注释掉部分代码
**<<NOT_NEEDED**# Remove libthread_db to ensure we use exactly the one we want.
rm -f $BUILD_SYSROOT/usr/lib/libthread_db*
rm -f $BUILD_SYSROOT/usr/include/thread_db.h
if [ "$NOTHREADS" != "yes" ] ; then
    # We're going to rebuild libthread_db.o from its source
    # that is under sources/android/libthread_db and place its header
    # and object file into the build sysroot.
    LIBTHREAD_DB_DIR=$ANDROID_NDK_ROOT/sources/android/libthread_db/gdb-$GDB_VERSION
    if [ ! -d "$LIBTHREAD_DB_DIR" ] ; then
        dump "ERROR: Missing directory: $LIBTHREAD_DB_DIR"
        exit 1
    fi
    # Small trick, to avoid calling ar, we store the single object file
    # with an .a suffix. The linker will handle that seamlessly.
    run cp $LIBTHREAD_DB_DIR/thread_db.h $BUILD_SYSROOT/usr/include/
    run $TOOLCHAIN_PREFIX-gcc --sysroot=$BUILD_SYSROOT -o $BUILD_SYSROOT/usr/lib/libthread_db.a -c $LIBTHREAD_DB_DIR/libthread_db.c
    if [ $? != 0 ] ; then
        dump "ERROR: Could not compile libthread_db.c!"
        exit 1
    fi
fi
**NOT_NEEDED**
#备份并修改prebuilt-common.sh 
simba@simba-Vostro-3400:~/gdb_build$ cp $ANDROID_SRC/ndk/build/tools/prebuilt-common.sh $ANDROID_SRC/ndk/build/tools/prebuilt-common.sh_back
simba@simba-Vostro-3400:~/gdb_build$ gedit $ANDROID_SRC/ndk/build/tools/prebuilt-common.sh
将
 get_toolchain_install ()
 {
     echo "$1/toolchains/$TOOLCHAIN/prebuilt/$HOST_TAG"
 }
修改为：
 get_toolchain_install ()
 {
     echo "$1/prebuilt/$HOST_TAG/toolchain/$TOOLCHAIN"
 }
**5.编译gdbserver**
simba@simba-Vostro-3400:~/gdb_build$ $ANDROID_SRC/ndk/build/tools/build-gdbserver.sh $HOME/gdb_build $ANDROID_SRC arm-eabi-4.4.3 --verbose --build-out=$HOME/gdb_build/install --gdb-version=7.1.x --sysroot=**$HOME/gdb_build/sysroot**
#注意此处一定要是用**绝对路径**指定sysroot的目录
**6.目标文件位于~/gdb_build/install目录下**
**7.参考文章**
[Android - How-to Rebuild gdbserver](http://www.omappedia.org/wiki/Android_-_How-to_Rebuild_gdbserver)
[再次在 cygwin 下编译 android toolchain](http://hi.baidu.com/oopsware/blog/item/50a24950a3833d7585352491.html)
[Android build gdbserver](http://blog.csdn.net/yuleslie/article/details/7226605)

