
# CMakeList.txt的简单使用 - 小花生的博客 - CSDN博客


2018年12月16日 16:43:43[Peanut_范](https://me.csdn.net/u013841196)阅读数：153



###### CMakeList.txt的简单使用
## CMake 简介
CMake 是一个跨平台的自动化建构系统,它使用一个名为 CMakeLists.txt 的文件来描述构建过程,可以产生标准的构建文件,如 Unix 的 Makefile 或Windows Visual C++ 的 projects/workspaces 。文件 CMakeLists.txt 需要手工编写,也可以通过编写脚本进行半自动的生成。CMake 提供了比 autoconfig 更简洁的语法。在 linux 平台下使用 CMake 生成 Makefile 并编译的流程如下:
1)编写 CmakeLists.txt。
2)执行命令“cmake PATH”或者“ccmake PATH”生成 Makefile ( PATH 是 CMakeLists.txt
所在的目录 )。
3)使用 make 命令进行编译。
第一个工程
源代码分布情况:
./eg_demo/main.cpp现假设我们的项目中只有一个源文件 main.cpp
源文件main.cpp内容：
\#include <iostream>
int main()
{
    std::cout<<"Hello world"<<std::endl;
    return 0;
}注：为了构建该项目,我们需要编写文件 CMakeLists.txt 并将其与 main.cpp 放在 同一个目录下。
CMakeLists.txt内容为：
PROJECT(Hello)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
ADD_EXECUTABLE(main ${DIR_SRCS})CMakeLists.txt 的语法比较简单,由命令、注释和空格组成,其中命令是不区分大小写的,符号"\#"后面的内容被认为是注释。命令由命令名称、小括号和参数组成,参数之间使用空格进行间隔。例如对于清单2的 CMakeLists.txt 文件:第一行是一条命令,名称是 PROJECT ,参数是 Hello,该命令表示项目的名称是 Hello 。第二行的命令限定了 CMake 的版本。第三行使用命令 AUX_SOURCE_DIRECTORY 将当前目录中的源文件名称赋值给变量 DIR_SRCS 。 CMake 手册中对命令 AUX_SOURCE_DIRECTORY 的描述如下:aux_source_directory(<dir> <variable>)该命令会把参数<dir>中所有的源文件名称赋值给参数<variable>。 第四行使用命令 ADD_EXECUTABLE 指示变量 DIR_SRCS 中的源文件需要编译 成一个名称为 main 的可执行文件。
CMake的编译基本就两个步骤：
cmake  指向CMakeLists.txt所在的目录，例如cmake … 表示CMakeLists.txt在当前目录的上一级目录。cmake后会生成很多编译的中间文件以及makefile文件，所以一般建议新建一个新的目录，专门用来编译，例如
mkdir build
cd build
cmake ..
makemake根据生成makefile文件，编译程序。
camke 的运行结果:
[eg_demo]$ mkdir build
[eg_demo]$ cd build/
[build]$ cmake ..
-- The C compiler identification is GNU 4.8.5
-- The CXX compiler identification is GNU 4.8.5
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /eg_demo/build
[build]$ make
make: Warning: File `Makefile' has modification time 969 s in the future
make[1]: Warning: File `CMakeFiles/Makefile2' has modification time 969 s in the future
make[2]: Warning: File `CMakeFiles/main.dir/flags.make' has modification time 969 s in the future
Scanning dependencies of target main
make[2]: warning:  Clock skew detected.  Your build may be incomplete.
make[2]: Warning: File `CMakeFiles/main.dir/flags.make' has modification time 969 s in the future
[ 50%] Building CXX object CMakeFiles/main.dir/main.cpp.o
[100%] Linking CXX executable main
make[2]: warning:  Clock skew detected.  Your build may be incomplete.
[100%] Built target main
make[1]: warning:  Clock skew detected.  Your build may be incomplete.
make: warning:  Clock skew detected.  Your build may be incomplete.
[build]$ ./main
Hello world第二个工程
源代码分布情况:
./eg_demo/main.cpp 、test.cpp 、test.h主函数main.cpp:
\#include <iostream>
\#include "test.h"
int main()
{
    std::cout<<"Hello world"<<std::endl;
	print_test();
    return 0;
}test.cpp内容：
\#include "test.h"
void print_test()
{
	std::cout<< "src:test"<<std::endl;
}test.h中内容：
\#include <iostream>
void print_test();CMakeLists.txt内容：
PROJECT(Hello)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
ADD_EXECUTABLE(main ${DIR_SRCS})注：和第一个项目一致
camke 的运行结果：
mkdir build
[eg_demo2]$ cd build/
[build]$ cmake ..
-- The C compiler identification is GNU 4.8.5
-- The CXX compiler identification is GNU 4.8.5
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: eg_demo2/build
[build]$ make
make: Warning: File `Makefile' has modification time 970 s in the future
make[1]: Warning: File `CMakeFiles/Makefile2' has modification time 970 s in the future
make[2]: Warning: File `CMakeFiles/main.dir/flags.make' has modification time 970 s in the future
Scanning dependencies of target main
make[2]: warning:  Clock skew detected.  Your build may be incomplete.
make[2]: Warning: File `CMakeFiles/main.dir/flags.make' has modification time 970 s in the future
[ 33%] Building CXX object CMakeFiles/main.dir/main.cpp.o
[ 66%] Building CXX object CMakeFiles/main.dir/test.cpp.o
[100%] Linking CXX executable main
make[2]: warning:  Clock skew detected.  Your build may be incomplete.
[100%] Built target main
make[1]: warning:  Clock skew detected.  Your build may be incomplete.
make: warning:  Clock skew detected.  Your build may be incomplete.
[build]$ ./main
Hello world
src:test第三个工程
源代码分布情况:
./eg_demo/main.cpp
./eg_demo/src/test.cpp 、test.h程序中的内容和第二个项目一致
注：多目录工程的CmakeLists.txt编写
主文件(eg_demo)的CMakeLists.txt：
PROJECT(Hello)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
INCLUDE_DIRECTORIES(./ ./src)
ADD_SUBDIRECTORY(src)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
ADD_EXECUTABLE(main ${DIR_SRCS})
TARGET_LINK_LIBRARIES(main test)子文件（src)的CMakeLists.txt：
AUX_SOURCE_DIRECTORY(. DIR_TEST_SRCS)
ADD_LIBRARY(test ${DIR_TEST_SRCS})camke 的运行结果：
mkdir build
[eg_demo3]$ cd build
[build]$ cmake ..
-- The C compiler identification is GNU 4.8.5
-- The CXX compiler identification is GNU 4.8.5
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
CMake Warning (dev) at src/CMakeLists.txt:2 (ADD_LIBRARY):
  Policy CMP0037 is not set: Target names should not be reserved and should
  match a validity pattern.  Run "cmake --help-policy CMP0037" for policy
  details.  Use the cmake_policy command to set the policy and suppress this
  warning.
  The target name "test" is reserved or not valid for certain CMake features,
  such as generator expressions, and may result in undefined behavior.
