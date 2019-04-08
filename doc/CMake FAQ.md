# CMake FAQ



**本帖子适用于初学者，StepByStep的入门CMake，CMake也是一门编程语言，只不过是针对编译和链接这种程序构建的过程的语言，学习基本语法就可以初步入门，后面需要什么复杂的功能，进阶掌握需要自己去查找官方文档，后续会给出查阅的一些方式**

## CMake中怎样获得一个目录下的所有源文件 ?

```
aux_source_directory(<dir> <variable>)
将dir中所有源文件（不包括头文件）保存到变量variable中
然后可以add_executable (name ${variable})这样使用。123
```

## CMake中如何生成动态库和静态库 ?

参考ADD_LIBRARY和SET_TARGET_PROPERTIES用法

## CMake中如何使用动态库和静态库（查找库的路径）?

参考INCLUDE_DIRECTORIES, LINK_DIRECTORIES, TARGET_LINK_LIBRARIES用法

## CMake中如何指定生成文件的输出路径 ?

- 如上ADD_SUBDIRECTORY的时候指定目标二进制文件输出路径（推荐使用下面这种）
- 使用SET命令重新定义EXECUTABLE_OUTPUT_PATH和LIBRARY_OUTPUT_PATH变量来指定最终的二进制文件的位置 
  `SET(EXECUTABLE\_OUTPUT\_PATH ${PROJECT\_BINARY\_DIR}/bin) SET(LIBRARY\_OUTPUT\_PATH ${PROJECT\_BINARY\_DIR}/lib) `
  上面的两条命令通常紧跟ADD_EXECUTABLE和ADD_LIBRARY,与其写在同一个CMakeLists.txt即可

## CMake中如何增加编译选项(编译宏定义)？

使用变量CMAKE_C_FLAGS添加C编译选项 
使用变量CMAKE_CXX_FLAGS添加C++编译选项 
使用ADD_DEFINITION添加

## CMake中如何增加头文件和库文件路径？

参考INCLUDE_DIRECTORIES命令用法 
include_directories与link_directories 
可以多次调用以设置多个路径 
link_directories仅对其后面的targets起作用

## CMake中如何在屏幕上打印信息？

参考MESSAGE用法

## CMake中如何给变量赋值？

参考SET和AUX_SOURCE_DIRECTORY用法

## CMake中如何区分Debug和Release版本 ？

- 建立debug/release两目录，分别在其中执行cmake -DCMAKE_BUILD_TYPE=Debug（或Release），需要编译不同版本时进入不同目录执行make即可；

Debug版会使用参数-g；Release版使用-O3 –DNDEBUG

- 另一种设置方法——例如DEBUG版设置编译参数DDEBUG

```
IF(DEBUG_mode)
    add_definitions(-DDEBUG)
ENDIF(DEBUG_mode)123
```

在执行cmake时增加参数即可，例如cmake -D DEBUG_mode=ON

## CMake中如何设置条件编译 ？

- 一种办法：例如debug版设置编译选项DEBUG，并且更改不应改变CMakelist.txt

```
使用option command，eg：
option(DEBUG_mode "ON for debug or OFF for release" ON)
IF(DEBUG_mode)
    add_definitions(-DDEBUG)
ENDIF()

    使其生效的方法：首先cmake生成makefile，
    然后make edit_cache编辑编译选项；
    Linux下会打开一个文本框，可以更改，该完后再make生成目标文件(emacs不支持make edit_cache)

    局限：这种方法不能直接设置生成的makefile，而是必须使用命令在make前设置参数；
    对于debug、release版本，相当于需要两个目录，
    分别先cmake一次，然后分别make edit_cache一次；12345678910111213
```

- 更好的办法：在执行cmake时直接通过参数指定一个开关项，生成相应的makefile——可以这样做，例如cmake –DDEBUGVERSION=ON
- *

## CMake中如何设置编译依赖项 ？

用于确保编译目标项目前依赖项必须先构建好：add_dependencies

## CMake中怎样在IF语句中使用字串或数字比较 ？

数字比较LESS、GREATER、EQUAL，字串比STRLESS、STRGREATER、STREQUAL，

```
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
set(AAA abc)
IF(AAA STREQUAL abc)
    message(STATUS "true")   #应该打印true
ENDIF()12345
```

## CMake中怎样根据OS指定编译选项 ？

```
IF( APPLE )
IF( UNIX )
IF( WIN32 )123
```

## CMake中windows VS中输出路径的问题 ？

如果要修改输出路径，通常采用SET(LIBRARY_OUTPUT_PATH XXX)即可，但是在WIN32下，会自动放到${XXX}/Debug或Release目录下，比如有一个插件，希望在输出目录的plugins子目录下，但是如果设定为SET(LIBRARY_OUTPUT_PATH XXX/plugins)，则文件输出目录为${XXX}/plugins/Debug，这显然不是我们希望的。做法是设置PREFIX，如SET_TARGET_PROPERTIES(abc PROPERTIES PREFIX plugins/)。同理，如果输出路径不管Debug、Release都放置一样，则设置PREFIX为../。