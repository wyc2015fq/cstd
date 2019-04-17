# 自己在linux上编译、链接、动态库和静态库的学习笔记 - DoubleLi - 博客园






在平常的项目中，我们都是使用公司要求的makefile、makedebug一类的文件，因此，在编译、链接、生成和链接动态库与静态库的时候，我们只是简单的使用一些已经设置的变量，只是简单的修改、添加一些文件名，或许这次我们编译通过了，但是，在某一个时候，可能出现了一个问题，无论简单与否，因为平常没有留意，导致的结果可能是花了好长时间才能解决。而如果平常只是简单的留一下心，或许这些问题都是可以避免的。 
因此，今天我自己使用几个文件，编译动态库、静态库以及动态库和静态库的嵌套调用等问题，尽量还原我在项目中遇到的问题，尽量让自己明白平常没有意识到的一些东西。 
需要用到的文件列表，如下： 
/****showcoor.cpp****/
#include "showcoor.h"
//显示某一坐标
int showcoor(int x)
{
        cout<<"coordinate:"<<x<<endl;
        return 0;
}


/****showcoor.h****/
#include <iostream>
using namespace std;


int showcoor(int x);




/****showpoint.cpp****/
#include "showpoint.h"
//显示点坐标
int showpoint(int x,int y,int z)
{
        showcoor(x);
        showcoor(y);
        showcoor(z);
        return 0;
}


/****showpoint.h****/
#include "showcoor.h"
int showpoint(int x,int y,int z);


/****main.cpp****/
#include "showpoint.h"
int main()
{
        showpoint(1,2,3);
        return 0;
}
1、编译生成可执行文件 
我们在单独编译的时候，只需要检查头文件，因为都放到了一个目录，所以单独编译的时候，不需要依赖其他文件，就可以生成目标文件（*.o），如下： 
g++ -c showcoor.cpp
g++ -c showpoint.cpp
g++ -c main.cpp
编译完成之后，就生成了对应的目标文件，如下： 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>ls *.o
main.o  showcoor.o  showpoint.o
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>
编译的时候，没有添加依赖的头文件，是因为默认编译的时候，在当前目录下去找。 
小测试1： 
如果我们把showpoint.cpp单独移动到一个目录下，再去编译一下。 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>mkdir temp
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>cp showpoint.cpp showpoint.h ./temp
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>cd ./temp
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>ls
showpoint.cpp


