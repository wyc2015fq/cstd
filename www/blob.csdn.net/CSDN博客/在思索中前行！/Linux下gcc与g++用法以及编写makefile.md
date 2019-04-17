# Linux下gcc与g++用法以及编写makefile - 在思索中前行！ - CSDN博客





2014年09月19日 15:20:59[_Tham](https://me.csdn.net/txl16211)阅读数：2940








1.gcc与g++编译流程：

1)编译流程：

![](http://img1.51cto.com/attachment/201005/202140169.jpg)

2)预处理：生成.i的预处理文件。

Ø 只激活预处理，这个不生成文件，需要把它重定向一个输出文件。

Ø 演示：

![](http://img1.51cto.com/attachment/201005/202229623.jpg)

3)编译：生成.s的编译文件。

Ø 只激活预处理和编译，把文件编译成汇编代码。

Ø 演示：

![](http://img1.51cto.com/attachment/201005/202316799.jpg)

4)汇编：生成.o的汇编文件。

Ø 只激活预处理、编译和汇编，把程序做成obj文件。

Ø 演示：

![](http://img1.51cto.com/attachment/201005/202338254.jpg)

5)链接：生成链接文件。

Ø 激活预处理、编译、汇编和链接。

Ø 演示：

![](http://img1.51cto.com/attachment/201005/202710955.jpg)

6)惯用：

