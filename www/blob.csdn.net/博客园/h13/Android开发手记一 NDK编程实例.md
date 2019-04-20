# Android开发手记一 NDK编程实例 - h13 - 博客园
Android 开发手记一 
---- NDK 编程实例 
       在 Android 上，应用程序的开发，大部分基于 Java 语言来实现。要使用 c 或是 c++ 的程序或库，就需要使用 NDK 来实现。 NDK 是 Native Development Kit 的简称。它是一个工具集，集成了 Android 的交叉编译环境，并提供了一套比较方便的 Makefile ，可以帮助开发者快速开发 C 或是 C++ 的动态库，并自动的将 so 和 java 程序打包成 apk ，在 Android 上运行。 
       好，闲话少说，我们以一个简单的实例，来讲解 NDK 的应用。 
       一 开发环境的搭建 
       这一步虽然没什么技术含量，但是对于初学者，有一个很好的入门指导，还是很有帮助的。 
1.1   Android SDK 的搭建 
       首先，要进行 Android 程序的开发， Android 的 SDK 是必须要安装的。当然， Java 环境也必不可少。我们先要安装 JDK 和 Eclipse ，这个可以选比较新的版本，因为 Android 新的 SDK 已经不支持旧版本了。 
       1.1.1 JDK 可以用 V5 或 V6 版本，下载地址 [http://java.sun.com/javase/downloads/index.jsp](http://java.sun.com/javase/downloads/index.jsp)
       1.1.2 Eclipse 可以用版本 version 3.4 or 3.5 ，下载地址 [http://www.eclipse.org/downloads/](http://www.eclipse.org/downloads/) . 当然，若你需要其他的 Java 开发环境，可以不用 Eclipse ，不过这样也就用不了 ADT(Android Development Tools) 插件了。推荐还是用 Eclipse 来进行开发比较好，毕竟比较权威和方便么。 
       1.1.3 安装 SDK 
       Android SDK 下载地址为 [http://androidappdocs.appspot.com/sdk/tools-notes.html](http://androidappdocs.appspot.com/sdk/tools-notes.html)
       1.1.4 为 Eclips 安装插件 ADT 。在 Eclipse 中，填加更新站点 [https://dl-ssl.google.com/android/eclipse/](https://dl-ssl.google.com/android/eclipse/) ， 然后选择安装 ADT. 
       1.1.5 接下来，我们选择 Android 平台和组件。若是在 window 系统下，运行 SDK Setup.exe ；若是在 Linux 系统下，运行 tools 目录下的 android 程序，就可以选择需要的 Android Platform 和组件。 
       完成以上工作后，就可以进行 Android 应用程序的开发了。可以用 Eclipse 创建一个 Android 工程，比较简单的 Hello Android ，然后在模拟器下运行。具体的操作可以参看 Android 开发网站的说明，上面有详细的步骤。 
       1.2 Android NDK 的搭建 
       上面我们搭建好了 SDK 的环境，可以开发 Java 应用程序了。要开发 C 的程序，还得搭建 NDK 环境。 
       NDK 给我们提供了以下内容： 
              libc (C library) headers 
              libm (math library) headers 
              JNI interface headers 
              bz (Zlib compression) headers 
              blog (Android logging) header 
              A Minimal set of headers for C++ support 
       1.2.1 NDK 的安装 
       下载 NDK 安装包，下载地址 [http://androidappdocs.appspot.com/sdk/ndk/index.html](http://androidappdocs.appspot.com/sdk/ndk/index.html) ，下载后解压即可使用。 
       1.2.2 若在 Linux 开发环境下那么，这样就可以使用了。若是在 window 环境下，还需要安装 cygwin 。 cygwin 下载地址： [http://www.cygwin.com/](http://www.cygwin.com/)
       这样， NDK 的环境也搭建好了。下面我们来进行实战演习。 
       二 NDK 开发实例 
       关于 NDK 的使用，首先需要了解一个概念： JNI 。什么是 JNI ？ 
       2.1 Hello-jni 
       这个是 NDK 自带的例子程序，安装官方网站的说明，一步步来，应该没有什么问题，这里就不细说了。 
       2.2 My God I did it 
       学习的第一步，就是模仿。我们依照上面 Hello-jni 的例子，在创建自己的 NDK 程序。在此过程中，对相关的内容和概念进行分析和说明。 
       首先，创建自己的 NDK 工程。我们在 ndk 的 sample 目录下创建自己的工程 myjni ，然后在这个文件夹子下，创建两个目录 jni 和 src ， jni 用来放我们的 c 文件， src 是调用的 c 库 java 接口文件。创建好目录，接着创建文件 jni/myjni.c ，该文件比较简单，就是输出一个字符串，内容如下 
#include <string.h> 
#include <stdio.h> 
#include <jni.h> 
#include <android/log.h> 
#define LOG_TAG "MYJNI" 
#define LOGI(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__) 
static char s_string[] = "My god, I did it!"; 
jstring 
Java_com_jpf_myjni_MyJNI_stringFromJNI( JNIEnv* env, 
                                        jobject thiz ) 
{ 
       LOGI("MyJNI is called!"); 
       return (*env)->NewStringUTF(env, s_string); 
} 
       这个程序，唯一和 hello-jni 不同的就是引用了 <android/log.h> 这个头文件。在该头文件中，声明了函数 __android_log_print(), 可以根据不同的 log 级别，输出 log ，方便代码的调试。在 NDK 中， printf() 没法输出，所以我们需要借助 log 库来将我们 c 代码库中需要输出的内容，通过 java 控制台输出。调用函数 __android_log_print(), 就可以在 Eclipse 中，查看 LogCat 来查看相关的输出信息了。 
       注意： 
       在 c 文件中，函数名这样定义： Java_com_jpf_myjni_MyJNI_stringFromJNI ，有什么讲究么？这个是 JNI 的标准，定义需要按照如下格式： 
       Java _packagename _classname _methodname , 
       例如： Java _com_jpf_myjni _MyJNI _stringFromJNI 
       接着创建文件 jni/Android.mk. 这个文件是我们本地 c 代码的 Makefile 。文件内容如下： 
LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_MODULE := myjni 
LOCAL_SRC_FILES := myjni.c 
LOCAL_LDLIBS += -llog 
include $(BUILD_SHARED_LIBRARY) 
       分别对上述 Makefile 的语句进行说明。 
       LOCAL_PATH := $(call my-dir) 这句用来指定编译的路径。通过调用宏 my-dir ，获取到当前工作的路径。 
       include $(CLEAR_VARS) CLEAR_VARS 这个变量是编译系统提供的，用来指明一个 GNU makefile 文件，添加这句，主要的目的是清理所有的 LOCAL_XXX. ，比如 LOCAL_MODULE ， LOCAL_LDLIBS 。在每个新模块的开始处，需要添加这句。 
       LOCAL_MODULE := myjni 这句定义了模块名称，将来编译的库就以此命名。若果编译的是动态库，那么库名就是 libmyjni.so. 需要注意的是，如果你定义 module 为 libmyjni ，那么系统在生成动态库的时候，就不要再为你添加 lib 的前缀了，生成德动态库名字还是 libmyjni.so. 
       LOCAL_LDLIBS += -llog 这句指定了需要另外链接的库。我们在代码中，用到了 log 库，所以这里加上这句。 
       include $(BUILD_SHARED_LIBRARY) 这句说明将来生产的库是共享库，及动态链接库。若需要生产静态库，可以这样写： include $(BUILD_STATIC_LIBRARY) 。 
       写完了 c 文件和 Makefile 文件，是否可以编译了呢？我们试一下。在 cygwin 中，进入工程目录，运行 ndk-build ，得到下面的结果： 
[Administrator@lenovo-0e47e162](mailto:Administrator@lenovo-0e47e162) /android/android-ndk-r4/samples/myndk 
$ ndk-build 
Android NDK: Could not find application's manifest from current directory. 
Android NDK: Please ensure that you are inside the project's directory ! 
/android/android-ndk-r4/build/core/build-local.mk:74: *** Android NDK: Aborting 
   .  Stop. 
       看到这个错误的意思是，缺少 manifest 文件。老版本的 NDk ，工程中有一个 apps ，里面包含了应用的程序文件和 Application.mk 。现在的版本，不需要我们自己编写 Application.mk, ，不过仍需要工程相关的配置信息。那么如何做到呢？需要手工去写 manifest 文件么？不需要。我们只需要在 Eclipse 中，创建工程就可以了，这些配置文件会自动生成。 
       前面讲过，在工程的 src 夹子下用来放置 Java 文件。我们打开 Eclipse ，然后新建一个 Android 工程，工程名就叫 MyJNI ，工程路径选择我们创建的 NDK 的路径。这里需要注意的是，工程名，包名等，需要和上面的 c 文件中的保持一致。 
(Java _com_jpf_myjni _MyJNI _stringFromJNI) 
       工程建立好后，编辑 src/com/jpf/myjni/MyJNI.java 文件，内容如下： 
package com.jpf.myjni; 
import android.app.Activity; 
import android.widget.TextView; 
import android.os.Bundle; 
public class MyJNI extends Activity { 
    /** Called when the activity is first created. */ 
    @Override 
    public void onCreate(Bundle savedInstanceState) { 
        super .onCreate(savedInstanceState); 
        TextView  tv = new TextView( this ); 
        tv.setText( stringFromJNI() ); 
        System. out .println( "Here we go ..." ); 
        setContentView(tv); 
        System. out .println( "Done!" ); 
    } 
    public native String  stringFromJNI(); 
    static { 
           System.loadLibrary ( "myjni" ); 
    } 
} 
       需要说明的几点： 
       public native String  stringFromJNI(); 这句申明，带有 native 关键字，说明该方法是本地方法。 
       System.loadLibrary ( "myjni" ); 这句就是用来加载我们的 c 动态库的。上面声明的方法，具体实现，就在我们加载的库中。 
       建立好工程，再次编译，在 cygwin 中运行 ndk-build ，结果 OK 。 
[Administrator@lenovo-0e47e162](mailto:Administrator@lenovo-0e47e162) /android/android-ndk-r4/samples/myndk 
$ ndk-build 
Compile thumb  : myjni <= /android/android-ndk-r4/samples/myndk/jni/myjni.c 
SharedLibrary  : libmyjni.so 
Install        : libmyjni.so => /android/android-ndk-r4/samples/myndk/libs/armea 
bi 
       我们看到，需要的共享库已经生成，并且安装好了。下面就可以生成 apk 了。 
       在 Cygwin 中进行工程的 build ，编译后，在工程的 bin 目录下，会看到我们的 apk 包。 
       好，我们试试看，能否正常运行。在 Eclipse 选择执行方式为 Android Application ，点击 run ，以下 console 的输出： 
[2010-07-07 14:26:18 - MyJNI] ------------------------------ 
[2010-07-07 14:26:18 - MyJNI] Android Launch! 
[2010-07-07 14:26:18 - MyJNI] adb is running normally. 
[2010-07-07 14:26:18 - MyJNI] Performing com.jpf.myjni.MyJNI activity launch 
[2010-07-07 14:26:18 - MyJNI] Automatic Target Mode: using existing emulator 'emulator-5554' running compatible AVD 'android21' 
[2010-07-07 14:26:18 - MyJNI] WARNING: Application does not specify an API level requirement! 
[2010-07-07 14:26:18 - MyJNI] Device API version is 7 (Android 2.1-update1) 
[2010-07-07 14:26:18 - MyJNI] Uploading MyJNI.apk onto device 'emulator-5554' 
[2010-07-07 14:26:18 - MyJNI] Installing MyJNI.apk... 
[2010-07-07 14:26:24 - MyJNI] Success! 
[2010-07-07 14:26:25 - MyJNI] Starting activity com.jpf.myjni.MyJNI on device 
[2010-07-07 14:26:29 - MyJNI] ActivityManager: Starting: Intent { act=android.intent.action.MAIN cat=[android.intent.category.LAUNCHER] cmp=com.jpf.myjni/.MyJNI } 
       上面的 warning ，是我们没有指定 API 的版本号。如下指定一下就没有这个 warning 了。 
       下图为执行的效果： 
       下图是我们查看 LogCat 的输出： 
       可以看到我们的输出 MYJNI ： MyJNI is called ！ 
       2.3 Study Hard 
       有了上面的基础，我们就可以用 NDK 来进行项目开发了。 
       我们经常会遇到这样的问题，就是将一些现有的，成熟的 C 库移植到 Android 平台上。通过上面我们的介绍，我们已经知道，我们需要用 JNI 来对现有的 C 库包装一下，然后提供 Java 接口，供上层调用。 
       首先的问题，就是 C 库的编译和测试。其实 Android 底层用的是 Linux 的内核，所以，和其他 Linux 程序开发一样，无法使进行交叉编译。不过， Android 有些特殊的地方，我们需要注意。下面就以一个很简单的例子，讲讲如何应用 NDK ，做一个 C 的应用终端测试程序。 
       首先，创建 study-hadr/study-hard.c 文件，程序非常简单，就是 Hello World 的 c 程序。 
#include <string.h> 
#include <stdio.h> 
static char s_string[] = "Study hard!"; 
int main() 
{ 
       printf("%s\n", s_string); 
       return 0; 
} 
       别看程序很简单，不过这个程序的编译可不简单。 
       若是在 Linux 下，只需要执行： 
       gcc –o study-hard study-hard.c  就可以生成应用程序 study-hard 了。 
       在 Android 下就不是这么简单了。在 Window 环境开发环境下，用到的交叉工具链，目录是 \android-ndk-r4\build\prebuilt\windows\arm-eabi-4.4.0 。 在这个目录的 bin 路径下，你会看到 arm-eabi 为前缀的诸多工具，这些就是 Android 用的编译工具。那么 c 库和 c 头文件又在哪里呢？对于 Android ，不同的 Platform ，有不同的库和头文件，需要我们自己选择。比如，现在我们要用 Platform5 ，那么 
       C 头文件的路径为： 
       \android-ndk-r4\build\platforms\android-5\arch-arm\usr\include 
       C 库的路径为： 
       \android-ndk-r4\build\platforms\android-5\arch-arm\usr\lib 
       好了，我们知道了 C 的编译工具链，知道了 C 库路径和 C 头文件路径，应该可以编译了。写个简单的 Makefile ，试一下，结果出错了。 crt0.o 没有找到。 
       这个错误很糟糕，指出在链接的时候，找不到 crt0.o 。我们在 Makefile 中添加如下几句： 
              LDFLAGS += -nostdlib 
       -nostdlib 表示不连接系统标准启动文件和标准库文件 . 只把指定的文件传递给连接器。 
       此时编译，结果为： 
       错误指出，在链接的时候，找不到 puts ，这个函数是 c 库中的，我们添加如下语句再次尝试： 
              LDFLAGS += -lc 
       我们修改链接选项，增加对 dl 库的链接， 再次尝试： 
       LDFLAGS += -lc –ldl 
       这次生成了可执行文件，不过还是有 warning ，在生成的可执行文件中，没有找到入口 _start 。这个问题也比较奇怪。我们查看下生成的可执行文件 : 
       readelf –a study-hard 
       发现生成的可执行文件，真的没有入口函数。这是为什么呢？ 
       在 Linux 下，用 -v 选项跟踪下 gcc 编译 hello world 程序的过程。会发现，在链接的过程中，除了 hello.o, 还会链接 crt1.o, crtn.o 等文件，正是这些文件，在生成可执行程序的过程中，组成了 elf 文件中程序入口和程序退出等相关的处理部分。 
       查看我们指定的 C 库： 
       会发现， C 库下有 crt 打头的三个 .o 文件。我们修改 Makefile ，链接 crtbegin 和 crtend 文件： 
EXTRA_OBJS := $(PATH_PREFIX)/lib/crtbegin_dynamic.o $(PATH_PREFIX)/lib/crtend_android.o 
    … … 
       $(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(EXTRA_OBJS) $(LDFLAGS) 
       再次编译，结果如下，此次终于编译成功了。 
       我们将编译好的程序放到 Android 上运行下看看效果。 
       显示程序没有找到。怎么回事呢？继续研究下 AndroidNDK 相关文档。我们还需要修改 Makefile 的一个地方： 
       LDFALGS += -Bdynamic -Wl,-dynamic-linker,/system/bin/linker 
       指定链接动态库，动态连接器为 /system/bin/linker 
       编译后，再次运行，终于看到了 “Study hard ！ ” 
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/L____J/archive/2010/08/04/5787759.aspx](http://blog.csdn.net/L____J/archive/2010/08/04/5787759.aspx)
