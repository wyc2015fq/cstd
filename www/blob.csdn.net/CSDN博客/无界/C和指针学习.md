
# C和指针学习 - 无界 - CSDN博客

置顶2008年06月24日 18:15:00[21aspnet](https://me.csdn.net/21aspnet)阅读数：34338



**C和指针学习**
最后更新时间：2012.12.3
原则：尽量短小精悍，实用，需要扩充阅读都是以链接形式出现
注意：本文主要是针对Unix环境下的C
**目 录**
**一.编译与链接**
**二.特殊字符**
**三.数据类型**
**四.修饰符**
**五.运算符**
**六.控制语句**
**七.函数**
**八.指针**
**九.数组**
**十.字符串**
**十一.结构**
**十二.union联合**
**十三.typedef声明**
**十四.预处理器**
**十五.输入输出**
**十六.文件**
**十七.内存**
**十八.异常**
**十九.链表**
**二十.树**
**正 文**
**一.编译与链接**
**1.编译**
\#cc program.c
生成a.out文件
这个名字是编译器默认的输出名。如果要修改可执行文件的名字可以加-o参数：gcc -o myexec main.c
这样就把main.c编译连接生成的可执行文件命名为myexec
**gcc编译器的编译自定义格式**
\#cc -o hello hello.c
\#gcc -o hello hello.c
使用gcc 编译器就会为我们生成一个hello的可执行文件
扩充阅读：Linux编译器GCC的使用
[http://blog.csdn.net/21aspnet/article/details/1534108](http://blog.csdn.net/21aspnet/article/details/1534108)
[http://blog.csdn.net/21aspnet/article/details/167420](http://blog.csdn.net/21aspnet/article/details/167420)
补充说明：如果你实在基础很差，那么需要在windows下借助VS2010这样的可视化工具来调试了，这样可以很清晰的看出内存中的指针
[http://blog.csdn.net/21aspnet/article/details/6723758](http://blog.csdn.net/21aspnet/article/details/6723758)
**2.执行**
\#./a.out
编译多个源文件
\#cc a.c   b.c   c.c
**3.产生目标文件**
\#cc -c program.c
产生program.o的目标文件以后供链接用
产生多个目标文件
\#cc -c program.c  a.c  b.c
编译指定名称文件
\#cc -o sea a.c
**4.链接几个目标文件**
\#cc a.o  b.o  c.o
**二.特殊字符**
**1.转义字符**
\\单斜线\
\\\\双斜线\\
注意不是\\\代表\\，因为要2个\\
\'单引号
\"双引号
\n换行
\r回车
\t制表
\f换页
\a警告
扩充阅读：C语言 格式控制符 和 转义字符
[http://blog.csdn.net/21aspnet/article/details/1535459](http://blog.csdn.net/21aspnet/article/details/1535459)

**2.注释**
方法一 /*   */
方法二 //
**3.保留字**
[http://blog.csdn.net/21aspnet/article/details/1539252](http://blog.csdn.net/21aspnet/article/details/1539252)

**4. size_t**
是为了方便系统之间的移植而定义的
在32位系统上   定义为   unsigned   int
在64位系统上   定义为   unsigned   long
更准确地说法是   在   32位系统上是32位无符号整形
在   64位系统上是64位无符号整形
size_t一般用来表示一种计数，比如有多少东西被拷贝等
**三.数据类型**
**1.整型**
singed 有符号
unsigned是无符号的意思，也就是说如果你的编译系统给int分配的存储单元的长度是2个字节的话，有符号的int 取值范围是-32768（即2^15）——32767（即2^15-1），而无符号的unsigned  int就是0-65535（2^16-1）

|类型
|最小范围
|
|char
|0-127
|
|signed char
|-127-127
|
|unsinged char
|0-255
|
|int
|-32767-32767
|
|long int
|-2147483647-2147483647
|
|unsinged int
|0-65536
|




int a=8;
int a=012;//8进制是0开头
int a=0x0a;//16进制是0x开头
3种输出printf("%d",a);都是10

**2.浮点**float double long double

**3.布尔值**
C语言中非0值为真，0值为假。
C语言没有布尔类型，任何一个整型的变量都可以充当布尔变量，用0表示False，其它数（默认为1）表示True。
a = 1;
if(a) {printf("a is T\n");}else{printf("a is F\n");}//输出T
a = 2;
if(a) {printf("a is T\n");}else{printf("a is F\n");}//输出T
a = -1;
if(a) {printf("a is T\n");}else{printf("a is F\n");}//输出T
a = 0;
if(a) {printf("a is T\n");}else{printf("a is F\n");}//输出F
if(!a) {printf("a is T\n");}else{printf("a is F\n");}//输出T
如果你想像Pascal一样使用true和false，那么你可以包含头文件stdbool.h。这样你可以定义变量为bool类型并赋值为true或false。例如：
\#include "stdbool.h"
`bool a = true;`
`if (a) printf("a is true");`
也可以使用
\#define False 0
\#define True 1

**4.类型转换**
float a;
int x=6,y=4;
a=x/y;
printf("%f",a)//输出1.000000
因为6/4为1；1转浮点为1.000000

如果不希望截断需要精确结果要类型转换
a=(float)x/y;

**5.****enum枚举**
enum A{a,b,c,d};
就是为了定义一组同属性的值，默认的最前面的是0，后面的元素依次+1；
但是注意，每个枚举都唯一定义一个类型，里面的元素的值不是唯一的，枚举成员的初始化只能通过同一枚举的成员进行！！
之所以不用整数而用枚举是为了提高程序的可读性！

```python
enum color
{
red,
green=2,
blue=4
}colorVal;

printf("%d\n",red);
```
![](http://hi.csdn.net/attachment/201108/26/0_13143891864e5E.gif)
**6.位字段**
结构体中字段冒号后数字表示该字段分配多少位

```python
#include <stdio.h> 
#define MALE 0 ;
#define FEMALE 1 ;
#define SINGLE 0 ;
#define MARRIED 1 ;
#define DIVORCED 2 ;
#define WIDOWED 3 ;
main( )
{
	struct employee
	{ 
		unsigned  gender : 1 ;
		unsigned  mar_status : 2 ;
		unsigned  hobby : 3 ;
		unsigned  scheme : 4 ;
	} ;
	struct employee  e ;
	e.gender = MALE ; 
	e.mar_status = DIVORCED ;
	e.hobby = 5 ;
	e.scheme = 9 ;
	printf ( "\nGender = %d", e.gender ) ;
	printf ( "\nMarital status = %d", e.mar_status ) ;
	printf ( "\nBytes occupied by e = %d", sizeof ( e ) ) ;
}
```
**7.左值和右值**
左值：对象被修改；
右值：使用对象的值。


**四.修饰符**

**1.变量**
取变量的值可以直接=变量
给变量赋值一定要&
**2.类型限定符const**
const是一个C语言的关键字，它限定一个变量不允许被改变。
使用const在一定程度上可以提高程序的健壮性，另外，在观看别人代码的时候，清晰理解const所起的作用，对理解对方的程序也有一些帮助。

const int a=15;//必须声明时赋值
int const a=15;//必须声明时赋值，const在前在后都可以
声明好以后再赋值会报错
const int a;
a=15
------------------------
其次：在函数中声明为const的形参是因为函数没有改变所指向的值的内容
int ax(const int *p)
{
int a=*p+1;
return a;
}

试图改变*p指向的值会报错
int display(const int * p)
{
return ++*p;
}
![](https://img-my.csdn.net/uploads/201211/23/1353682387_3112.jpg)
**const int和int const是一样的效果**
int display(int const * p)
{
return ++*p;
}

可以执行的，因为此刻是*p可以改变，不能改变的是p
int display(int  * const p)
{
return ++*p;
}
如果上述3段代码中return ++*p;改为return *p++;那么完全相反的结果
总结：只要const在*前那么保护的就是指针指向的结果，如果const在*后那么保护的是指针
扩充阅读：[http://blog.csdn.net/21aspnet/article/details/160197](http://blog.csdn.net/21aspnet/article/details/160197)

**3.存储类型**
|存储类型
|存储位置
|默认初始值
|作用域
|生存周期
|
|自动|内存|不可预料|限定在变量定义的局部块|从变量定义处到控制还处在变量定义的块内|
|寄存器|cpu寄存器|无意义|变量定义的局部块|从变量定义处到控制还处在变量定义的块内|
|静态|内存|0|变量定义的局部块|不同函数调用间，变量值不变|
|外部|内存|0|全局|程序结束前|



**4.static 变量**
是C程序编译器以固定地址存放的变量，只要程序不结束，内存不被释放．
static int a=5;
函数内的static变量只有函数内可以访问；
函数外的static变量只有该文件内的函数可以访问。
1).局部变量在函数内重新进入时保持原有的值不会初始化
\#include<stdio.h>
int fun1()
{
static int v=0;
v++;
return v;
}
int main(int argc, char *argv[])
{
int a1,a2,a3,a4;
a1=fun1();
a2=fun1();
a3=fun1();
a4=fun1();
return 0;
}
输出：1234
2).与extern一起使用，创建一种私有机制，对函数和变量限制的作用。静态外部变量作用域从声明位置开始直到当前文件结束。对于当前文件更前位置或者其他文件函数不可见。
\#include<stdio.h>
static int v=0;
int fun1()
{
v++;
return v;
}
int fun2()
{
v++;
return v;
}
int main(int argc, char *argv[])
{
int a1,a2,a3,a4;
a1=fun1();
a2=fun2();
a3=fun1();
a4=fun2();
return 0;
}
输出：1234

**5.static函数**
函数如果没有声明那么就是extern，外部链接
如果是static的函数，说明是内部链接，即只有此函数的文件内部可以调用。
好处：易维护；减少名字污染
扩充阅读：[C语言的一个关键字——static](http://blog.csdn.net/21aspnet/article/details/1535573)

**6.extern变量**
extern int i;
extern存储类型使几个源文件共享同一个变量
外部变量　定义在程序外部，所有的函数和程序段都可以使用．
extern可以置于变量或者函数前，以标示变量或者函数的定义在别的文件中，提示编译器遇到此变量和函数时在其他模块中寻找其定义。
另外，extern也可用来进行链接指定。
头文件f1.h
int a=5;//定义
主文件f2.c
\#include "f.h" //引用，注意不能<"f.h">
extern int a;//声明
printf("%d",a);
**7.auto 变量**
是用堆栈(stack)方式占用储存器空间，因此，当执行此区段是，系统会立即为这个变量分配存储器空间，而程序执行完后，这个堆栈立即被系统收回．在大括号{}内声明．

**8.register 变量**
寄存器变量,是由寄存器分配空间,访问速度比访问内存快,加快执行速度.寄存器大小有限.
注意：register int a=1;只能函数内 而不能函数外，由于寄存器的数量有限（不同的cpu寄存器数目不一），**不能定义任意多个寄存器变量**
extern 和static可以函数外
register int a=1;
但是如果int *b=&a;这样会报错，因为不可以取到寄存器变量的地址。
但是使用VS2010调试时可以用&a.
扩充阅读：[寄存器register介绍](http://blog.csdn.net/21aspnet/article/details/257511)
扩充阅读：[变量属性](http://blog.csdn.net/21aspnet/article/details/2560072)
**9.volatile变量**
编译器不要对变量优化，每次使用变量，直接从内存调入寄存器操作，然后结果返回内存。
否则，编译器可能会对变量代码优化。
volatile int i;


**10.链接属性**
external外部
internal内部
none无

**五.运算符**
**1.操作符**
+-*/%

**2.逻辑运算符**
&&与
||或
!非
**3.条件运算符**
a>5?b-6:c/2
a大于5就执行b-6否则执行c/2
**4.++**
**i++和++i**
作用一样，就是将i的值加1.
但是如果除了++之外还有别的运算符的话就要考虑先加后加的问题了。
例如：
while(i++<10);
先将i的值与10比较，再将i的值加1.
while(i++<10);
先将i的值与10比较，再将i的值加1.
**5.位移和位操作符**
<<**左移**用法：
格式是：a<<m，a和m必须是整型表达式，要求m>=0。
功能：将整型数a按二进制位向左移动m位，高位移出后，低位补0。
>>**右移**用法：
格式是：a>>m，a和m必须是整型表达式，要求m>=0。
功能：将整型数a按二进制位向右移动m位，低位移出后，高位补0
&位与
|位或
^位非

**六.控制语句**
**1.if...else...**
如果if后没有大括号--{，那么if判定为真只会调用之后的第一句代码，其余的走else逻辑。
需要注意的是真假的判断，参考23条。
int a=0;
int a=NULL;
if(a)
{
//a为非0和NULL才进来
}
int d1=NULL;
char* s1;
struct node* head=createNode("0","0");
if(d1)
{
d1=1;
}int d1=NULL;
char* s1;
struct node* head=createNode("0","0");
if(d1)
{
d1=1;
}int d1=NULL;
char* s1;
struct node* head=createNode("0","0");
if(d1)
{
d1=1;
}

**2.while循环**
i=0;a=0;
while(i<10)
{
a=a+i;
i++;
}
break：退出大循环
continue：退出本次循环

**3.for循环**
常用写法
int i;
for(i=0;i<2;i++)
{
}

特殊写法
for(;;)

**4.do....while循环**
无论如何要先做一次do然后去while

**5.switch语句**
switch(ch){
case "A":
i+1;
break;
case "B":
i+2;
break;
default:
i=0;
}

6**.goto跳转**
goto AA;
AA:if...else
建议不要用goto

**七.函数**
**1.定义**
int a=1;
int b=2;
ext1(a,b);
ext1(int x.int y)
{
int temp=x;
x=y;
y=temp;
prinft("x=%d,y=%d",x,y)
}

**2.内联函数inline**
首先要声明
inline int 函数名(参数)
后面要有一个真实的函数体
inline int 函数名(参数)
{
}
只有三二行的代码，建议使用内联！这样运行速度会很快，因为加入了inline 可以减少了，跳入函数和跳出函数的步骤！inline 定义的类的内联函数，函数的代码被放入符号表中，在使用时直接进行替换，（像宏一样展开），没有了调用的开销，效率也很高。
另外要注意，内联函数一般只会用在函数内容非常简单的时候，这是因为，内联函数的代码会在任何调用它的地方展开，如果函数太复杂，代码膨胀带来的恶果很可能会大于效率的提高带来的益处。

**3.宏定义**
\#define SQUARE(x)   x*x
程序中写SQUARE(3)   实际等于3*3
宏使程序速度更快，但是宏增加程序的大小。
但是，将参数传递给函数，再从函数获得返回值，时间开销大。而宏已经在编译前存储到源码中。

**4.main函数的参数**
argc：命令行参数个数；
argc：命令行参数数组；
env：环境变量数组；
\#include <stdio.h>
int main(int argc,char *argv[],char *env[])
{
int i;
for(i=0; i<argc; i++)
printf("argv[%d]=%s\n",i,argv[i]);
for(i=0; env[i]!=NULL; i++)
printf("env[%d]:%s\n",i,env[i]);
return 5;
}
![](https://img-my.csdn.net/uploads/201204/22/1335084818_9827.gif)
**八.指针**
指针只能指向地址！
指针的指针是为了取得“地址”，因为指针只是指向“主体”
**指针三视图**
图1：指针和变量关系
![](https://img-my.csdn.net/uploads/201211/30/1354272853_3803.jpg)

图2：指向同一个地址的指针
![](https://img-my.csdn.net/uploads/201212/03/1354507480_6200.jpg)

图3：指向动态分配结构体的指针
typedef struct node
{
int value;
//注意不要在此用Node，不然会有“警告：从不兼容的指针类型赋值”
struct node *link;
}Node;
![](https://img-my.csdn.net/uploads/201212/03/1354517400_8790.jpg)

**=====================指针总结======================**
首先：声明和使用要分开，不能混用
声明一个指针出三值：指针变量的地址，指针指向变量的地址，指针指向变量的值(间接)
普通变量只有两值：变量地址，变量值

声明：
int * 指针变量=&变量；

使用：
1.  (* 指针声明) 即：&地址对于的内存值，也就是变量值；
2.  指针声明      即：&地址，也就是变量地址
3.  &指针声明   即：指针变量的地址
**=====================指针总结======================**

int  a;
int *b=&a;
int **c=&b;
a=1;
*b=2;
指针要对应级别，一级b对应一级&a，二级c对应二级&b。
如果传值原代码块无星
main()
{
swap(a,b,c)
}
swap(int a,int *b,int **c)
{
a=*b+**c;//这样不能改变值
*b=a+**c;//可以改变值
}
函数里要改变原值只有指针
printf("%d",*b);//输出2
printf("%d",a);//输出2
//指针和字符串
char * ch="abc";
char * cp=ch;
printf("%s",cp);//输出abc
//指针和字符
char  ch=‘a’;
char * cp=&ch;
printf("%c",*cp);//输出a
//指针和数组
int a1[]={1,2,3,4,5};
printf("%d",&a1[2]);
int *b=&a1[2];

**空指针**
void*这不叫空指针,这叫无确切类型指针.这个指针指向一块内存,却没有告诉程序该用何种方式来解释这片内存.所以这种类型的指针不能直接进行取内容的操作.必须先转成别的类型的指针才可以把内容解释出来.
还有'\0',这也不是空指针所指的内容. '\0'是表示一个字符串的结尾而已,并不是NULL的意思.
真正的空指针是说,这个指针没有指向一块有意义的内存,比如说:
char* k;
这里这个k就叫空指针.我们并未让它指向任意地点.
又或者
char* k = NULL;
这里这个k也叫空指针,因为它指向NULL 也就是0,注意是整数0,不是'\0'
一个空指针我们也无法对它进行取内容操作.
空指针只有在真正指向了一块有意义的内存后,我们才能对它取内容.也就是说要这样
k = "hello world!";
这时k就不是空指针了.

```python
int i=2;
    void *p;
    p =&i;
	printf("Values of I is=%d\n",p);//输出地址
	printf("Values of I is=%d\n",*p);//报错
    printf("Values of I is=%d\n",(*(int *)p));//输出i值
```
void *使用场景：当进行纯粹的内存操作时，或者传递一个指向未定类型的指针时，可以使用void指针,void指针也常常用作函数指针。
**用空指针终止对递归数据结构的间接引用**
**用空指针作函数调用失败时的返回值。**
**用空指针作警戒值**

**NULL指针**
char *q;
q=NULL;
printf("Location 0 contains %d\n",q);
输出0说明机器允许读取内存地址0；否则说不允许

**指针++**
int i=1;
int *p;
p =&i;
printf("%d\n",p);
(*p)++;
printf("%d\n",p);
printf("%d\n",*p);
*p++;
printf("%d\n",p);
printf("%d\n",*p);
输出：
-1078959176
-1078959176
2
-1078959172
-1078959172
可以看出(*p)++改变指针指向的值， *p++改变指针自己，因为int在32位下是4位。

**函数指针**

```python
#include <stdio.h>
void display();
void main()
{
	
	void( *func_ptr )();
	func_ptr = display;  /* assign address of function */ 
	printf("\nAddress of function display is %u", func_ptr );
	( *func_ptr)( );  /* invokes the function display( ) */
}
void display()
{
	puts ("\nLong live viruses!!");
}
```
输出：
Address of function display is 134513698
Long live viruses!!

**函数指针使用场景**
1.Windows中回调机制
2.C++运行期间动态绑定函数

**指针的指针****
主要用在链式数据结构中，特别是当函数的实际参数是指针变量时。有时候希望函数通过指针指向别处的方式改变此变量。而这就需要指向指针的指针。
为了从链表中删除一个元素，向函数传递一个待改变的指向指针的指针。

扩展阅读：
把指针说透
[http://blog.csdn.net/21aspnet/article/details/317866](http://blog.csdn.net/21aspnet/article/details/317866)
C指针本质
[http://blog.csdn.net/21aspnet/article/details/1539652](http://blog.csdn.net/21aspnet/article/details/1539652)

**九.数组**
**1.数组定义**
int a[]={1,2,3,4,5}
a[0]//用序号输出数组元素，默认下标从0开始
求数组长度：printf("%d\n",sizeof(a)/sizeof(a[0]));
int *ap=a+2;
ap[0];//输出3
int a[]={1,2,3,4,5,6};//自动计算数组长度
循环数组可以用2种方法
int i;
for(int i;i<6;i++)
{
a[i];这样输出
或者*(a+i);因为*(a)指向数组第一个元素的指针
}

**2.指针访问数组**
int *p=a;注意不是&a;
可以三种方法输出
p[i];方法一就是数组元素
*(p+i);方法二
for()
{
printf("%d",*p);//方法三
p++;
}
如果  int  const p=a;
用上述代码linux下一样可以编译
**3.数组的地址：**
int num[]={24,25,26}
数组的首地址有三种方法获取：
num
&num
&num[0]

```python
int num[3]={11,2,13};
   printf("\n%d",num);
   printf("\n%d",&num);
   printf("\n%d",&num[0]);
```
输出都是同一地址

**4.数组的值**：
数组的值有四种方法获取：
num[i]
*(num+i)
*(i+num)
i[num]

地址：&num[i]
值：num[i]

```python
#include <stdio.h>  
 int main(void)  
{  
   int marks[3]={11,2,13};
   printf("\n%d",marks);
   printf("\n%d",&marks[0]);
   int i=77,*p;
   p=&i;
   printf("\n%d",*p);
   for(i=0;i<10;i++)
   {
   p++;
   printf("\n%d",*p);
   }
}
```
![](https://img-my.csdn.net/uploads/201211/21/1353504618_1122.jpg)
**5.传递数组**
```python
#include <stdio.h>  
 int main(void)  
{  
   int marks[3]={11,2,13};
   
   
   int i=77,*p;
   /*
   printf("\n%d",marks);
   printf("\n%d",&marks[0]);
   p=&i;
   printf("\n%d",*p);
   for(i=0;i<10;i++)
   {
   p++;
   printf("\n%d",*p);
   }
   
   for(i=0;i<10;i++)
   {
   printf("Please input :\n");
   scanf("%d",&marks[i]);
   }
   
*/
   for(i=0;i<3;i++)
   {
   printf("\n%d",&marks[i]);
   display(&marks[i]);
   }
   
   display(marks);
   display(&marks[0]);
   
   printf("\n");
}  
display(int *m)
{
printf("\n%d",*m);
}
```

![](https://img-my.csdn.net/uploads/201211/21/1353506012_1672.jpg)

**6.多维数组**
char a[][10]={
"abc",
"cnn",
"bbc",
};
char *names[]={
"abc",
"cnn",
"bbc",
};
printf("\n%s",names[2]);
printf("\n%s",&names[2]);
printf("\n%s",a[2]);
printf("\n%s",&a[2][0]);
printf("\n%s",&a[2][1]);
输出
bbc
bbc
bbc
bc

内存视图
int a[2][3]={1,2,3,4,5,6};
![](http://hi.csdn.net/attachment/201108/26/0_1314384047X3ub.gif)
a[0][0]=1;
a[0][1]=2;
a[0][2]=3;
int a[2][3]={
{1,2,3},
{4,5,6}
};
多维数组只有第一维可以缺省，其余维都要显示写出
int a[][3]={1,2,3,4,5,6};
//声明指针数组，每个指针元素都初始化为指向各个不同的字符串常量
char const keyword[]={
"do";
"doo";
"doooo";
"do";
"doo";
"do";
"doooooo";
}
//声明矩阵，每一维长度都是9，不够用0补齐
char const keyword[][9]={
"do";
"doo";
"doooo";
"do";
"doo";
"do";
"doooooo";
}
扩展阅读：C语言中字符数组和字符串指针分析
[http://blog.csdn.net/21aspnet/article/details/1539928](http://blog.csdn.net/21aspnet/article/details/1539928)
**十.字符串**
**1.定义**char msg[]={'a','b','c'}
char msg[]={'a','b','c','\0'}
char msg[]={"ABC"}
char *p;或者char *p1="ABC";
p=msg;
一开始不指定字符串内容，就需要定义长度：char  s[20];
**2.输出字符串**
方法一
char name[]={'A','B','C'};
int i=0;
while(i<=2)
{
printf("\n%c",name[i]);
i++;
}
方法二
while(name[i]!='\0')
{
printf("\n%c",name[i]);
i++;
}
方法三
while(*p!='\0')
{
printf("\n%c",*p);
p++;
}
方法四
char name[]={"ABC"};
printf("\n%s",name);

**数组赋值**
数组=数组             [不可以]
指针=字符串         [可以]
指针=地址         [可以]
strcpy(数组,数组)  [可以]

**指针字符串数组内存示意图**
char day[15] = "abcdefghijklmn";//定义一
char day[] = "abcdefghijklmn";//定义一
char * str="abcdefghijklmn";//定义二
windows下VS2010调试窗口看区别
![](http://hi.csdn.net/attachment/201108/26/0_1314384453I0C0.gif)
char str[20]="0123456789";//str是编译期大小已经固定的数组
int a=strlen(str); //a=10;//strlen()在运行起确定
int b=sizeof(str); //而b=20;//sizeof()在编译期确定
sizeof 运算符是用来求内存容量字节的大小的。而strlen是用来求字符串实际长度的。

**3.二维字符数组**
char a[][10]={
"abc",
"cnn",
"bbc",
};
char *names[]={
"abc",
"cnn",
"bbc",
};
printf("\n%s",names[2]);
printf("\n%s",&a[2][0]);
**内存示意图**
char *a[4]={"this","is","a","test"};
char **p=a;
![](http://hi.csdn.net/attachment/201108/27/0_1314459517Cog2.gif)
扩展阅读：C语言字符串处理库函数
[http://blog.csdn.net/21aspnet/article/details/1539970](http://blog.csdn.net/21aspnet/article/details/1539970)
**十一.结构**
**1.声明**
注意结构一定要};结束
声明一
struct  simple{
int a;
int b;
};
使用
struct  simple s1;
s1.a=1;
声明二
typedef struct {
int a;
int b;
};simple
使用
simple s1;
s1.a=1;
注意：方法二比一少写一个struct
**2.结构初始化**
typedef struct {
int a;
int b;
};simple{1,2}
使用
simple.a;//输出1
**3.指向结构的指针**
区结构地址必须要用&，但是数组却即可以用&也可以不用
struct data {
int a;
int b;
};data1{1,2}
struct data *p;
p=&data1;
(*p).a;//输出1
p->b;//输出2

```python
struct book 
   {
   char name[20];
   int callno;
   };
   struct book b1={"abc",100};
   struct book *b2;
   b2=&b1;
   printf("\n%s %d",b1.name,b1.callno);
   printf("\n%s %d",b2->name,(*b2).callno);
```
输出：abc 100
abc 100

**4.成员运算符点"."和指向结构体成员运算符"->"的区别**
两者都是用来引用结构体变量的成员，但它们的应用环境是完全不一样，前者是用在一般结构体变量中，而后者是与指向结构体变量的指针连用，例如：有定义
struct student
{
long num;
float score;
};
struct student stud, *ptr=&stud;
则stud.num、stud.score、ptr->num等都是正确的引用方式，但ptr.num、stud->num就是不允许的，其实ptr->num相当于(*ptr).num，只是为了更为直观而专门提供了这->运算符。
最后指出，这两者都具有最高优先级，按自左向右的方向结合。

**5.结构数组参数**
```python
#include <stdio.h>  
struct book
   {
   char name[20];
   int callno;
   };
 int main(void)  
{  
 struct book b1={"abc",100};
   struct book *b2;
   b2=&b1;
   printf("\n%s %d",b1.name,b1.callno);
   printf("\n%s %d",b2->name,(*b2).callno);
   show(b1);
   show_(b2);
   printf("\n");
}  
show(struct book m)
{
printf("\n%d",m.callno);
}
show_(struct book  *m)
{
printf("\n%d",(*m).callno);
printf("\n%d",m->callno);
}
```
输出：abc 100
abc 100
100
100
所以：**->就相当于***

扩展阅读：
[结构的成员访问](http://blog.csdn.net/21aspnet/article/details/150259)
[typedef struct 用法详解和用法小结](http://blog.csdn.net/21aspnet/article/details/1534137)
[typedef的四个用途和两大陷阱](http://blog.csdn.net/21aspnet/article/details/1534102)

**十二.union联合**
**1.定义**
联合也叫共同体，和结构struct很像
struct可以定义一个包含多个不同变量的类型，每一个变量在内存中占有自己独立的内存空间，可以同时存储不同类型的数据。
uniion也可以定义一个包含多个不同变量类型，但这些变量只共有同一个内存空间，每次只能使用其中的一种变量存储数据。
问题是这样做有什么意义？
union{
int   a;
float   b;
char     c;
}a;
a.b=10.123;  printf("%d",a.a);
虽然没有初始化a，一样可以使用！因为a,b,c共用同一块内存。

**十三.typedef声明**
typedef为C语言的关键字，作用是为一种数据类型定义一个新名字。
这里的数据类型包括内部数据类型（int,char等）和自定义的数据类型（struct等）。
在编程中使用typedef目的一般有两个，一个是给变量一个易记且意义明确的新名字，另一个是简化一些比较复杂的类型声明。
typedef与结构结合使用
typedef struct tagMyStruct
{
int iNum;
long lLength;
} MyStruct;

这语句实际上完成两个操作：
1) 定义一个新的结构类型
struct tagMyStruct
{
int iNum;
long lLength;
};
2) typedef为这个新的结构起了一个名字，叫MyStruct。
typedef struct tagMyStruct MyStruct;
因此，MyStruct实际上相当于struct tagMyStruct，我们可以使用MyStruct varName来定义变量。
typedef int k;
k k1=10;
此声明定义了一个 int 的同义字，名字为k。注意 typedef 并不创建新的类型。它仅仅为现有类型添加一个同义字。
typedef struct node
{
int value;
struct node *link;
}Node;
注意不要写成
typedef struct node
{
int value;
Node *link;
}Node;
不然会有警告：警告：从不兼容的指针类型赋值
所以在结构中的嵌套结构不要用typedef的声明！

**十四.预处理器**
**1.define常量**
\#defined MMX 50
printf("%d",MMX);
使用define更高效，可读性更好。

**2.include文件包含**
\#include "filename"

**3.条件编译**
形式一
\#ifdef
\#else
\#endif
形式二
\#ifndef
\#else
\#endif
形式三
\#if
\#else
\#endif
非0 为真
选择功能是否加入，在编译阶段。
作用一：调试开关。
\#define DEBUG//定义可调试 不定义则不
\#ifnedf DEBUG
\#ifdef
作用二：便于确定哪些头文件没有编译
\#ifnedf FILE_H_
\#define FILE_H_
**总结：**
\#define     定义宏
\#undef      取消已定义的宏
\#if         如果给定条件为真，则编译下面代码
\#ifdef      如果宏已经定义，则编译下面代码
\#ifndef     如果宏没有定义，则编译下面代码
\#elif       如果前面的\#if给定条件不为真，当前条件为真，则编译下面代码
\#endif      结束一个\#if……\#else条件编译块
\#error      停止编译并显示错误信息
扩展阅读：[http://blog.csdn.net/21aspnet/article/details/6737612](http://blog.csdn.net/21aspnet/article/details/6737612)

**十五.输入输出**
**1.Printf输出**
**格式**
%d有符号短整型    %u无符号短整型
%ld有符号长整型   %lu无符号长整型
%x无符号16进制    %o无符号8进制
%f浮点型                 %lf double浮点型
%c字符                    %s字符串
printf("putout %d %f",a,b)
注意：printf会输出中文乱码，其实是SecureCRT这样的终端编码格式问题，设置为默认即可。

**控制符**
dd 指定字段宽度的位数
.     区分字段宽度与精度的小数点
-     在指定的字段狂度内左对齐\n换行符
printf("%4d",a);//
输出：1

**2.scanf输入**
int d=0;
注意是scanf("%d",&d)
这里千万要注意如果写为scanf("d%",&d)一样可以编译通过，但是会引起隐含的bug!!!
注意是&d而不是d，因为是改变变量的地址对应的内容，而不是直接取其值
\#include <stdio.h>
main()
{
int a=1;
char c='z';
char s[]="hello";
scanf("%d %c %s",&a,&c,&s);
printf("\n%d",a);
printf("\n%c",c);
printf("\n%s",s);
printf("\n");
}
注意：scanf会以回车结束，但是读到空白就认为输入结束，空白后面的内容会忽视。。。

**3.gets与puts**
warning: the `gets' function is dangerous and should not be used.
gets从键盘获得一个字符串，按回车终止，但是使用gets会提示警告！
puts一次输出一个字符串与printf不同，prints中可以插入字符。所以puts只能用多个变通实现。
char ss[30];
gets(ss);
printf("\n%s",ss);
puts("\noutput:");
puts(ss);
printf("\n");

**4.fgets与fputs**
fgets可以避免scanf的局限
fgets()函数的基本用法为： fgets(char * s,int size,FILE * stream);
char name[20];
printf("\n 输入任意字符 ： ");
fgets(name, 20, stdin);//stdin 意思是键盘输入
fputs(name, stdout); //stdout 输出
printf("\n");

**fgets比gets安全**！
为了安全，gets少用，因为其没有指定输入字符的大小，限制输入缓冲区得大小，如果输入的字符大于定义的数组长度，会发生内存越界，堆栈溢出。后果非常严重！
fgets会指定大小，如果超出数组大小，会自动根据定义数组的长度截断。
![](https://img-my.csdn.net/uploads/201211/25/1353826208_5629.jpg)

**十六.文件**
**fopen打开文件**
**FILE**结构体已定义在stdio.h中
**文件末尾标志**是ASCII码26，fgetc读到该特殊字符返回宏指令EOF
**fclose关闭文件**

**1.字符读写**
**fgetc**从当前指针位置读取一个字符，然后将指针前进一个位置，指向下一个字符，返回读取的字符。
**fputc**将字符写入文件

**读文件套路**
FILE * fp;
fp=fopen("文件地址","r");
char ch;
ch=fgetc(fp)

```python
#include <stdio.h>
#include <stdlib.h>
void main()
{
FILE * fp;
char ch;
fp=fopen("a.c","r");
if(fp==NULL){
//如果文件不存在的话
puts("can't open file！");
exit(1);
}
while(1)
{
ch=fgetc(fp);
if(ch==EOF)
{
break;
}
printf("%c",ch);
}
fclose(fp);
}
```
注意：如果不包含头文件stdlib.h，编译后就出现了警告：隐式声明与内建函数’exit’不兼容。
文件打开模式：
无法打开返回NULL
r:读。
w:写。
a:末尾追加。文件不存在则创建。
r+:开头读写
w+:覆盖读写
a+:末尾追加读写
对于二进制文件要加b
rb,wb等

**2.字符串读写**
**puts**
**gets**
**fputs**将字符串写入文件
**fgets**一次只读取一行

\#include <stdio.h>
\#include <stdlib.h>
\#include <string.h>
main( )
{
FILE  *fp ;
char  s[80] ;
fp = fopen ("pr1.txt", "w");
if (fp == NULL)
{
puts ( "Cannot open file") ;
exit(1) ;
}
printf ("\nEnter a few lines of text:\n");
while (strlen (gets (s)) > 0)
{
fputs (s, fp);
fputs ("\n", fp);
}
fclose (fp) ;
}

\#include <stdio.h>
\#include <stdlib.h>
\#include <string.h>
void main()
{
FILE * fp;
char s[30];
fp=fopen("a.txt","r");
if(fp==NULL){
puts("can't open file!");
exit(1);
}
while(fgets(s,20,fp)!=NULL)
{
printf("%s",s);
//fputs(s,fp);
//fputs("\n",fp);
}
fclose(fp);
}
如果是来自键盘：fgets(name, 20, stdin);//stdin 意思是键盘输入

**3.fprintf和fscanf行**
这两个函数分别是格式化输出和格式化输入函数，按照指定的格式输入数据或者在屏幕上输出数据，例如“结构”。
fprintf与fscanf和printf与scanf的区别是多了用于存储和读取的文件
在运用fprintf与fscanf时，在向文件输出数据及从文件读取数据时，分隔符应该相一致。

```python
#include <stdio.h>
#include <stdlib.h>
main( )
{
    FILE  *fp ;
    struct emp 
    {
        char  name[40] ;
        int  age ;
        float  bs ;
    };
    struct emp  e;
    fp = fopen ( "pr1.txt", "w" ) ;
    if ( fp == NULL )    
    {
        puts ( "Cannot open file" ) ;
        exit(1) ;
    }
    int i;
    for(i=0;i<4;i++)
    {
    
        printf ( "\nEnter name, age and basic salary: " );
        scanf ( "%s %d %f", e.name, &e.age, &e.bs ) ;
        fprintf ( fp, "%s %d %f\n", e.name, e.age, e.bs ); 
    }
    fclose ( fp );
    
    fp = fopen ( "pr1.txt", "r" ) ;
    if ( fp == NULL )
    {
        puts ( "Cannot open file" ) ;
        exit(1) ;
    }
    while ( fscanf ( fp, "%s %d %f", e.name, &e.age, &e.bs ) != EOF )
        printf ( "\n%s %d %f", e.name, e.age, e.bs ) ; 
    fclose ( fp ) ;
}
```
输出：
Enter name, age and basic salary: 北京 2008 51
Enter name, age and basic salary: 上海 2010 234.7
Enter name, age and basic salary: 广州 2011 34
Enter name, age and basic salary: 香港 2020 78
北京 2008 51.000000
上海 2010 234.699997
广州 2011 34.000000
香港 2020 78.000000


**4.fwrite和fread块**
fwrite和fread函数读写大的数据块。主要用于二进制流。
1.函数功能
用来读写一个数据块。
2.一般调用形式
fread(buffer,size,count,fp);
fwrite(buffer,size,count,fp);
3.说明
（1）buffer：是一个指针，对fread来说，它是读入数据的存放地址。对fwrite来说，是要输出数据的地址。
（2）size：要读写的字节数；
（3）count:要进行读写多少个size字节的数据项；
（4）fp:文件型指针。
注意：
1 完成次写操(fwrite())作后必须关闭流(fclose());
2 完成一次读操作(fread())后，如果没有关闭流(fclose()),则指针(FILE * fp)自动向后移动前一次读写的长度，不关闭流继续下一次读操作则接着上次的输出继续输出;
3 fprintf() ： 按格式输入到流，其原型是int fprintf(FILE *stream, const char *format[, argument, ...]);其用法和printf()相同，不过不是写到控制台，而是写到流罢了。注意的是返回值为此次操作写入到文件的字节数。

```python
#include <stdio.h>
#include <stdlib.h>
main( )
{
	FILE  *fp ;
	struct emp 
	{
		char  name[40] ;
		int  age ;
		float  bs ;
	};
	struct emp  e;
	fp = fopen ( "pr1.txt", "w" ) ;
	if ( fp == NULL )	
	{
		puts ( "Cannot open file" ) ;
		exit(1) ;
	}
	int i;
	for(i=0;i<4;i++)
	{
		printf ( "\nEnter name, age and basic salary: " );
		scanf ( "%s %d %f", e.name, &e.age, &e.bs ) ;
		//fprintf ( fp, "%s %d %f\n", e.name, e.age, e.bs ); 
		fwrite(&e,sizeof(e),1,fp);
	}
	fclose ( fp );
	
	fp = fopen ( "pr1.txt", "r" ) ;
	if ( fp == NULL )
	{
		puts ( "Cannot open file" ) ;
		exit(1) ;
	}	
	//while (fscanf ( fp, "%s %d %f", e.name, &e.age, &e.bs ) != EOF)
	while (fread (&e,sizeof(e),1,fp)==1)
		printf ( "\n%s %d %f", e.name, e.age, e.bs ) ; 
	fclose ( fp ) ;
}
```


**5.文件复制**

复制文本文件**（用r）**

```python
#include <stdio.h>
#include <stdlib.h>
main( )
{
	FILE  *fs, *ft ;
	char  ch ;
	fs = fopen ( "pr1.txt", "r" ) ;
	if ( fs == NULL )
	{
		puts ( "Cannot open source file" ) ;
		exit(1) ;
	}
	ft = fopen ( "pr2.txt", "w" ) ;
	if ( ft == NULL )
	{
		puts ( "Cannot open target file" ) ;
		fclose ( fs ) ;
		exit(1) ;
	}
	while ( 1 )
	{
		ch = fgetc ( fs ) ;
		if ( ch == EOF )
			break ;
		else
			fputc ( ch, ft ) ;
	}
	fclose ( fs ) ;
	fclose ( ft ) ;
}
```
复制二进制文件**（用rb）**

```python
#include <stdio.h>
#include <stdlib.h>
main( )
{
	FILE  *fs, *ft ;
	int ch ;
	fs = fopen ( "pr1.exe", "rb" ) ;
	if ( fs == NULL )
	{
		puts ( "Cannot open source file" ) ;
		exit(1) ;
	}
	ft = fopen ( "newpr1.exe", "wb" ) ;
	if ( ft == NULL )
	{
		puts ( "Cannot open target file" ) ;
		fclose ( fs ) ;
		exit(1) ;
	}
	while ( 1 )
	{
		ch = fgetc ( fs ) ;
		if ( ch == EOF )
			break ;
		else
			fputc ( ch, ft ) ;
	}
	fclose ( fs ) ;
	fclose ( ft ) ;
}
```


**十七.内存**
**1.内存分配**
程序中局部变量在栈上分配空间，系统自动分配管理；
动态分配内存在堆上，需要用户释放。
都在<stdlib.h>
申请内存：malloc
重新申请：realloc
释放内存：free

\#include <stdio.h>
\#include <stdlib.h>
main()
{
int i;
char c,*p;
p = (char *)malloc(10);//分配十个字节的缓存区
for(i=0;;i++)
{
c = getchar();//从键盘读取单个字符数据
if(i>9)
{
p = (char *)realloc(p,1);//重新增加申请一个字节的空间
}
if(c == '\n')
{
p[i] = '\0';
break;
}
else
{
p[i] = c;//将输入的字符保存到分配的缓存区
}
}
printf("%s\n",p);
free(p);//释放内存
}

**2.memset**
void *memset(void *s, int ch, size_t n);
函数解释：将s中前n个字节替换为ch并返回s；
memset:作用是在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种最快方法。
\#include <stdio.h>
\#include <stdlib.h>
\#include <string.h>
main(){
char a[5];
memset(a,'1',sizeof(a));
printf("%s\n",a);

char buffer[] = "Hello world\n";
printf("Buffer before memset: %s\n", buffer);
memset(buffer, '*', strlen(buffer) );
printf("Buffer after memset: %s\n", buffer);
}
输出：
![](https://img-my.csdn.net/uploads/201204/24/1335264119_3128.jpg)


**3.动态内存分配**
动态内存分配是为了链表，否则只能用数组
使用malloc和free分配和释放
\#include <malloc.h>//malloc
\#include <stdlib.h>//exit’
int * p;
p=malloc(100);
if(p==NULL)
{
printf("Out of memory!\n");
exit(1);
}
实际中p=malloc(sizeof(int));获得足够1个整数的内存
实际中p=malloc(10*sizeof(int));获得足够10个整数的内存
扩展阅读：C语言内存动态分配
[http://blog.csdn.net/21aspnet/article/details/146968](http://blog.csdn.net/21aspnet/article/details/146968)

**十八.异常**
**1.perror**
perror("fff");
perror ()用 来 将 上 一 个 函 数 发 生 错 误 的 原 因 输 出 到 标 准 设备 (stderr) 。参数 s 所指的字符串会先打印出,后面再加上错误原因字符串。此错误原因依照全局变量error 的值来决定要输出的字符串。
\# ./a.out
fff: Success

**十****九.****链表**
**定义链表**
//先定义一个结构体
typedef struct node{
int data;//数据域
struct node * next;//指针域  ，是struct node的成员，又指向struct node类型的的数据，这里存放下个结点的地址
}Node;
建立链表
//返回类型是之前定义的结构体
Node * creat()
{
//需要先定义3个节点
Node * head=NULL;//头结点
Node *p=NULL;//指向原链表的最后一个结点
Node *s=NULL;//指向新节点
head=(Node*)malloc(sizeof(Node));//动态分配内存空间
head->next=NULL;//开始头结点也就是尾结点
p=head;//p一开始指向头结点
//用while循环
int c=1;//为0是退出循环的条件
int d=0;
while(c)
{
//使用scanf接受用户数据数据
scanf("%d",&d);
if (d!=0)//继续输入
{
s==(Node*)malloc(sizeof(Node));//定义新节点s,和定义head一样
s->data=d;//给新节点赋值
p->next=s;//将第一个结点链接在第二个结点后面
p=s;//p继续指向当前节点
}
else
{
c=0;//用户输入0结束 输入
}
}
p->next=NULL;//结束输入时将最后结点定义为尾结点
return head;
}
//输出结点
output(Node * head)
{
//新定义一个指针用于遍历
//其实如果head->data会是0;单链表巧妙的从第二个结点开始，头结点没用，但是并不代表没用值！
Node * p=head->next;
while(p!=NULL)
{
printf("%d",p->data);
p=p->next;//注意while循环里一定要让P指针不断往下指向
}
}
//查找制定元素的顺序
searchdata(Node *  head)
{
int t=0;
printf("请输入您要查找的值：\n");
scanf("%d",&t);
Node * p=head->next;
int k=0;
while(p!=NULL)
{
k++;
if(p->data==t)
{
printf("%d是第%d个元素：\n",t,k);
break;
}
p=p->next;
}
if(p==NULL)
{
printf("没找到");
}
}
//元素之间插入值
insertdata(Node *  head)
{
Node *s=NULL;//指向新节点
s=(Node*)malloc(sizeof(Node));//动态分配内存空间
int t=0;
int i=0;
printf("请输入您要查找的值：\n");
scanf("%d",&t);
printf("请输入您要插在那个元素后面：\n");
scanf("%d",&i);
Node * p=head->next;
while(p!=NULL)
{
if(p->data==i)
{
s->data=t;
s->next=p->next;//注意是先指向新节点，再改旧节点
p-next=s;
printf("插入成功：\n");
break;
}
p=p->next;
}
if(p==NULL)
{
printf("没找到\n");
}
}
扩展阅读：
链表的C语言实现[http://blog.csdn.net/21aspnet/article/details/146968](http://blog.csdn.net/21aspnet/article/details/146968)
单链表功能大全[http://blog.csdn.net/21aspnet/article/details/159053](http://blog.csdn.net/21aspnet/article/details/159053)
**二十.树**
**二十一.图**

**二十二.算法**

**最后：杂项****1.随机数**
srand():生成随机数种子
rand():生成随机数
\#include <stdio.h>
\#include <stdlib.h>
main()
{
int i,j;
srand((int)time(0));//开始注释这里
for(i=0;i<10;i++)
{
j=1+(int)(10.0*rand()/(RAND_MAX+1.0));
printf("%d ",j);
}
}
输出：
![](https://img-my.csdn.net/uploads/201204/25/1335346321_7518.jpg)
说明：没有调用srand每次产生的都是一样的！

