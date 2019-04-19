# 利用BLCR加快android的启动过程 - xqhrs232的专栏 - CSDN博客
2012年02月29日 15:58:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：734标签：[android																[path																[module																[callback																[jni																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/shuaiff/article/details/5894646](http://blog.csdn.net/shuaiff/article/details/5894646)
**利用BLCR加快android的启动过程**
** 作者：帅文  **
**摘要**：介绍了利用blcr对android启动速度进行优化的原理、实施步骤和注意点，在虚拟机上验证获得缩短10秒以上启动时间。
**关键词**：blcr  android  启动速度   zygote   类加载
**引言** 随着google的android操作系统在手机、平板电脑等领域大量使用，android的启动速度慢也成为许多使用者抱怨的缺点。相比手机这类平时较少开关机的设备，平板电脑在实际使用中开关的频率相对频繁，开机速度是影响客户感受的一个重要因素。Android启动过程分为linux内核载入，文件系统挂接，zygote进程启动和软件包扫描几个主要过程。其中耗时大户发生在zygote的类载入和软件包扫描过程两处，只要减少这两处的时间，启动速度就会发生明显的改变。如何有效加快启动速度是众多android产品制造者都感兴趣的技术。本文就缩短android
 启动过程中公用类加载部分的一种技术做介绍，利用该方法，在虚拟机实际测试获得良好的结果。
**背景知识** Zygote是android中的核心进程，其负责android其他应用和服务的孵化，zygote启动过程慢的一个原因在于
启动过程中需要提前加载公用类（由文件preloaded-classes定义），这种加载是android设计人员特意根据linux和嵌入式系统特性设计的，网络上有人尝试将这些类的加载去除以加快开机速度，这种违背设计者初衷的方法被否决（参考1）。对于单个进程而言，如果使用到这些公共类，都必须完成对应类的载入并初始化，由于zygote是后续所有android的父进程，采用Class.forName处理的公共类会被载入到内存并完成静态初始化，提前加载可以避免每个子进程调用时候需要生成公用类的副本（linux Copy-on-write特点），进而减少内存占用量以及后续启动其他程序的花销。这个加载过程关系到后续的性能，所以不能简单的跳过。载入的过程主要是对内存的操作过程，其中包括了大量的内存分配释放过程，该过程由于有上千个类需要操作而变得耗时较长。在实际应用中，由于framework部分较少升级，故这些公共类是不会被动态删减，考虑到这些特点，采用check
 point方式每次直接将zygote还原到完成类加载的阶段避开频繁的类操作显然可以提高速度。
BLCR (BerkeleyLab Checkpoint/Restart)是应用于linux下的check point/restore软件，它可以将正在运行于linux上的应用当前的运行点保存成为一个文件并且在以后的时间可以按照需要将该程序直接恢复到保存时候的状态。该软件官方网站在：[http://ftg.lbl.gov/checkpoint](http://ftg.lbl.gov/checkpoint)。根据其FAQ介绍BLCR对保存和恢复的程序有部分限制：1 无法保存和恢复打开的sockets（TCP/IP,Unix
 domain等）2 无法保存和恢复Sys V IPC对象。3 在处理还没有回收僵尸子进程的父进程是需要注意。尽管有部分限制，只要在使用时候注意这些限制，利用其保存和恢复程序运行点和内存的能力完全可以避免程序每次不必要的初始化动作。Zygote在加载公共类前时候除了堆外其他资源使用很少，通过仔细的调整/恢复BLCR无法恢复的一些状态，是可以用check point形式来跳过类加载以缩短启动时间。 BLCR 包括内核驱动和应用库两部分，外部应用通过链接应用库和内核驱动进行交换实现应用的保存和恢复。下面就针对emulator下的android2.2介绍实现blcr的具体过程。
**实施过称**
**一 软件准备**：
blcr-0.8.2
android2.2froyo
android-goldfish-2.6.29
[http://sourceforge.net/projects/android-dfb/](http://sourceforge.net/projects/android-dfb/)pthread的补丁
编译android和内核在这里不详述，需要指出的有两点。
1 编译内核时候注意加入可加载模块支持（Enable loadablemodule support），缺省的goldfish内核配置是不支持的。
2 需要对android的bionic的线程库进行扩展，扩展方法是采用上述软件的pthread文件替换相应线程库文件。
**二 编译blcr的内核驱动模块和应用**直接使用android内建的编译器即可对内核驱动进行编译。编译过程只要指定正确的编译器路径和内核路径即可顺利编译。生成的内核可加载模块分别是：
cr_module/kbuild/blcr.ko
blcr_imports/kbuild/blcr_imports.ko
顺利编译blcr应用部分需要使用打过扩展补丁的pthread。补丁方法为直接取代bionic目录下对应文件，并且修改bionic/libc/Android.mk,开启对应的编译开关，在libc_common_cflags加入-DUCLIBC_LINUXTHREAD_OLD_EXTENSTION宏。
Blcr文件也需要如下修改才能正常编译和运行。
其中文件libcr/cr_libinit.c:
rc = __cri_ksigaction(signum, (act ?&ksa : NULL), (oact ? &oksa : NULL), (_NSIG/8), &errno);改动为：
rc = __cri_ksigaction(signum, (act ?&ksa : NULL), (oact ? &oksa : NULL), (_NSIG/4), &errno);
将_NSIG/8改动为_NSIG/4修改的原因为在bionic中定义NSIG=32,而内核为64，造成kernel/signal.c中rt_sigaction调用认为参数错误而返回错误。在android系统中对应非prelink的动态库调用dlopen(NULL, RTLD_LAZY);（即查找自己）会异常，需要屏蔽cr_libinit.c函数cri_init对dlopen的调用。
在blcr工程中加入Android.mk，需要注意的是下面几个文件需要编译为arm而非缺省的thumb指令：cr_async.c.  cr_core.c cr_sig_sync.c  cr_cs.c  cr_syscall.c，否则编译无法通过。
** 三  zygote加入checkpoint 支持** 当系统启动zygote服务时候先判断是否存在checkpoint文件，如果有则调用cr_restart载入保存的checkpoint文件，否则按照正常的zygote流程进行。由于zygote是其他android的父进程，其生成的许多进程/线程都有socket等checkpoint无法恢复的限制因素，故不适合将checkpoint放到zygote启动过后点，本文选择在ZygoteInit.java的main后面的preloadResources()完成后进行。这个过程刚好加载完毕耗时长的公用类而且基本没有使用很多限制资源。原生的android在preload前有创建socket动作，可以调整到preloadResources后面，调整后的样子如下（斜体代码为调整顺序部分）：
CheckPoint cp=newCheckPoint();
try {
   //Start profiling the zygote initialization.
SamplingProfilerIntegration.start();
  preloadClasses();
  preloadResources();
  cp.checkPoint("/data/zygote.blcr");
***registerZygoteSocket();***
其中CheckPoint 类是c扩展的java调用接口，通过jni调用checkpoint库函数实现checkpoint动作，下文列出了jni部分内容。该部分仅为一个参考模板，产品化过程应该做更多异常处理和合理化调整工作。
android_blcr_checkpoint.cpp
#defineLOG_TAG "BLCR"
namespaceandroid {
staticint my_callback(void* arg)
{
   int rc;
    LOGV(__FUNCTION__);
    rc = cr_checkpoint(0);
 return 0;
}
staticvoid checkPoint(JNIEnv* env, jobject object,jstring file) {
    LOGV(__FUNCTION__);
     pid_t my_pid;
      int rc,fd;
    struct stat s;
   cr_checkpoint_handle_t my_handle;
     cr_callback_id_t cb_id;
     FILE *f;
     const jchar* str =env->GetStringCritical(file, 0);
    String8 filename;
    if (str) {
        filename = String8(str,env->GetStringLength(file));
        env->ReleaseStringCritical(file,str);
        }
        else {
               LOGE("checkPoint,file nameis null");
                    return;
            }
   LOGI("checkPoint,filename=%s",filename);
   //does the file exist?
    f = fopen(filename, "r");
     if(f==NULL) //create and save checkpointto it.
       {
        my_pid = cr_init();
          if(my_pid<0)
                 {
                  LOGE("cr_initfailed,return:%d",my_pid);
                         return;
                 }
        cb_id =cr_register_callback(my_callback, NULL, CR_SIGNAL_CONTEXT);
        if (cb_id < 0) 
                 {
              LOGV("cr_register_callback() unexpectedly returned %d/n",cb_id);
              return;
            } 
       else 
           {
          LOGV("cr_register_callback() correctly returned %d/n", cb_id);
           }
      /* Request a checkpoint of ourself */
     fd =crut_checkpoint_request(&my_handle, filename);
    if (fd < 0) 
          {
         LOGE("crut_checkpoint_request() unexpectedly returned 0x%x/n",fd);
         return ;
          }
    rc = stat(filename, &s);
    if (rc) {
       LOGE("stat() unexpectedly returned%d/n", rc);
       return;
    } else {
       LOGV("stat(context.%d) correctlyreturned 0/n", my_pid);
    }
    if (s.st_size == 0) {
       LOGE("context file unexpectedlyempty/n");
       return;
    } else {
       LOGV("context.%d isnon-empty/n", my_pid);
    }
    /* Reap the checkpoint request */
    rc = crut_checkpoint_wait(&my_handle,fd);
    if (rc < 0) {
       LOGE("crut_checkpoint_wait() #1unexpectedly returned 0x%x/n", rc);
       return;
    }    }}
/*
 * JNI registration.
 */
staticJNINativeMethod gMethods[] = {
    /* name, signature, funcPtr */
       {"checkPoint","(Ljava/lang/String;)V", (void *)checkPoint},
};
intregister_android_blcr_checkpoint(JNIEnv* env)
{
    return jniRegisterNativeMethods(env,"android/blcr/CheckPoint",
        gMethods, NELEM(gMethods));
}
}
为了正常使用上面的jni接口，需要把注册函数注册到frameworks/base/core/jni/ AndroidRuntime.cpp，在gRegJNI[]中加入REG_JNI(register_android_blcr_checkpoint)。同时创建CheckPoint.java 文件提供给java调用：
packageandroid.blcr;
publicclass CheckPoint {
    public CheckPoint() {
    }
    public native void checkPoint(String fileName);
}
正常启动blcr还需要修改init.rc，调整启动zygote的方式。修改点包括载人blcr内核驱动：
insmod/system/lib/modules/blcr_imports.ko
insmod/system/lib/modules/blcr.ko
和启动blcr服务方式，将
servicezygote /system/bin/app_process -Xzygote /system/bin --zygote--start-system-server
替换为：service zygote/system/bin/quick.sh -Xzygote /system/bin --zygote --start-system-server
其中quick.sh脚本内容为：
#!/system/bin/busybox sh
ec()
{
 /system/bin/busybox echo $* >/dev/console;
}
if [ -f /data/zygote.blcr ]; then 
  ec"***************load saved zygote********************" 
 /system/bin/cr_restart -f /data/zygote.blcr ;
else 
  ec"optimised by blcr" 
  ec"**************start a new zygote******************"
  ec"you can contact the author through Email:shuaiwen@yahoo.com.cn"
  ec"this is only an original demon version"
  ec"release pid:$$."
 /system/bin/cr_checkpoint
  ec"release pid:$$."
 /system/bin/cr_checkpoint
  ec"release pid:$$."
 /system/bin/app_process  $*
fi
需要指出的是运行上面几个不带参数的cr_checkpoint目的为将后面的zygote的pid退后一些，以防下次checkpoint时候遇到pid冲突而导致checkpoint失败。
加入该功能后当系统首次启动会创建/data/zygote.blcr文件，由于需要checkpoint，导致比正常启动时间慢较多。后续只要该文件存在，系统就会跨过类加载过程，进而加快启动速度。
**4 测试验证**
下表列出了虚拟机加入blcr前和blcr后的10次的启动速度。
Emulator启动参数如下：
emulator.exe  -kernel zImage -show-kernel -partition-size200 -memory 200 -skindir ./skins -skin WQVGA432 -shell -sysdir ./ -datauserdata.img -ramdisk ramdisk.img -system system.img -sdcard sdcard.img  -sdcard sdcard.img
|**模式**|**启动时间（单位：秒）**| | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|
|采用blcr|42|43|42|36|42|41|42|38|39|43|
|正常启动|64|63|50|57|65|54|52|45|53|49|
启用blcr后平均启动时间为41，相比正常启动平均时间55秒快了14秒。启动时间的改善是明显的。
参考1：[http://osdir.com/ml/android-platform/2009-07/msg00276.html](http://osdir.com/ml/android-platform/2009-07/msg00276.html)
附部分android.mk文件
cat ./libcr/Android.mk
# Copyright 2006 The Android Open Source Project
LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := libcutils
LOCAL_SHARED_LIBRARIES += libdl
LOCAL_SHARED_LIBRARIES += libc
lib_SOURCES = /
 cr_omit.c 
LOCAL_SRC_FILES:= $(lib_SOURCES)
LOCAL_C_INCLUDES := /
$(KERNEL_HEADERS) /
$(LOCAL_PATH)/../include /
$(LOCAL_PATH)/../    /
      $(LOCAL_PATH)/arch/arm/ 
LOCAL_CFLAGS := -DHAVE_CONFIG_H -DSHUAIWEN
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE := libcr_omit
include $(BUILD_SHARED_LIBRARY)
#libcr_run.so
include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := libcutils
LOCAL_SHARED_LIBRARIES += libdl
LOCAL_SHARED_LIBRARIES += libc
lib_SOURCES = /
             cr_run.c 
LOCAL_SRC_FILES:= $(lib_SOURCES)
LOCAL_C_INCLUDES := /
$(KERNEL_HEADERS) /
$(LOCAL_PATH)/../include /
$(LOCAL_PATH)/../    /
      $(LOCAL_PATH)/arch/arm/ 
LOCAL_CFLAGS := -DHAVE_CONFIG_H -DSHUAIWEN
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE := libcr_run
include $(BUILD_SHARED_LIBRARY)
#libcr
include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := libcutils
LOCAL_SHARED_LIBRARIES += libdl
LOCAL_SHARED_LIBRARIES += libc
lib_SOURCES = /
              cr_async.c.arm /
              cr_trace.c /
              cr_core.c.arm  /
              cr_sig_sync.c.arm /
              cr_cs.c.arm  /
              cr_pthread.c /
              cr_strerror.c  /
              cr_request.c /
              cr_syscall.c.arm /
 cr_omit.c   /
              cr_run.c 
LOCAL_SRC_FILES:= $(lib_SOURCES)
LOCAL_C_INCLUDES := /
$(KERNEL_HEADERS) /
$(LOCAL_PATH)/../include /
$(LOCAL_PATH)/../    /
      $(LOCAL_PATH)/arch/arm/ 
LOCAL_CFLAGS := -DHAVE_CONFIG_H  -DSHUAIWEN -g
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE := libcr
include $(BUILD_SHARED_LIBRARY)
cat ./util/cr_restart/Android.mk
LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := libcutils
LOCAL_SHARED_LIBRARIES += libdl
LOCAL_SHARED_LIBRARIES += libc
LOCAL_SHARED_LIBRARIES += libcr_run
LOCAL_SHARED_LIBRARIES += libcr
LOCAL_SRC_FILES:=cr_restart.c
LOCAL_C_INCLUDES := /
$(KERNEL_HEADERS) /
$(LOCAL_PATH)/../../include /
      $(LOCAL_PATH)/arch/arm/ 
LOCAL_MODULE := cr_restart
include $(BUILD_EXECUTABLE)
cat ./util/cr_restart/Android.mk
LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := libcutils
LOCAL_SHARED_LIBRARIES += libdl
LOCAL_SHARED_LIBRARIES += libc
LOCAL_SHARED_LIBRARIES += libcr_run
LOCAL_SHARED_LIBRARIES += libcr
LOCAL_SRC_FILES:=cr_restart.c
LOCAL_C_INCLUDES := /
$(KERNEL_HEADERS) /
$(LOCAL_PATH)/../../include /
      $(LOCAL_PATH)/arch/arm/ 
LOCAL_MODULE := cr_restart
include $(BUILD_EXECUTABLE)
[root@wen blcr-0.8.2]# 
[root@wen blcr-0.8.2]# 
[root@wen blcr-0.8.2]# cat ./util/cr_checkpoint/Android.mk
LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := libcutils
LOCAL_SHARED_LIBRARIES += libdl
LOCAL_SHARED_LIBRARIES += libc
LOCAL_SHARED_LIBRARIES += libcr_run
LOCAL_SHARED_LIBRARIES += libcr
LOCAL_SRC_FILES:=cr_checkpoint.c
LOCAL_C_INCLUDES := /
$(KERNEL_HEADERS) /
$(LOCAL_PATH)/../../include /
      $(LOCAL_PATH)/arch/arm/ 
LOCAL_MODULE := cr_checkpoint
include $(BUILD_EXECUTABLE)
下面是crut_util_libcr.c，放到上面同一个目录下面。你也可以自己从blcr中复制出来。
/*
 * Berkeley Lab Checkpoint/Restart (BLCR) for Linux is Copyright (c)
 * 2008, The Regents of the University of California, through Lawrence
 * Berkeley National Laboratory (subject to receipt of any required
 * approvals from the U.S. Dept. of Energy).  All rights reserved.
 *
 * Portions may be copyrighted by others, as may be noted in specific
 * copyright notices within specific files.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id: crut_util_libcr.c,v 1.5 2008/12/26 10:50:35 phargrov Exp $
 *
 * Utility functions for BLCR tests (libcr-dependent portions)
 */
#define _LARGEFILE64_SOURCE 1   /* For O_LARGEFILE */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "crut_util.h"
/* checkpoint request/poll wrappers for simpler code */
int
crut_checkpoint_request(cr_checkpoint_handle_t *handle_p, const char *filename) {
    int rc;
    cr_checkpoint_args_t my_args;
    if (filename) {
        /* remove existing context file, if any */
        (void)unlink(filename);
        /* open the context file */
        rc = open(filename, O_WRONLY | O_CREAT | O_TRUNC | O_LARGEFILE, 0600);
    } else {
/* NULL -> /dev/null */
        rc = open("/dev/null", O_WRONLY | O_LARGEFILE);
    }
    if (rc < 0) {
        perror("open");
        return rc;
    }
    cr_initialize_checkpoint_args_t(&my_args);
    my_args.cr_fd = rc; /* still holds the return from open() */
    my_args.cr_scope = CR_SCOPE_PROC;
    /* issue the request */
    rc = cr_request_checkpoint(&my_args, handle_p);
    if (rc < 0) {
        (void)close(my_args.cr_fd);
        if (filename) (void)unlink(filename);
        perror("cr_request_checkpoint");
        return rc;
    }
    return my_args.cr_fd;
}
int
crut_checkpoint_wait(cr_checkpoint_handle_t *handle_p, int fd) {
    int rc, save_err;
    do {
        rc = cr_poll_checkpoint(handle_p, NULL);
        if (rc < 0) {
            if ((rc == CR_POLL_CHKPT_ERR_POST) && (errno == CR_ERESTARTED)) {
                /* restarting -- not an error */
                rc = 1; /* Signify RESTART to caller */
            } else if (errno == EINTR) {
                /* poll was interrupted by a signal -- retry */
continue;
            } else {
                /* return the error to caller */
                break;
            }
        } else if (rc == 0) {
            fprintf(stderr, "cr_poll_checkpoint returned unexpected 0/n");
            rc = -1;
            goto out;
        } else {
            rc = 0; /* Signify CONTINUE to caller */
}
    } while (rc < 0);
    save_err = errno;
#if 0 // Nothing in the testsuite needs this, but your APP might want it.
    (void)fsync(fd);
#endif
    (void)close(fd);
    errno = save_err;
out:
    return rc;
}
int
crut_checkpoint_block(const char *filename) {
    cr_checkpoint_handle_t my_handle;
    int ret, fd, save_err;
    fd = crut_checkpoint_request(&my_handle, filename);
    if (fd < 0) return fd;
    ret = crut_checkpoint_wait(&my_handle, fd);
    save_err = errno;
    (void)close(fd);
    errno = save_err;
    return ret;
}
 许多人问道关于编译内核模块的方法，这里我补充一下，比较简单的方法是建立一个脚本，比如叫run.sh,内容如下：
#!/bin/bash
curpath=`pwd`
#rm -rf `find ./ -name Makefile`
export PATH=$PATH:/mydroid/prebuilt/linux-x86/toolchain/arm-eabi-4.4.0/bin
TOOLCHAIN=/mydroid/prebuilt/linux-x86/toolchain/arm-eabi-4.4.0/bin
export ARCH=arm
./configure KCC=$TOOLCHAIN/arm-eabi-gcc  LD=$TOOLCHAIN/arm-eabi-ld --with-linux-src=$curpath/../../kernel-common --with-linux=$curpath/../../kernel-common  host_alias=arm-linux
cd blcr_imports/
make
echo generated kernel module:`ls kbuild/*.ko`
cd ../cr_module/
make
echo generated kernel module:`ls kbuild/*.ko`
上面部分内容请根据自己的情况调整一下路径，比如上面的内核路径这样的东西。
 全部工程我已经上传到csdn下载中，名称为：blcr-0.8.2-android   关键字：blcr   android   不需要资源分。
