# Android.mk解析 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年09月30日 09:37:24[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：186
[https://developer.android.com/ndk/guides/android_mk.html#over](https://developer.android.com/ndk/guides/android_mk.html#over)
LOCAL_STATIC_LIBRARIES
PREBUILT_STATIC_LIBRARY 与PREBUILT_SHARED_LIBRARY
静态库可生成共享库
Android.mk 文件必须首先定义 LOCAL_PATH 变量：
LOCAL_PATH := $(call my-dir)
此变量表示源文件在开发树中的位置。在这里，构建系统提供的宏函数 my-dir 将返回当前目录（包含 Android.mk 文件本身的目录）的路径。
下一行声明 CLEAR_VARS 变量，其值由构建系统提供。
include $(CLEAR_VARS)
CLEAR_VARS 变量可为您清除许多 LOCAL_XXX 变量，例如 LOCAL_MODULE、LOCAL_SRC_FILES 和 LOCAL_STATIC_LIBRARIES。
 不会清除 LOCAL_PATH。
枚举源文件，以空格分隔多个文件：
LOCAL_SRC_FILES := hello-jni.c
LOCAL_SRC_FILES 变量必须包含要构建到模块中的 C 和/或 C++ 源文件列表。
定义自己的变量，建议在名称前附加 MY_。

PREBUILT_SHARED_LIBRARY:
这里的 LOCAL_SRC_FILES 值不能是源文件，而必须是指向预建共享库的单一路径，例如 foo/libfoo.so
也可使用 LOCAL_PREBUILTS 变量引用另一个模块中的预建库。
务必使用 Unix 样式的正斜杠 (/)
LOCAL_CPP_FEATURES: 建议使用此变量，而不要直接在 LOCAL_CPPFLAGS 定义中启用 -frtti 和 -fexceptions。使用此变量可让构建系统对每个模块使用适当的标志。使用 LOCAL_CPPFLAGS 会导致编译器对所有模块使用所有指定的标志
指明您的代码依赖于特定 C++ 功能。对于预构建的库，此变量还可声明二进制文件依赖哪些功能
描述值的顺序不重要。
LOCAL_C_INCLUDES: include 搜索路径
在通过 LOCAL_CFLAGS 或 LOCAL_CPPFLAGS 设置任何对应的
 include 标志之前定义此变量。
LOCAL_CFLAGS
在构建 C 和 C++ 源文件时要传递的编译器标志
在 android-ndk-1.5_r1 中，相应的标志只适用于 C 源文件，而不适用于 C++ 源文件。
使用 LOCAL_CPPFLAGS 只为 C++ 源文件指定标志
指定其他 include 路径：
LOCAL_CFLAGS += -I<path>,但使用 LOCAL_C_INCLUDES 更好
LOCAL_STATIC_LIBRARIES
当前模块依赖的静态库模块列表
如果当前模块是共享库或可执行文件，此变量将强制这些库链接到生成的二进制文件
如果当前模块是静态库，表示依赖当前模块的模块也会依赖列出的库。
LOCAL_SHARED_LIBRARIES
模块在运行时依赖的共享库模块列表
LOCAL_WHOLE_STATIC_LIBRARIES
LOCAL_STATIC_LIBRARIES 的变体，表示链接器应将相关的库模块视为*整个存档*。 如需了解有关整个存档的详细信息，请参阅
 GNU 链接器关于 --whole-archive 标志的[文档](http://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_node/ld_3.html)。
当多个静态库之间具有循环相依关系时，此变量很有用。 使用此变量构建共享库时，将会强制构建系统将所有对象文件从静态库添加到最终二进制文件。 但在生成可执行文件时不会发生这样的情况。
LOCAL_LDLIBS
构建共享库或可执行文件时的链接器标志列表，可让您使用 -l 前缀传递特定系统库的名称。 例如，以下示例指示链接器生成在加载时链接到 /system/lib/libz.so 的模块：
LOCAL_LDLIBS := -lz
如需了解此 NDK 版本中可以链接的已公开系统库列表，请参阅 [Android
 NDK 原生 API](https://developer.android.com/ndk/guides/stable_apis.html)。
LOCAL_LDFLAGS
构建共享库或可执行文件时供构建系统使用的其他链接器标志列表
如果为静态库定义此变量，构建系统会忽略它，并且 ndk-build 会显示一则警告。
并非所有基于 ARMv7 的 CPU 都支持 NEON 指令集扩展。因此，必须执行运行时检测以便在运行时安全地使用此代码。 如需了解详细信息，请参阅 [NEON
 支持](https://developer.android.com/ndk/guides/cpu-arm-neon.html)和[cpufeatures 库](https://developer.android.com/ndk/guides/cpu-features.html)。
NDK 提供的函数宏
使用 $(call <function>) 对它们估值；它们返回文本信息。
all-subdir-makefiles
返回位于当前 my-dir 路径所有子目录中的 Android.mk 文件列表。
import-module
用于按模块的名称查找和包含模块的 Android.mk 文件的函数。
 典型的示例如下所示：
$(call import-module,<name>)
在此示例中，构建系统查找 NDK_MODULE_PATH 环境变量引用的目录列表中以 <name> 标记的模块，并且自动为您包含其Android.mk 文件。
[http://android.mk/](http://android.mk/)
