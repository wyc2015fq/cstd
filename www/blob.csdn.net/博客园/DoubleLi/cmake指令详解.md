# cmake指令详解 - DoubleLi - 博客园






**所需文件名：CmakeLists.txt,在需要操作的每个目录下都需要**

# **PROJECT(工程名字)**

    这条指令会自动创建两个变量：

<projectname>_BINARY_DIR(二进制文件保存路径)    <projectname>_SOURCE_DIR（源代码路径）

cmake系统也帮助我们预定义了PROJECT_BINARY_DIR和PROJECT_SOURCE_DIR其值与上述对应相等



# **SET(变量名 变量值)**

    SET(VAR [VALUE] [CACHE TYPEDOCSTRING [FORCE]])

    SET(SRC_LIST main.c t1.ct2.c)

    SET(SRC_LIST main.c)



# MESSAGE



**MESSAGE([SEND_ERROR | STATUS | FATAL_ERROR] “message to display” …)**
向终端输出用户定义的信息或变量的值
SEND_ERROR, 产生错误,生成过程被跳过
STATUS, 输出前缀为—的信息
FATAL_ERROR, 立即终止所有cmake过程



# **ADD_EXECUTABLE**



**ADD_EXECUTABLE(可执行文件名  生成该可执行文件的源文件)**

说明源文件需要编译出的可执行文件名

例：

 ADD_EXECUTABLE(hello ${SRC_LIST})

说明SRC_LIST变量中的源文件需要编译出名为hello的可执行文件



# **ADD_SUBDIRECTORY**



**ADD_SUBDIRECTORY(src_dir [binary_dir] [EXCLUDE_FROM_ALL])**
向当前工程添加存放源文件的子目录，并可以指定中间二进制和目标二进制的存放位置
EXCLUDE_FROM_ALL含义：将这个目录从编译过程中排除



**SET(EXECUTABLE_OUTPUT_PATH${PROJECT_BINARY_DIR}/bin)更改生成的可执行文件路径**

**SET(LIBRARY_OUTPUT_PATH${PROJECT_BINARY_DIR}/lib)更改生成的库文件路径**







# **ADD_LIBRARY**



**ADD_LIBRARY(libname [SHARED|STATIC|MODULE] [EXCLUDE_FROM_ALL] source1 source2 ... sourceN)**

生成动态静态库


例：

ADD_LIBRARY(hello SHARED ${LIBHELLO_SRC})



# **SET_TARGET_PROPERTIES**

设置目标的一些属性来改变它们构建的方式。

  set_target_properties(target1 target2 ...
                        PROPERTIES prop1 value1
                        prop2 value2 ...)
　　为一个目标设置属性。该命令的语法是列出所有你想要变更的文件，然后提供你想要设置的值。你能够使用任何你想要的属性/值对，并且在随后的代码中调用**GET_TARGET_PROPERTY**命令取出属性的值。

　　影响一个目标输出文件的名字的属性详述如下。

PREFIX和SUFFIX属性覆盖了默认的目标名前缀（比如lib）和后缀（比如.so）。

IMPORT_PREFIX和IMPORT_SUFFIX是与之等价的属性，不过针对的是DLL（共享库目标）的导入库。

在构建目标时，OUTPUT_NAME属性设置目标的真实名字，并且可以用来辅助创建两个具有相同名字的目标，即使CMake需要唯一的逻辑目标名。<CONFIG>_OUTPUT_NAME可以为不同的配置设置输出的目标名字。当目标在指定的配置名<CONFIG>（全部大写，例如DEBUG_POSTFIX）下被构建时，<CONFIG>_POSTFIX为目标的真实名字设置一个后缀。该属性的值在目标创建时被初始化为CMAKE_<CONFIG>_POSTFIX的值（可执行目标除外，因为较早的CMake版本不会为可执行文件使用这个属性。）

