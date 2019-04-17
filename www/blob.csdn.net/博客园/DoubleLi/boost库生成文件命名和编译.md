# boost库生成文件命名和编译 - DoubleLi - 博客园






生成文件命名规则：boost中有许多库，有的库需要编译、而有的库不需要编译，只需包含头文件就可以使用。编译生成的文件名字普遍较长，同一个库根据编译链接选项不同，又可以生成多个不同名字的文件。生成的文件名字是很长，可是这样带的信息才多，也就容易识别出用途。其实生成文件的名字遵循一定的规则，有着固定的格式。识别这种格式，能帮助我们更高效地使用boost库。生成文件名字格式如： 

   BOOST_LIB_PREFIX + BOOST_LIB_NAME + "-" + BOOST_LIB_TOOLSET + "-" + BOOST_LIB_THREAD_OPT + "-" + BOOST_LIB_RT_OPT + "-" + BOOST_LIB_VERSION  

   这些定义为:

　　BOOST_LIB_PREFIX: 静态库为 "lib" (否则无,是用动态链接库)

　　BOOST_LIB_NAME: 库的基本名称 ( 比方说 boost_regex).

　　BOOST_LIB_TOOLSET: 编译工具集名称 ( 比如：vc6, vc7, bcb5 )

　　BOOST_LIB_THREAD_OPT: 多线程为 "-mt" ，否则为空

　　BOOST_LIB_RT_OPT: 指示使用的运行库的后缀, 

   组合下面的一个或者更多字符:

   s 静态运行库，指的是静态链接到运行时库(不出现表示动态).

   g 调试/诊断 runtime (release if not present).

   d 调试版本 (不出现表示 release 版 ).

   p STLPort 版本.

   注：对 vc 来说，gd 总是一起出现

　　BOOST_LIB_VERSION: Boost 版本, Boost 版本 x.y 表示为 x_y形式.

 编译：为了简化boost库的编译,boost库中带了一个用来编译的工具,名字是bjam.exe或者b2.exe.

1:运行boost下的bootstap.bat脚本就会自动生上述的两个编译工具,并且拷贝到boost目录下. 也可以进入tools/build目录下找到类似的脚本或者项目源码来编译.

2: bjam.exe的参数

|Feature|Allowed values|Notes|
|----|----|----|
|variant|debug,release||
|link|shared,static|Determines if Boost.Build creates shared or static libraries|
|threading|single,multi|Cause the produced binaries to be thread-safe. This requires proper support in the source code itself.|
|address-model|32,64|Explicitly request either 32-bit or 64-bit code generation. This typically requires that your compiler is appropriately configured. Please refer to [the section called “C++ Compilers”](file:///E:/Library/boost_1_48_0/doc/html/bbv2/reference.html#bbv2.reference.tools.compilers) and your compiler documentation in case of problems.|
|toolset|(Depends on configuration)|The C++ compiler to use. See [the section called “C++ Compilers”](file:///E:/Library/boost_1_48_0/doc/html/bbv2/reference.html#bbv2.reference.tools.compilers) for a detailed list.(Vs2008)msvc-8.0 (vs2010)msvc-10.0|
|include|(Arbitrary string)|Additional include paths for C and C++ compilers.|
|define|(Arbitrary string)|Additional macro definitions for C and C++ compilers. The string should be either SYMBOL or SYMBOL=VALUE|
|cxxflags|(Arbitrary string)|Custom options to pass to the C++ compiler.|
|cflags|(Arbitrary string)|Custom options to pass to the C compiler.|
|linkflags|(Arbitrary string)|Custom options to pass to the C++ linker.|
|runtime-link|shared,static|Determines if shared or static version of C and C++ runtimes should be used.|


|--build-dir=<builddir>|编译的临时文件会放在builddir里(这样比较好管理，编译完就可以把它删除了)|
|----|----|
|--stagedir=<stagedir>|存放编译后库文件的路径，默认是stage|
|--build-type=complete|编译所有版本，不然只会编译一小部分版本（确切地说是相当于:variant=release, threading=multi;link=shared|static;runtime-link=shared）|
|variant=debug|release|决定编译什么版本（对应文件中的d 调试版本 不出现表示 release 版）|
|link=static|shared|决定使用静态库还是动态库。（对应文件中的BOOST_LIB_PREFIX ）|
|threading=single|multi|决定使用单线程还是多线程库。（对应文件中的BOOST_LIB_THREAD_OPT）|
|runtime-link=static|shared|决定是静态还是动态链接C/C++标准库。（对应文件中的BOOST_LIB_THREAD_OPT）|
|--with-<library>|只编译指定的库，如输入--with-regex就只编译regex库了。|
|--show-libraries|显示需要编译的库名称|




bjam.exe --toolset=msvc-10.0 --with-date_time runtimelink=static link=static stage 
意思是要生静态库,该静态库静态链接C运行时库
生成的文件名字是:libboost_date_time-vc100-mt-sgd-1_48.lib(debug version),libboost_date_time-vc100-mt-s-1_48.lib(release version) 两个文件.

bjam.exe --toolset=msvc-10.0 --with-date_time runtimelink=shared link=static stage
意思是要生静态库,该静态库动态链接C运行时库
生成的文件名字是:libboost_date_time-vc100-mt-gd-1_48.lib(debug verion),libboost_date_time-vc100-mt-1_48.lib(release version) 两个文件.

bjam.exe --toolset=msvc-10.0 --with-date_time runtimelink=shared link=shared stage
意思是要生动态库,该动态库动态链接C运行时库
生成的文件名字是:boost_date_time-vc100-mt-gd-1_48.lib(debug version),boost_date_time-vc100-mt-1_48.lib(release version) 两个文件.
生成的dll名字是:boost_date_time-vc100-mt-gd-1_48.dll(debug version),boost_date_time-vc100-mt-1_48.dll(release version)

编译选项方面还有install等参数.









