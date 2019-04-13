
# NDK配置debug环境时：Error:FAILURE: Build failed with an exception - 小小情意 - 博客园






# [NDK配置debug环境时：Error:FAILURE: Build failed with an exception](https://www.cnblogs.com/xiaoxiaoqingyi/p/7149861.html)
Error:FAILURE: Build failed with an exception.
* What went wrong:
Execution failed for task ':app:externalNativeBuildDebug'.
> Build command failed.
Error while executing process E:\Android\Sdk\ndk-bundle\ndk-build.cmd with arguments {NDK_PROJECT_PATH=null APP_BUILD_SCRIPT=G:\project\Android\Anti-shake-arithmetic\app\src\main\jni\Android.mk NDK_APPLICATION_MK=G:\project\Android\Anti-shake-arithmetic\app\src\main\jni\Application.mk APP_ABI=armeabi NDK_ALL_ABIS=armeabi NDK_DEBUG=1 APP_PLATFORM=android-15 NDK_OUT=G:/project/Android/Anti-shake-arithmetic/app/build/intermediates/ndkBuild/debug/obj NDK_LIBS_OUT=G:\project\Android\Anti-shake-arithmetic\app\build\intermediates\ndkBuild\debug\lib G:/project/Android/Anti-shake-arithmetic/app/build/intermediates/ndkBuild/debug/obj/local/armeabi/libNDKUtils.so}
[armeabi] StaticLibrary : libclapack1.a
process_begin: CreateProcess(NULL, E:/Android/Sdk/ndk-bundle/build//../toolchains/arm-linux-androideabi-4.9/prebuilt/windows-x86_64/bin/arm-linux-androideabi-ar crsD G:/project/Android/Anti-shake-arithmetic/app/build/intermediates/ndkBuild/debug/obj/local/armeabi/libclapack1.a
make: *** [G:/project/Android/Anti-shake-arithmetic/app/build/intermediates/ndkBuild/debug/obj/local/armeabi/libclapack1.a] Error 206
* Try:
Run with --stacktrace option to get the stack trace. Run with --info or --debug option to get more log output.
出现上述错误，是我在NDK开发中，想在 Android Studio 配置 debug 环境（具体步骤：[http://www.cnblogs.com/xiaoxiaoqingyi/p/7143536.html](http://www.cnblogs.com/xiaoxiaoqingyi/p/7143536.html)），于是在 gradle 配置文件下 添加了如下节点：
externalNativeBuild {//采用ndk-buildndkBuild {
           path'src/main/jni/Android.mk'}//如采用 cmake 使用如下配置//cmake {//path 'src/main/jni/CMakeLists.txt'//}}
在没添加这些节点的时候， 我编译是能通过的，且能运行 .so动态库。加入这些节点后，就出现了上面的错误。第一反应，我看到的是NDK_PROJECT_PATH=null，想到应该是 .so 库没有衔接好，于是我google了好久，大概有如下解决方案，你可以一 一去尝试：
1、修改 module 下 gradle 里 sourceSets 指定动态库路径节点，该节点用来配置 jni 库的路径。Android Studio 环境下，默认的 jniLibs 路径是：src/main/jniLibs。如果你是从旧项目移植过来的话，可能没有该目录，你可以在 android 节点中添加如下配置：
//指定动态库路径sourceSets {
    main {
        jni.srcDirs= []//disable automatic ndk-build call, which ignore our Android.mkjniLibs.srcDir'src/main/libs'}
}
2、也有方案说，需要把 编译好的 动态库放到 app/libs 目录下， 因为这个目录默认是存放 jar包，把编译好的动态库放入 app/libs 目录后，再在 android 节点中添加如下配置：
sourceSets {
        main {
            jniLibs.srcDirs= ['libs‘]}
    }
3、如果上述都两种方案都没解决你的问题，这很大可能是 你的Android.mk 配置问题。你的Android.mk 是否有一个 总的 Android.mk，然后include 了其它子Android.mk ？ 如果是，你应该重新配置好你的子Android.mk。像 OpenCV native库， 它就是用一个 总的 Android.mk，include 了其它子Android.mk，OpenCV 库是配置是没有问题的，我编译成功过。
我得到最开始的那个错误是 因为我在 配置levmar 库时，levmar 使用了第三方的库(clapack)，而子Android.mk 没有配置好，最后，我把 clapack 库去了，因为该项目刚好也没有使用到 clapack 库。最后，编译成功了。
上述我也说了，我的目的是为了能debug C/C++ 代码，编译成功后，其实debug 环境是配置好了，但还是debug 不了，就是说，在那行代码设置了断点，运行到那里了，还是停不下来。 只有C/C++代码报错的时候， debug才检测到。这个问题我一直没有解决，很有可能的原因是，我使用了比较多的 第三方编译好的动态库，而直接使用源代码，如 ffmpeg，opencv。
参考资料：
[https://stackoverflow.com/questions/21096819/jni-and-gradle-in-android-studio](https://stackoverflow.com/questions/21096819/jni-and-gradle-in-android-studio)









