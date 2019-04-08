# CMake总结





# 预定义变量

## 基本变量

- PROJECT_SOURCE_DIR cmake命令后紧跟的目录，一般是工程的根目录
- PROJECT_BINARY_DIR 执行cmake命令的目录,通常是${PROJECT_SOURCE_DIR}/build
- CMAKE_INCLUDE_PATH 系统环境变量,非cmake变量
- CMAKE_LIBRARY_PATH 系统环境变量,非cmake变量
- CMAKE_CURRENT_SOURCE_DIR 当前处理的CMakeLists.txt所在的路径
- CMAKE_CURRENT_BINARY_DIR target编译目录 
  使用ADD_SURDIRECTORY(src bin)可以更改此变量的值 
  SET(EXECUTABLE_OUTPUT_PATH <新路径>)并不会对此变量有影响,只是改变了最终目标文件的存储路径
- CMAKE_CURRENT_LIST_FILE 输出调用这个变量的CMakeLists.txt的完整路径
- CMAKE_CURRENT_LIST_LINE 输出这个变量所在的行
- CMAKE_MODULE_PATH 定义自己的cmake模块所在的路径 
  这个变量用于定义自己的cmake模块所在的路径，如果你的工程比较复杂，有可能自己编写一些cmake模块，比如SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake),然后可以用INCLUDE命令来调用自己的模块
- EXECUTABLE_OUTPUT_PATH 重新定义目标二进制可执行文件的存放位置
- LIBRARY_OUTPUT_PATH 重新定义目标链接库文件的存放位置
- PROJECT_NAME 返回通过PROJECT指令定义的项目名称
- CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS 用来控制IF ELSE语句的书写方式

## 环境变量

- 使用$ENV{NAME}可以调用系统的环境变量，比如： 
  MESSAGE（STATUS “HOME dir：”$ENV{HOME}）

- 设置环境变量的方式，SET(ENV{name} value)

- CMAKE_INCLUDE_CURRENT_DIR

   

  - 自动添加CMAKE_CURRENT_BINARY_DIR和CMAKE_CURRENT_SOURCE_DIR到当前处理 
    的CMakeLists.txt。 
    相当于在每个CMakeLists.txt加入： 
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

- CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE

   

  - 将工程提供的头文件目录始终至于系统头文件目录的前面，当你定义的头文件确实跟系统发 
    生冲突时可以提供一些帮助。

- CMAKE_INCLUDE_PATH、CMAKE_LIBRARY_PATH

   

  - 务必注意，这两个是环境变量而不是cmake变量。
  - 使用方法是要在bash中用export或者在csh中使用set命令设置或者CMAKE_INCLUDE_PATH=/home/include cmake ..等方式。

> 也就是，如果头文件没有存放在常规路径(/usr/include, /usr/local/include等)， 
> 则可以通过这些变量就行弥补。 
> 比如hello.h存放在/usr/include/hello目录，所以直接查找肯定是找不到的。 
> 前面我们直接使用了绝对路径INCLUDE_DIRECTORIES(/usr/include/hello)告诉工 
> 程这个头文件目录。为了将程序更智能一点， 
> 我们可以使用CMAKE_INCLUDE_PATH来进行，使用bash的方法 
> 如下： 
> export CMAKE_INCLUDE_PATH=/usr/include/hello 
> 然后在头文件中将INCLUDE_DIRECTORIES(/usr/include/hello)替换为： 
> FIND_PATH(myHeader hello.h) 
> IF(myHeader) 
> INCLUDE_DIRECTORIES(${myHeader}) 
> ENDIF(myHeader)
>
> 上述的一些指令我们在后面会介绍。 
> 这里简单说明一下，FIND_PATH用来在指定路径中搜索文件名，比如： 
> FIND_PATH(myHeader NAMES hello.h PATHS /usr/include 
> /usr/include/hello) 
> 这里我们没有指定路径，但是，cmake仍然可以帮我们找到hello.h存放的路径，就是因 
> 为我们设置了环境变量CMAKE_INCLUDE_PATH。 
> 如果你不使用FIND_PATH，CMAKE_INCLUDE_PATH变量的设置是没有作用的，你不能指 
> 望它会直接为编译器命令添加参数-I。 
> 以此为例，CMAKE_LIBRARY_PATH可以用在FIND_LIBRARY中。 
> 同样，因为这些变量直接为FIND_指令所使用，所以所有使用FIND_指令的cmake模块都 
> 会受益。

## 系统信息

- CMAKE_MAJOR_VERSION cmake主版本号,如3.2.2中的3
- CMAKE_MINOR_VERSION cmake次版本号,如3.2.2中的2
- CMAKE_PATCH_VERSION cmake补丁等级,如3.2.2中的2
- CMAKE_SYSTEM 系统名称,例如Linux-2.6.22
- CAMKE_SYSTEM_NAME 不包含版本的系统名,如Linux
- CMAKE_SYSTEM_VERSION 系统版本,如2.6.22
- CMAKE_SYSTEM_PROCESSOR 处理器名称,如i686
- UNIX 在所有的类UNIX平台为TRUE,包括OS X和cygwin
- WIN32 在所有的win32平台为TRUE,包括cygwin

## 开关选项

