# 《UNIX环境高级编程》笔记——进程（一） - yixianfeng41的专栏 - CSDN博客
2015年12月31日 11:27:05[第2梦](https://me.csdn.net/yixianfeng41)阅读数：1173
所属专栏：[Linux/Unix 网络编程](https://blog.csdn.net/column/details/14513.html)
## 本篇主要介绍进程环境。
### **1. Main函数**
C程序总是从main函数开始执行，main函数原型为 int main(int argc,char *argv[]); 其中，argc是命令行参数的数目，argv是指向参数的各个指针所构成的数组。
当内核执行一个C程序时（使用exec函数），在调用main前先调用一个特殊的启动例程，可执行文件将此启动例程指定为程序的起始地址。启动例程从内核取得命令行参数和环境变量值，然后为按上述方式调用main函数坐好安排。
### **2. 进程终止**
**五种正常的：**
1）从main函数返回；
2）最后一个线程从其启动例程返回；
3）最后一个线程调用pthread_exit；
4）调用exit或者调用_exit或_Exit，注意这三个函数的区别，_exit和_Exit立即进入内核，exit则先执行一些清理工作（比如为打开的流调用fclose函数）工作，然后进入内核。
**三种异常的：**
1）调用abort；
2）接到一个信号并终止；
3）最后一个线程对你取消做出响应。
终止处理程序atexit
```cpp
#include <stdlib.h>
  int atexit(void (*func) (void));
```
其中atexit的参数是一个函数地址，当调用此函数时无需向它传送任何参数，也不期望它返回一个值。Exit调用这些函数的顺序与它们登记时候的顺序相反。同一个函数如若被登记多次，则也会被调用多次。
终止处理程序实例：
```cpp
#include	"ourhdr.h"
static void	my_exit1(void), my_exit2(void);
int main(void) {
   if (atexit(my_exit2) != 0)
       err_sys("can't register my_exit2");
   if (atexit(my_exit1) != 0)
       err_sys("can't register my_exit1");
   if (atexit(my_exit1) != 0)
       err_sys("can't register my_exit1");
   printf("main is done\n");
   return(0);
}
static void my_exit1(void) {
   printf("first exit handler\n");
}
static void my_exit2(void) {
   printf("second exit handler\n");
}
```
执行改程序结果:
```cpp
$ a.out
main is done
first exit handler
first exit handler
second exit handler
```
### **3. C程序的存储空间布局**
1）正文段。机器指令部分。
2）初始化数据段。通常将此段称为数据段，它包含了程序中需明确地赋初值的变量。
3）非初始化数据段。
4）栈。自动变量以及每次函数调用时多需保存的信息都存放在此段中。
5）堆。通常在堆中进行动态存储分配。
### **4. 静态库、共享库、动态加载库**
**静态库**就是一些目标文件的集合，以.a结尾。静态库在程序链接的时候使用，链接器会将程序中使用到函数的代码从库文件中拷贝到应用程序中。一旦链接完成，在执行程序的时候就不需要静态库了。 由于每个使用静态库的应用程序都需要拷贝所用函数的代码，所以静态链接的文件会比较大。
**共享库**以.so结尾. (so ==share object) 在程序的链接时候并不像静态库那样在拷贝使用函数的代码，而只是作些标记。然后在程序开始启动运行的时候，动态地加载所需模块。所以，应用程序在运行的时候仍然需要共享库的支持。
 共享库链接出来的文件比静态库要小得多。
**动态加载库**(dynamicallyloaded (DL) libraries)是指在程序运行过程中可以加载的函数库。而不是像共享库一样在程序启动的时候加载。DL对于实现插件和模块非常有用，因为他们可以让程序在允许时等待插件的加载。在Linux中，动态库的文件格式跟共享库没有区别，主要区别在于共享库是运行时加载。有专门的一组API用于完成打开动态库，查找符号，处理出错，关闭动态库等功能。
### **5. 存储器分配**
```cpp
#include <stdlib.h>
void *malloc(size_t size);
void *calloc(size_t nobj, size_t size);
void *realloc(void *ptr, size_t newsize);
Return: nonnull pointer if OK,  NULL on error
void free(void *ptr);
```
1）malloc.分配指定字节数的存储区。此存储区中值不确定。
2）Calloc。为指定长度的对象分配存储空间。该空间中的每一位都初始化为0.
3）Realloc。更改以前分配去的长度。当增加长度时，可能需将以前分配去的内容移到另一个足够大的区域，以便在尾端提供增加的存储区，而新增区域内的初始值则不确定。
### **6. 环境变量**
每个进程都有相关联的环境列表（Environment List），环境列表由多个环境字符串组成。环境字符串被定义成name=value格式。name叫做环境变量（Environment Variable）。
当一个进程被创建时，该进程继承父进程的环境列表。这个特性可以作为父进程和子进程通信的一种方法。比如，当父进程要创建子进程时，可以先设置某个环境变量，子进程随后在自己的环境列表中读取该环境变量。不过这种通信是单向的（one-way）和一次性的（once-only），子进程在创建后，就完全拥有和父进程独立环境列表了。程序可以通过检查环境变量来改变程序的特性(像使用命令行参数一样)。
程序中如何使用环境列表：
**获得环境列表**
1）通过全局变量char **environ，environ和argv很相似；也可以通过int main(int argc, char *argv[], char *envp[])中的envp来访问。
[](http://blog.chinaunix.net/attachment/201112/13/10231348_1323776561LfMB.jpg)
2）使用程序提供的API。
```cpp
char *getenv(const char *name)
```
注意点：
a.该函数返回指定环境变量的字符值。
b.不要直接修改返回值，因为在类UNIX的大部分实现中，返回指针实际上是指向环境字符串中的value部分的。如果要改变该值，使用setenv()或putenv()。
c.当程序得到一个返回指针后，要用一个私有buff来保存该值。因为在某些实现中，该函数内部使用一个静态buff来保存需要返回的值，这使得在下一次函数被调用后，静态buff中的值被其他值覆盖。
**设置环境变量**
```cpp
int putenv(char *string)
```
注意点：
a成功返回0，失败返回非0（不是-1）。
b.string的格式为”name=value”。
c.不要把string声明成自动变量，string将成为环境列表的一部分，也就是说，该程序不会复制string到环境列表中，而是把string当做列表的一部分，当程序中改变string，会影响该进程的环境变量。
d.使用该函数通常是父进程想让它所创建的子进程继承某个改变的环境变量。或者是想让自己根据改变的环境变量来改变程序的特性（先改变某些环境变量，然后执行exec()函数群来调用自己，程序重启，然后根据这些改变的环境变量来改变程序运行的特性）。
```cpp
int setenv(const char *name, const char *value, int overwrite)
int unsetenv(const char *name)
```
注意点：
a、setenv()会创建一个新的buff来存储参数“name=value“（跟putenv对比），我们没必要再name后面或value前面添加”=”号，因为在该函数中会自动添加。
b.只有在overwrite为非0，环境变量才会改变。
**清除环境变量**
```cpp
int clearenv(void)
```
注意点：
a.该函数清除整个环境列表。也可以通过environ=NULL来清除。
b.当该函数跟setenv()一起使用时，会产生内存泄露。因为clearenv()不会清除setenv()分配的内存。这两个函数不会频繁的调用，所有通常不是什么大问题。
### **7. Setjmp和longjmp**
|int setjmp(jmp_buf env)|建立本地的jmp_buf缓冲区并且初始化，用于将来跳转回此处。这个子程序保存程序的调用环境于env参数所指的缓冲区，env将被longjmp使用。如果是从setjmp直接调用返回，setjmp返回值为0。如果是从longjmp恢复的程序调用环境返回，setjmp返回非零值。|
|----|----|
|void longjmp(jmp_buf env, int value)|恢复env所指的缓冲区中的程序调用环境上下文，env所指缓冲区的内容是由setjmp子程序调用所保存。value的值从longjmp传递给setjmp。longjmp完成后，程序从对应的setjmp调用处继续执行，如同setjmp调用刚刚完成。如果value传递给longjmp零值，setjmp的返回值为1；否则，setjmp的返回值为value。|
setjmp与longjmp结合使用时，它们必 须有严格的先后执行顺序，也即先调用setjmp函数，之后再调用longjmp函数，以恢复到先前被保存的“程序执行点”。否 则，如果在setjmp调用之前，执行longjmp函数，将导致程序的执行流变的不可预测，很容易导致程序崩溃而退出。
C 语言的运行控制模型，是一个基于栈结构的指令执行序列。表示出来就是 call / return ：调用一个函数，然后用 return 指令从一个函数返回。setjmp/longjmp 实际上完成的也是一种调用返回的模型。setjmp 相当于 call ，longjmp 则是 return 。
如何区别从setjmp函数的两种不同返回方式呢？
当setjmp函数第1次被调用时，它返回0。当setjmp作为longjmp的执行结果再次返回时，它的返回值是longjmp的第2个参数，它必须是个非0值。通过检查它的返回值，程序可以判断是否调用了longjmp。如果存在多个longjmp，也可以由此判断哪个longjmp被调用。
何时使用非本地跳转？
如果存在一长串的函数调用链，即使只有最深层的那个函数发现了错误，调用链中的所有函数都必须返回并检查错误代码。在这种情况下使用setjmp和longjmp去除了中间函数的错误代码逻辑，从而对他们进行了简化。
### **8. 自动、寄存器和易失变量**
众所周知，C中的变量主要有全局变量、局部变量和静态变量。
**全局变量**：在函数外面定义的变量为全局变量，工程内的所有文件都可以访问他；而其他的文件要想使用这个变量，必须用extern来声明这个变量，这个声明叫做引用声明。
**局部变量**：局部变量，如不作专门说明为自动变量，都是动态分配存储空间，存储在动态存储区中，这种变量叫自动变量。这种变量只在定义它们的时候才创建，在定义它们的函数返回时系统回收变量所占存储空间。对这些变量存储空间的分配和回收是由系统自动完成的，所以也叫自动变量。一般情况下，不作专门说明的局部变量，均是自动变量。自动变量也可用关键字auto作出说明。
**静态变量**：静态变量与自动变量的本质区别是，静态变量并不像自动变量那样使用堆栈机制来使用内存。而是为静态变量分配固定的内存，在程序运行的整个过程中，它都会被保持，而不会不销毁。这就是说静态变量的持续性是程序运行的整个周期。这有利于我们共享一些数据。
变量有作用域、链接属性和存储类型3个属性，以下是一些总结：
变量类型             声明位置                  存储位置           作用域                      声明为static后的链接属性
全局变量           所有代码块之外            内存             从声明到文件末      不允许从其他文件访问
局部变量           代码块起始处                 堆栈             整个代码块     不在堆栈，它的值在整个执行期保持
了解以上三种变量后，再看自动变量、寄存器变量和易失变量；
**自动变量**：自动地存储于内存中，地址随机，即局部变量
**寄存器变量**：在c语言当中可以使用寄存器变量来优化程序的性能,最常见的是在一个函数体当中,将一个常用的变量声明为寄存器变量: register int ra; 如果可能的话,编译器就会为它分配一个单独的寄存器，在整个函数执行期间对这个变量的操作全都是对这个寄存器进行操作,这时候就不用频繁地去访存了,自然就提高了性能.但是寄存器变量不是强制性的,也就是,即使你使用register关键字去声明一个变量为寄存器变量，编译器还是有可能把它作为一个普通的变量而不是寄存器变量来使用的.
**易失变量**：volatile(嵌入式程序员必须掌握的)，volatile最初的意思是表示汽油容易挥发,在c中的作用大概有两点（1）表示变量是易失的,易变的. （2）强制访存操作,防止编译器去优化,告诉编译器每次必须去内存中取值,而不是从寄存器或者缓存.
### **9. 资源限制**
每一个进程都有一组资源限制，其中一些可以用getrlimit和setrlimit函数查询和更改。
```cpp
int getrlimit(	int resource, struct rlimit *rlptr	);
int setrlimit(	int resource, const struct rlimit *rlptr );
Return: 0 if OK, nonzero on error
```
Soft limit: can be changed by any process to  <=  hard limit
Hard limit: can be changed by any process to  >=³ soft limit (irreversible!)