[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call/temp>g++ -c showpoint.cpp      
In file included from showpoint.cpp:1:
showpoint.h:1:22: error: showcoor.h: No such file or directory
showpoint.cpp: In function 'int showpoint(int, int, int)':
showpoint.cpp:4: error: 'showcoor' was not declared in this scope
通过上面可以确定的是，在编译的时候，因为头文件里包含了其他头文件“showcoor.h”，而该头文件不在当前目录，而且不在系统指定的目录，因此，才会提示找不到头文件的错误。这个时候，我们就需要手动的指定头文件所在的目录，使用“-I路径” 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call/temp>g++ -I../ -c showpoint.cpp
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call/temp>ls
showpoint.cpp  showpoint.h  showpoint.o
通过生成的目标文件，最后进行链接，就可以生成最终的可执行文件。 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ -o main main.o showpoint.o showcoor.o
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>./main
coordinate:1
coordinate:2
coordinate:3
通过最后的链接，我们还可以看到函数的依赖关系。 
小测试2： 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ -o main main.o showpoint.o           
showpoint.o: In function `showpoint(int, int, int)':
showpoint.cpp:(.text+0x17): undefined reference to `showcoor(int)'
showpoint.cpp:(.text+0x21): undefined reference to `showcoor(int)'
showpoint.cpp:(.text+0x2b): undefined reference to `showcoor(int)'
collect2: ld returned 1 exit status
因为showpoint.cpp会调用`showcoor(int)'函数，因此，缺少“showcoor.o”文件时，链接的时候，就会提示找不到该函数。 
小测试3： 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ -o main main.o showcoor.o            
main.o: In function `main':
main.cpp:(.text+0x14): undefined reference to `showpoint(int, int, int)'
collect2: ld returned 1 exit status
因为main.cpp会首先调用`showpoint(int, int, int)'函数，因此缺少“showpoint.o”文件时，链接的时候，就会提示找不到“showpoint”函数。 
小测试3： 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ -o main showpoint.o showcoor.o        
/usr/lib/gcc/x86_64-redhat-linux/4.4.6/../../../../lib64/crt1.o: In function `_start':
(.text+0x20): undefined reference to `main'
collect2: ld returned 1 exit status
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>
很明显，因为找不到“main”函数，无法生成最终的可执行文件。 
2、生成静态库 
静态函数库： 
这类库的名字一般是libxxx.a；利用静态函数库编译成的文件比较大，因为整个函数库的所有数据都会被整合进目标代码中，他的优点就显而易见了，即编译后的执行程序不需要外部的函数库支持，因为所有使用的函数都已经被编译进去了。当然这也会成为他的缺点，因为如果静态函数库改变了，那么你的程序必须重新编译。 
扩展阅读： http://tech.ccidnet.com/art/2583/20080303/1378433_1.html  
生成静态库需要使用“ar -cr” 
c   Create the archive，也就是创建静态库 
r   Insert the files member... into archive (with replacement)，也就是没有生成，有的话替换。 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ -c showcoor.cpp             
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>ar -cr libshowcoor.a showcoor.o 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>ls *.a    
libshowcoor.a
当然，我们还可以直接通过“.cpp”文件直接生成静态库。如下： 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>ar -cr libshowcoor.a showcoor.cpp
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>ls *.a           
libshowcoor.a
3、动态库 
动态函数库： 
这类库的名字一般是libxxx.so;相对于静态函数库，动态函数库在编译的时候并没有被编译进目标代码中，你的程序执行到相关函数时才调用该函数库里的相应函数，因此动态函数库所产生的可执行文件比较小。由于函数库没有被整合进你的程序，而是程序运行时动态的申请并调用，所以程序的运行环境中必须提供相应的库。动态函数库的改变并不影响你的程序，所以动态函数库的升级比较方便。 
扩展阅读： http://tech.ccidnet.com/art/2583/20080303/1378433_1.html  
生成动态库的方式是：g++ -shared -fPCI -o libXXX.so *.o  
-fpic 使输出的对象模块是按照可重定位地址方式生成的。 
-shared指定把对应的源文件生成对应的动态链接库文件。 
下面是对于-fpic的详细解释： 
在 Linux 下制作动态链接库，“标准” 的做法是编译成位置无关代码（Position Independent Code，PIC），然后链接成一个动态链接库。 
扩展阅读： http://www.linuxidc.com/Linux/2011-06/37268.htm  
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>gcc -shared -fPIC -o libshowpoint.so showpoint.cpp
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>ls *.so
libshowpoint.so
上述方法是通过.cpp文件直接生成动态库。其实，也可以通过.o文件生成动态库，如下： 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ -c showpoint.cpp
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ -shared -fPIC -o libshowpoint.so showpoint.o  
/usr/bin/ld: showpoint.o: relocation R_X86_64_32 against `.bss' can not be used when making a shared object; recompile with -fPIC
showpoint.o: could not read symbols: Bad value
collect2: ld returned 1 exit status
上面错误产生的原因就是因为参数“-fPIC”的原因，因为最终生成该动态库的时候，使用了参数“-fPIC”，表示生成的动态库是“位置无关代码”，而在生成“showpoint.o”的时候，是位置相关性的，所以，错误信息也提示了，让使用“-fPIC”重新编译（recompile with -fPIC）。 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_callg++ -fPIC -c showpoint.cpp                      
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_callg++ -shared -fPIC -o libshowpoint.so showpoint.op              
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>ls *.so
libshowpoint.so
测试4： 
当然，我们通过上述方式，只是把函数“showpoint”函数封装成了，如果我们在main函数只调用“libshowpoint.so”的话，会出现问题的，如下： 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ main.cpp -L./ -lshowpoint
.//libshowpoint.so: undefined reference to `showcoor(int)'
collect2: ld returned 1 exit status

注：-L后面跟的是要连接的库的路径
-l库名，动态库的命名为libXXX.so的话，就可以使用-lXXX的方式引用该库，如果是静态库，并且命名为libXXX.a，则同样可以使用lXXX的方式引用该库。 
这个时候，我们还需要使用函数“showcoor”所在的库，因此，我们有必要把函数“showcoor”也封装到动态库中。 
当然，我们在最后生成可执行文件的时候，也可以链接同时链接生成的动态库和静态库，如下： 
[billing_dx@bmcs1 nest_call]$ g++ -o main main.cpp -L./ -lshowpoint -lshowcoor
[billing_dx@bmcs1 nest_call]$ ./main
coordinate:1
coordinate:2
coordinate:3
我们可以看到，调用动态库和静态库的方式是一样的，都是“-L路径 -l库名”。 
4、库的嵌套调用 
既然我们已经把函数“showcoor”封装成了静态库，因此我们完全可以通过使用静态库“libshowcoor.a”来生成我们最终的动态库“libshowpoint.so”，因为静态库可以理解成是“目标文件（*.o）的打包”。 
测试5：动态库调用静态库 
我们通过已有的静态库生成最终的动态库 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ -fPIC -c showpoint.cpp
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>gcc -shared -fPIC -o libshowpoint.so showpoint.o  -L./ -lshowcoor
/usr/bin/ld: .//libshowcoor.a(showcoor.o): relocation R_X86_64_32 against `.rodata' can not be used when making a shared object; recompile with -fPIC
.//libshowcoor.a: could not read symbols: Bad value
collect2: ld returned 1 exit status
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>
上述错误提示和原来的没有添加“-fPIC”生成的“*.o”文件生成动态库出现的错误相同，而且也给给出了解决方法，静态库的生成重新使用“-fPIC”进行编译。 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ -fPIC -c showcoor.cpp
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>ar -cr libshowcoor.a showcoor.o
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ -fPIC -c showpoint.cpp          
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ -shared -fPIC -o libshowpoint.so showpoint.o -L./ -lshowcoor
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>ls *.so
libshowpoint.so
这样最终生成的动态库，就同时包含“showpoint”函数和“showoccr”函数，调用如下： 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ main.cpp -L./ -lshowpoint
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>./main
coordinate:1
coordinate:2
coordinate:3
5、使用静态库调用的顺序问题 
如果生成了多个动态库，我们的调用顺序对最终生成的文件没有影响，测试如下： 
[billing_dx@bmcs1 nest_call]$ g++ -fPIC -c showpoint.cpp
[billing_dx@bmcs1 nest_call]$ g++ -shared -fPIC -o libshowpoint.so showpoint.o
[billing_dx@bmcs1 nest_call]$ g++ -fPIC -c showcoor.cpp
[billing_dx@bmcs1 nest_call]$ g++ -shared -fPIC -o libshowcoor.so showcoor.o
[billing_dx@bmcs1 nest_call]$ ls *.so
libshowcoor.so  libshowpoint.so
[billing_dx@bmcs1 nest_call]$ g++ -o main main.cpp -L./ -lshowcoor -lshowpoint
[billing_dx@bmcs1 nest_call]$ ./main
coordinate:1
coordinate:2
coordinate:3


[billing_dx@bmcs1 nest_call]$ g++ -o main main.cpp -L./ -lshowpoint -lshowcoor
[billing_dx@bmcs1 nest_call]$ ./main
coordinate:1
coordinate:2
coordinate:3
但是，如果我们使用了静态库，调用顺序就有影响了，否则的话，编译的时候会因为找不到函数而出错。 
测试6： 
[billing_dx@bmcs1 nest_call]$ g++ -c showcoor.cpp
[billing_dx@bmcs1 nest_call]$ ar -cr libshowcoor.a showcoor.o  
[billing_dx@bmcs1 nest_call]$ g++ -c showpoint.cpp
[billing_dx@bmcs1 nest_call]$ ar -cr libshowpoint.a showpoint.o  
[billing_dx@bmcs1 nest_call]$ ls *.a
libshowcoor.a  libshowpoint.a
[billing_dx@bmcs1 nest_call]$ g++ -o main main.cpp -L./ -lshowpoint -lshowcoor
[billing_dx@bmcs1 nest_call]$ ./main
coordinate:1
coordinate:2
coordinate:3
调整静态库的调用顺序： 
[billing_dx@bmcs1 nest_call]$g++ -o main main.cpp -L./ -lshowcoor -lshowpoint
.//libshowpoint.a(showpoint.o): In function `showpoint(int, int, int)':
showpoint.cpp:(.text+0x17): undefined reference to `showcoor(int)'
showpoint.cpp:(.text+0x21): undefined reference to `showcoor(int)'
showpoint.cpp:(.text+0x2b): undefined reference to `showcoor(int)'
collect2: ld returned 1 exit status
通过上面，我们可以知道，被调用的库应该放到调用库的后面。因为函数“showpoint”中调用了函数“showcoor”，因此，函数“showcoor”对应的库应该放到后面才行。 
因此，如果遇到了程序需要调用A库，而A库调用B库，那么我们应该写成“g++ -o main main.cpp -L./ -libA.so -libB.so”,也就是网上所说的，调用静态库的时候，于是基础的库，越是放到最后。 
6、库内部函数的查看 
nm用来列出目标文件的符号清单。 
扩展阅读： http://www.linuxidc.com/Linux/2011-05/35777.htm  
nm 命令使用以下符号（用同样的字符表示弱符号作为全局符号）之一来表示文件符号类型： 
A    Global absolute 符号。 
a    Local absolute 符号。 
B    Global bss 符号。 
b    Local bss 符号。 
D    Global data 符号。 
d    Local data 符号。 
f    源文件名称符号。 
T    Global text 符号。 
t    Local text 符号。 
U    未定义符号。 
扩展阅读： http://blog.chinaunix.net/uid-28458801-id-3475711.html  
上述包含“showcoor”函数和“showpoint”函数时生成的动态库，查看内部函数： 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>nm -C libshowpoint.so |grep show
0000000000000a5f t global constructors keyed to showcoor.cpp
00000000000009ba t global constructors keyed to showpoint.cpp
00000000000009d0 T showcoor(int)
000000000000093c T showpoint(int, int, int)
如果采用只封装“showpoint”函数的方式生成的动态库，查看内部函数： 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ -fPIC -c showpoint.cpp
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>gcc -shared -fPIC -o libshowpoint.so showpoint.o  
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>nm -C libshowpoint.so |grep show
000000000000076a t global constructors keyed to showpoint.cpp
                 U showcoor(int)
00000000000006ec T showpoint(int, int, int)
我们可以看到，这个时候，虽然在“showpoint”函数中调用“showcoor”函数，但是该函数在库中并未定义，所以类型是“U”。 
7、库依赖关系的查看 
ldd命令用于判断某个可执行的 binary 档案含有什么动态函式库。 
但是ldd本身不是一个程序，而仅是一个shell脚本： 
$ which ldd 
/usr/bin/ldd 
$ file /usr/bin/ldd  
/usr/bin/ldd: Bourne-Again shell script text executable 
扩展阅读： http://blog.chinaunix.net/uid-23622436-id-3235778.html  
使用上述包含“showcoor”函数和“showpoint”函数时生成的动态库，生成可执行文件，查看依赖库： 
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>gcc -shared -fPIC -o libshowpoint.so showpoint.cpp showcoor.cpp
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ -o main main.cpp -L./ -lshowpoint
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>./main  
coordinate:1
coordinate:2
coordinate:3
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>ldd main |grep show
        libshowpoint.so => /account/work/ymm/test/library/nest_call/libshowpoint.so (0x00002ba77c1ad000)
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>
如果两个函数分别生成一个动态库的话，查看依赖库：     
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>gcc -shared -fPIC -o libshowpoint.so showpoint.cpp
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>gcc -shared -fPIC -o libshowcoor.so showcoor.cpp  
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>ls *.so
libshowcoor.so  libshowpoint.so
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>g++ -o main main.cpp -L./ -lshowpoint -lshowcoor
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>./main  
coordinate:1
coordinate:2
coordinate:3
[billing_dx@bmcs1]:/account/work/ymm/test/library/nest_call>ldd main|grep show
        libshowpoint.so => /account/work/ymm/test/library/nest_call/libshowpoint.so (0x00002abf7504a000)
        libshowcoor.so => /account/work/ymm/test/library/nest_call/libshowcoor.so (0x00002abf7524b000)
可以看到，通过这种方式，就引用了两个库。 
通过这个简单的例子，虽然不能说让自己精通编译、链接、静态库和动态库，但是，我想，对于我的学习还是很有帮助的。最起码，当我下次在遇到类似的问题的时候，我可以更容易的想到是因为什么原因。而这，就够了！ 









