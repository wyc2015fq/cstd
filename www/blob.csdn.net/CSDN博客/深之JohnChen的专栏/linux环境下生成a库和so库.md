# linux环境下生成a库和so库 - 深之JohnChen的专栏 - CSDN博客

2018年03月20日 14:44:06[byxdaz](https://me.csdn.net/byxdaz)阅读数：550


一、编译链接库（a库和so库）方法
一般linux环境下的链接库分为a库和so库，分别表示静态链接库和动态链接库，其作用是把C程序编译好做成一种可执行链接文件，c主程序文件调用这些程序的函数接口是可以使用a库或so库，在主程序中只需要include含有库中提供的函数接口声明的头文件即可。
在linux环境中, 使用ar命令创建静态库文件.如下是命令的选项:           
d -----从指定的静态库文件中删除文件           
m -----把文件移动到指定的静态库文件中           
p -----把静态库文件中指定的文件输出到标准输出           
q -----快速地把文件追加到静态库文件中           
r -----把文件插入到静态库文件中           
t -----显示静态库文件中文件的列表           
x -----从静态库文件中提取文件       
还有多个修饰符修改以上基本选项,详细请man ar 
ar 命令的命令行格式如下:       
ar [-]{dmpqrtx}[abcfilNoPsSuvV] [membername] [count] archive files... 参数archive定义库的名称, files是库文件中包含的目标文件的清单, 用空格分隔每个文件. 比如创建一个静态库文件的命令如下:  
ar -r libapue.a error.o errorlog.o lockreg.o 

动态库 
1.创建共享库      gcc -fPIC -shared -o libapue.so error.o errorlog.o 这样就创建了共享库!。
2.编译共享库     假设共享库位于当前目录(即跟程序文件相同的目录中) gcc -o test -L. -lapue test.c 这样就编译出了不包含函数代码可执行文件了,但是你运行时会发现linux动态加载器打不到libapue.so文件. 可以用ldd 命令查看可执行文件依赖什么共享库: ldd test 如何才能让动态加载器发现库文件呢?有两种方法可以解决: LD_LIBRARY_PATH 环境变量和/etc/ld.so.conf文件     
1).环境变量        export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:."     
2).修改/etc/ld.so.conf文件.位于/etc/ld.so.conf 一般应用程序的库文件不与系统库文件放在同一个目录下,一般把应用程序的共享库文件放在/usr/local/lib下,新建一个属于自己的目录apue,然后把刚才libapue.so复制过去就行了 同时在/etc/ld.so.conf中新增一行: /usr/local/lib/apue 以后在编译程序时加上编译选项: -L/usr/local/lib/apue -lapue 这样就可以使用这个libapue.so共享库了。

Linux 下标准库链接的三种方式（全静态 , 半静态 (libgcc,libstdc++), 全动态）及其各自利弊。

|标准库连接方式|示例连接选项|优点|缺点|
|----|----|----|----|
|-static -pthread -lrt -ldl|不会发生应用程序在 不同 Linux 版本下的标准库不兼容问题。|生成的文件比较大，应用程序功能受限（不能调用动态库等）| |
|-pthread -lrt -ldl|生成文件是三者中最小的|比较容易发生应用程序在 不同 Linux 版本下标准库依赖不兼容问题。| |
|-static-libgcc -L. -pthread -lrt -ldl|灵活度大，能够针对不同的标准库采取不同的链接策略，从而避免不兼容问题发生。结合了全静态与全动态两种链接方式的优点。|比较难识别哪些库容易发生不兼容问题，目前只有依靠经验积累。某些功能会因选择的标准库版本而丧失。| |
二、a库和so库生成实例
1、生成a库
首先，你需要写几个你要调用的函数的源文件，如print.cpp、sum.ccpp等。其代码如下：
//print.h
#include <stdio.h>
void printhello();

//print.cpp
#include "print.h"

void printhello()
{
 printf("Hello, world\n");
}

//sum.h
#include <stdio.h>
void sum(int a,int b);

//sum.cpp
#include "sum.h"

void sum(int a,int b)
{
 int r = 0;
 r = a + b;
 return r;
}

开始编译，将源文件编译成.o文件。命令行指令如下：
g++ -c print.cpp sum.cpp
然后开始生成a库，指令如下：
ar rcs libtest.a print.o sum.o
注意：链接库前缀必须以lib开头
得到libtest.a库文件后，然后将.a库链接到主程序中，写主程序main.cpp和头文件main.h 。
//main.h
int printhello();
int sum(int a,int b);

//main.cpp
#include "main.h"
int main(void)
{
 printhello();
 int a = sum(10,1);
 printf("%d\n",a);
 return 0;
}
加载a库，生成可执行文件并执行，指令如下：g++ main.cpp -L. -ltest -o test

2、生成so库
利用上面所写过的print.cpp和sum.cpp生成.so库的指令如下：
g++ print.cpp sum.cpp -fPIC -shared -o libtest.so
同样so库的前缀必须为lib，然后将.so库链接到主程序main.c中，这里需要注意一下，因为动态库的特性，编译器会到指定的目录去寻找动态库，目录的地址在/etc/ld.so.conf.d/ 目录里的libc.conf文件里，你可以在里面加一行地址表示你so库的位置，更改完conf文件里的内容，记得输入命令行：ldconfig。
你还可以将so库复制到默认的目录下。这里是将so库复制到了默认目录下，生成可执行文件并运行，指令如下：
g++ main.cpp -L. -ltest -o test

静态链接库和动态链接库的区别在于，主程序在运行前，静态链接库的链接固定写入在程序中，而动态链接库则是在每次程序运行再加载链接。
在加载动态链接库的时候，有可能会遇到加载不到的错误，原因在于系统默认加载的动态链接库路径里没有找到你的动态库，有三种解决方法：
1.在执行gcc main.c -L. -ltest -o main 前，执行 export LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH
2.将你so所在的目录写到/etc/ld.so.conf文件里，然后执行ldconfig。
3.将你的so放在/etc/ld.so.conf里的路径位置里。

三、Linux 静态库转换为动态库
使用ar -x 命令将.a文件转为.so文件。
ar -x mylib.a

gcc -shared *.o -o mylib.so

linux中如何将多个静态库合并成一个


![](https://img-blog.csdn.net/2018032014404516?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2J5eGRheg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

四、gcc系强制链接静态库（同时有.so和.a）

１. 坑多的办法
-static
如果需要链接成不依赖任何so文件的程序，用ldd查看显示为"not a dynamic executable"，但是这个选项时不推荐的。
2. 普遍的做法 
就是直接连接.a文件全路径，这没啥好说的，就把它当.o文件一样链接。
g++ test-boost.cpp -o test -I /home/johnchen/boost_1_56_0/include /home/johnchen/boost_1_56_0/lib/libboost_system.a /home/johnchen/boost_1_56_0/lib/libboost_filesystem.a
3. 优雅的做法
既然是库，-l和-L才是正派的做法，比如同一目录下有libxxx.a文件和libxxx.so文件，gcc默认会链接so，改变这一默认行为的方法就是：将"-lxxx"改为"-l:libxxx.a"
g++ test-boost.cpp -o test -I /home/johnchen/boost_1_56_0/include -L /home/johnchen/boost_1_56_0/lib -l:libboost_system.a -l:libboost_filesystem.a