　　LINK_FLAGS属性可以用来为一个目标的链接阶段添加额外的标志。LINK_FLAGS_<CONFIG>将为配置<CONFIG>添加链接标志，例如DEBUG，RELEASE，MINSIZEREL，RELWITHDEBINFO。DEFINE_SYMBOL属性设置了编译一个共享库中的源文件时才会被定义的预处理器符号名。如果这个值没有被设置的话，那么它会被设置为默认值target_EXPORTS（如果目标不是一个合法的C标示符的话可以用一些替代标志）。这对于检测头文件是包含在它们的库以内还是以外很有帮助，从而可以合理设置dllexport/dllimport修饰符（注意，只有在编译到的时候，这个符号才会被定义；因此猜测在代码中，判断预处理符号是否被定义可以知道依赖库是导入的还是导出的——译注）。COMPILE_FLAGS属性可以设置附加的编译器标志，它们会在构建目标内的源文件时被用到。它也可以用来传递附加的预处理器定义。

　　LINKER_LANGUAGE属性用来改变链接可执行文件或共享库的工具。默认的值是设置与库中的文件相匹配的语言。CXX和C是这个属性的公共值。

　　对于共享库，VERSION和SOVERSION属性分别可以用来指定构建的版本号以及API版本号。当构建或者安装时，如果平台支持符号链接并且链接器支持so名字，那么恰当的符号链接会被创建。如果只指定两者中的一个，缺失的另一个假定为具有相同的版本号。对于可执行文件，VERSION可以被用来指定构建版本号。当构建或者安装时，如果该平台支持符号链接，那么合适的符号链接会被创建。对于在Windows系统而言，共享库和可执行文件的VERSION属性被解析成为一个"major.minor"的版本号。这些版本号被用做该二进制文件的镜像版本。