- BUILD_SHARED_LIBS 控制默认的库编译方式。如果未进行设置,使用ADD_LIBRARY时又没有指定库类型,默认编译生成的库都是静态库
- CMAKE_C_FLAGS 设置C编译选项，也可以通过指令ADD_DEFINITIONS()添加
- CMAKE_CXX_FLAGS 设置C++编译选项，也可以通过指令ADD_DEFINITIONS()添加。
- CMAKE_C_COMPILER：指定C编译器
- CMAKE_CXX_COMPILER：指定C++编译器
- CMAKE_BUILD_TYPE:：build 类型(Debug, Release, …)，CMAKE_BUILD_TYPE=Debug

## 变量的使用

- 在CMakeLists.txt中指定，使用set
- cmake命令中使用，如cmake -DBUILD_SHARED_LIBS=OFF















# 常用命令

## 基本语法规则

- cmake变量使用${}方式取值,但是在IF控制语句中是直接使用变量名

- 环境变量使用$ENV{}方式取值,使用SET(ENV{VAR} VALUE)赋值

- 指令(参数1 参数2…) 
  参数使用括弧括起,参数之间使用空格或分号分开。

  ```cmake
  以ADD_EXECUTABLE指令为例：
  ADD_EXECUTABLE(hello main.c func.c)或者
  ADD_EXECUTABLE(hello main.c;func.c)123
  ```

- 指令是大小写无关的,参数和变量是大小写相关的。推荐你全部使用**大写**指令。

## 基本命令

### PROJECT 

PROJECT(projectname [CXX] [C] [Java]) 
指定工程名称,并可指定工程支持的语言。支持语言列表可忽略,默认支持所有语言；指定项目名称，生成的VC项目的名称；

### SET 

SET(VAR [VALUE] [CACHE TYPE DOCSTRING [FORCE]]) 
定义变量(可以定义多个VALUE,如SET(SRC_LIST main.c util.c reactor.c))

### MESSAGE 

MESSAGE([SEND_ERROR | STATUS | FATAL_ERROR] “message to display” …) 
向终端输出用户定义的信息或变量的值 
SEND_ERROR, 产生错误,生成过程被跳过 
STATUS, 输出前缀为–的信息 
FATAL_ERROR, 立即终止所有cmake过程

### ADD_EXECUTABLE

ADD_EXECUTABLE(bin_file_name ${SRC_LIST})

生成可执行文件，编译可执行程序，指定编译，好像也可以添加.o文件

- add_executable (helloDemo demo.cxx demo_b.cxx) #将cxx编译成可执行文件

### ADD_LIBRARY 

ADD_LIBRARY(libname [SHARED | STATIC | MODULE] [EXCLUDE_FROM_ALL] SRC_LIST) 
生成动态库或静态库 
SHARED 动态库 
STATIC 静态库 
MODULE 在使用dyld的系统有效,若不支持dyld,等同于SHARED 
EXCLUDE_FROM_ALL 表示该库不会被默认构建

### SET_TARGET_PROPERTIES 

设置输出的名称,设置动态库的版本和API版本

### CMAKE_MINIMUM_REQUIRED 

CMAKE_MINIMUM_REQUIRED(VERSION version_number [FATAL_ERROR]) 
声明CMake的版本要求

### ADD_SUBDIRECTORY 

ADD_SUBDIRECTORY(src_dir [binary_dir] [EXCLUDE_FROM_ALL]) 
向当前工程添加存放源文件的子目录,并可以指定中间二进制和目标二进制的存放位置 
EXCLUDE_FROM_ALL含义：将这个目录从编译过程中排除

### INCLUDE_DIRECTORIES

INCLUDE_DIRECTORIES([AFTER | BEFORE] [SYSTEM] dir1 dir2 … )

向工程添加多个特定的头文件搜索路径,路径之间用空格分隔,如果路径包含空格,可以使用双引号将它括起来,默认的行为为追加到当前头文件搜索路径的后面。指定头文件的搜索路径，相当于指定gcc的-I参数。有如下两种方式可以控制搜索路径添加的位置：

- CMAKE_INCLUDE_DIRECTORIES_BEFORE,通过SET这个cmake变量为on,可以将添加的头文件搜索路径放在已有路径的前面
- 通过AFTER或BEFORE参数,也可以控制是追加还是置前
- INCLUDE_DIRECTORIES (${HELLO_SOURCE_DIR}/Hello) #增加Hello为include目录

### LINK_DIRECTORIES

LINK_DIRECTORIES(dir1 dir2 …)

添加非标准的共享库搜索路径，动态链接库或静态链接库的搜索路径，相当于gcc的-L参数

- LINK_DIRECTORIES(${HELLO_BINARY_DIR}/Hello) #增加Hello为link目录

### TARGET_LINK_LIBRARIES

TARGET_LINK_LIBRARIES(target lib1 lib2 …)

为target添加需要链接的共享库，添加链接库,相同于指定-l参数

- target_link_libraries(demo Hello) #将可执行文件与Hello连接成最终文件demo

### ADD_DEFINITIONS

 在C/C++编译器添加-D定义，添加编译参数 ADD_DEFINITIONS(-DENABLE_DEBUG -DABC),参数之间用空格分隔如果你的代码中定义了#ifdef ENABLE_DEBUG #endif，这个代码块就会生效。如果要添加其他的编译器开关，可以通过CMAKE_C_FLAGS变量和CMAKE_CXX_FLAGS变量设置。

