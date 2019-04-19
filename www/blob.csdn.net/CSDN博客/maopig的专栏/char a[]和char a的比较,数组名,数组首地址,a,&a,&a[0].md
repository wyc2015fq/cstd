# char a[]和char *a的比较,数组名,数组首地址,a,&a,&a[0] - maopig的专栏 - CSDN博客
2011年11月14日 23:15:00[maopig](https://me.csdn.net/maopig)阅读数：4879

**char a[]和char *a的比较**
指针和数组存在着一些本质的区别。当然，在某种情况下，比如数组作为函数的参数进行传递时，由于该数组自动退化为同类型的指针，所以在函数内部，作为函数参数传递进来的指针与数组确实具有一定的一致性，但这只是一种比较特殊的情况而已，在本质上，两者是有区别的。请看以下的例子：
char a[] = "1234567";
char *p = "1234567"; 
上述两个变量的内存布局分别如下：
数组a需要在内存中占用8个字节的空间，这段内存区通过名字a来标志。指针p则需要4个字节的空间来存放地址，这4个字节用名字p来标志。其中存放的地址几乎可以指向任何地方，也可以哪里都不指，即空指针。目前这个p指向某地连续的8个字节，即字符串"1234567"。
另外，例 如：对于a[2]和p[2]，二者都返回字符‘i’，但是编译器产生的执行代码却不一样。对于a[2]，执行代码是从a的位置开始，向后移动2两个字节，然后取出其中的字符。对于p[2]，执行代码是从p的位置取出一个地址，在其上加2，然后取出对应内存中的字符。
p指针变量本身在栈上，指向的内容在静态存储区；
a只是个数组名，本身不占运行时程序的空间，只是在源程序中用来标记一个字符数组（即其首地址），而数组也是存储在栈上的。
char s1[] = "a"; 
char *s2 = "b"; 
a是在运行时刻赋值的； 
而b是在编译时就确定的； 
但是，在以后的存取中，在栈上的数组比指针所指向的字符串(例如堆)快。 
比如： 
int main() 
{ 
char a = 1; 
char c[] = "1234567890"; 
char *p ="1234567890"; 
a = c[1]; 
a = p[1]; 
return 0; 
} 
对应的汇编代码 
10: a = c[1]; 
00401067 8A 4D F1 mov cl,byte ptr [ebp-0Fh] 
0040106A 88 4D FC mov byte ptr [ebp-4],cl 
11: a = p[1]; 
0040106D 8B 55 EC mov edx,dword ptr [ebp-14h] 
00401070 8A 42 01 mov al,byte ptr [edx+1] 
00401073 88 45 FC mov byte ptr [ebp-4],al 
第一种在读取时直接就把字符串中的元素读到寄存器cl中，而第二种则要先把指针值读到edx中，在根据edx读取字符，显然慢了。
#include<stdio.h>
#include<stdlib.h>
char *function1()
{
     char *a="cdefgh";//在靜態存儲區分配，一直到程序結束 
     return a;
}
char *function2()
{
     char a[]="cdefgh";//在棧中分配，函數結束時釋放 
     return a;
}
char a[]="cdefgh";
/*這個是常量字串的拷貝,
相當於strcpy(a,"cdefgh"),
這樣寫都會有字串拷貝,
造成時間和空間上的開銷,
如果字串很長儘量不要這樣寫,
由於字元陣列a在棧上,
所以在函數結束後它便無效了..
---------------
char *a="cdefgh";
a直接指向常量字串,
這個字串保存在靜態存儲區中...
所以在函數結束後,它返回的位址仍然有效..
*/
int  main()
{
    char test[]="123";
    test[0]='a';//可以修改數組內部元素的值 
    char* test1="456";
    *test1='7';//test1不可以修改，是const char*的類型的值 
    /*char *a="cdefgh";
    此時a為const char*，
    也就是說你不能改變*a的值。
    char a[]="cdefgh";你可以通過a[i]改變它的值。*/
    char *i=NULL ;
    char *j=NULL;
    i= function1();//結吉確定 
    j= function2();//結果不確定 
    printf("/n%s/n",i);
    printf("/n%s/n",j);
    system("pause");
}
/*(1)function1()的a和function2()的a都是自動變數，都在棧上分配空間
(2)function1()的a分配的空間=sizeof(char *)=sizeof(void *),
   任何指標的大小都是相同的,指向靜態資料區存的"cdefgh"
(3)function2()的a分配的空間=strlen("cdefgh")+1,並且用來保存"cdefgh"
(4)返回的指標，function1指向靜態資料區，function1指向棧（已自動釋放）
   故function1的值是對的
*/
=============================================================
(1)指针数组: 是数组,但数组中的每个元素都是指针
int *p[5];//如p[2]是指针,可*p[ 2]=3;
(2)指向数组的指针: 是个指针,但它指向的是一个数组
int a[5];
int (*p)[5];//与前一行比较,*p相当于a,即p=&a;就像:int m;int *pm;//*pm就相当于m.pm=&m;
p= &a;//可与前一行合并为int (*p)[5]=&a; 
----------------------
a代表这个数组，它是一个指针，指向第一个元素 
这里一定要记住，a是数组名，&a代表的不是取a这个变量的地址，而是取数组元素的地址
---------------------------------------
a     的类型是 int[5]       数组
&a    的类型是 int(*)[5]    指针——指向int[5]数组的指针
&a[0] 的类型是 int *         指针——指向int类型的指针。
sizeof(a)=20;
sizeof(*a)=4 =sizeof(a[0]);
sizeof(*&a)=20;//因为p=&a,所以=sizeof(*p),而*p=a,所以=sizeof(a)=20;
---------------------------------------
int a[5]={1,2,3,4,5};
int *ptr=(int *)(&a+1);
printf("%d,%d",*(a+1),*(ptr-1));//输出：2,5
指针加1要根据指针类型加上一定的值，不同类型的指针+1之后增加的大小不同,指针只是一个内存地址,但指针指向地址的长度可能不同,如char *pc与int *pi,sizeof(pc)=sizeof(pi)=4,但为什么输出时,cout<<*pc只从内存处取一个字符,而cout<<*pi则从内存处取4个字符,这是由指针类型(char,int)决定的.对A* p;p+1=(A*)(p的地址值+sizeof(A)),如pc+1=pc+sizeof(char)=(char*)(pc的地址值+1个字节),而pi+1=pc+sizeof(int)=(int*)(pi的地址值+4个字节).
对代码中的&a+1,&a是数组指针，其类型为int (*)[5],因为p=&a,也即是p的类型.所以&a+1=&a+sizeof(A),其中A为int[5]:(把A=int[5]代入A* p,即相当于int(*p)[5]).所以&a+1=&a的地址值+5*4字节,即变为数组a的结束地址的下一个地址(即&a[5]),&a+1仍是int (*)[5]类型,经(int *)(&a+1)强制转化为int*,赋值给ptr.后面的ptr-1=ptr-sizeof(int)=ptr的地址值-4个字节,即变为数组a的最后一个元素的地址&a[4],*(ptr-1)即为a[4],故输出为5.
而a+1=a+sizeof(int):(此处a退化为int*,故对于A* p,A为int)=a的地址值+4个字节,即是&a[1],则*(a+1)即a[1],故输出2.
又如:
double t[]={1, 2, 578, 111,90} ;
double *pt= &t[3];//指向值为111
int *ptInt= reinterpret_cast< int * > (pt);
char *ptCh= reinterpret_cast< char * > (pt);
cout<< *( pt- 1)<< "\t"<< *(reinterpret_cast<double*>(ptInt-2))<<"\t"<<
*(reinterpret_cast<double*>(ptCh-8));//最后输出结果全为578 
---------------------
void Fun( int *p)与void Fun( int p[])是一样的,即函数列表中的数组此时退化成了指针