　　还有一些属性用来指定RPATH规则。INSTALL_RPATH是一个分号分隔的list，它指定了在安装目标时使用的rpath（针对支持rpath的平台而言）（-rpath在gcc中用于在编译时指定加载动态库的路径；优先级较系统库路径要高。详情参见[http://www.cmake.org/Wiki/CMake_RPATH_handling#What_is_RPATH_.3F](http://www.cmake.org/Wiki/CMake_RPATH_handling#What_is_RPATH_.3F)——译注）。INSTALL_RPATH_USE_LINK_PATH是一个布尔值属性，如果它被设置为真，那么在链接器的搜索路径中以及工程之外的目录会被附加到INSTALL_RPATH之后。SKIP_BUILD_RPATH是一个布尔值属性，它指定了是否跳过一个rpath的自动生成过程，从而可以从构建树开始运行。BUILD_WITH_INSTALL_RPATH是一个布尔值属性，它指定了是否将在构建树上的目标与INSTALL_RPATH链接。该属性要优先于SKIP_BUILD_RPATH，因此避免了安装之前的重新链接。INSTALL_NAME_DIR是一个字符串属性，它用于在Mac OSX系统上，指定了被安装的目标中使用的共享库的"install_name"域的目录部分。如果目标已经被创建，变量CMAKE_INSTALL_RPATH, CMAKE_INSTALL_RPATH_USE_LINK_PATH, CMAKE_SKIP_BUILD_RPATH, CMAKE_BUILD_WITH_INSTALL_RPATH和CMAKE_INSTALL_NAME_DIR的值会被用来初始化这个属性。

　　PROJECT_LABEL属性可以用来在IDE环境，比如visual studio，中改变目标的名字。 VS_KEYWORD可以用来改变visual studio的关键字，例如如果该选项被设置为Qt4VSv1.0的话，QT集成将会运行得更好。

　　VS_SCC_PROJECTNAME, VS_SCC_LOCALPATH, VS_SCC_PROVIDER可以被设置，从而增加在一个VS工程文件中对源码控制绑定的支持。

　　PRE_INSTALL_SCRIPT和POST_INSTALL_SCRIPT属性是在安装一个目标之前及之后指定运行CMake脚本的旧格式。只有当使用旧式的INSTALL_TARGETS来安装目标时，才能使用这两个属性。使用INSTALL命令代替这种用法。

　　EXCLUDE_FROM_DEFAULT_BUILD属性被visual studio生成器使用。如果属性值设置为1，那么当你选择"构建解决方案"时，目标将不会成为默认构建的一部分。


SET_TARGET_PROPERTIES(hello_static PROPERTIES OUTPUT_NAME "hello")



同时生成动态静态库

    ADD_LIBRARY(hello SHARED ${LIBHELLO_SRC})

    ADD_LIBRARY(hello_static STATIC ${LIBHELLO_SRC})

    SET_TARGET_PROPERTIES(hello_static PROPERTIES OUTPUT_NAME "hello")

        SET_TARGET_PROPERTIES(hello PROPERTIES CLEAN_DIRECT_OUTPUT 1)

SET_TARGET_PROPERTIES(hello_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

控制版本

        SET_TARGET_PROPERTIES(hello PROPERTIES VERSION 1.2 SOVERSION 1)

VERSION指代动态库版本，SOVERSION指代API版本。



#  INSTALL

**INSTALL(TARGETS hellohello_static LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)**

INSTALL(FILES hello.h DESTINATIONinclude/hello)

注意，静态库要使用ARCHIVE关键字

cmake -DCMAKE_INSTALL_PREFIX=/usr ..[路径]





# **INCLUDE_DIRECTORIES（追加标志 头文件路径）**

**INCLUDE_DIRECTORIES([AFTER|BEFORE] [SYSTEM] dir1 dir2 ...)**

 向工程添加多个特定的头文件搜索路径，路径之间用空格分隔，如果路径包含空格，可以使用双引号将它括起来。默认的行为是追加到当前头文件搜索路径的后面。有如下两种方式可以控制搜索路径添加的位置：
- CMAKE_INCLUDE_DIRECTORIES_BEFORE,通过SET这个cmake变量为on,可以将添加的头文件搜索路径放在已有路径的前面
- 通过AFTER或BEFORE参数,也可以控制是追加还是置前





# **LINK_DIRECTORIES（库文件路径）**

LINK_DIRECTORIES(directory1 directory2 ...)



# **TARGET_LINK_LIBRARIES **

**设置目标要连接库文件的名称**

    TARGET_LINK_LIBRARIES(target library1 <debug | optimized> library2 ..)

    TARGET_LINK_LIBRARIES(main hello)  # 连接libhello.so库

    TARGET_LINK_LIBRARIES(main libhello.a)

    TARGET_LINK_LIBRARIES(main libhello.so)

#  环境变量

**使用$ENV{NAME}指令就可以调用系统的环境变量**





# **系统信息**

1,CMAKE_MAJOR_VERSION，CMAKE主版本号，比如2.4.6中的2

2,CMAKE_MINOR_VERSION，CMAKE次版本号，比如2.4.6中的4

3,CMAKE_PATCH_VERSION，CMAKE补丁等级，比如2.4.6 中的6

4,CMAKE_SYSTEM，系统名称，比如[Linux](http://lib.csdn.net/base/linux)-2.6.22

5,CMAKE_SYSTEM_NAME，不包含版本的系统名，比如[linux](http://lib.csdn.net/base/linux)

6,CMAKE_SYSTEM_VERSION，系统版本，比如2.6.22

7,CMAKE_SYSTEM_PROCESSOR，处理器名称，比如i686.

8,UNIX，在所有的类UNIX平台为TRUE，包括OS X和cygwin

9,WIN32，在所有的win32平台为TRUE，包括cygwin



# **主要的开关选项：**

1，CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS，用来控制IF ELSE语句的书写方式，在

下一节语法部分会讲到。

2，BUILD_SHARED_LIBS

这个开关用来控制默认的库编译方式，如果不进行设置，使用ADD_LIBRARY并没有指定库

类型的情况下，默认编译生成的库都是静态库。

如果SET(BUILD_SHARED_LIBS ON)后，默认生成的为动态库。

３，CMAKE_C_FLAGS

设置C编译选项，也可以通过指令ADD_DEFINITIONS()添加。

4，CMAKE_CXX_FLAGS

设置C++编译选项，也可以通过指令ADD_DEFINITIONS()添加。



# **ADD_DEFINITIONS**

**ADD_DEFINITIONS(-DENABLE_DEBUG-DABC)，定义宏**





# **ADD_DEPENDENCIES**

定义target依赖的其他target，确保在编译本target之前，其他的target已经被构建。

ADD_DEPENDENCIES(target-name depend-target1depend-target2 ...)

 让一个顶层目标依赖于其他的顶层目标。一个顶层目标是由命令ADD_EXECUTABLE，ADD_LIBRARY，或者ADD_CUSTOM_TARGET产生的目标。为这些命令的输出引入依赖性可以保证某个目标在其他的目标之前被构建。查看ADD_CUSTOM_TARGET和ADD_CUSTOM_COMMAND命令的DEPENDS选项，可以了解如何根据自定义规则引入文件级的依赖性。查看SET_SOURCE_FILES_PROPERTIES命令的OBJECT_DEPENDS选项，可以了解如何为目标文件引入文件级的依赖性。



# **ADD_TEST与ENABLE_TESTING指令**

**ADD_TEST(testname Exenamearg1 arg2 ...)**

如果没有在同一个CMakeLists.txt中打开ENABLE_TESTING()指令，任何ADD_TEST都是无效的。

ADD_TEST(mytest${PROJECT_BINARY_DIR}/bin/main)

ENABLE_TESTING()

生成Makefile后，就可以运行make test来执行[测试](http://lib.csdn.net/base/softwaretest)了。



# **AUX_SOURCE_DIRECTORY**

    AUX_SOURCE_DIRECTORY(dirVARIABLE)

    作用是发现一个目录下所有的源代码文件并将列表存储在一个变量中

    AUX_SOURCE_DIRECTORY(. SRC_LIST)

ADD_EXECUTABLE(main ${SRC_LIST})



# **CMAKE_MINIMUM_REQUIRED**

**CMAKE_MINIMUM_REQUIRED(VERSION2.5 FATAL_ERROR)最低版本CMAKE要求**



# **EXEC_PROGRAM**

**EXEC_PROGRAM(Executable[directory in which to run]**

[ARGS <arguments to executable>]

[OUTPUT_VARIABLE <var>]

[RETURN_VALUE <var>])

用于在指定的目录运行某个程序，通过ARGS添加参数，如果要获取输出和返回值，可通过

OUTPUT_VARIABLE和RETURN_VALUE分别定义两个变量.

    举个简单的例子，我们要在src目录执行ls命令，并把结果和返回值存下来。

可以直接在src/CMakeLists.txt中添加：

EXEC_PROGRAM(ls ARGS "*.c" OUTPUT_VARIABLE LS_OUTPUTRETURN_VALUE LS_RVALUE)

IF(not LS_RVALUE)

MESSAGE(STATUS "ls result: "${LS_OUTPUT})

ENDIF(not LS_RVALUE)



# **FILE指令**

文件操作指令，基本语法为:


file(WRITE filename "message to write"... )
  file(APPEND filename "message to write"... )
  file(READ filename variable [LIMIT numBytes] [OFFSET offset] [HEX])
  file(STRINGS filename variable [LIMIT_COUNT num]
       [LIMIT_INPUT numBytes] [LIMIT_OUTPUT numBytes]
       [LENGTH_MINIMUM numBytes] [LENGTH_MAXIMUM numBytes]
       [NEWLINE_CONSUME] [REGEX regex]
       [NO_HEX_CONVERSION])
  file(GLOB variable [RELATIVE path] [globbing expressions]...)
  file(GLOB_RECURSE variable [RELATIVE path] 
       [FOLLOW_SYMLINKS] [globbing expressions]...)
  file(RENAME <oldname> <newname>)
  file(REMOVE [file1 ...])
  file(REMOVE_RECURSE [file1 ...])
  file(MAKE_DIRECTORY [directory1 directory2 ...])
  file(RELATIVE_PATH variable directory file)
  file(TO_CMAKE_PATH path result)
  file(TO_NATIVE_PATH path result)
  file(DOWNLOAD url file [TIMEOUT timeout] [STATUS status] [LOG log]
       [EXPECTED_MD5 sum] [SHOW_PROGRESS])


　　WRITE选项将会写一条消息到名为filename的文件中。如果文件已经存在，该命令会覆盖已有的文件；如果文件不存在，它将创建该文件。

　　APPEND选项和WRITE选项一样，将会写一条消息到名为filename的文件中，只是该消息会附加到文件末尾。

　　READ选项将会读一个文件中的内容并将其存储在变量里。读文件的位置从offset开始，最多读numBytes个字节。如果指定了HEX参数，二进制代码将会转换为十六进制表达方式，并存储在变量里。

　　STRINGS将会从一个文件中将一个ASCII字符串的list解析出来，然后存储在variable变量中。文件中的二进制数据会被忽略。回车换行符会被忽略。它也可以用在Intel的Hex和Motorola的S-记录文件；读取它们时，它们会被自动转换为二进制格式。可以使用NO_HEX_CONVERSION选项禁止这项功能。LIMIT_COUNT选项设定了返回的字符串的最大数量。LIMIT_INPUT设置了从输入文件中读取的最大字节数。LIMIT_OUTPUT设置了在输出变量中存储的最大字节数。LENGTH_MINIMUM设置了要返回的字符串的最小长度；小于该长度的字符串会被忽略。LENGTH_MAXIMUM设置了返回字符串的最大长度；更长的字符串会被分割成不长于最大长度的字符串。NEWLINE_CONSUME选项允许新行被包含到字符串中，而不是终止它们。REGEX选项指定了一个待返回的字符串必须满足的正则表达式。典型的使用方式是：

```
file(STRINGS myfile.txt myfile)
```

该命令在变量myfile中存储了一个list，该list中每个项是输入文件中的一行文本。
　　GLOB选项将会为所有匹配查询表达式的文件生成一个文件list，并将该list存储进变量variable里。文件名查询表达式与正则表达式类似，只不过更加简单。如果为一个表达式指定了RELATIVE标志，返回的结果将会是相对于给定路径的相对路径。文件名查询表达式的例子有：
   *.cxx      - 匹配所有扩展名为cxx的文件。
   *.vt?      - 匹配所有扩展名是vta,...,vtz的文件。
   f[3-5].txt - 匹配文件f3.txt, f4.txt, f5.txt。
　　GLOB_RECURSE选项将会生成一个类似于通常的GLOB选项的list，只是它会寻访所有那些匹配目录的子路径并同时匹配查询表达式的文件。作为符号链接的子路径只有在给定FOLLOW_SYMLINKS选项或者cmake策略CMP0009被设置为NEW时，才会被寻访到。参见cmake --help-policy CMP0009 查询跟多有用的信息。

使用递归查询的例子有：

```
/dir/*.py  - 匹配所有在/dir及其子目录下的python文件。
```

　　MAKE_DIRECTORY选项将会创建指定的目录，如果它们的父目录不存在时，同样也会创建。（类似于mkdir命令——译注）

　　RENAME选项对同一个文件系统下的一个文件或目录重命名。（类似于mv命令——译注）

　　REMOVE选项将会删除指定的文件，包括在子路径下的文件。（类似于rm命令——译注）

　　REMOVE_RECURSE选项会删除给定的文件以及目录，包括非空目录。（类似于rm -r 命令——译注）

　　RELATIVE_PATH选项会确定从direcroty参数到指定文件的相对路径。

　　TO_CMAKE_PATH选项会把path转换为一个以unix的 / 开头的cmake风格的路径。输入可以是一个单一的路径，也可以是一个系统路径，比如"$ENV{PATH}"。注意，在调用TO_CMAKE_PATH的ENV周围的双引号只能有一个参数(Note the double quotes around the ENV call TO_CMAKE_PATH only takes one argument. 原文如此。quotes和后面的takes让人后纠结，这句话翻译可能有误。欢迎指正——译注)。

　　TO_NATIVE_PATH选项与TO_CMAKE_PATH选项很相似，但是它会把cmake风格的路径转换为本地路径风格：windows下用\，而unix下用/。

　　DOWNLOAD 将给定的URL下载到指定的文件中。如果指定了LOG var选项，下载日志将会被输出到var中。如果指定了STATUS var选项，下载操作的状态会被输出到var中。该状态返回值是一个长度为2的list。list的第一个元素是操作的数字返回值，第二个返回值是错误的字符串值。错误信息如果是数字0，操作中没有发生错误。如果指定了TIMEOUT time选项，在time秒之后，操作会超时退出；time应该是整数。如果指定了EXPECTED_MD5 sum选项，下载操作会认证下载的文件的实际MD5和是否与期望值匹配。如果不匹配，操作将返回一个错误。如果指定了SHOW_PROGRESS选项，进度信息会以状态信息的形式被打印出来，直到操作完成。

　　file命令还提供了COPY和INSTALL两种格式：
  file(<COPY|INSTALL> files... DESTINATION <dir>
       [FILE_PERMISSIONS permissions...]
       [DIRECTORY_PERMISSIONS permissions...]
       [NO_SOURCE_PERMISSIONS] [USE_SOURCE_PERMISSIONS]
       [FILES_MATCHING]
       [[PATTERN <pattern> | REGEX <regex>]
        [EXCLUDE] [PERMISSIONS permissions...]] [...])
　　COPY版本把文件、目录以及符号连接拷贝到一个目标文件夹。相对输入路径的评估是基于当前的源代码目录进行的，相对目标路径的评估是基于当前的构建目录进行的。复制过程将保留输入文件的时间戳；并且如果目标路径处存在同名同时间戳的文件，复制命令会把它优化掉。赋值过程将保留输入文件的访问权限，除非显式指定权限或指定NO_SOURCE_PERMISSIONS选项（默认是USE_SOURCE_PERMISSIONS）。参见install(DIRECTORY)命令中关于权限（permissions），PATTERN，REGEX和EXCLUDE选项的文档。

　　INSTALL版本与COPY版本只有十分微小的差别：它会打印状态信息，并且默认使用NO_SOURCE_PERMISSIONS选项。install命令生成的安装脚本使用这个版本（它会使用一些没有在文档中涉及的内部使用的选项。）





**INCLUDE指令**

**用来载入CMakeLists.txt文件，也用于载入预定义的cmake模块.**

INCLUDE(file1 [OPTIONAL])

INCLUDE(module [OPTIONAL])

OPTIONAL参数的作用是文件不存在也不会产生错误。

你可以指定载入一个文件，如果定义的是一个模块，那么将在CMAKE_MODULE_PATH中搜

索这个模块并载入。

载入的内容将在处理到 INCLUDE 语句时直接执行。

[https://cmake.org/cmake/help/v3.0/command/include.html](https://cmake.org/cmake/help/v3.0/command/include.html)



**FIND_系列指令主要包含一下指令：**

FIND_FILE(<VAR> name1 path1 path2 ...)

VAR变量代表找到的文件全路径，包含文件名

FIND_LIBRARY(<VAR> name1 path1 path2 ...)

VAR变量表示找到的库全路径，包含库文件名

FIND_PATH(<VAR> name1 path1 path2 ...)

VAR变量代表包含这个文件的路径。

FIND_PROGRAM(<VAR> name1 path1 path2 ...)

VAR变量代表包含这个程序的全路径。

FIND_PACKAGE(<name> [major.minor] [QUIET][NO_MODULE] [[REQUIRED|COMPONENTS] [componets...]])

用来调用预定义在CMAKE_MODULE_PATH下的Find<name>.cmake模块，你也可以自己

定义Find<name>模块，通过SET(CMAKE_MODULE_PATH dir)将其放入工程的某个目录

中供工程使用

FIND_LIBRARY示例：

FIND_LIBRARY(libX X11 /usr/lib)



IF(NOT libX)

MESSAGE(FATAL_ERROR “libX not found”)

ENDIF(NOT libX)





**原本的条件语句**

IF(WIN32)

MESSAGE(STATUS “This is windows.”)

#作一些Windows相关的操作

ELSE(WIN32)

MESSAGE(STATUS “This is not windows”)

#作一些非Windows相关的操作

ENDIF(WIN32)

通过设置SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

上述代码可写为

IF(WIN32)

ELSE()

ENDIF()

还可以

IF(WIN32)

#do something related to WIN32

ELSEIF(UNIX)

#do something related to UNIX

ELSEIF(APPLE)

#do something related to APPLE

ENDIF(WIN32)



WHILE指令的语法是：

WHILE(condition)

COMMAND1(ARGS ...)

COMMAND2(ARGS ...)

...

ENDWHILE(condition)



FOREACH指令的使用方法有三种形式：

列表

FOREACH(loop_var arg1 arg2...)

COMMAND1(ARGS ...)

COMMAND2(ARGS ...)

...

ENDFOREACH(loop_var)

像我们前面使用的AUX_SOURCE_DIRECTORY的例子

AUX_SOURCE_DIRECTORY(. SRC_LIST)

FOREACH(F ${SRC_LIST})

MESSAGE(${F})

ENDFOREACH(F)



范围

FOREACH(loop_var RANGE total)

ENDFOREACH(loop_var)

从0到total以１为步进举例如下：

FOREACH(VAR RANGE 10)

MESSAGE(${VAR})

ENDFOREACH(VAR)

最终得到的输出是：0-10

范围和步进

FOREACH(loop_var RANGE start stop [step])

ENDFOREACH(loop_var)

从start开始到stop结束，以step为步进，举例如下

FOREACH(A RANGE 5 15 3)

MESSAGE(${A})

ENDFOREACH(A)

最终得到的结果是：

5

8

11

14

这个指令需要注意的是，直到遇到ENDFOREACH指令，整个语句块才会得到真正的执行。







**FIND_PACKAGE(CURL)**

IF(CURL_FOUND)

INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(curltest${CURL_LIBRARY})

ELSE(CURL_FOUND)

MESSAGE(FATAL_ERROR ”CURL library not found”)

ENDIF(CURL_FOUND)



**可以自己定义一些FIND_PACKAGE包，比如**

定义cmake/FindHELLO.cmake模块





FIND_PATH(HELLO_INCLUDE_DIR hello.h /usr/include/hello /usr/local/include/hello)

FIND_LIBRARY(HELLO_LIBRARY NAMES hello PATH /usr/lib /usr/local/lib)

IF (HELLO_INCLUDE_DIR AND HELLO_LIBRARY)

SET(HELLO_FOUND TRUE)

ENDIF (HELLO_INCLUDE_DIR AND HELLO_LIBRARY)

IF (HELLO_FOUND)

IF (NOT HELLO_FIND_QUIETLY)

MESSAGE(STATUS "FoundHello: ${HELLO_LIBRARY}")

ENDIF (NOT HELLO_FIND_QUIETLY)

ELSE (HELLO_FOUND)

IF (HELLO_FIND_REQUIRED)

MESSAGE(FATAL_ERROR"Could not find hello library")

ENDIF (HELLO_FIND_REQUIRED)

ENDIF (HELLO_FOUND)



那么可以使用

FIND_PACKAGE(HELLO)

IF(HELLO_FOUND)

ADD_EXECUTABLE(hello main.c)

INCLUDE_DIRECTORIES(${HELLO_INCLUDE_DIR})

TARGET_LINK_LIBRARIES(hello${HELLO_LIBRARY})

ENDIF(HELLO_FOUND)

来操作，不过操作之前需要设置cmake搜索路径

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)



[http://blog.csdn.net/lsg32/article/details/7082154](http://blog.csdn.net/lsg32/article/details/7082154)









