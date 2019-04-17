# GCC 编译使用动态链接库和静态链接库 - DoubleLi - 博客园






1 库的分类

根据链接时期的不同，库又有静态库和动态库之分。

静态库是在链接阶段被链接的（好像是废话，但事实就是这样），所以生成的可执行文件就不受库的影响了，即使库被删除了，程序依然可以成功运行。

有别于静态库，动态库的链接是在程序执行的时候被链接的。所以，即使程序编译完，库仍须保留在系统上，以供程序运行时调用。（TODO：链接动态库时链接阶段到底做了什么）

2 静态库和动态库的比较

链接静态库其实从某种意义上来说也是一种粘贴复制，只不过它操作的对象是目标代码而不是源码而已。因为静态库被链接后库就直接嵌入可执行文件中了，这样就带来了两个问题。

首先就是系统空间被浪费了。这是显而易见的，想象一下，如果多个程序链接了同一个库，则每一个生成的可执行文件就都会有一个库的副本，必然会浪费系统空间。

再者，人非圣贤，即使是精心调试的库，也难免会有错。一旦发现了库中有bug，挽救起来就比较麻烦了。必须一一把链接该库的程序找出来，然后重新编译。

而动态库的出现正弥补了静态库的以上弊端。因为动态库是在程序运行时被链接的，所以磁盘上只须保留一份副本，因此节约了磁盘空间。如果发现了bug或要升级也很简单，只要用新的库把原来的替换掉就行了。

那么，是不是静态库就一无是处了呢？

答曰：非也非也。不是有句话么：存在即是合理。静态库既然没有湮没在滔滔的历史长河中，就必然有它的用武之地。想象一下这样的情况：如果你用libpcap库编了一个程序，要给被人运行，而他的系统上没有装pcap库，该怎么解决呢？最简单的办法就是编译该程序时把所有要链接的库都链接它们的静态库，这样，就可以在别人的系统上直接运行该程序了。

所谓有得必有失，正因为动态库在程序运行时被链接，故程序的运行速度和链接静态库的版本相比必然会打折扣。然而瑕不掩瑜，动态库的不足相对于它带来的好处在现今硬件下简直是微不足道的，所以链接程序在链接时一般是优先链接动态库的，除非用-static参数指定链接静态库。







**动态链接库**

1. 创建动态链接库





**[cpp]**[view plain](http://blog.csdn.net/orzlzro/article/details/6460058#)[copy](http://blog.csdn.net/orzlzro/article/details/6460058#)



- #include<stdio.h>  
- void hello()  
- {  
-         printf("hello world/n");  
- }  






用命令gcc -shared hello.c -o libhello.so编译为动态库。可以看到，当前目录下多了一个文件libhello.so。



2. 再编辑一个测试文件test.c，内容如下





**[cpp]**[view plain](http://blog.csdn.net/orzlzro/article/details/6460058#)[copy](http://blog.csdn.net/orzlzro/article/details/6460058#)



- #include<stdio.h>  
- int main()  
- {  
-         printf("call hello()");  
-         hello();  
- }  






编译 gcc test.c -lhello

-l 选项告诉编译器要使用hello这个库。奇怪的地方是动态库的名字是libhello.so,这里却使用hello.



但这样还不行，编译会出错。

In function `main':



test.c:(.text+0x1d): undefined reference to `hello'

collect2: ld returned 1 exit status



这是因为hello这个库在我们自己的路径中，编译器找不到。

需要使用-L选项，告诉hello库的位置



gcc test.c -lhello -L. -o test



-L .告诉编译器在当前目录中查找库文件





3. 编译成功后执行./test, 仍然出错

说找不到库



有两种方法：

一、可以把当前路径加入 /etc/ld.so.conf中然后运行ldconfig，或者以当前路径为参数运行ldconfig（要有root权限才行）。

二、把当前路径加入环境变量LD_LIBRARY_PATH中



当然，如果你觉得不会引起混乱的话，可以直接把该库拷入/lib,/usr/lib/等位置（无可避免，这样做也要有权限），这样链接器和加载器就都可以准确的找到该库了。





我们采用第二种方法：



export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH



这样，再执行就成功了。











**下面再讲讲静态链接库**

仍使用刚才的hello.c和test.c。



1. gcc -c hello.c 注意这里没有使用-shared选项

2. 把目标文件归档    ar -r libhello.a hello.o





    程序 ar 配合参数 -r 创建一个新库 libhello.a 并将命令行中列出的对象文件插入。采用这种方法，如果库不存在的话，参数 -r 将创建一个新的库，而如果库存在的话，将用新的模块替换原来的模块。

3. 在程序中链接静态库

           gcc test.c -lhello -L. -static -o hello.static 





或者   gcc test.c libhello.a -L. -o hello.static



生成的hello.static就不再依赖libhello.a了













**两个有用的命令**





file程序是用来判断文件类型的，在file命令下，所有文件都会原形毕露的。

顺便说一个技巧。有时在 windows下用浏览器下载tar.gz或tar.bz2文件，后缀名会变成奇怪的tar.tar，到Linux有些新手就不知怎么解压了。但 Linux下的文件类型并不受文件后缀名的影响，所以我们可以先用命令file xxx.tar.tar看一下文件类型，然后用tar加适当的参数解压。
||
|----|



另外，还可以借助程序ldd实用程序来判断。

ldd是用来打印目标程序（由命令行参数指定）所链接的所有动态库的信息的，如果目标程序没有链接动态库，则打印“not a dynamic executable”，ldd的用法请参考manpage。









