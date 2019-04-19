# android下 C++单元测试 及 代码覆盖率 - u013366022的专栏 - CSDN博客
2018年07月23日 14:02:52[slitaz](https://me.csdn.net/u013366022)阅读数：432
# **一、**简单介绍****
gtest是一款google提供的强大的测试框架，测试案例的编写也比较简单，gtest案例的编写可以参考系列博文:http://www.cnblogs.com/coderzh/archive/2009/03/31/1426758.html。
Gcov是进行代码运行的覆盖率统计的工具，它随着gcc的发布一起发布的，它的使用也很简单，需要在编译和链接的时候加上-fprofile-arcs -ftest-coverage生成二进制文件，gcov主要使用.gcno和.gcda两个文件，.gcno是由-ftest-coverage产生的，它包含了重建基本块图和相应的块的源码的行号的信息。.gcda是由加了-fprofile-arcs编译参数的编译后的文件运行所产生的，它包含了弧跳变的次数和其他的概要信息。gcda文件的生成需要先执行可执行文件才能生成。
lcov代码覆盖率统计工具，是gcov的延伸版本，提供程序实际执行的信息（统计某行代码被执行的次数），其基于HTML的输出通过浏览器以清晰的图表形式呈现覆盖率统计结果。locv相关详细介绍可以参考博文：[https://my.oschina.net/alphajay/blog/33725](https://my.oschina.net/alphajay/blog/33725)。
# **二、****搭建gtest与gmock开发环境**
1. 下载源码包：https://github.com/google/googletest
2. 解码：unzip googletest
![](gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%88.files/gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%881232.png)
![](https://img-blog.csdn.net/20180723203227235?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMzNjYwMjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 1) 编译gtest
①　cd googletest-master/googletest
②　创建jni目录。创建jni/Android.mk：
```
LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_CPP_EXTENSION := .cc 
LOCAL_MODULE := libgtest 
LOCAL_C_INCLUDES := include . 
LOCAL_SRC_FILES := ../src/gtest-all.cc 
include $(BUILD_STATIC_LIBRARY) 
include $(CLEAR_VARS) 
LOCAL_CPP_EXTENSION := .cc 
LOCAL_MODULE := libgtest_main 
LOCAL_C_INCLUDES := include . 
LOCAL_SRC_FILES := ../src/gtest_main.cc 
LOCAL_STATIC_LIBRARIES := libgtest 
include $(BUILD_STATIC_LIBRARY)
```
③　创建jni/Application.mk
`APP_STL := gnustl_static #STL`
④　ndk-build编译，创建静态库
![](gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%88.files/gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%881847.png)
![](https://img-blog.csdn.net/20180723203333790?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMzNjYwMjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2) 编译gmock，编译gmock与编译gtest方法类似。
①　cd googletest-master/googletest
②　创建jni目录。创建jni/Android.mk
```
LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_CPP_EXTENSION := .cc 
LOCAL_MODULE := libgmock 
LOCAL_C_INCLUDES := include . 
LOCAL_C_INCLUDES += ../../googletest/include 
LOCAL_SRC_FILES := ../src/gmock-all.cc 
include $(BUILD_STATIC_LIBRARY) 
include $(CLEAR_VARS) 
LOCAL_CPP_EXTENSION := .cc 
LOCAL_MODULE := libgmock_main 
LOCAL_C_INCLUDES := include . 
LOCAL_SRC_FILES := ../src/gmock_main.cc 
LOCAL_STATIC_LIBRARIES := libgmock 
include $(BUILD_STATIC_LIBRARY)
```
③　创建jni/Application.mk
`APP_STL := gnustl_static #STL`
④　ndk-build编译，创建静态库
![](gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%88.files/gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%882527.png)
![](https://img-blog.csdn.net/2018072320334752?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMzNjYwMjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# **三、****编译安装lcov**
1. 下载源码包：[http://ltp.sourceforge.net/coverage/lcov.php](http://ltp.sourceforge.net/coverage/lcov.php)
2. 解压：tar xvzf lcov-1.13.tar.gz
3. cd lcov-1.13
4. 修改文件：lcov-1.13/bin/genifo。将第68行
`our $gcov_tool = "gcov"; `
修改成自己的交叉编译器的gcov。
`our $gcov_tool = "/xxxx/android-ndk-r12e/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86/bin/arm-linux-androideabi-gcov"; `
比如交叉编译工具是/opt/android-ndk-r10e/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86/bin/arm-linux-androideabi-gcc，那么就改为：our $gcov_tool = "/opt/android-ndk-r10e/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86/bin/arm-linux-androideabi-gcov"
5. sudo make install
# **四、****Demo测试**
1. 目录结构：
![](gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%88.files/gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%883224.png)
![](https://img-blog.csdn.net/20180723203447259?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMzNjYwMjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
myTest.cpp
```cpp
#include "myTest.h" 
int myTest(int intValue) 
{ 
if (intValue > 255) { 
return 255; 
} else if (intValue < 100) { 
return 0; 
} else { 
return intValue; 
} 
} 
myTest.h
#ifndef MYTEST_H_ 
#define MYTEST_H_ 
int myTest(int intValue); 
#endif // MYTEST_H_
```
testCase.cpp
```cpp
#include "gtest/gtest.h" 
#include "myTest.h" 
TEST(MYTEST, 1) 
{ 
EXPECT_EQ(5, myTest(150)); 
} 
TEST(MYTEST, 2) 
{ 
EXPECT_EQ(100, myTest(150)); 
} 
TEST(MYTEST, 3) 
{ 
EXPECT_EQ(255, myTest(300)); 
} 
int main(int argc, char *argv[]) 
{ 
testing::InitGoogleTest(&argc, argv); 
return RUN_ALL_TESTS(); 
}
```
Android.mk
```
LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_MODULE := gtest 
LOCAL_SRC_FILES := ../external/libgtest.a 
include $(PREBUILT_STATIC_LIBRARY) 
include $(CLEAR_VARS) 
LOCAL_MODULE := myTest 
LOCAL_C_INCLUDES+= $(LOCAL_PATH)/../include 
LOCAL_STATIC_LIBRARIES := gtest 
LOCAL_CFLAGS := --coverage 
LOCAL_LDFLAGS := --coverage 
LOCAL_SRC_FILES := myTest.cpp 
LOCAL_SRC_FILES +=testCase.cpp 
include $(BUILD_EXECUTABLE)
```
Application.mk
```
APP_STL := gnustl_static 
APP_OPTIM := debug
```
2. ndk-build，编译生成可执行程序
![](gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%88.files/gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%884379.png)
![](https://img-blog.csdn.net/20180723203508153?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMzNjYwMjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3. 将obj/local/armeabi/下的可执行程序push到设备上。
`adb push obj/local/armeabi/myTest /data/local/tmp/`
4. 设置gcda文件生成路径，运行可执行程序
`adb shell GCOV_PREFIX=/data/local/tmp/ /data/local/tmp/myTest `
Demo运行结束如下所示：
![](gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%88.files/gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%884567.png)
![](https://img-blog.csdn.net/20180723203522704?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMzNjYwMjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
5. 将生成的.gcda文件拷贝到源码obj目录
`adb pull /data/local/tmp/code/work/test_project/obj obj `
6. 利用genhtml在当前coverage目录下生成报表。
`genhtml coverage.info --branch-coverage -o coverage`
报表效果如下：
![](https://img-blog.csdn.net/20180723203552121?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMzNjYwMjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
当前coverage目录下生![](gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%88.files/gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%884751.png)
![](gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%88.files/gtest%20gmock%20lcov%E4%BB%A3%E7%A0%81%E8%A6%86%E7%9B%96%E7%8E%87%E6%B5%8B%E8%AF%95%E6%96%B9%E6%A1%884753.png)
![](https://img-blog.csdn.net/20180723203609483?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTMzNjYwMjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