![](http://img1.51cto.com/attachment/201005/202738894.jpg)




## **1、GCC/G++基本命了简介**

gcc & g++现在是gnu中最主要和最流行的c & c++编译器 。
g++是c++的命令，以.cpp为主，对于c语言后缀名一般为.c。这时候命令换做gcc即可。其实是无关紧要的。
其实编译器是根据gcc还是g++来确定是按照C标准还是C++标准编译链接。
下面以Test.cpp为例：
命令： g++ Test.cpp
功能：生成默认为a.exe的文件，这个过程包含了编译和链接。
再说下-o命令，-o命令表示输出的意思，gcc/g++命令是非常灵活的，你不指定输出的文件名的时候默认生成的是.exe文件。
你要输出Test.exe的话可以用：g++ -o Test.exe Test.cpp。-o命令是输出的意思，这样就输出了Test.exe。

gcc/g++在执行编译工作的时候，总共需要以下几步:




## 1.预处理,生成.i的文件[预处理器cpp]



命令：g++ -E Test.cpp > Test.i 

> 
功能：输出预处理后的文件，linux下以.i为后缀名。只激活预处理,这个不生成文件,你需要把它重定向到一个输出文件里 。这一步主要做了这些事情：宏的替换，还有注释的消除，还有找到相关的库文件。用编辑器打开Test.i会发现有很多很多代码，你只需要看最后部分就会发现，预处理做了宏的替换，还有注释的消除，可以理解为无关代码的清除。下面是Test.i文件的最后部分，可以看见宏的替换和注释的消除。


## 2.将预处理后的文件不转换成汇编语言,生成文件.s[编译器egcs]
命令:g++ -S Test.cpp
功能:会生成Test.s文件，.s文件表示是汇编文件，用编辑器打开就都是汇编指令。

## 3.有汇编变为目标代码(机器代码)生成.o的文件[汇编器as]
命令:g++ -c Test.cpp 
功能：.o是GCC生成的目标文件，除非你是做编译器和连接器调试开发的，否则打开这种.o没有任何意义。二进制机器码一般人也读不了。

## 4.连接目标代码,生成可执行程序[链接器ld]
命令：g++ Test.o
 -L F:\vs2008\VC\include\iostream
功能：将.o文件与所需的库文件链接整合形成.exe文件，这就是可执行文件。-L 表示链接，这里我后面写的是绝对路径，相对各人电脑不同

在上面各个步骤中你可以用-o命令输出你自己想要的各种名字。比如最后一个命令，用下面的输出Test.exe
你可以g++ Test.o -o Test.exe -L F:\vs2008\VC\include\iostream



## [Linux GCC常用命令](http://www.cnblogs.com/ggjucheng/archive/2011/12/14/2287738.html)



#### 1简介

GCC 的意思也只是 GNU C Compiler 而已。经过了这么多年的发展，GCC 已经不仅仅能支持 C 语言；它现在还支持 Ada 语言、C++ 语言、Java 语言、Objective C 语言、Pascal 语言、COBOL语言，以及支持函数式编程和逻辑编程的 Mercury 语言，等等。而 GCC 也不再单只是 GNU C 语言编译器的意思了，而是变成了
 GNU Compiler Collection 也即是 GNU 编译器家族的意思了。另一方面，说到 GCC 对于操作系统平台及硬件平台支持，概括起来就是一句话：无所不在。

#### 2简单编译

示例程序如下：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

//test.c
#include <stdio.h>
int main(void)
{
    printf("Hello World!\n");
    return 0;
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)



这个程序，一步到位的编译指令是:


```
gcc test.c -o test
```

实质上，上述编译过程是分为四个阶段进行的，即预处理(也称预编译，Preprocessing)、编译(Compilation)、汇编 (Assembly)和连接(Linking)。

##### 2.1预处理

```
gcc -E test.c -o test.i 或 gcc -E test.c
```

可以输出test.i文件中存放着test.c经预处理之后的代码。打开test.i文件，看一看，就明白了。后面那条指令，是直接在命令行窗口中输出预处理后的代码.

gcc的-E选项，可以让编译器在预处理后停止，并输出预处理结果。在本例中，预处理结果就是将stdio.h 文件中的内容插入到test.c中了。

##### 2.2编译为汇编代码(Compilation)

预处理之后，可直接对生成的test.i文件编译，生成汇编代码：

```
gcc -S test.i -o test.s
```

gcc的-S选项，表示在程序编译期间，在生成汇编代码后，停止，-o输出汇编代码文件。

##### 2.3汇编(Assembly)

对于上一小节中生成的汇编代码文件test.s，gas汇编器负责将其编译为目标文件，如下：

```
gcc -c test.s -o test.o
```

##### 2.4连接(Linking)

gcc连接器是gas提供的，负责将程序的目标文件与所需的所有附加的目标文件连接起来，最终生成可执行文件。附加的目标文件包括静态连接库和动态连接库。

对于上一小节中生成的test.o，将其与Ｃ标准输入输出库进行连接，最终生成程序test

```
gcc test.o -o test
```

在命令行窗口中，执行./test, 让它说HelloWorld吧！

#### 3多个程序文件的编译

通常整个程序是由多个源文件组成的，相应地也就形成了多个编译单元，使用GCC能够很好地管理这些编译单元。假设有一个由test1.c和 test2.c两个源文件组成的程序，为了对它们进行编译，并最终生成可执行程序test，可以使用下面这条命令：

gcc test1.c test2.c -o test

如果同时处理的文件不止一个，GCC仍然会按照预处理、编译和链接的过程依次进行。如果深究起来，上面这条命令大致相当于依次执行如下三条命令：

```
gcc -c test1.c -o test1.o
gcc -c test2.c -o test2.o
gcc test1.o test2.o -o test
```

#### 4检错

```
gcc -pedantic illcode.c -o illcode
```

-pedantic编译选项并不能保证被编译程序与ANSI/ISO C标准的完全兼容，它仅仅只能用来帮助Linux程序员离这个目标越来越近。或者换句话说，-pedantic选项能够帮助程序员发现一些不符合 ANSI/ISO C标准的代码，但不是全部，事实上只有ANSI/ISO C语言标准中要求进行编译器诊断的那些情况，才有可能被GCC发现并提出警告。

除了-pedantic之外，GCC还有一些其它编译选项也能够产生有用的警告信息。这些选项大多以-W开头，其中最有价值的当数-Wall了，使用它能够使GCC产生尽可能多的警告信息。

```
gcc -Wall illcode.c -o illcode
```

GCC给出的警告信息虽然从严格意义上说不能算作错误，但却很可能成为错误的栖身之所。一个优秀的Linux程序员应该尽量避免产生警告信息，使自己的代码始终保持标准、健壮的特性。所以将警告信息当成编码错误来对待，是一种值得赞扬的行为！所以，在编译程序时带上-Werror选项，那么GCC会在所有产生警告的地方停止编译，迫使程序员对自己的代码进行修改，如下：

```
gcc -Werror test.c -o test
```

#### 5库文件连接

开发软件时，完全不使用第三方函数库的情况是比较少见的，通常来讲都需要借助许多函数库的支持才能够完成相应的功能。从程序员的角度看，函数库实际上就是一些头文件（.h）和库文件（so、或lib、dll）的集合。。虽然Linux下的大多数函数都默认将头文件放到/usr/include/目录下，而库文件则放到/usr/lib/目录下；Windows所使用的库文件主要放在Visual
 Stido的目录下的include和lib，以及系统文件夹下。但也有的时候，我们要用的库不再这些目录下，所以GCC在编译时必须用自己的办法来查找所需要的头文件和库文件。

例如我们的程序test.c是在linux上使用c连接mysql，这个时候我们需要去mysql官网下载MySQL Connectors的C库，下载下来解压之后，有一个include文件夹，里面包含mysql connectors的头文件，还有一个lib文件夹，里面包含二进制so文件libmysqlclient.so

其中inclulde文件夹的路径是/usr/dev/mysql/include,lib文件夹是/usr/dev/mysql/lib

##### 5.1编译成可执行文件

首先我们要进行编译test.c为目标文件，这个时候需要执行

```
gcc –c –I /usr/dev/mysql/include test.c –o test.o
```

##### 5.2链接

最后我们把所有目标文件链接成可执行文件:

```
gcc –L /usr/dev/mysql/lib –lmysqlclient test.o –o test
```

Linux下的库文件分为两大类分别是动态链接库（通常以.so结尾）和静态链接库（通常以.a结尾），二者的区别仅在于程序执行时所需的代码是在运行时动态加载的，还是在编译时静态加载的。

##### 5.3强制链接时使用静态链接库

默认情况下， GCC在链接时优先使用动态链接库，只有当动态链接库不存在时才考虑使用静态链接库，如果需要的话可以在编译时加上-static选项，强制使用静态链接库。

在/usr/dev/mysql/lib目录下有链接时所需要的库文件libmysqlclient.so和libmysqlclient.a，为了让GCC在链接时只用到静态链接库，可以使用下面的命令:

```
gcc –L /usr/dev/mysql/lib –static –lmysqlclient test.o –o test
```

静态库链接时搜索路径顺序：

1. ld会去找GCC命令中的参数-L

2. 再找gcc的环境变量LIBRARY_PATH

3. 再找内定目录 /lib /usr/lib /usr/local/lib 这是当初compile gcc时写在程序内的
动态链接时、执行时搜索路径顺序:

1. 编译目标代码时指定的动态库搜索路径

2. 环境变量LD_LIBRARY_PATH指定的动态库搜索路径

3. 配置文件/etc/ld.so.conf中指定的动态库搜索路径

4. 默认的动态库搜索路径/lib

5. 默认的动态库搜索路径/usr/lib
有关环境变量：

LIBRARY_PATH环境变量：指定程序静态链接库文件搜索路径

LD_LIBRARY_PATH环境变量：指定程序动态链接库文件搜索路径
## **3、15个常用GCC命令简介**






GCC编译器非常强大 ，在各个发行的linux系统中都非常流行，本文介绍的是一些常用的gcc编译选项




下面这段代码将回围绕整个文章:


辑main.c如下.





**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- #include<stdio.h>
- 
- int main(void)  
- {  
-    printf("\n The Geek Stuff\n");  
- return 0;  
- }  



GCC编译选项

1.指定输出可执行文件的名字


使用最基本的gcc编译格式





**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- gcc main.c  

执行完上面这句命令，会在当前目录下输出一个名为a.out的可执行文件。



使用 -o选项可以指定输出的可执行文件名称。





**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- gcc main.c -o main  

执行完上面语句会在当前目录下输出一个名为main的可执行文件。



要想知道gcc编译器编译执行的过程请参考下面这个链接 

[http://www.thegeekstuff.com/2011/10/c-program-to-an-executable/](http://www.thegeekstuff.com/2011/10/c-program-to-an-executable/)


2.让所有编译警告都显示出来

编辑一段带警告的代码如下：





**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- <span style="font-size:14px;">#include<stdio.h>  
- 
- int main(void)  
- {  
- int i;  
-    printf("\n The Geek Stuff [%d]\n", i);  
- return 0;  
- }</span>  




**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -Wall main.c -o main  
- main.c: In function â€˜mainâ€™:  
- main.c:6:10: warning: â€˜iâ€™ is used uninitialized in this function [-Wuninitialized]  



执行gcc -Wall main.c -o main 会得到未初始化变量i的警告.


3.指定 -E编译选项，使得只输出预编译结果





**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -E main.c > main.i  


上面这条gcc 编译命令会将输出重定向到输出文件当中。上面的例子中，main.i文件中的内容就是执行-E选项gcc命令的结果。



4.通过编译选项 -S 输出汇编代码





**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- gcc -S main.c > main.s  

main.s 会包含main.c的汇编输出代码



5.指定-C 输出编译后的代码





**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- gcc -C main.c  


执行上面这条命令会输出main.o文件，包含机器指令代码或者编译后的代码。



6.通过编译选项-save-temps 输出所有的中间代码。



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -save-temps main.c  
- 
- $ ls  
- a.out  main.c  main.i  main.o  main.s  



7.链接共享库(动态链接库)指定编译选项 -l





**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- gcc  -Wall main.c -o main -lCPPfile  

gcc命令指出再执行链接main.c 代码时，会链接上-lCPPfile.so动态链接库来完成生成main可执行文件。



8.指定编译选项-fPIC创建独立的(无关联的)地址信息代码。

当创建动态链接库时，独立位置信息(position
 independent)代码也需要生成。这可以帮助动态链接库或者跟多的加载地址信息来替代其他相对的地址信息。所以-fPIC这个选项作用很大，能快速准确定位错误地址。

下面是一个例子，





**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -c -Wall -Werror -fPIC Cfile.c  
- $ gcc -shared -o libCfile.so Cfile.o  

9.打印输出有个执行过程的信息 使用-V选项


当编译源文件时，-V选项会显示所有编译步骤的调试信息。

例子:





**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -Wall -v main.c -o main  
- Using built-in specs.  
- COLLECT_GCC=gcc  
- COLLECT_LTO_WRAPPER=/usr/lib/gcc/i686-linux-gnu/4.6/lto-wrapper  
- Target: i686-linux-gnu  
- Configured with: ../src/configure -v --with-pkgversion='Ubuntu/Linaro 4.6.3-1ubuntu5' --with-bugurl=file:///usr/share/doc/gcc-4.6/README.Bugs --enable-languages=c,c++,fortran,objc,obj-c++ --prefix=/usr --program-suffix=-4.6 --enable-shared --enable-linker-build-id --with-system-zlib --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --with-gxx-include-dir=/usr/include/c++/4.6 --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --enable-gnu-unique-object --enable-plugin --enable-objc-gc --enable-targets=all --disable-werror --with-arch-32=i686 --with-tune=generic --enable-checking=release --build=i686-linux-gnu --host=i686-linux-gnu --target=i686-linux-gnu
- Thread model: posix  
- gcc version 4.6.3 (Ubuntu/Linaro 4.6.3-1ubuntu5)  
- ...  
- ...  
- ...  

10.指定编译选项-ansi，支持ISO C89 programs.


通过-ansi选项开启支ISO　Ｃ89风格．

看如下代码：





**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- #include<stdio.h>
- 
- int main(void)  
- {  
- // Print the string
-    printf("\n The Geek Stuff\n");  
- return 0;  
- }  

执行上面代码附加-ansi编译选项，编译器会输出错误因为c++ 不支持ISO C89风格。




**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -Wall -ansi main.c -o main  
- main.c: In function â€˜mainâ€™:  
- main.c:5:3: error: expected expression before â€˜/â€™ token  

gcc 会抛出上面错误信息。


11.指定编译选项 -funsigned-char选项将char类型解释为unsigned
 char类型。

通过这个编译选项，char 类型会被认为是unsigned
 char.

例子:



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- #include<stdio.h>
- 
- int main(void)  
- {  
- char c = -10;  
- // Print the string
-    printf("\n The Geek Stuff [%d]\n", c);  
- return 0;  
- }  

执行上面代码输出：


**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -Wall -funsigned-char main.c -o main  
- $ ./main  
- 
-  The Geek Stuff [246]  

12.指定编译选项 -fsigned-char选项将unsigned char类型解释为 char类型。


**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -Wall -fsigned-char main.c -o main  
- $ ./main  
- 
-  The Geek Stuff [-10]  



13.指定-D选项 开启编译时的宏


例子如下:



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- #include<stdio.h>
- 
- int main(void)  
- {  
- #ifdef MY_MACRO
-   printf("\n Macro defined \n");  
- #endif
- char c = -10;  
- // Print the string
-    printf("\n The Geek Stuff [%d]\n", c);  
- return 0;  
- }  


通过编译选项 可以直接定义宏




**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -Wall -DMY_MACRO main.c -o main  
- $ ./main  
- 
-  Macro defined   
- 
-  The Geek Stuff [-10]  

14.将编译警告转换成错误.



编译警告很多时候会被我们忽视，在特殊场合我们还是需要重视编译警告，如果能把编译警告变长直接输出错误，那我们的重视程度会提高很多并去解决。


example:



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- #include<stdio.h>
- 
- int main(void)  
- {  
- char c;  
- // Print the string
-    printf("\n The Geek Stuff [%d]\n", c);  
- return 0;  
- }  




**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -Wall -Werror main.c -o main  
- main.c: In function â€˜mainâ€™:  
- main.c:7:10: error: â€˜câ€™ is used uninitialized in this function [-Werror=uninitialized]  
- cc1: all warnings being treated as errors  

上述代码未初始化变量c，警告变成了错误提示.



15.通过文件指定编译选项，指定@编译选项


比较神奇的功能。可以使用@编译选项然后跟着文件名，一个以上的编译选项用空格 隔开。


example:



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ cat opt_file  
- -Wall -omain  




**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc main.c @opt_file  
- main.c: In function ‘main’:  
- main.c:6:11: warning: ‘i’ is used uninitialized in this function [-Wuninitialized]  
- 
- $ ls main  
- main  

GCC编译器非常强大 ，在各个发行的linux系统中都非常流行，本文介绍的是一些常用的gcc编译选项




下面这段代码将回围绕整个文章:


编辑main.c如下.





**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- #include<stdio.h>
- 
- int main(void)  
- {  
-    printf("\n The Geek Stuff\n");  
- return 0;  
- }  



GCC编译选项

1.指定输出可执行文件的名字


使用最基本的gcc编译格式



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- gcc main.c  

执行完上面这句命令，会在当前目录下输出一个名为a.out的可执行文件。



使用 -o选项可以指定输出的可执行文件名称。



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- gcc main.c -o main  


执行完上面语句会在当前目录下输出一个名为main的可执行文件。



要想知道gcc编译器编译执行的过程请参考下面这个链接 

[http://www.thegeekstuff.com/2011/10/c-program-to-an-executable/](http://www.thegeekstuff.com/2011/10/c-program-to-an-executable/)


2.让所有编译警告都显示出来

编辑一段带警告的代码如下：



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- <span style="font-size:14px;">#include<stdio.h>  
- 
- int main(void)  
- {  
- int i;  
-    printf("\n The Geek Stuff [%d]\n", i);  
- return 0;  
- }</span>  




**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -Wall main.c -o main  
- main.c: In function â€˜mainâ€™:  
- main.c:6:10: warning: â€˜iâ€™ is used uninitialized in this function [-Wuninitialized]  



执行gcc -Wall main.c -o main 会得到未初始化变量i的警告.


3.指定 -E编译选项，使得只输出预编译结果





**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -E main.c > main.i  


上面这条gcc 编译命令会将输出重定向到输出文件当中。上面的例子中，main.i文件中的内容就是执行-E选项gcc命令的结果。



4.通过编译选项 -S 输出汇编代码





**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- gcc -S main.c > main.s  

main.s 会包含main.c的汇编输出代码



5.指定-C 输出编译后的代码



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- gcc -C main.c  

执行上面这条命令会输出main.o文件，包含机器指令代码或者编译后的代码。



6.通过编译选项-save-temps 输出所有的中间代码。



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -save-temps main.c  
- 
- $ ls  
- a.out  main.c  main.i  main.o  main.s  



7.链接共享库(动态链接库)指定编译选项 -l



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- gcc  -Wall main.c -o main -lCPPfile  

gcc命令指出再执行链接main.c 代码时，会链接上-lCPPfile.so动态链接库来完成生成main可执行文件。



8.指定编译选项-fPIC创建独立的(无关联的)地址信息代码。

当创建动态链接库时，独立位置信息(position
 independent)代码也需要生成。这可以帮助动态链接库或者跟多的加载地址信息来替代其他相对的地址信息。所以-fPIC这个选项作用很大，能快速准确定位错误地址。

下面是一个例子，



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -c -Wall -Werror -fPIC Cfile.c  
- $ gcc -shared -o libCfile.so Cfile.o  

9.打印输出有个执行过程的信息 使用-V选项


当编译源文件时，-V选项会显示所有编译步骤的调试信息。

例子:





**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -Wall -v main.c -o main  
- Using built-in specs.  
- COLLECT_GCC=gcc  
- COLLECT_LTO_WRAPPER=/usr/lib/gcc/i686-linux-gnu/4.6/lto-wrapper  
- Target: i686-linux-gnu  
- Configured with: ../src/configure -v --with-pkgversion='Ubuntu/Linaro 4.6.3-1ubuntu5' --with-bugurl=file:///usr/share/doc/gcc-4.6/README.Bugs --enable-languages=c,c++,fortran,objc,obj-c++ --prefix=/usr --program-suffix=-4.6 --enable-shared --enable-linker-build-id --with-system-zlib --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --with-gxx-include-dir=/usr/include/c++/4.6 --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --enable-gnu-unique-object --enable-plugin --enable-objc-gc --enable-targets=all --disable-werror --with-arch-32=i686 --with-tune=generic --enable-checking=release --build=i686-linux-gnu --host=i686-linux-gnu --target=i686-linux-gnu
- Thread model: posix  
- gcc version 4.6.3 (Ubuntu/Linaro 4.6.3-1ubuntu5)  
- ...  
- ...  
- ...  

10.指定编译选项-ansi，支持ISO C89 programs.


通过-ansi选项开启支ISO　Ｃ89风格．

看如下代码：



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- #include<stdio.h>
- 
- int main(void)  
- {  
- // Print the string
-    printf("\n The Geek Stuff\n");  
- return 0;  
- }  

执行上面代码附加-ansi编译选项，编译器会输出错误因为c++ 不支持ISO C89风格。






**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -Wall -ansi main.c -o main  
- main.c: In function â€˜mainâ€™:  
- main.c:5:3: error: expected expression before â€˜/â€™ token  

gcc 会抛出上面错误信息。


11.指定编译选项 -funsigned-char选项将char类型解释为unsigned
 char类型。

通过这个编译选项，char 类型会被认为是unsigned
 char.

例子:



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- #include<stdio.h>
- 
- int main(void)  
- {  
- char c = -10;  
- // Print the string
-    printf("\n The Geek Stuff [%d]\n", c);  
- return 0;  
- }  

执行上面代码输出：


**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -Wall -funsigned-char main.c -o main  
- $ ./main  
- 
-  The Geek Stuff [246]  

12.指定编译选项 -fsigned-char选项将unsigned char类型解释为 char类型。


**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -Wall -fsigned-char main.c -o main  
- $ ./main  
- 
-  The Geek Stuff [-10]  



13.指定-D选项 开启编译时的宏


例子如下:



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- #include<stdio.h>
- 
- int main(void)  
- {  
- #ifdef MY_MACRO
-   printf("\n Macro defined \n");  
- #endif
- char c = -10;  
- // Print the string
-    printf("\n The Geek Stuff [%d]\n", c);  
- return 0;  
- }  

通过编译选项 可以直接定义宏




**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -Wall -DMY_MACRO main.c -o main  
- $ ./main  
- 
-  Macro defined   
- 
-  The Geek Stuff [-10]  

14.将编译警告转换成错误.



编译警告很多时候会被我们忽视，在特殊场合我们还是需要重视编译警告，如果能把编译警告变长直接输出错误，那我们的重视程度会提高很多并去解决。


example:



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- #include<stdio.h>
- 
- int main(void)  
- {  
- char c;  
- // Print the string
-    printf("\n The Geek Stuff [%d]\n", c);  
- return 0;  
- }  




**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc -Wall -Werror main.c -o main  
- main.c: In function â€˜mainâ€™:  
- main.c:7:10: error: â€˜câ€™ is used uninitialized in this function [-Werror=uninitialized]  
- cc1: all warnings being treated as errors  

上述代码未初始化变量c，警告变成了错误提示.



15.通过文件指定编译选项，指定@编译选项


比较神奇的功能。可以使用@编译选项然后跟着文件名，一个以上的编译选项用空格 隔开。


example:



**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ cat opt_file  
- -Wall -omain  




**[cpp]**[view
 plain](http://blog.csdn.net/typename/article/details/8170213#)[copy](http://blog.csdn.net/typename/article/details/8170213#)



- $ gcc main.c @opt_file  
- main.c: In function ‘main’:  
- main.c:6:11: warning: ‘i’ is used uninitialized in this function [-Wuninitialized]  
- 
- $ ls main  
- main
```