- add_definitions(-DDEBUG)将在gcc命令行添加DEBUG宏定义
- add_definitions( “-Wall -ansi –pedantic –g”)

### ADD_DEPENDENCIES 

ADD_DEPENDENCIES(target-name depend-target1 depend-target2 …) 
定义target依赖的其他target,确保target在构建之前,其依赖的target已经构建完毕

### ADD_TEST与ENABLE_TESTING 

ENABLE_TESTING指令用来控制Makefile是否构建test目标，涉及工程所有目录。语 
法很简单，没有任何参数，ENABLE_TESTING()，一般情况这个指令放在工程的主 
CMakeLists.txt中。 
ADD_TEST(testname Exename arg1 arg2 …) 
testname是自定义的test名称，Exename可以是构建的目标文件也可以是外部脚本等 
等。后面连接传递给可执行文件的参数。如果没有在同一个CMakeLists.txt中打开 
ENABLE_TESTING()指令，任何ADD_TEST都是无效的。 
比如我们前面的Helloworld例子，可以在工程主CMakeLists.txt中添加 
ADD_TEST(mytest ${PROJECT_BINARY_DIR}/bin/main) 
ENABLE_TESTING() 
生成Makefile后，就可以运行make test来执行测试了。

### AUX_SOURCE_DIRECTORY 

AUX_SOURCE_DIRECTORY(dir VAR) 
发现一个目录下所有的源代码文件并将列表存储在一个变量中 
把当前目录下的所有源码文件名赋给变量DIR_HELLO_SRCS 
你也可以通过后面提到的FOREACH指令来处理这个LIST

### EXEC_PROGRAM 

EXEC_PROGRAM(Executable [dir where to run] [ARGS <args>][OUTPUT_VARIABLE ] [RETURN_VALUE <value>]) 
用于在指定目录运行某个程序（默认为当前CMakeLists.txt所在目录）,通过ARGS添加参数,通过OUTPUT_VARIABLE和RETURN_VALUE获取输出和返回值,如下示例 
`在src中运行ls命令,在src/CMakeLists.txt添加 EXEC_PROGRAM(ls ARGS "*.c" OUTPUT_VARIABLE LS_OUTPUT RETURN_VALUE LS_RVALUE) IF (not LS_RVALUE) MESSAGE(STATUS "ls result: " ${LS_OUTPUT}) # 缩进仅为美观,语法无要求 ENDIF(not LS_RVALUE) `
在cmake 生成Makefile的过程中，就会执行ls命令，如果返回0，则说明成功执行， 
那么就输出ls *.c的结果。关于IF语句，后面的控制指令会提到。

### FILE

文件操作指令，基本语法为:

- FILE(WRITE filename “message to write”… ) 
  WRITE 将一则信息写入文件’filename’中，如果该文件存在，它会覆盖它，如果不存在，它会创建该文件。

- FILE(APPEND filename “message to write”… ) 
  APPEND 如同WRITE，区别在于它将信息内容追加到文件末尾。

- FILE(READ filename variable) 
  READ 会读取文件的内容并将其存入到变量中。它会在给定的偏移量处开始读取最多numBytes个字节。如果指定了HEX参数，二进制数据将会被转换成十进制表示形式并存储到变量中。

- file(STRINGS filename variable [LIMIT_COUNT num] 
  [LIMIT_INPUT numBytes] [LIMIT_OUTPUTnumBytes] 
  [LENGTH_MINIMUM numBytes] [LENGTH_MAXIMUMnumBytes] 
  [NEWLINE_CONSUME] [REGEX regex] 
  [NO_HEX_CONVERSION]) 
  STRINGS 从文件中解析出ASCII字符串列表并存储在变量中。文件中的二进制数据将被忽略。回车符(CR)也会被忽略。也能解析Intel Hex和Motorola S-record文件，这两种文件在读取是会自动转换为二进制格式，可以使用参数NO_HEX_CONVERSION 禁用这种自动转换。LIMIT_COUNT设置可返回的最大数量的字符串。LIMIT_INPUT 设置从输入文件中可读取的最大字节数。LIMIT_OUTPUT设置了存储在输出变量中最大的字节数。 LENGTH_MINIMUM设置了返回的字符串的最小长度。小于这个长度的字符串将被忽略。 LENGTH_MAXIMUM 设置返回的字符串的最大长度。大于这个长度的字符串将被切分为长度不大于于最大长度值的子字符串。NEWLINE_CONSUME 允许换行符包含进字符串中而不是截断它们。REGEX 指定了返回的字符串必须匹配的正则表达式的模式。典型用法: 
  file(STRINGS myfile.txt myfile)， 将输入文件的每行内容存储在变量”myfile”中。

