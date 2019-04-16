# FFmpeg(1)-创建支持FFmpeg的AS项目 - 夜行过客 - 博客园







# [FFmpeg(1)-创建支持FFmpeg的AS项目](https://www.cnblogs.com/yongdaimi/p/9791206.html)





**一.新建Android Studio项目**

注意点：

1.在“Create Android Project”栏目时，须勾选“Include C++ support” 复选框；

2.在“Customize C++ Support 栏目”时，C++ Standard 选择下拉列表中的“C++11”。



**二.在AS项目中配置支持FFmpeg**

注意点：

**1.添加FFmpeg依赖库。包括生成的头文件和.so文件。**

1.1)  将编译FFmpeg生成的include目录拷贝到项目的app目录下。

![](https://img2018.cnblogs.com/blog/653161/201810/653161-20181015141544460-344564908.png)

1.2) 将编译FFmpeg生成的lib目录拷贝到项目的libs目录下。

PS：这里需要区分不同的CPU平台，如果FFmpeg生成的库是针对armeabi-v7a的，则放到libs下的armeabi-v7a目录，针对x86的，则放到x86目录下。

![](https://img2018.cnblogs.com/blog/653161/201810/653161-20181015142010773-1221457414.png)

**2.配置FFmpeg库的环境变量。主要用于告诉CMake FFmpeg头文件的路径，库文件的路径，以及编译链接的内容。**

2.1) 声明头文件及用到的库文件路径。打开app目录下的 CMakeLists.txt ,在原有内容基础上添加下列代码：

```
#添加头文件路径
include_directories(include)

#设置FFmpeg库所在路径
set(FF ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI})
add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${FF}/libavcodec.so)
```

include_directories(include) 用于声明头文件的路径，我们这里是在include目录下。

add_library 则用于声明库文件的路径，这里的 add_library 不仅可用于添加源码类型的库(如.cpp文件)，也可以添加编译好的so库(我们这里使用的是libavcodec.so这个库)。这里的 avcodec 并不是so库的文件名，仅是一个简写的库名。后面的 set_target_properties 中须指定库文件所在的详细路径，与常用的Android.mk文件不同，这里需要写全前缀(lib)和后缀(.so)。

2.2) 链接指定的so库。找到CMakeLists.txt中的 target_link_libraries 项，在原有的 native-lib 后添加上面用的库名： avcodec

![](https://img2018.cnblogs.com/blog/653161/201810/653161-20181015143654637-118938093.png)

这段代码是让CMake能够链接指定的so库，否则即使包含头文件，在编译时也会报函数undefined的错误。（将所有用到的库全部链接到一个库当中，这样做的好处是在java中只需要引入一个native-lib库就行了）

**3.声明项目库文件路径**

打开app目录下的build.gradle文件，在defalutConfig大括号下添加下列代码：

```
externalNativeBuild {
            cmake {
                cppFlags "-std=c++11"
            }
            ndk {
                abiFilters "armeabi-v7a"
            }
        }
        sourceSets {
            main {
                jniLibs.srcDirs=['libs']
            }
        }
```

![](https://img2018.cnblogs.com/blog/653161/201810/653161-20181015151003998-664528418.png)

这里的  abiFilters 用于告诉编译器我们只加载armv7a的版本， jniLibs.srcDirs 则指定我们的库文件是放在libs目录下。

**4.调用函数测试FFmpeg是否可以正常工作**

打开cpp目录下的 native-lib.cpp 文件中，在文件顶部添加下列代码：

```
extern "C" {
#include <libavcodec/avcodec.h>
}
```

修改stringFromJNI()函数

![](https://img2018.cnblogs.com/blog/653161/201810/653161-20181015151345473-498404052.png)

编译运行，如果程序显示下面类型的内容，则说明FFmpeg已经配置并加载成功了。

![](https://img2018.cnblogs.com/blog/653161/201810/653161-20181015151820598-144711061.png)














