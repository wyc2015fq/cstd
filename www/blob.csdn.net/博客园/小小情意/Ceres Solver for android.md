
# Ceres Solver for android - 小小情意 - 博客园






# [Ceres Solver for android](https://www.cnblogs.com/xiaoxiaoqingyi/p/6653065.html)
最近开发中，需要对图片做一些处理与线性技术，这时就用到了Ceres Solver。如何把Ceres Solver集成到Android里呢？ 官网给了一个解决方案，简洁明了：
Download the Android NDK version r9d or later. Run ndk-build from inside the jni directory. Use the libceres.a that gets created.（意思是：下载NDK r9d 或更新版本，然后进入 下载最新ceres-solver包的jni目录里，运行ndk-build 得到 libceres.a 静态包）
步骤如下：
1、添加NDK的环境变量，不懂的可以搜索安装教程。
2、运行 =》cmd，打开命令行窗口， cd 进入到下载ceres-solver包的jni目录里，运行 ndk-build
报：jni/Android.mk:80: *** Ceres requires NDK version r9d or greater . Stop. 错误！
查看 Android.mk 80 行，可以知道它是判断当前NDK的版本，运行了 jni 目录下的shell脚本（assert_ndk_version.sh）。
但是我发现我的NDK已经是最新了，为什么还报这异常呢？ 查看当前的NDK版本，[可以点击这](http://www.cnblogs.com/xiaoxiaoqingyi/p/6604468.html)。
认真查看了 assert_ndk_version.sh 文件的内容，发现，它是查找NDK根目录下的一个叫 RELEASE.TXT 文件， 但我们没有。所以新建一个， 然后在里面加入 r14b (64-bit) (替换成你的NDK版本)
在MAC 系统上可以跑通，在windows 上还会继续报上面的错误，因为windows 不能直接跑shell脚本。 直接了当，Android.mk 79—81行 用 \# 注释掉。 然后就会编译出 libceres.a 静态文件。
但是这样编译出来的静态文件，在运用的时候，还是会报错，打开 ceres-solver包的jni目录下的
Application.mk文件，可以看到：
APP_CPPFLAGS += -fno-exceptions
APP_CPPFLAGS += -fno-rtti
APP_STL := c++_static

|名称|说明>|功能|
|[libstdc++（默认）](https://developer.android.com/ndk/guides/cpp-support.html?hl=zh-cn#system)|默认最小系统 C++ 运行时库。|不适用|
|[gabi++_static](https://developer.android.com/ndk/guides/cpp-support.html?hl=zh-cn#ga)|GAbi++ 运行时（静态）。|C++ 异常和 RTTI|
|[gabi++_shared](https://developer.android.com/ndk/guides/cpp-support.html?hl=zh-cn#ga)|GAbi++ 运行时（共享）。|C++ 异常和 RTTI|
|[stlport_static](https://developer.android.com/ndk/guides/cpp-support.html?hl=zh-cn#stl)|STLport 运行时（静态）。|C++ 异常和 RTTI；标准库|
|[stlport_shared](https://developer.android.com/ndk/guides/cpp-support.html?hl=zh-cn#stl)|STLport 运行时（共享）。|C++ 异常和 RTTI；标准库|
|[gnustl_static](https://developer.android.com/ndk/guides/cpp-support.html?hl=zh-cn#gn)|GNU STL（静态）。|C++ 异常和 RTTI；标准库|
|[gnustl_shared](https://developer.android.com/ndk/guides/cpp-support.html?hl=zh-cn#gn)|GNU STL（共享）。|C++ 异常和 RTTI；标准库|
|[c++_static](https://developer.android.com/ndk/guides/cpp-support.html?hl=zh-cn#cs)|LLVM libc++ 运行时（静态）。|C++ 异常和 RTTI；标准库|
|[c++_shared](https://developer.android.com/ndk/guides/cpp-support.html?hl=zh-cn#cs)|LLVM libc++ 运行时（共享）。|C++ 异常和 RTTI；标准库|

c++_static 我们平常比较少用到（因为我不熟悉C/C++），所以将上述三个参数改为：
APP_STL := gnustl_static
APP_CPPFLAGS += -frtti
APP_CPPFLAGS += -fexceptions
然后ndk-build，得到一个正确的 libceres.a 静态文件。 (欢迎留言讨论)
**参考资料： http://tech.sandyeggi.com/2013/10/using-ceres-solver-in-android-ndk.html**

**我编译好的ceres-solver 项目****[http://pan.baidu.com/s/1bo3ENoj](http://pan.baidu.com/s/1bo3ENoj)**