- FILE(GLOB variable [RELATIVE path] [globbing expressions]…)

  GLOB 会产生一个由所有匹配globbing表达式的文件组成的列表，并将其保存到变量中。Globbing 表达式与正则表达式类似，但更简单。如果指定了RELATIVE 标记，返回的结果将是与指定的路径相对的路径构成的列表。 (通常不推荐使用GLOB命令来从源码树中收集源文件列表。原因是：如果CMakeLists.txt文件没有改变，即便在该源码树中添加或删除文件，产生的构建系统也不会知道何时该要求CMake重新产生构建文件。globbing 表达式包括：

   

  - .cxx - match all files with extension cxx
  - .vt? - match all files with extension vta,…,vtz
  - f[3-5].txt - match files f3.txt,f4.txt, f5.txt

- FILE(GLOB_RECURSE variable [RELATIVE path][globbing expressions]…) 
  GLOB_RECURSE 与GLOB类似，区别在于它会遍历匹配目录的所有文件以及子目录下面的文件。对于属于符号链接的子目录，只有FOLLOW_SYMLINKS指定一或者cmake策略CMP0009没有设置为NEW时，才会遍历这些目录。 
  Examples of recursive globbing include: 
  /dir/*.py - match all python files in /dir and subdirectories

- file(RENAME oldname newname) 
  RENAME 将文件系统中的文件或目录移动到目标位置，并自动替换目标位置处的文件或目录。

- FILE(REMOVE [directory]…) 
  REMOVE 会删除指定的文件以及子目录下的文件。

- FILE(REMOVE_RECURSE [directory]…) 
  REMOVE_RECURSE 会删除指定的文件及子目录，包括非空目录。

- FILE(MAKE_DIRECTORY [directory]…) 
  MAKE_DIRECTORY在指定目录处创建子目录，如果它们的父目录不存在，也会创建它们的父目录。

- FILE(RELATIVE_PATH variable directory file) 
  RELATIVE_PAT推断出指定文件相对于特定目录的路径。

- FILE(TO_CMAKE_PATH path result) 
  TO_CMAKE_PATH会将路径转换成cmake风格的路径表达形式。

- FILE(TO_NATIVE_PATH path result) 
  TO_NATIVE_PATH与TO_CMAKE_PATH类似，但执行反向操作，将cmake风格的路径转换为操作系统特定风格的路径表式形式。

- configure_file: 将一份文件拷贝到另一个位置并修改它的内容。

   

  configure_file(input output [COPYONLY] [ESCAPE_QUOTES] [@ONLY])

   

  > 将文件input拷贝到output然后替换文件内容中引用到的变量值。如果input是相对路径，它被评估的基础路径是当前源码路径。input必须是一个文件，而不是个路径。如果output是一个相对路径，它被评估的基础路径是当前二进制文件路径。如果output是一个已有的路径，那么输入文件将会以它原来的名字放到那个路径下。 
  > 该命令替换掉在输入文件中，以${VAR}格式或@VAR@格式引用的任意变量，如同它们的值是由CMake确定的一样。 如果一个变量还未定义，它会被替换为空。如果指定了COPYONLY选项，那么变量就不会展开。如果指定了ESCAPE_QUOTES选项，那么所有被替换的变量将会按照C语言的规则被转义。该文件将会以CMake变量的当前值被配置。如果指定了@ONLY选项，只有@VAR@格式的变量会被替换而${VAR}格式的变量则会被忽略。这对于配置使用\${VAR}格式的脚本文件比较有用。任何类似于#cmakedefine VAR的定义语句将会被替换为#define VAR或者/* #undef VAR */，视CMake中对VAR变量的设置而定。任何类似于#cmakedefine01 VAR的定义语句将会被替换为#define VAR 1或#define VAR 0，视VAR被评估为TRUE或FALSE而定。configure_file的作用是让普通文件也能使用CMake中的变量。

```cmake
message(STATUS "current dir: ${CMAKE_CURRENT_SOURCE_DIR}")  
file(WRITE test1.txt "Some messages to Write\n" )  
file(APPEND test1.txt "Another message to write\n")  
file(READ test1.txt CONTENTS LIMIT 4 OFFSET 12)  
message(STATUS "contents of test1.txt is: \n ${CONTENTS}")  
file(MD5 ${CMAKE_CURRENT_SOURCE_DIR}/test1.txt HASH_CONTENTS)  
message(STATUS "hash contents of test1.txt is: \n ${HASH_CONTENTS}")  
file(STRINGS test1.txt PARSED_STRINGS)  
message(STATUS "\n strings of test1.txt is: \n ${PARSED_STRINGS}")  
file(GLOB files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.*")  
message(STATUS  "files: ${files}")  
file(MAKE_DIRECTORY dir1 dir2)  
file(RENAME dir2 dir3)  
file(REMOVE dir3)  
file(REMOVE_RECURSE dir3)  
file(RELATIVE_PATH relative_path ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/test1.txt)  
message(STATUS "relative path : ${relative_path}")  
file(TO_CMAKE_PATH "$ENV{PATH}" cmake_path)  
message(STATUS "cmake path: ${cmake_path}")  
file(TO_NATIVE_PATH "/usr/local/sbin;/usr/local/bin" native_path)  
message(STATUS "native path: ${native_path}")  
file(DOWNLOAD "http://www.baidu.com" ${CMAKE_CURRENT_SOURCE_DIR}/index.html SHOW_PROGRESS)  
file(COPY test1.txt DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/dir1)  
file(INSTALL test1.txt DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/dir1)  
```

### INCLUDE

```cmake
INCLUDE(<file|module> [OPTIONAL] [RESULT_VARIABLE <VAR>]
                    [NO_POLICY_SCOPE])
INCLUDE(file [OPTIONAL])  #用来载入CMakeLists.txt文件
INCLUDE(module [OPTIONAL]) #用来载入预定义的cmake模块

#OPTIONAL参数的左右是文件不存在也不会产生错误

```

可以载入一个文件,也可以载入预定义模块（模块会在CMAKE_MODULE_PATH指定的路径进行搜索) 载入的内容将在处理到INCLUDE语句时直接执行。

