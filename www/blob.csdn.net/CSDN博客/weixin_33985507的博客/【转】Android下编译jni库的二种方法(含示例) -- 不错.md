# 【转】Android下编译jni库的二种方法(含示例) -- 不错 - weixin_33985507的博客 - CSDN博客
2015年06月06日 08:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
原文网址：http://blog.sina.com.cn/s/blog_3e3fcadd01011384.html
总结如下：两种方法是：
1）使用Android源码中的Make系统
2）使用NDK（从NDK r5开始）
---------------------------------
源码要求： 如果Android工程的顶层目录为my_android_project_name：
1） my_android_project_name/jni/Android.mk文件的示例内容如下（以生成libjni_mouse.so的C代码为例）：
  LOCAL_PATH:= $(call my-dir)
  include $(CLEAR_VARS)
  LOCAL_PRELINK_MODULE := false
  LOCAL_MODULE := libjni_mouse
  LOCAL_SRC_FILES := jni_mouse.c
  LOCAL_C_INCLUDES := $(JNI_H_INCLUDE)
  LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib  -llog
  LOCAL_SHARED_LIBRARIES := libcutils
  include $(BUILD_SHARED_LIBRARY)
2）源文件的部分内容(仅供参考)：
  #include <stdio.h>
  #include <unistd.h>
  #include <errno.h>
  #include <linux/input.h>
  #include <linux/uinput.h>
  #include <jni.h>                    // located in $JDK/include
  #include <cutils/log.h>         // for Android Make system: $Android_SRC/system/core/include/cutils/log.h
  //#include <android/log.h>   // for NDK compiler
  #define LOG_DBG(tag,fmt,args...)  __android_log_print(ANDROID_LOG_DEBUG,tag,fmt,##args)
  #define LOG_ERR(tag,fmt,args...)  __android_log_print(ANDROID_LOG_ERROR,tag,fmt,##args)
  #ifdef ___DEBUG
    #define DBG(fmt, args...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
  #else
    #define DBG(fmt, args...)
  #endif //___DEBUG
  #define OUT(fmt, args...)    __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)
  #define JNIREG_CLASS            "com/my_organization/my_pkg/JniMouse"
  static JNINativeMethod JniMouseMethods[] = {
     { "nativeOpenMouse",            "()I",                   (void*) openMouse },
     { "nativeCloseMouse",           "()V",                   (void*) closeMouse },
     { "nativeMoveCursor",           "(II)V",                 (void*) moveCursor },
     ...
  };
  static int registerNativeMethods(JNIEnv* env, const char* className, JNINativeMethod* methods,
                                                       int   numMethods)
  {   
    jclass clazz;
    // C++ way:  clazz= env->FindClass(className);
    clazz = (*env)->FindClass(env, className);
    if (clazz == NULL) {
        LOG_ERR(TAG, "Native registration error: unable to find class\n");
        return JNI_FALSE;
    }
    // C++ way:  if (env->RegisterNatives(clazz, methods, numMethods) < 0)
    if ((*env)->RegisterNatives(env, clazz, methods, numMethods) < 0) {
        LOG_ERR(TAG, "RegisterNatives failed\n");
        return JNI_FALSE;
    }
    return JNI_TRUE;
  }
  static int registerNatives(JNIEnv* env)
  {
    if (!registerNativeMethods(env, JNIREG_CLASS, JniMouseMethods,
                 sizeof(JniMouseMethods) / sizeof(JniMouseMethods[0])))
        return JNI_FALSE;
    return JNI_TRUE;
  }
  jint JNI_OnLoad(JavaVM* vm, void* reserved)
  {
   JNIEnv* env = NULL;
   jint result = -1;
    // this is for C++: if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
    if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOG_ERR(TAG, "ERROR: GetEnv failed\n");
        goto bail;
    }
    assert(env != NULL);
    if (!registerNatives(env)) {
        LOG_ERR(TAG, "ERROR: jni_mouse native registration failed\n");
        goto bail;
    }
    result = JNI_VERSION_1_4;
  bail:
    return result;
  }
---------------------------------
编译过程具体如下：
1）使用Android源码中的Make系统:
$  cd   $ANDROID_SRC    # (e.g. /u_data/android/sourcecode/and22)
$  .      build/envsetup.sh
# 这样就会生成许多shell工具，实际上是shell函数。
$  lunch  MY_LUNCH_NUMBER    # 如果你的项目需要用到PLATFORM相关的内容（共享库/java库等）
# 把Android工程的顶层目录符号链接（或者拷贝）到 $ANDROID_SRC/packages/apps，如
$  ln  -fs  /abs/path/to/my_android_project_name  $ANDROID_SRC/packages/apps
$  mmm  packages/apps/my_android_project_name
# 如果没有错误，就会生成以下文件（取决于你使用的PRODUCT名字）：
#       $ANDROID_SRC/out/target/product/$PRODUCT_NAME/system/lib/libjni_mouse.so
# 最后拷贝生成的库文件到Android工程的libs/armeabi/目录下：
$  mkdir -p $ANDROID_SRC/packages/apps/my_android_project_name/libs/armeabi
$  cp  $ANDROID_SRC/out/target/product/$PRODUCT_NAME/system/lib/libjni_mouse.so   \ 
          $ANDROID_SRC/packages/apps/my_android_project_name/libs/armeabi
#  这样，在eclipse中正常编译/生成.apk文件时，就会把生成的jni库自动包含进.apk文件中，这可以使用unzip命令来验证：
#      cp my.apk /tmp/tmp/my.zip; cd /tmp/tmp; unzip my.zip; ls -l  lib/armeabi/lib*.*
2）使用NDK（from NDK r5）
从NDK r5开始，NDK正式作为独立的工具来编译jni的代码，但它往往缺少某些头文件，如<linux/uinput.h>。
0）准备环境变量，如：
   $ export  NDK=/u_data/android/sourcecode/android-ndk-r6b
   $ export        CC=$NDK/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/arm-linux-androideabi-gcc
   $ export  STRIP=$NDK/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/arm-linux-androideabi-strip
   $ export  SYSROOT=$NDK/platforms/android-8/arch-arm   # 要看你使用的目标Android的版本 :)
1） 对于缺少的头文件，可以借用PC上的x86下的同名文件，如：
    cd   $NDK/platforms/android-8/arch-arm/usr/include/linux
    ln   -fs   ../../../../arch-x86/usr/include/linux/uinput.h   .
2） 编译出可执行文件（如果有main函数），如：
   $   $CC --sysroot=$SYSROOT   -o  my_jni_mouse_exe    jni_mouse.c   my_main_func.c
   #注： '--sysroot=dir'：Use dir as the logical root directory for headers and libraries.
3） 编译出共享库（如果没有main函数），如：
   $   $CC --sysroot=$SYSROOT -fPIC     -o     jni_mouse.o     -c  jni_mouse.c        # optional '-o ...'
   $   $CC --sysroot=$SYSROOT -shared -o libjni_mouse.so        jni_mouse.o
   # 再strip一下，以便减少其大小（直接覆盖原来的库文件）：
   $   $STRIP    libjni_mouse.so
4）(可选)链接如上刚生成的共享库：
   $   $CC --sysroot=$SYSROOT -L. -ljni_mouse  -o  my_hello  my_hello.c
<EOF>
