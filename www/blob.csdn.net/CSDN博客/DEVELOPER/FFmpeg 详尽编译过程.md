# FFmpeg 详尽编译过程 - DEVELOPER - CSDN博客





2018年02月08日 10:32:26[学术袁](https://me.csdn.net/u012827205)阅读数：2990
所属专栏：[Android-开发](https://blog.csdn.net/column/details/20398.html)









## 内容摘要
- 准备linux系统 ubuntu14.04
- 安装 git
- 安装 vim(vimcdoc-1.5.0.tar.gz)
- 安装 jdk(jdk-8u151-linux-x64.tar.gz)
- 安装 ndk(android-ndk-r10e-linux-x86_64.bin)
- 配置jdk、sdk和ndk环境
- FFmpeg 编译
- 问题处理
- 将已编译的FFmpeg 移植到 android studio
- 移植问题处理

## 略过内容
- 准备linux系统 ubuntu14.04
- 安装 git
- 安装 vim(vimcdoc-1.5.0.tar.gz)
- 安装 jdk(jdk-8u151-linux-x64.tar.gz)
- 安装 ndk(android-ndk-r14-linux-x86_64.bin)
- 配置jdk、sdk和ndk环境 

这个过程在上一篇可以看到！

## FFmpeg 编译

### 下载源码

下载 FFmpeg 源码，点击进入 [https://github.com/FFmpeg/FFmpeg/releases/tag/n3.1.11](https://github.com/FFmpeg/FFmpeg/releases/tag/n3.1.11)

解压并进入到目录<这个是我的目录>：`/home/ztman/opt/FFmpeg-n3.1.11#`

### 修改配置

修改FFmpeg下的configure配置文件生成标准的 .so，而不是 .so.xxx，将变量右侧的内容改为以下配置：

```java
SLIBNAME_WITH_MAJOR='$(SLIBPREF)$(FULLNAME)-$(LIBMAJOR)$(SLIBSUF)'  
LIB_INSTALL_EXTRA_CMD='$$(RANLIB) "$(LIBDIR)/$(LIBNAME)"'  
SLIB_INSTALL_NAME='$(SLIBNAME_WITH_MAJOR)'  
SLIB_INSTALL_LINKS='$(SLIBNAME)'
```

即

![这里写图片描述](https://img-blog.csdn.net/20180202111624313?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对这几行代码的查找可以通过vim命令快速定位

![这里写图片描述](https://img-blog.csdn.net/20180202111713750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 编写脚本

```java
#!/bin/bash  
NDK=/home/ztman/opt/ndk14/android-ndk-r14b  
SYSROOT=${NDK}/platforms/android-23/arch-arm  
TOOLCHAIN=${NDK}/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64  

function build_so  
{  
./configure \  
--prefix=$PREFIX \  
--enable-shared \  
--disable-static \  
--disable-doc \  
--disable-ffmpeg \  
--disable-ffplay \  
--disable-ffprobe \  
--disable-ffserver \  
--disable-avdevice \  
--disable-doc \  
--disable-symver \  
--cross-prefix=${TOOLCHAIN}/bin/arm-linux-androideabi- \  
--target-os=linux \  
--arch=arm \  
--enable-cross-compile \  
--sysroot=$SYSROOT \  
--extra-cflags="-Os -fpic $ADDI_CFLAGS" \  
--extra-ldflags="$ADDI_LDFLAGS" \  
$ADDITIONAL_CONFIGURE_FLAG  
make clean  
make  
make install  
}  

CPU=arm  
PREFIX=./android/$CPU  
ADDI_CFLAGS="-marm"  
build_so
```

即

![这里写图片描述](https://img-blog.csdn.net/20180202111856193?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果在此想要了解shell脚本的编写规范，[可以点击进入](http://c.biancheng.net/cpp/u/shell/)

执行脚本命令：**“./buildff.sh**“之前要执行修改这个文件权限的命令如**“chmod +x buildff.sh”**。然后从图中我们看一下编译的过程，

![这里写图片描述](https://img-blog.csdn.net/20180202112013869?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20180202112025488?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20180202112032730?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这样就顺利成功了！并可以在脚本中设置的路径找到已经成功编译好的.so文件

![这里写图片描述](https://img-blog.csdn.net/20180202112054172?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其实，整个编译过程就是这样的。

参考资料： 
[http://c.biancheng.net/cpp/u/shell/](http://c.biancheng.net/cpp/u/shell/)
[http://blog.csdn.net/andrexpert/article/details/73823740](http://blog.csdn.net/andrexpert/article/details/73823740)
[https://righere.github.io/2016/10/10/build-ffmpeg4Android/](https://righere.github.io/2016/10/10/build-ffmpeg4Android/)
[http://zhengxiaoyong.me/2016/11/13/%E5%88%9D%E8%AF%86FFmpeg%E7%BC%96%E8%AF%91%E9%82%A3%E4%BA%9B%E4%BA%8B/](http://zhengxiaoyong.me/2016/11/13/%E5%88%9D%E8%AF%86FFmpeg%E7%BC%96%E8%AF%91%E9%82%A3%E4%BA%9B%E4%BA%8B/)
[http://blog.csdn.net/true100/article/details/53411096](http://blog.csdn.net/true100/article/details/53411096)
[https://www.jianshu.com/p/fdad78aba1e7](https://www.jianshu.com/p/fdad78aba1e7)
[http://bbs.chinaffmpeg.com/forum.php?mod=viewthread&tid=456](http://bbs.chinaffmpeg.com/forum.php?mod=viewthread&tid=456)
## 问题处理

对于整个编译的过程我的感触是非常深刻的，因为我在这个编译的过程中踏到了好多的“雷”，每个来的很是突然，都是触不及防的突然。简单叙述几个，希望对看到这篇博客的朋友有所帮助。 
**注意一**，您的 ndk 环境是否配置成功，这个最关键，她的成功与否直接影响你的后续操作。 
**注意二**，在写脚本.sh文件的时候，请注意您的 ndk 、SYSROOT 和 TOOLCHAIN 的路径值是否真的填写正确。 
**个人概率性介绍**，我在后来测试的时候，就是把别人的 .sh 脚本程序代码，直接拷贝到，我的 linux 系统下使用 vim 编译器打开的脚本中，然后更换自己的路径。然后使用命令 `./xxx.sh` 执行之后，没出现什么问题，成功。从我个人出发，我只想强调一下一般情况下你从良心博主那里拷贝到的代码是可以放心作为程序脚本使用的。并不会出现一些什么dos和unix的编码不同的什么原因会导致无法使程序脚本正常编译问题。并且此次成功前提是在我故意把`~/.bashrc` 中设置ndk环境的路写错进行测试的。说明了什么，自己琢磨一下程序脚本就能明白，我想大家懂得！ 

但是必然有朋友说，为什么我是按照他们的步骤一步一步走，但是还是行不通呢？对呀，为什么？但让有很多的原因，且描述一下我遇到的这些突发状况。现造几个错误的现象解释一下，如下
**问题现象一**
`C compiler test filed.`
![这里写图片描述](https://img-blog.csdn.net/20180202144958215?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

导致这种现象是因为写的程序脚本中`SYSROOT=${NDK}/platforms/android-8/arch-arm` 的`android-8` 的版本过低导致的，提高到21以上就好了。
**问题现象二**
![这里写图片描述](https://img-blog.csdn.net/20180202153205576?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

导致这种现象的原因是，1，当你通过命令`./configure --help` 可以查看到在FFmpeg3.4这个版本中，并没有 `--disable-ffserver`这个操作选项；那么注释了这一样代码会成功吗？答案是 NO，下面是注释过之后执行的过程log
```java
In file included from /home/ztman/opt/ndk14/android-ndk-r14b/platforms/android-21/arch-arm/usr/include/asm/termbits.h:19:0,
                 from /home/ztman/opt/ndk14/android-ndk-r14b/platforms/android-21/arch-arm/usr/include/asm-generic/termios.h:21,
                 from /home/ztman/opt/ndk14/android-ndk-r14b/platforms/android-21/arch-arm/usr/include/asm/termios.h:19,
                 from /home/ztman/opt/ndk14/android-ndk-r14b/platforms/android-21/arch-arm/usr/include/linux/termios.h:22,
                 from /home/ztman/opt/ndk14/android-ndk-r14b/platforms/android-21/arch-arm/usr/include/sys/ioctl.h:37,
                 from ./libavutil/timer.h:36,
                 from ./libavutil/internal.h:42,
                 from ./libavutil/common.h:491,
                 from libavcodec/mathops.h:27,
                 from libavcodec/aaccoder.c:38:
libavcodec/aaccoder.c: In function 'search_for_ms':

libavcodec/aaccoder.c: In function 'search_for_ms':
libavcodec/aaccoder.c:803:25: error: expected identifier or '(' before numeric constant
                     int B0 = 0, B1 = 0;
                         ^
libavcodec/aaccoder.c:865:28: error: lvalue required as left operand of assignment
                         B0 += b1+b2;
                            ^
libavcodec/aaccoder.c:866:25: error: 'B1' undeclared (first use in this function)
                         B1 += b3+b4;
                         ^
libavcodec/aaccoder.c:866:25: note: each undeclared identifier is reported only once for each function it appears in
```

即 
![这里写图片描述](https://img-blog.csdn.net/20180202153934076?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后我在google上查了一下，结果说是他们源码上的一些还未解决的bug！我想也是，因为使用的太崭新了。 

下面是运行的程序脚本内容 
![这里写图片描述](https://img-blog.csdn.net/20180202154919573?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后我要表述的**重点**就是：编译 FFmpeg 的过程也不一定会很顺利，到底是否顺利，**关键**就在你的**编译搭配**。 

什么是”编译搭配”？就是你在选择FFmpeg进行编译的时候，你所选择的 **NDK 和 FFmpeg** 使用的是什么**版本**，他们的匹配度决定了你的成功顺利与否！！
## 将已编译的FFmpeg 移植到 android studio

### 前期处理

![这里写图片描述](https://img-blog.csdn.net/20180206165817574?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从个人的处理上讲，个人便把已经编译过的linux中的上图**arm**文件拷贝到windows中；然后在windows中删去下图中被选中的链接文件<链接文件不需要>；这时候在windows中就会拿到一个全新的文件夹 *arm*
![这里写图片描述](https://img-blog.csdn.net/20180206165916930?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 移植到android studio

在anroid studio 中新建一个project，调整到project目录下，在`app/src/main` 下新建 `jni`文件夹，并拷贝*arm*下的整个**include 和 lib**文件目录到**jni**下，并新建文件`Android.mk  Application.mk  ffmpeg_jni.c` 到`jni/` 下。且与目录`include 和 lib` 处于同级别目录。 
![这里写图片描述](https://img-blog.csdn.net/20180206171551708?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Android.mk

```java
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE:= libavcodec
LOCAL_SRC_FILES:= lib/libavcodec-57.so
LOCAL_EXPORT_C_INCLUDES:= $(LOCAL_PATH)/include
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE:= libavformat
LOCAL_SRC_FILES:= lib/libavformat-57.so
LOCAL_EXPORT_C_INCLUDES:= $(LOCAL_PATH)/include
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE:= libswscale
LOCAL_SRC_FILES:= lib/libswscale-4.so
LOCAL_EXPORT_C_INCLUDES:= $(LOCAL_PATH)/include
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE:= libavutil
LOCAL_SRC_FILES:= lib/libavutil-55.so
LOCAL_EXPORT_C_INCLUDES:= $(LOCAL_PATH)/include
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE:= libavfilter
LOCAL_SRC_FILES:= lib/libavfilter-6.so
LOCAL_EXPORT_C_INCLUDES:= $(LOCAL_PATH)/include
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE:= libswresample
LOCAL_SRC_FILES:= lib/libswresample-2.so
LOCAL_EXPORT_C_INCLUDES:= $(LOCAL_PATH)/include
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE:= FFmpegCodec
LOCAL_SRC_FILES:= ffmpeg_jni.c
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_LDLIBS := -llog  -lz
LOCAL_SHARED_LIBRARIES := avcodec avfilter avformat avutil  swresample swscale
include $(BUILD_SHARED_LIBRARY)
```

Application.mk

```java
APP_ABI :=armeabi
APP_PLATFORM := android-16
APP_OPTIM := release
```

ffmpeg_jni.c

```java
#include <stdio.h>
#include "libavformat/avformat.h"
#include <libavfilter/avfilter.h>
#include <jni.h>
JNIEXPORT jstring Java_org_ztman_ffmpeg_MainActivity_printInfo(JNIEnv* env, jobject obj)
{
    char info[4000] = { 0 };
    int count = 100;  //输出前100个codec名字

    av_register_all();//初始化所有decoder和encoder,注册所有容器类型和codec

    AVCodec *c_temp = av_codec_next(NULL);
    while (c_temp != NULL && count > 0){
        //输出解码器和编码器
        if(c_temp->decode != NULL){
            sprintf(info,"%s[Dec]",info);
        }
        else{
            sprintf(info,"%s[Enc]",info);
        }

        sprintf(info,"%s[%10s]\n",info,c_temp->name);

        c_temp = c_temp->next;
        count--;
    }
    return (*env)->NewStringUTF(env, info);
 }
```

### 执行 ndk 编译

![这里写图片描述](https://img-blog.csdn.net/20180206171941610?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

使用android studio打开终端，进入到项目中jni所在的目录，执行命令`ndk-build` 。成功之后，会看到在main目录下多了两个文件 **libs 和 obj**
![这里写图片描述](https://img-blog.csdn.net/20180206172837215?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20180206173158261?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果切换到Android的浏览模式是这样的`jniLibs/`
![这里写图片描述](https://img-blog.csdn.net/20180206173324150?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### module/build.gradle配置

```java
apply plugin: 'com.android.application'

android {
    compileSdkVersion 26
    buildToolsVersion "26.0.2"

    defaultConfig {
        applicationId "org.ztman.ffmpeg"
        minSdkVersion 21
        targetSdkVersion 26
        versionCode 1
        versionName "1.0"

        testInstrumentationRunner "android.support.test.runner.AndroidJUnitRunner"

        //编译FFmpeg之后，进行移植需要
        ndk {
            abiFilter "armeabi"     //在Application.mk里面设置保持一致，如果是编译了多个平台的，可以指定编译多平台的或者
            moduleName "FFmpegCodec"    //jni模块的名字，与android.mk文件保持一致
            ldLibs "log", "z", "m", "jnigraphics", "android"    //这里可以设置ldLibs选项，比如你想调试jni的时候必须添加"log"参数
        }
    }
    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'
        }
    }


    //编译FFmpeg之后，进行移植需要
    externalNativeBuild {
        ndkBuild {
            path 'src/main/jni/Android.mk'
        }

    }

    //编译FFmpeg之后，进行移植需要
    sourceSets.main {
        jni.srcDirs = ['src/main/res'] //disable automatic ndk-build
        jniLibs.srcDirs = ['src/main/libs/armeabi']
    }

}

dependencies {
    compile fileTree(dir: 'libs', include: ['*.jar'])
    androidTestCompile('com.android.support.test.espresso:espresso-core:2.2.2', {
        exclude group: 'com.android.support', module: 'support-annotations'
    })
    compile 'com.android.support:appcompat-v7:26.+'
    compile 'com.android.support.constraint:constraint-layout:1.0.2'
    testCompile 'junit:junit:4.12'
}
```

### 配置后的测试

```java
public class MainActivity extends AppCompatActivity {

    static {
        try {
            System.loadLibrary("avcodec-57");
            System.loadLibrary("avfilter-6");
            System.loadLibrary("avformat-57");
            System.loadLibrary("avutil-55");
            System.loadLibrary("FFmpegCodec");
            System.loadLibrary("swresample-2");
            System.loadLibrary("swscale-4");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    public  static native String printInfo();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final TextView tvContent = (TextView) findViewById(R.id.contentPanel);
        tvContent.setText(printInfo());
    }
}
```

## 移植问题处理

问题一：出现`ndk-build命令出现“ndk-build不是内部或外部命令,也不是可运行的程序或批处理文件`

解决方案：查看是否具备ndk，然后像配置环境即可； 

问题二：出现`Android NDK:Application targets deprecated ABI(s):armeabi`
![这里写图片描述](https://img-blog.csdn.net/20180206172336242?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

解决方案：[修改你的ndk下的文件内容](http://www.tengmuz.com/index.php/2017/12/03/solve-jni-error-application-targets-deprecated-abis-armeabi/)
**编写C文件注意事项**

在上面编写的**Android.mk**中，我们制定了后缀名**.c**文件的名字为**ffmpeg_jni.c**，所以**c**文件要与**Android.mk**的`LOCAL_SRC_FILES:= ffmpeg_jni.c`与此保持一致；在app/build.gradle中模块ndk{}中的`moduleName "FFmpegCodec"` 的module名字，即

```java
ndk {
        abiFilter "armeabi"     //在Application.mk里面设置保持一致，如果是编译了多个平台的，可以指定编译多平台的或者
        moduleName "FFmpegCodec"    //jni模块的名字，与android.mk文件保持一致
        ldLibs "log", "z", "m", "jnigraphics", "android"    //这里可以设置ldLibs选项，比如你想调试jni的时候必须添加"log"参数
    }
```

要与**Android.mk**的  `LOCAL_MODULE:= FFmpegCodec` 对应；

**.c 文件中函数的申明规则**

编写文件名为**.c**的文件，声明**java**调用接口，函数命名需要按照**Java_包名*类名*方法名**的形式来编写，区分大小写。比如，上面写的.c文件中的方法名的命名是`Java_org_ztman_ffmpeg_MainActivity_printInfo` 表示在类`MainActivity` 中调用方法`printInfo`

**build.gradle中配置的解释**

```java
externalNativeBuild {
    ndkBuild {
        path 'src/main/jni/Android.mk'
    }
}
```

表示**.mk**文件的路径

```java
sourceSets.main {
    jni.srcDirs = [] //disable automatic ndk-build
    jniLibs.srcDirs = ['src/main/libs/armeabi']
}
```

表示完成编译之后，我们的生成的.so文件在哪个目录下面

```java
ndk {
        abiFilter "armeabi"     //在Application.mk里面设置保持一致，如果是编译了多个平台的，可以指定编译多平台的或者
        moduleName "FFmpegCodec"    //jni模块的名字，与android.mk文件保持一致
        ldLibs "log", "z", "m", "jnigraphics", "android"    //这里可以设置ldLibs选项，比如你想调试jni的时候必须添加"log"参数
    }
```

表示执行ndk-build的时候还要执行哪些附加选项，即可以看做是对**.mk**文件的补充