从给定的文件中读取CMake的清单文件代码。在清单文件中的命令会被立即处理，就像它们是写在这条include命令展开的地方一样。如果指定了OPTIONAL选项，那么如果被包含文件不存在的话，不会报错。如果指定了RESULT_VARIABLE选项，那么var或者会被设置为被包含文件的完整路径，或者是NOTFOUND，表示没有找到该文件。

如果指定的是一个模块（module）而不是一个文件，查找的对象会变成路径CMAKE_MODULE_PATH下的文件.camke。

- get_filename_component( VAR FileName 
  PATH|ABSOLUTE|NAME|EXT|NAME_WE|REALPATH 
  [CACHE] ) 
  将变量VAR设置为路径(PATH)，文件名(NAME)，文件扩展名(EXT)，去掉扩展名的文件名(NAME_WE)，完整路径(ABSOLUTE)，或者所有符号链接被解析出的完整路径(REALPATH)。注意，路径会被转换为Unix的反斜杠(/)，并且没有结尾的反斜杠。该命令已经考虑了最长的文件扩展名。如果指定了CACHE选项，得到的变量会被加到cache中。

- get_filename_component(VAR FileName 
  PROGRAM [PROGRAM_ARGS ARG_VAR] 
  [CACHE]) 
  在FileName中的程序将会在系统搜索路径中被查找，或者是一个完整路径。如果与PRPGRAM一起给定了PROGRAM_ARGS选项，那么任何在Filename字符串中出现的命令行中选项将会从程序名中分割出来并存储在变量中。这可以用来从一个命令行字符串中分离程序名及其选项。

- OPTION 
  为用户提供一个可选项, 
  option(option_variable “描述选项的帮助性文字” [initial value]) 
  该命令为用户提供了一个在ON和OFF中做出选择的选项。如果没有指定初始值，将会使用OFF作为初值。如果有些选项依赖于其他选项的值，参见CMakeDependentOption模块的帮助文件。

- target_include_directories

```cmake
# target_include_directories was added in CMake 2.8.11 and is the recommended
# way to set include directories. For lesser versions, we revert to set_property
if(CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories(\${HDF5_INCLUDE_DIRS})
else()
  target_include_directories(${program} PUBLIC ${HDF5_INCLUDE_DIRS})
endif()
```

- target_compile_options

```cmake
# target_compile_options was added in CMake 2.8.12 and is the recommended way to
# set compile flags. Note that this sets the COMPILE_OPTIONS property (also
# available only in 2.8.12+) rather than the COMPILE_FLAGS property, which is
# deprecated. The former can handle lists whereas the latter cannot.
if(CMAKE_VERSION VERSION_LESS 2.8.12)
  string(REPLACE ";" " " f90flags "${f90flags}")
  set_property(TARGET ${program} PROPERTY COMPILE_FLAGS "${f90flags}")
else()
  target_compile_options(${program} PUBLIC ${f90flags})
endif()
```





## 控制指令

### IF 

语法：

```cmake
IF (expression)
  COMMAND1(ARGS ...)
  COMMAND2(ARGS ...)
  ...
ELSE (expression)
  COMMAND1(ARGS ...)
  COMMAND2(ARGS ...)
  ...
ENDIF (expression) # 一定要有ENDIF与IF对应
```

另外一个指令是ELSEIF，总体把握一个原则，凡是出现IF的地方一定要有对应的 
ENDIF.出现ELSEIF的地方，ENDIF是可选的。 
IF (expression), expression不为：空,0,N,NO,OFF,FALSE,NOTFOUND或<var>_NOTFOUND,为真 
IF (not exp), 与上面相反 
IF (var1 AND var2) 
IF (var1 OR var2) 
IF (COMMAND cmd) 如果cmd确实是命令并可调用,为真 
IF (EXISTS dir) IF (EXISTS file) 如果目录或文件存在,为真 
IF (file1 IS_NEWER_THAN file2),当file1比file2新,或file1/file2中有一个不存在时为真,文件名需使用全路径 
IF (IS_DIRECTORY dir) 当dir是目录时,为真 
IF (DEFINED var) 如果变量被定义,为真 
IF (var MATCHES regex) 此处var可以用var名,也可以用${var} 
IF (string MATCHES regex)

```bash
当给定的变量或者字符串能够匹配正则表达式regex时为真。比如：
IF ("hello" MATCHES "ell")
  MESSAGE("true")
ENDIF ("hello" MATCHES "ell")
```

#### **数字比较表达式** 

IF (variable LESS number) 
IF (string LESS number) 
IF (variable GREATER number) 
IF (string GREATER number) 
IF (variable EQUAL number) 
IF (string EQUAL number)

#### **按照字母表顺序进行比较** 

IF (variable STRLESS string) 
IF (string STRLESS string) 
IF (variable STRGREATER string) 
IF (string STRGREATER string) 
IF (variable STREQUAL string) 
IF (string STREQUAL string)

