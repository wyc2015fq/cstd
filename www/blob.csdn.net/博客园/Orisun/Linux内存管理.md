# Linux内存管理 - Orisun - 博客园







# [Linux内存管理](https://www.cnblogs.com/zhangchaoyang/articles/1820787.html)





orisun@orisun-desktop:~/Program$size memory2

text data bss dec hexfilename

2074 284 16 2374 946memory2

可以看到一个可执行程序在存储（没有调入内存）时分为代码区，数据区，未初始化数据区三部分。

（1）代码区存放CPU执行的机器指令。通常代码区是共享的，即其它执行程序可调用它。代码段（code segment/text segment）通常是只读的，有些构架也允许自行修改。

（2）数据区存放已初始化的全局变量，静态变量（包括全局和局部的），常量。static全局变量和static函数只能在当前文件中被调用。

（3）未初始化数据区（Block Started by Symbol,BSS)存放全局未初始化的变量。BSS的数据在程序开始执行之前被初始化为0或NULL。

代码区所在的地址空间最低，往上依次是数据区和BSS区，并且数据区和BSS区在内存中是紧挨着的。

text段和data段在编译时已分配了空间，而bss段并不占用可执行文件的大小，它是由链接器来获取内存的。

bss段（未手动初始化的数据）并不给该段的数据分配空间，只是记录数据所需空间的大小。

data（已手动初始化的数据）段则为数据分配空间，数据保存在目标文件中。



数据段包含经过初始化的全局变量以及它们的值。BSS段的大小从可执行文件中得到 ，然后链接器得到这个大小的内存块，紧跟在数据段后面。当这个内存区进入程序的地址空间后全部清零。包含数据段和BSS段的整个区段此时通常称为数据区。





可执行程序在运行时又多出了两个区域：栈区和堆区。

（4）栈区。由编译器自动释放，存放函数的参数值，局部变量等。每当一个函数被调用时，该函数的返回类型和一些调用的信息被存储到栈中。然后这个被调用的函数再为它的自动变量和临时变量在栈上分配空间。每调用一个函数一个新的栈就会被使用。栈区是从高地址位向低地址位增长的，是一块连续的内在区域，最大容量是由系统预先定义好的，申请的栈空间超过这个界限时会提示溢出，用户能从栈中获取的空间较小。

（5）堆区。用于动态内存分配，位于BSS和栈中间的地址位。由程序员申请分配（malloc)和释放（free）。堆是从低地址位向高地址位增长，采用链式存储结构。频繁地malloc/free造成内存空间的不连续，产生碎片。当申请堆空间时库函数按照一定的算法搜索可用的足够大的空间。因此堆的效率比栈要低的多。



举个例子说明各种变量存放在什么区：

int a=0; //a在全局已初始化数据区

char *p1; //p1在BSS区（未初始化全局变量）

main()

{

int b; //b为局部变量，在栈区

char s[]="abd"; //s为局部数组变量，在栈区

//"abc"为字符串常量，存储在已初始化数据区

char *p1,*p2; //p1,p2为局部变量，在栈区

char *p3="123456"; //p3在栈区，123456\0在已初始化数据区

static int c=0; //c为局部（静态）数据，在已初始化数据区

//静态局部变量会自动初始化(因为BSS区自动用0或NULL初始化)

p1=(char*)malloc(10); //分配得来的10个字节的区域在堆区

p2=(char*)malloc(20); //分配得来的20个字节的区域在堆区

free(p1);

free(p2);

p1=NULL; //显示地将p1置为NULL，避免以后错误地使用p1

p2=NULL;

}



我们再写一个程序，输出各变量的内存空间：

#include<stdio.h>

#include<malloc.h>

#include<unistd.h>

#include<alloca.h>



extern void afunc(void);

extern etext,edata,end;



intbss_var;//未初始化全局变量存储在BSS区

intdata_var=42;//初始化全局就是存储在数据区

#define SHW_ADR(ID,I) printf("the%8s\t is at address:%8x\n",ID,&I);//打印地址宏



int main(int argc,char *argv[])

