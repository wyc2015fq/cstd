
# NDK 开发实例一（Android.mk环境配置下） - 小小情意 - 博客园






# [NDK 开发实例一（Android.mk环境配置下）](https://www.cnblogs.com/xiaoxiaoqingyi/p/6524165.html)
在我写这篇文章的时候，Android Studio已经是2.3版本了，已经集成CMake 编译工具，
用户只需在 新建项目的时候，添加选项（Include C++ support），Android Studio就会自动
帮你部署好相应的NDK环境。但我还是想尝试使用 Android.mk 配置文件方式来实现NDK开发，
了解其配置的原理，然后再使用 CMake 来部署，这样能了解两者间的区别。后续的文章就会写
到用CMake来部署NDK环境。
假设你已经安装好了 Android Studio 与NDK 环境。
建议将ndk-build所在目录加入PATH环境变量，以便后续调试。
1、打开 File => Project Structure 如下：
![](https://images2015.cnblogs.com/blog/804587/201703/804587-20170309092541172-1566739857.png)
如果选框中为空，说明你没有配置好NDK环境。
2、copy 路径， 然后打开如下 windows 系统下的环境变量
![](https://images2015.cnblogs.com/blog/804587/201703/804587-20170309092801938-75896894.png)
在 Path 变量里添加你的 NDK路径，即上一步copy的内容，记住在路径前添加 ; 分号。
确定之后，就在Windows 命令窗口 输入 ndk-build，得到如下图，说明你配置成功了。
![](https://images2015.cnblogs.com/blog/804587/201703/804587-20170309092954875-1511758131.png)
下面我们来新建一个Android项目，实现一个简单的NDK方法。
1、创建一个NDKUtils.Java类， 新建一个 native 方法，
public native String invokeCmethod();
然后在该类 添加如下代码，即载入库的含义：
static {
System.loadLibrary("NDKUtils");//导入生成的链接库文件
}
2、点击 菜单 Build =>Make Project，就会编译项目，然后在
你的项目跟路径 app\build\intermediates\classes\debug\com\magicing\eigenndk
可以看到 NDKUtils.class 文件（com\magicing\eigenndk 是包名，对应你自己的包名）
3、进入Android Studio命令行页面:
![](https://images2015.cnblogs.com/blog/804587/201703/804587-20170309093053344-1570725197.png)
如果你没有这个选项，则你从 View菜单栏里 Tool Windows => Terminal 调出该页面。
输入如下命令：
cd app/src/main   // 进入该路径
然后执行下面编译命令
javah -d jni -classpath F:\AndroidSDK\platforms\android- 23\android.jar;..\..\build\intermediates\classes\debug com.magicing.ndktest2.NDKUtils
解释（根据自身的情况来设置）：
F:\AndroidSDK\platforms\android-23\android.jar;   //是你的SDK路径，
..\..\build\intermediates\classes\debug                  //相对路径，都一样
com.magicing.ndktest2.NDKUtils                          // NDKUtils 的命名空间
4、当上述的命令执行成功后，在app/src/main 多了一个 jni 文件夹，里面有一个
com_magicing_eigenndk_NDKUtils.h 文件。在该路径下copy 一个相同名，后缀
为 .c 的文件 com_magicing_eigenndk_NDKUtils.c 。然后该文件就是C 语言的实现文件。
把如下方法调整一下（完善参数命名，且添加一个大括号），得到如下：
/*
* Class: com_magicing_eigenndk_NDKUtils
* Method: invokeCmethod
* Signature: ()Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_com_magicing_eigenndk_NDKUtils_invokeCmethod
(JNIEnv *env, jobject obj){
return (*env)->NewStringUTF(env,"NDK 获取Native 字符串");
}
5、在jni 目录下创建 Android.mk 配置文件，内容如下：
LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := NDKUtils
LOCAL_SRC_FILES := com_magicing_eigenndk_NDKUtils.c
include $(BUILD_SHARED_LIBRARY)
Android.mk 官网介绍[https://developer.android.com/ndk/guides/android_mk.html](https://developer.android.com/ndk/guides/android_mk.html)
6、在jni 目录下创建 Application.mk 配置文件，内容如下：
APP_PLATFORM := android-23
APP_ABI := armeabi
Application.mk 官网介绍[https://developer.android.com/ndk/guides/application_mk.html](https://developer.android.com/ndk/guides/application_mk.html)
7、然后继续在命令行端口执行 ndk-build 命令，得到如下:

![](https://images2015.cnblogs.com/blog/804587/201703/804587-20170309093342922-427536950.png)

该命令是编译C文件，然后生成 .so 文件，生成在 app\src\main\libs 目录下。
ndk-build 还有很多参数，大家可以去查询一下其它资料，如果有时间，我后续篇幅详细讲一下。
8、配置根目录下的 build.gradle 文件

![](https://images2015.cnblogs.com/blog/804587/201703/804587-20170309093505641-505226564.png)
主要添加如下代码：
sourceSets{
main{
jni.srcDirs = [] // disable automatic ndk-build call, which ignore our Android.mk
jniLibs.srcDir 'src/main/libs'
}
}
这样， native invokeCmethod 方法算是衔接好了，只有在MainActivity 上实例化这个类，然后
调用 这个方法，就能输出相应的字符串，这样就简单的实现NDK功能。
![](https://images2015.cnblogs.com/blog/804587/201703/804587-20170309093712250-350820247.png)
项目的目录大概如下：
![](https://images2015.cnblogs.com/blog/804587/201703/804587-20170309093759031-1927639333.png)
Demo下载链接：[http://files.cnblogs.com/files/xiaoxiaoqingyi/EigenNdk.rar](http://files.cnblogs.com/files/xiaoxiaoqingyi/EigenNdk.rar)