```bash
一个小例子,用来判断平台差异：
IF (WIN32)
  MESSAGE(STATUS “This is windows.”)
ELSE (WIN32)
  MESSAGE(STATUS “This is not windows”)
ENDIF (WIN32)
上述代码用来控制在不同的平台进行不同的控制,
但是,阅读起来却并不是那么舒服,ELSE(WIN32)之类的语句很容易引起歧义。
可以SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
这时候就可以写成:
IF (WIN32)
ELSE ()
ENDIF ()
配合ELSEIF使用,可能的写法是这样:
IF (WIN32)
  #do something related to WIN32
ELSEIF (UNIX)
  #do something related to UNIX
ELSEIF(APPLE)
  #do something related to APPLE
ENDIF (WIN32)
```

### WHILE 

语法：

```bash
WHILE(condition)
  COMMAND1(ARGS ...)
  COMMAND2(ARGS ...)
  ...
ENDWHILE(condition)
```

其真假判断条件可以参考IF指令

### FOREACH

 

FOREACH指令的使用方法有三种形式：

 

#### 1. 列表 

   语法： 
   `FOREACH(loop_var arg1 arg2 ...) COMMAND1(ARGS ...) COMMAND2(ARGS ...) ... ENDFOREACH(loop_var) `
   示例： 
   `AUX_SOURCE_DIRECTORY(. SRC_LIST) FOREACH(F ${SRC_LIST}) MESSAGE(${F}) ENDFOREACH(F) `

#### 2. 范围 
   FOREACH(loop_var RANGE total) 
   COMMAND1(ARGS …) 
   COMMAND2(ARGS …) 
   … 
   ENDFOREACH(loop_var) 
   `从0到total以１为步进 FOREACH(VAR RANGE 10) MESSAGE(${VAR}) ENDFOREACH(VAR) 输出： 0 1 2 3 4 5 6 7 8 9 10 `

#### 3. 范围和步进 
   语法： 
   `bash FOREACH(loop_var RANGE start stop [step]) COMMAND1(ARGS ...) COMMAND2(ARGS ...) ... ENDFOREACH(loop_var) `
   从start开始到stop结束,以step为步进, 
   **注意：**直到遇到ENDFOREACH指令,整个语句块才会得到真正的执行。 
   `FOREACH(A RANGE 5 15 3) MESSAGE(${A}) ENDFOREACH(A) 输出： 5 8 11 14 `

## 函数和宏定义

函数会为变量创建一个局部作用域，而宏则使用全局作用域。范例：

- 定义一个宏 hello

```cmake
macro(hello MESSAGE)
   message(${MESSAGE})
endmacro()

# 调用宏 hello
hello("hello world")
```

- 定义一个函数 hello

```cmake
function(hello MESSAGE)
   message(${MESSAGE})
endfunction()123
```

函数和宏可以通过命令 return() 返回，但是函数和宏的返回值必须通过参数传递出去。例如：

```cmake
cmake_minimum_required(VERSION 2.8)
function(get_func RESULT)
    #RESULT 的值为实参的值，因此需要使用 ${RESULT}
    #这里使用 PARENT_SCOPE 是因为函数会构建一个局部作用域
    set(${RESULT} "Hello Function" PARENT_SCOPE)
endfunction()

macro(get_macro RESULT)
   set(${RESULT} "Hello Macro")
endmacro()

get_func(V1)
#输出 Hello Function
message(${V1}) 
get_macro(V2)
#输出 Hello Macro
message(${V2})
```







## INSTALL指令

安装的需要有两种，一种是从代码编译后直接make install安装，一种是打包时的指定目录安装。 
这里需要引入一个新的cmake 指令 INSTALL和一个非常有用的变量CMAKE_INSTALL_PREFIX。 
CMAKE_INSTALL_PREFIX变量类似于configure脚本的 –prefix，常见的使用方法看 
起来是这个样子： 
cmake -DCMAKE_INSTALL_PREFIX=/usr .

INSTALL指令用于定义安装规则，安装的内容可以包括目标二进制、动态库、静态库以及 
文件、目录、脚本等。

INSTALL指令包含了各种安装类型，我们需要一个个分开解释：

### 目标文件的安装

```cmake
INSTALL(TARGETS targets...
        [[ARCHIVE|LIBRARY|RUNTIME]
                   [DESTINATION <dir>]
                   [PERMISSIONS permissions...]
                   [CONFIGURATIONS
        [Debug|Release|...]]
                   [COMPONENT <component>]
                   [OPTIONAL]
                ] [...])
```

参数中的TARGETS后面跟的就是我们通过ADD_EXECUTABLE或者ADD_LIBRARY定义的 
目标文件，可能是可执行二进制、动态库、静态库。

目标类型也就相对应的有三种，ARCHIVE特指静态库，LIBRARY特指动态库，RUNTIME 
特指可执行目标二进制。

DESTINATION定义了安装的路径，如果路径以/开头，那么指的是绝对路径，这时候 
CMAKE_INSTALL_PREFIX其实就无效了。如果你希望使用CMAKE_INSTALL_PREFIX来 
定义安装路径，就要写成相对路径，即不要以/开头，那么安装后的路径就是 
`${CMAKE_INSTALL_PREFIX}/<DESTINATION定义的路径>`

