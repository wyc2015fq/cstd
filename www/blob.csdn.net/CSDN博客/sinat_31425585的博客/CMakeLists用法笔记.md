# CMakeLists用法笔记 - sinat_31425585的博客 - CSDN博客
2018年10月24日 21:58:58[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：126
做一个学习笔记，仅仅是笔记而已。
**1、设置编译选项：**
这里可以利用CMAKE_C_FLAGS（增加C编译选项）和CMAKE_CXX_FLAGS（增加C++编译选项），可以设置的编译选项有：
1) -fPIC
    fPIC的全称是 Position Independent Code， 用于生成位置无关代码。不加fPIC编译的so必须要在加载到用户程序的地址空间时重定向所有表目，它里面不能引用其它地方的代码，加了fPIC实现真正意义上的多个进程共享so文件。
2) -openmp：开启并行计算功能。
3) 设置优化程度：
    -O0表明不做优化，-O3表示设置为最高优化程度。
4) 开启c++ 11支持：-std=c++11
**2、设置编译版本要求**
```cpp
cmake_minimum_required(VERSION 3.1)
```
**3、指定项目名称**
```cpp
project(mylib)
```
**4、设置lib保存路径**
```cpp
set(CMAKE_LIBARARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/install/lib)
```
**5、设置可执行文件输出路径**
```cpp
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/install/bin)
```
**6、设置包含路径**
```cpp
include_directories(${PROJECT_SOURCE_DIR}/include)
```
**7、编译成一个库**
```cpp
add_library(mylib SHARED my.cc)
```
**8、指定依赖关系**
```cpp
target_link_libraries(mylib ${OpenCV_LIBS})
```
**9、打印信息**
```cpp
message(STATUS "Build test code for Linux x86")
```
参考资料：
[1] [https://blog.csdn.net/sinc00/article/details/44833839](https://blog.csdn.net/sinc00/article/details/44833839)
~~~~~未完待续~~~~~
