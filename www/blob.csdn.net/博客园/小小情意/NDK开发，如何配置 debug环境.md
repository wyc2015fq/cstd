
# NDK开发，如何配置 debug环境 - 小小情意 - 博客园






# [NDK开发，如何配置 debug环境](https://www.cnblogs.com/xiaoxiaoqingyi/p/7143536.html)
刚开始做NDK 开发的时候，Android Studio 还没提供了 native C/C++ 设置断点 调试，我们都是通过输出 日志来调试，这样费时耗力。Android Studio 应该是在 2.2 版本才提供的设置断点 debug 功能，同时在该版本也提供了 cmake 编译。
我目前在做 NDK 开发的时候，还是习惯用 NDK-Build（也就是设置 Android.mk） 来开发，我先简单说一下怎么用输出日志来调试：
1、首先在 Android.mk 设置MODULE 添加日志库，如下代码：
include $(CLEAR_VARS)
LOCAL_MODULE :=ndktest
LOCAL_SRC_FILES :=native_lib.cLOCAL_LDLIBS+= -lloginclude $(BUILD_SHARED_LIBRARY)
2、然后在 native_lib.c 实现文件添加：
\#include <android/log.h>//为了方便调用，将输出宏定义\#defineLOG_TAG "NATIVE_OUTPUT"\#defineLOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))\#defineLOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
3、最后就能在 native_lib.c 实现文件的 方法里输出日志了，如：
LOGI("run here");
LOGI("run i=%d", i);
上述是比较繁琐的调试方法，每次写完日志，又要重新编译一下代码。现在叙述一下怎么搭建 native  C/C++ 断点调试方法：
1、首先要把你的 Android Studio 升级到 2.2 或更高，且在Module下 build.gradle 的buildToolsVersion 参数 要 设置 25 或以上： buildToolsVersion '25.0.0'
2、如果你是新创建项目， 你可以在New Project 时候， 在 Application name 选项下 有个复选框（Include C++ support） 选上，IDE 在构建项目的时候，会帮你配置好了 debug 环境。
3、如果你是从旧项目移植过来的（也就是以前用 Android.mk配置），你可以打开 Module下 build.gradle文件
defaultConfig节点中，添加ndk，如下：
ndk {
     moduleName"imagetool"abiFilters"armeabi"//编译支持的平台//.. 根据自己的 Application.mk 的配置，按 gradle 规则配置一下}
然后在Android 的节点中，添加 externalNativeBuild 节点：
externalNativeBuild {
    ndkBuild {//Android.mk 的相对路径path"src/main/jni/Android.mk"}
}
接着 sync Project 同步一下项目，你会看见项目多了几个文件夹：
![](https://images2015.cnblogs.com/blog/804587/201707/804587-20170709223402165-1929662562.png)
当你点击 Run Debug 按钮 右边 的Attach debugger to Android process  按钮，如果有 Native、Dual 选择：
![](https://images2015.cnblogs.com/blog/804587/201707/804587-20170709223434884-683095772.png)

当你点击 Run Debug 按钮 右边 的Attach debugger to Android process  按钮，如果有 Native、Dual 选择：
说明你配置成功了， 可以在 native-lib.c 实现文件 设置断点来调试了。 上述截图 弹框 是debuger 配置，默认是 auto 选项，你可以设置 成 native 用来debug 你的natvie 代码。
debug 到 native 的时候如下：
![](https://images2015.cnblogs.com/blog/804587/201707/804587-20170709223511962-1532551956.png)
你可以使用 LLDB 作为你的Debug 辅助工具， LLDB怎么使用，这一章不细说，你可以查阅其它资料。
参考资料：
[https://developer.android.google.cn/studio/debug/index.html](https://developer.android.google.cn/studio/debug/index.html)
[https://developer.android.google.cn/index.html](https://developer.android.google.cn/index.html)（Android 大陆官网）
tips：假设你已经配置好了NDK开发环境，以及 LLDB 插件。
在 debug的时候如下出现如下错误：
Error:FAILURE: Build failed with an exception.
* What went wrong:
Execution failed for task ':app:externalNativeBuildDebug'.
> Build command failed.
Error while executing process E:\Android\Sdk\ndk-bundle\ndk-build.cmd with arguments {NDK_PROJECT_PATH=null APP_BUILD_SCRIPT=G:\project\Android\Anti-shake-arithmetic\app\src\main\jni\Android.mk NDK_APPLICATION_MK=G:\project\Android\Anti-shake-arithmetic\app\src\main\jni\Application.mk APP_ABI=armeabi NDK_ALL_ABIS=armeabi NDK_DEBUG=1 APP_PLATFORM=android-15 NDK_OUT=G:/project/Android/Anti-shake-arithmetic/app/build/intermediates/ndkBuild/debug/obj NDK_LIBS_OUT=G:\project\Android\Anti-shake-arithmetic\app\build\intermediates\ndkBuild\debug\lib G:/project/Android/Anti-shake-arithmetic/app/build/intermediates/ndkBuild/debug/obj/local/armeabi/libNDKUtils.so}
[armeabi] StaticLibrary  : libclapack1.a
process_begin: CreateProcess(NULL, E:/Android/Sdk/ndk-bundle/build//../toolchains/arm-linux-androideabi-4.9/prebuilt/windows-x86_64/bin/arm-linux-androideabi-ar crsD ....
很有可能是你   defaultConfig节点中，ndk 节点的配置信息与 Application.mk 不匹配，需要你 在ndk 节点添加上去