举个简单的例子：

```cmake
INSTALL(TARGETS myrun mylib mystaticlib
       RUNTIME DESTINATION bin
       LIBRARY DESTINATION lib
       ARCHIVE DESTINATION libstatic
)
```

上面的例子会将： 
可执行二进制myrun安装到`${CMAKE_INSTALL_PREFIX}/bin`目录 
动态库libmylib安装到`${CMAKE_INSTALL_PREFIX}/lib`目录 
静态库libmystaticlib安装到`${CMAKE_INSTALL_PREFIX}/libstatic`目录 
特别注意的是你不需要关心TARGETS具体生成的路径，只需要写上TARGETS名称就可以 
了。

### **普通文件的安装**

```cmake
INSTALL(FILES files... DESTINATION <dir>
         [PERMISSIONS permissions...]
         [CONFIGURATIONS [Debug|Release|...]]
         [COMPONENT <component>]
         [RENAME <name>] [OPTIONAL])
可用于安装一般文件，并可以指定访问权限，文件名是此指令所在路径下的相对路径。
如果默认不定义权限PERMISSIONS，安装后的权限为,OWNER_WRITE,OWNER_READ,
GROUP_READ,和WORLD_READ，即644权限。
```

### **非目标文件的可执行程序安装**(比如脚本之类)

```cmake
INSTALL(PROGRAMS files... DESTINATION <dir>
     [PERMISSIONS permissions...]
     [CONFIGURATIONS [Debug|Release|...]]
     [COMPONENT <component>]
     [RENAME <name>] [OPTIONAL])
```

跟上面的FILES指令使用方法一样，唯一的不同是安装后权限为: 
OWNER_EXECUTE, GROUP_EXECUTE, 和WORLD_EXECUTE，即755权限

### **目录的安装**

```cmake
INSTALL(DIRECTORY dirs... DESTINATION <dir>
     [FILE_PERMISSIONS permissions...]
     [DIRECTORY_PERMISSIONS permissions...]
     [USE_SOURCE_PERMISSIONS]
     [CONFIGURATIONS [Debug|Release|...]]
     [COMPONENT <component>]
     [[PATTERN <pattern> | REGEX <regex>]
      [EXCLUDE] [PERMISSIONS permissions...]] [...])
```

这里主要介绍其中的DIRECTORY、PATTERN以及PERMISSIONS参数。 
DIRECTORY后面连接的是所在Source目录的相对路径，但务必注意： 
abc和abc/有很大的区别。 
abc意味着abc这个目录会安装在目标路径下； 
abc/意味着abc这个目录的内容会被安装在目标路径下； 
如果目录名不以/结尾，那么这个目录将被安装为目标路径下的abc，如果目录名以/结尾， 
代表将这个目录中的内容安装到目标路径，但不包括这个目录本身。 
PATTERN用于使用正则表达式进行过滤， 
PERMISSIONS用于指定PATTERN过滤后的文件权限。

我们来看一个例子:

```cmake
INSTALL(DIRECTORY icons scripts/ DESTINATION share/myproj
        PATTERN "CVS" EXCLUDE
        PATTERN "scripts/*"
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
        GROUP_EXECUTE GROUP_READ)
```

