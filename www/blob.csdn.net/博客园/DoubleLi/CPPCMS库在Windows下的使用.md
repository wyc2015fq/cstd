# CPPCMS库在Windows下的使用 - DoubleLi - 博客园






**标题：CPPCMS库在Windows下的使用**
时间：2012-7
作者：Kagula

**环境：**
[1]WinXP SP3
[2]VisualStudio2008 SP1
[3]ZLib 1.2.7
[4]PCRE 8.31
[5]Python 2.7.3

**第一步：配置编译环境**
**ZLib库**
[1-1]进行VS2008命令行模式(若无特殊说明，全部命令均在这种模式下运行)
[1-2]zlib库 1.2.7下载并解压到e:\sdk
[1-3]进入到**E:\SDK\zlib-1.2.7\win32**目录下面，执行下面的命令，生成INSTALL.vcproj文件
**cmake .. -G"Visual Studio 9 2008"**
不跟参考调用cmake命令会打印出使用帮忙。
[1-4]
使用VS2008打开ALL_BUILD.vcproj文件编译出现unistd.h文件末尾错误，在文件最后加入一个回车换行再编译就可以了。
缺省编译选项是MinSizeRel（对生成的文件大小进行优化&发行版 编译模式）。
在E:\SDK\zlib-1.2.7\win32\MinSizeRel文件夹下为生成对应的lib和dll文件，还有测试用的exe文件。

**PCRE库**
[2-1]在下面的网址
ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/pcre-8.31.zip
下载并解压PCRE源码包到"E:\SDK\pcre-8.31"路径下
[2-2]运行下面命令生成ALL_BUILD。vcproj文件，双击打开，并在VS2008中BUILD!
**E:\SDK\pcre-8.31>cmake -G"Visual Studio 9 2008"**
[2-3]如果在“MinSizeRel”下编译
会自动建立**E:\SDK\pcre-8.31\MinSizeRel**目录，并在这个目录里生成DLL、LIB、EXE等文件

**Python**
[3]在Win下安装Python2.7.3
我是装在C:\Python27路径下的，把这个路径添加到Path环境变量中

**ICU(多国语言支持，可选)**
[4]在下面的地址
http://download.icu-project.org/files/icu4c/49.1.2/icu4c-49_1_2-Win32-msvc10.zip
下载并解压缩到E:\SDK目录
悲剧，只支持Visual Studio 2010。

**OpenSSL1.0.1(可选)**
[5]如何下载编译这个库，参考网上相关文档，这里懒的写了。

**第二步：编译CPPCMS**
[1]
E:\SDK\pcre-8.31;E:\SDK\zlib-1.2.7

[2]进入E:\SDK\cppcms-1.0.1目录下，运行下面的命令
**E:\SDK\pcre-8.31>cmake -G"Visual Studio 9 2008"-DCMAKE_INCLUDE_PATH=E:\SDK\pcre-8.31;E:\SDK\zlib-1.2.7 -DCMAKE_LIBRARY_PATH=E:\SDK\pcre-8.31\MinSizeRel;E:\SDK\zlib-1.2.7\win32\MinSizeRel**
**注意，上面是为了阅读方便才把一行内容break成三行的，实际输入命令还是在一行里。**
[3]
生成ALL_BUILD.vcproj，双击打开。"MinSizeRel"方式编译（即生成最小文件的Release版本）
编译booster工程时，需要[Configuration Properties]->[C/C++]->[Preprocessor]->[Preprocessor Definitions]中添加
**“PCRE_STATIC”宏**，否则pcre_free函数的实现会找不到，参考资料[1]。
编译cppcms工程前，修改zlib.h文件，**把原“#include "zconf.h"”代码段改为“#include "win32\zconf.h"”**代码段。
然后编译Solution里全部的工程通过。

如果你修改了CPPCMS依赖库的头文件和库文件位置，则用cmake命令重新生成vcproj文件，再编译。
这样要比你一个个手动更改编译选项要方便多了。

**第三步：我的第一个CPPCMS程序**
[1]
在VS2008中配置以下项目
配置头文件路径
E:\SDK\cppcms-1.0.1
E:\SDK\cppcms-1.0.1\booster

配置依赖库文件路径
----------------------------------------------
Release版本的
E:\SDK\cppcms-1.0.1\MinSizeRel
----------------------------------------------
Debug版本的(默认)
E:\SDK\cppcms-1.0.1\Debug
----------------------------------------------

