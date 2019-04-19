# g++基本用法 - 建建的博客 - CSDN博客
2017年03月13日 14:20:22[纪建](https://me.csdn.net/u013898698)阅读数：89
个人分类：[Linux](https://blog.csdn.net/u013898698/article/category/6754383)
 用法：g++[选项]文件...
- 
g++编译流程：
![](https://img-blog.csdn.net/20131202150532234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmljaGVyZzg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**[cpp]**[view
 plain](http://blog.csdn.net/richerg85/article/details/17073805#)[copy](http://blog.csdn.net/richerg85/article/details/17073805#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/92233/fork)
- main.cxx  
- #include <iostream>
- 
- usingnamespace std;  
- 
- int main(void)  
- {  
-         cout<<"Hello World!"<<endl;  
- return 0;  
- }  
① g++ -E仅作预处理，不进行编译、汇编和链接
如果你想查看某一个文件的预处理过程，可以
Administrator@72cec870236147e/home/liujl/mytest
$g++ -E main.cxx
这样会直接在terminal中显示预处理过程，如果main.cxx中包含include，例如#include<iostream>,只能看到部分预处理结果；这个只显示预处理，不生成文件；如果你想查看详细的预处理，可以重定向到一个文件中，如：
Administrator@72cec870236147e/home/liujl/mytest
$g++ -E main.cxx -o main.i
这样你可以查看main.i中显示的内容，里面会有类似：
#1 "/usr/lib/gcc/i686-pc-cygwin/4.5.3/include/c++/bits/stringfwd.h"1 3
可以参考官方预处理输出文档：
[http://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html](http://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html)[](http://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html%E6%88%96%E8%80%85)
[或者](http://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html%E6%88%96%E8%80%85)：
[http://stackoverflow.com/questions/15679756/g-e-option-output](http://stackoverflow.com/questions/15679756/g-e-option-output)
②g++ -s 编译到汇编语言，不进行汇编和链接,即只激活预处理和编译，生成汇编语言
Administrator@72cec870236147e/home/liujl/mytest
$g++ -s main.i -o main.s
③g++ -c 编译、汇编到目标代码，不进行链接,即生成目标文件（.o）
Administrator@72cec870236147e/home/liujl/mytest
$g++ -c main.s -o main.o
会生成一个main.o的目标文件
④g++ -o 生成链接文件
Administrator@72cec870236147e/home/liujl/mytest
$g++ main.o printf1.o printf2.o -o main
(注意，如果各个文件有依赖关系，在生成链接文件之前步骤每个文件单独操作都没有问题，但是在链接的时候会考虑依赖关系，所以上面语句把main中的依赖添加上面了)
⑤./main 执行
这是最后一步了，执行-o后面的文件，如果没有-o，系统会默认生成一个a.out文件（执行./a.out）。
2、g++关键参数介绍
上文在g++编译过程中已经介绍过-E、-S、-c、-o，再简单介绍-x和-M
①-x <语言> 指定其后输入文件的语言
允许的语言包括：cc++ assembler none
‘none’意味着恢复默认行为，即根据文件的扩展名猜测源文件的语言
Administrator@72cec870236147e/home/liujl/mytest
$g++ -x c main.cxx
main.cxx:1:20:致命错误：iostream：Nosuch file or directory
编译中断。
用c语言编译main.cxx,由于main.cxx中有#include<iostream>,是c语言中没有的，所以会报错误
Administrator@72cec870236147e/home/liujl/mytest
$g++ -x c++ main.cxx，编译就没问题。
②g++ -M main.cxx 生成文件关联信息。包含目标文件所依赖的所有源代码。
Administrator@72cec870236147e/home/liujl/mytest
$g++ -M main.cxx
main1.o:main1.cxx /usr/lib/gcc/i686-pc-cygwin/4.5.3/include/c++/iostream \
/usr/lib/gcc/i686-pc-cygwin/4.5.3/include/c++/i686-pc-cygwin/bits/[C++](http://lib.csdn.net/base/cplusplus)config.h\
/usr/lib/gcc/i686-pc-cygwin/4.5.3/include/c++/i686-pc-cygwin/bits/os_defines.h\
/usr/lib/gcc/i686-pc-cygwin/4.5.3/include/c++/i686-pc-cygwin/bits/cpu_defines.h\
/usr/lib/gcc/i686-pc-cygwin/4.5.3/include/c++/ostream \
/usr/lib/gcc/i686-pc-cygwin/4.5.3/include/c++/[iOS](http://lib.csdn.net/base/ios) \
/usr/lib/gcc/i686-pc-cygwin/4.5.3/include/c++/iosfwd \
/usr/lib/gcc/i686-pc-cygwin/4.5.3/include/c++/bits/stringfwd.h \
/usr/lib/gcc/i686-pc-cygwin/4.5.3/include/c++/bits/postypes.h \
/usr/lib/gcc/i686-pc-cygwin/4.5.3/include/c++/cwchar \