这条指令的执行结果是： 
将icons目录安装到 <prefix>/share/myproj，将scripts/中的内容安装到 
<prefix>/share/myproj 
不包含目录名为CVS的目录，对于scripts/*文件指定权限为 OWNER_EXECUTE 
OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ.

### **安装时cmake脚本的执行**

```cmake
INSTALL([[SCRIPT <file>] [CODE <code>]] [...])
SCRIPT参数用于在安装时调用cmake脚本文件（也就是<abc>.cmake文件）
CODE参数用于执行CMAKE指令，必须以双引号括起来。比如：
INSTALL(CODE "MESSAGE(\"Sample install message.\")")
```





## FIND_指令

- FIND_FILE(<VAR> name path1 path2 …) 
  VAR变量代表找到的文件全路径,包含文件名

- FIND_LIBRARY(<VAR> name path1 path2 …) 
  VAR变量代表找到的库全路径,包含库文件名

- FIND_PATH(<VAR> name path1 path2 …) 
  VAR变量代表包含这个文件的路径

- FIND_PROGRAM(<VAR> name path1 path2 …) 
  VAR变量代表包含这个程序的全路径

- FIND_PACKAGE(<name> [major.minor] [QUIET] [NO_MODULE] [[REQUIRED | COMPONENTS] [componets …]]) 
  用来调用预定义在CMAKE_MODULE_PATH下的Find<name>.cmake模块,你也可以自己定义Find<name> 
  模块,通过SET(CMAKE_MODULE_PATH dir)将其放入工程的某个目录供工程使用

  ```bash
  FIND_LIBRARY(libX X11 /usr/lib)
  IF (NOT libx)
      MESSAGE(FATAL_ERROR "libX not found")
  ENDIF(NOT libX)
  ```



### module

> 使用cmake本身提供的基本指令来管理复杂工程是一件复杂的事情，所以，cmake设计成了可扩展的架构，可以通过编写一些通用的模块来扩展cmake.

系统预定义的Find模块的使用，系统中提供了其他各种模块，一般情况需要使用INCLUDE指令显式的调用, FIND_PACKAGE指令是一个特例，可以直接调用预定义的模块。

```cmake
#对于系统预定义的Find<name>.cmake模块，使用方法一般如下所示：
FIND_PACKAGE(NAME)
IF(NAME_FOUND)
    INCLUDE_DIRECTORIES(${NAME_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(targetname ${NAME_LIBRARY})
ELSE(NAME_FOUND)
    MESSAGE(FATAL_ERROR ”NAME library not found”)
ENDIF(NAME_FOUND)
```

每一个模块都会定义以下3个变量

```
<name>_FOUND
<name>_INCLUDE_DIR or <name>_INCLUDES
<name>_LIBRARY or <name>_LIBRARIES123
```

> 你可以通过<name>_FOUND来判断模块是否被找到，如果没有找到，按照工程的需要关闭某些特性、给出提醒或者中止编译，上面的例子就是报出致命错误并终止构建。
>
> 如果<name>_FOUND为真 
> 则将<name>_INCLUDE_DIR加入INCLUDE_DIRECTORIES, 
> 将<name>_LIBRARY加入TARGET_LINK_LIBRARIES中。

```cmake
#我们再来看一个复杂的例子，通过<name>_FOUND来控制工程特性：
SET(mySources viewer.c)
SET(optionalSources)
SET(optionalLibs)
FIND_PACKAGE(JPEG)
IF(JPEG_FOUND)
    SET(optionalSources ${optionalSources} jpegview.c)
    INCLUDE_DIRECTORIES( ${JPEG_INCLUDE_DIR} )
    SET(optionalLibs ${optionalLibs} ${JPEG_LIBRARIES} )
    ADD_DEFINITIONS(-DENABLE_JPEG_SUPPORT)
ENDIF(JPEG_FOUND)

IF(PNG_FOUND)
    SET(optionalSources ${optionalSources} pngview.c)
    INCLUDE_DIRECTORIES( ${PNG_INCLUDE_DIR} )
    SET(optionalLibs ${optionalLibs} ${PNG_LIBRARIES} )
    ADD_DEFINITIONS(-DENABLE_PNG_SUPPORT)
ENDIF(PNG_FOUND)

ADD_EXECUTABLE(viewer ${mySources} ${optionalSources} )
TARGET_LINK_LIBRARIES(viewer ${optionalLibs})
#通过判断系统是否提供了JPEG库来决定程序是否支持JPEG功能。
```

### 自己编写的Find模块

> 基于我们前面的libhello共享库，编写一个FindHello.cmake模块。

```cmake
FIND_PATH(HELLO_INCLUDE_DIR hello.h <绝对路径>)
FIND_LIBRARY(HELLO_LIBRARY NAMES hello <绝对路径>)
#可以直接写libhello.so or libhello.a

IF (HELLO_INCLUDE_DIR AND HELLO_LIBRARY)
    SET(HELLO_FOUND TRUE)
ENDIF (HELLO_INCLUDE_DIR AND HELLO_LIBRARY)

IF (HELLO_FOUND)
    IF (NOT HELLO_FIND_QUIETLY)
    MESSAGE(STATUS "Found Hello: ${HELLO_LIBRARY}")
    ENDIF (NOT HELLO_FIND_QUIETLY)
ELSE (HELLO_FOUND)
    IF (HELLO_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find hello library")
    ENDIF (HELLO_FIND_REQUIRED)
ENDIF (HELLO_FOUND)
```

> 前面的NAME例子中我们使用了最简单的FIND_PACKAGE指令，其实他可以使用多种参数，QUIET参数，对应与我们编写的FindHELLO中的HELLO_FIND_QUIETLY，如果不指定这个参数，就会执行： 
> MESSAGE(STATUS “Found Hello: ${HELLO_LIBRARY}”) 
> REQUIRED参数，其含义是指这个共享库是否是工程必须的，如果使用了这个参数，说明这个链接库是必备库，如果找不到这个链接库，则工程不能编译。对应于FindHELLO.cmake模块中的HELLO_FIND_REQUIRED变量。同样，我们在上面的模块中定义了HELLO_FOUND,HELLO_INCLUDE_DIR,HELLO_LIBRARY变量供开发者在FIND_PACKAGE指令中使用。

下面建立src/main.cpp，内容为：

```cpp
#include <hello.h>
int main()
{
    HelloFunc();
    return 0;
}
```

建立src/CMakeLists.txt文件，内容如下：

```cmake
FIND_PACKAGE(HELLO)
IF(HELLO_FOUND)
    ADD_EXECUTABLE(hi main.cpp)
    INCLUDE_DIRECTORIES(${HELLO_INCLUDE_DIR})
    MESSAGE(STATUS "LINK " ${HELLO_LIBRARY})
    TARGET_LINK_LIBRARIES(hi ${HELLO_LIBRARY})
ELSE(HELLO_FOUND)
    MESSAGE(STATUS "Could not find hello library")
ENDIF(HELLO_FOUND)
```

为了能够让工程找到FindHELLO.cmake模块(存放在工程中的cmake目录)我们在主工程文件CMakeLists.txt中加入：

```cmake
PROJECT(MODULE)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")
MESSAGE(STATUS "CMAKE_MODULE_PATH " ${PROJECT_SOURCE_DIR}/cmake)
ADD_SUBDIRECTORY(src)
```