This warning is for project developers.  Use -Wno-dev to suppress it.
-- Configuring done
-- Generating done
-- Build files have been written to: /eg_demo3/build
[build]$ make
make: Warning: File `Makefile' has modification time 937 s in the future
make[1]: Warning: File `CMakeFiles/Makefile2' has modification time 938 s in the future
make[2]: Warning: File `src/CMakeFiles/test.dir/flags.make' has modification time 937 s in the future
Scanning dependencies of target test
make[2]: warning:  Clock skew detected.  Your build may be incomplete.
make[2]: Warning: File `src/CMakeFiles/test.dir/flags.make' has modification time 937 s in the future
[ 25%] Building CXX object src/CMakeFiles/test.dir/test.cpp.o
[ 50%] Linking CXX static library libtest.a
make[2]: warning:  Clock skew detected.  Your build may be incomplete.
[ 50%] Built target test
make[2]: Warning: File `CMakeFiles/main.dir/flags.make' has modification time 934 s in the future
Scanning dependencies of target main
make[2]: warning:  Clock skew detected.  Your build may be incomplete.
make[2]: Warning: File `CMakeFiles/main.dir/flags.make' has modification time 934 s in the future
[ 75%] Building CXX object CMakeFiles/main.dir/main.cpp.o
[100%] Linking CXX executable main
make[2]: warning:  Clock skew detected.  Your build may be incomplete.
[100%] Built target main
make[1]: warning:  Clock skew detected.  Your build may be incomplete.
make: warning:  Clock skew detected.  Your build may be incomplete.
[build]$ ./main
Hello world
src:test参考：[https://www.ibm.com/developerworks/cn/linux/l-cn-cmake/index.html](https://www.ibm.com/developerworks/cn/linux/l-cn-cmake/index.html)
[https://blog.csdn.net/ktigerhero3/article/details/70313350/](https://blog.csdn.net/ktigerhero3/article/details/70313350/)
[http://www.cnblogs.com/cv-pr/p/6206921.html](http://www.cnblogs.com/cv-pr/p/6206921.html)
---
注：博众家之所长，集群英之荟萃。