{

char *p,*b,*nb;

printf("Address etext:%8x\tAddress edata %8x\t Addess end %8x\t\n",&etext,&edata,&end);

SHW_ADR("main",main);//查看代码段main函数位置

SHW_ADR("afunc",afunc);//查看代码段afunc函数位置

printf("\nbss Locatoin:\n");

SHW_ADR("bss_var",bss_var);//查看BSS段变量地址

printf("\ndata Location:\n");

SHW_ADR("data_var",data_var);//查看数据段变量

printf("\nStack Loation:\n");

afunc();

p=(char*)alloca(32);//从栈中分配空间

if(p!=NULL)

{

printf("\nthe char p in stackstart from:\t",p);

printf("\nthe char p in stackend in:\t",(p+32*sizeof(char)));

}

b=(char*)malloc(32*sizeof(char));//从堆中分配空间

nb=(char*)malloc(16*sizeof(char));//从堆中分配空间

printf("\nHeapLocations:\n");

printf("the Heap start:%p\n",b);//堆起始地址

printf("the Heap End:%p\n",(nb+16*sizeof(char)));//堆结束位置

printf("\np,b and nb inStack\n");

SHW_ADR("p",p);//显示栈中数据p的位置

SHW_ADR("b",b);//显示栈中数据b的位置

SHW_ADR("nb",nb);//显示栈中数据nb的位置

free(b);//释放申请的空间，以避免内存泄露

free(nb);

}



void afunc(void)

{

static int longlevel=0;//静态数据存储在数据段中

int stack_var;//局部变量，存储在栈区

if(++level==5)

return;

printf("stack_var is at:%p\n",&stack_var);

SHW_ADR("stack_var in stacksection",stack_var);

SHW_ADR("Leval in datasection",level);

afunc();

}

运行结果显示：

Address etext: 8048818 Addressedata 804a028 Addess end 804a038

the main is at address:80484d4

the afunc is at address:80486e3



bss Locatoin:

the bss_var is at address:804a034



data Location:

the data_var is at address:804a024



Stack Loation:

stack_var is at: 0xbff4a87c

the stack_var in stack section isat address:bff4a87c

the Leval in data section is ataddress: 804a030

stack_var is at: 0xbff4a84c

the stack_var in stack section isat address:bff4a84c

the Leval in data section is ataddress: 804a030

stack_var is at: 0xbff4a81c

the stack_var in stack section isat address:bff4a81c

the Leval in data section is ataddress: 804a030

stack_var is at: 0xbff4a7ec

the stack_var in stack section isat address:bff4a7ec

the Leval in data section is ataddress: 804a030



the char p in stack start from:

the char p in stack end in:

Heap Locations:

the Heap start: 0x9b24008

the Heap End: 0x9b24040



p,b and nb in Stack

the p is ataddress:bff4a8b8

the b is ataddress:bff4a8b4

the nb is ataddress:bff4a8b0

分析：

由于afunc函数嵌套调用自己，所以很快就把栈空间用完了（sfunc只被调用了四次）。这样一来p=(char*)alloca(32)想再从栈中申请空间就失败了。



内存管理函数：

void *malloc(size_t size)

void free(void *ptr)

(typedef unsigned int size_t;

这里插入一段对void*的解释：

void*这不叫空指针,这叫无确切类型指针.这个指针指向一块内存,却没有告诉程序该用何种方式来解释这片内存.所以这种类型的指针不能直接进行取内容的操作.必须先转成别的类型的指针才可以把内容解释出来.

还有'\0',这也不是空指针所指的内容.'\0'是表示一个字符串的结尾而已,并不是NULL的意思.

真正的空指针是说,这个指针没有指向一块有意义的内存,比如说:

char* k;

这里这个k就叫空指针.我们并未让它指向任意地点.

又或者

char* k = NULL;

这里这个k也叫空指针,因为它指向NULL也就是0,注意是整数0,不是'\0'

一个空指针我们也无法对它进行取内容操作.

空指针只有在真正指向了一块有意义的内存后,我们才能对它取内容.也就是说要这样

k = "hello world!";

这时k就不是空指针了.)

malloc在内存的动态存储区中分配一个长度为size字节的连续空间，其参数是无称号整型，返回一个指向所分配的连续空间的起始地址的指针。分配空间不成功（如内存不足）时返回一个NULL指针。

free释放掉内存空间。

这两个函数的库头文件为stdlib.h.

void *realloc(void *ptr,size_tsize)

当需要扩大一块内存空间时realloc试图直接从堆的当前内存段后面获得更多的内在空间，并返回原指针;如果空间不够就使用第一个能够满足这个要求的内存块，并将当前数据复制到新位置，释放原来的数据块;如果申请空间失败，返回NULL。

calloc是malloc的简单包装，它把动态分配的内存空间进行初始化，全部清0。此函数的实现描述：

void *calloc(size_t nmemb,size_tsize)

{

void *p;

size_t total;

total=nmemb*size;

p=malloc(total);

if(p!=NULL)//申请空间

memset(p,'\0',total);//初始化\0

return p;

}

alloca()函数在栈中分配size个内存空间（函数返回时自动释放掉空间，无需程序员手动释放），并将空间初始化为0。