[2]在VS2008里新建C++的Win32 Console项目
链接库依赖项中加入“cppcms.lib”



[3]参考资料[2]中代码新建下面的代码





**[cpp]**[view plain](http://blog.csdn.net/lee353086/article/details/7755809#)[copy](http://blog.csdn.net/lee353086/article/details/7755809#)



- #include <cppcms/json.h>    
- #include <iostream>  
- 
- using namespace std;  
- class response {  
- public:   
- //0 is ok  
- //other values are wrong  
- int status;  
-         string message;  
-     };  
- 
- namespace cppcms {  
- namespace json {  
- template<>  
- struct traits<response> {  
- static void set(value &v, response const& in) {  
-                 v.set("status", in.status);  
-                 v.set("message", in.message);  
-             }  
-         };  
-     }  
- }    
- 
- int main(int argc, char** argv) {  
-     response r1;  
-     r1.status=0;  
-     r1.message="ok";  
-     cout<< cppcms::json::value(r1)<<endl;  
- 
-     response r2;  
-     r2.status=1;  
-     r2.message="failed";  
-     cout<< cppcms::json::value(r2)<<endl;  
- 
- return 0;  
- }  







[4]
把下面路径中的文件复制到当前项目路径中
----------------------------------------------
MiniSizeRel版本的
E:\SDK\cppcms-1.0.1\booster\MinSizeRel\booster.dll
E:\SDK\zlib-1.2.7\win32\MinSizeRel\zlib.dll
E:\SDK\cppcms-1.0.1\MinSizeRel\cppcms.dll
----------------------------------------------
Debug版本的(默认)
E:\SDK\zlib-1.2.7\win32\Debug\zlibd.dll
E:\SDK\cppcms-1.0.1\booster\Debug\boost.dll
E:\SDK\cppcms-1.0.1\Debug\cppcms.dll
----------------------------------------------

[5]参考资料[2]的代码建立项目并在Release下可以正常运行。
但是在Debug模式下，虽然能编译，但不能正常运行。
一般我们为了调试方便，会在DEBUG方式下运行程序，所以你还必须编译出各个第三方库的DEBUG版本。

[6]默认情况下cppcms第三方库进行Debug同Release版本之间的切换不方便
[6-1]修改booster库的Debug编译选项
[Configuration Properties]->[Linker]->[General]->[Output File]->[原booster.dll改为boosterd.dll]
[Configuration Properties]->[Linker]->[Advanced]->[Import Library]->[原booster.lib改为boosterd.lib]

[6-2]修改cppcms项目的Debug编译选项
[Configuration Properties]->[Linker]->[Input]->[Additional Dependencies]->[原booster.lib改为boosterd.lib]
[原E:\SDK\zlib-1.2.7\win32\MinSizeRel\zlib.lib改为E:\SDK\zlib-1.2.7\win32\Debug\zlibd.lib]
[原E:\SDK\pcre-8.31\MinSizeRel\pcre.lib改为E:\SDK\pcre-8.31\MinSizeRel\pcred.lib]
[Configuration Properties]->[Linker]->[General]->[Output File]->[cppcmsd.dll]
[Configuration Properties]->[Linker]->[Advanced]->[Import Library]->[cppcmsd.lib]
----------------------------------------------------------------------------------------
这样就可以把DEBUG方式的LIB路径加入到VS2008中，而不会和原来的路径冲突。
把DEBUG方式的DLL文件复制到当前项目文件夹也不会把Release方式的DLL覆盖，方便了开发。
----------------------------------------------------------------------------------------
后来发现，Debug版本的DLL要放到上一级（Solution级别）的Debug目录中才能被EXE(Debug模式)找到，
但是Release版本的DLL文件只要放到当前项目.vcproj文件所在的目录就可以了，不纠结了... ..。
现在Debug模式设置依赖库为cppcmsd.lib就可以正常跑参考资料[2]中的代码。

[7]参考cppcms项目自带的json例子，进行深入学习。


**参考资料**
[1]PCRE_free找不到的问题
http://forums.codeblocks.org/index.php?topic=9577.0
[2]《C++序列化和反序列化》
http://www.cnblogs.com/fengye-dream/archive/2012/03/21/2409964.html
[3]《cppcms官网》
http://cppcms.com/wikipp/en/page/main









