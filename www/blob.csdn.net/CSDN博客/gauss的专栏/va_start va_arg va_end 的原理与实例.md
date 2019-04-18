# va_start va_arg va_end 的原理与实例 - gauss的专栏 - CSDN博客
2013年01月04日 22:18:54[gauss](https://me.csdn.net/mathlmx)阅读数：201
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)
转自：http://blog.csdn.net/sunboy_2050/article/details/6189072
func( Type para1, Type para2, Type para3, ... ) 
{ 
      /****** Step 1 ******/ 
      va_list ap; 
      va_start( ap, para3 ); //一定要“...”之前的那个参数**ap指向para后的第一个可变参数。 
      /****** Step 2 ******/ 
      //此时ap指向第一个可变参数 
      //调用va_arg取得里面的值 
      Type xx = va_arg( ap, Type );  
      //Type一定要相同，如: 
      //char *p = va_arg( ap, char *); 
      //int i = va_arg( ap, int ); 
      //如果有多个参数继续调用va_arg 
      /****** Step 3 ******/ 
      va_end(ap); //For robust! 
} 
◎研究： 
typedef char *    va_list;//va_list 等价于char*即字符指针。 
#define va_start _crt_va_start//注意下面的替代。 
#define va_arg _crt_va_arg 
#define va_end _crt_va_end 
#define  _crt_va_start(ap,v)    ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) ) 
#define _crt_va_arg(ap,t)      ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) ) 
#define _crt_va_end(ap)        ( ap = (va_list)0 )  
va_list argptr; 
C语言的函数是从右向左压入堆栈的,调用va_start后， 
按定义的宏运算，_ADDRESSOF得到v所在的地址，然后这个 
地址加上v的大小，则使ap指向第一个可变参数如图:      
    栈底 高地址 
    | .......      
    | 函数返回地址 
    | .......       
    | 函数最后一个参数 
    | ....                        
    | 函数第一个可变参数       <--va_start后ap指向  
    | 函数最后一个固定参数 
    | 函数第一个固定参数  
    栈顶 低地址 
然后，用va_arg()取得类型t的可变参数值, 先是让ap指向下一个参数: 
ap += _INTSIZEOF(t)，然后在减去_INTSIZEOF(t)，使得表达式结果为 
ap之前的值，即当前需要得到的参数的地址，强制转换成指向此参数的 
类型的指针，然后用*取值 
最后，用va_end(ap)，给ap初始化，保持健壮性。 
example:(chenguiming) 
#include    <stdio.h>    
   #include    <ctype.h>    
   #include<stdlib.h>    
   #include    <stdarg.h>    
   int    average(    int    first,    ...    )      //变参数函数,C++里也有  **…表明后面有好多可变的参数。 
   {    
         int    count=0,i=first,sum=0;    
         va_list    maker;           //va_list    类型数据可以保存函数的所有参数,做为一个列表一样保存。Va_list即是char*表明maker是一个字符型的指针。   
         va_start(maker,first);    //设置列表的起始位置   **frist只是和maker在一起做参数，这并不说明maker指向frist而是指向first之后的第一个可变的参数，而frist是作为一个固定参数，因为它在…之前。这时候frist指向3。
         while(i!=-1)    
         {    
         sum+=i;    
         count++;    
         i=va_arg(maker,int);//返回maker列表的当前值,并指向列表的下一个位置    
         }    
//第一次：I=2，sum=2； 
第二次：I=3，因为va_start(maker,first);则sum=2+3=5；同时i=va_arg(maker,int)又使I=4； 
第三次：I=4，sum=5+4=9,同理I=4； 
第四次I=4，sum=9+4=13同理I=-1 
         return    sum/count;    
   }    
   void    main(void)    
   {    
   printf(    "Average    is:    %d/n",    average(    2,    3,    4,4,    -1    )    );   
   }    
注意它们的头文件stdarg..h 
std很正常arg 是参数的意思； 
辅助理解： 
va_start(arg_ptr, argN)：使参数列表指针arg_ptr指向函数参数列表中的第一个可选参数，说明：argN是位于第一个可选参数之前的固定参数，（或者说，最后一个固定参数；…之前的一个参数），函数参数列表中参数在内存中的顺序与函数声明时的顺序是一致的。如果有一va函数的声明是void va_test(char a, char b, char c, …)，则它的固定参数依次是a,b,c，最后一个固定参数argN为c，因此就是va_start(arg_ptr, c)。
　　va_arg(arg_ptr, type)：返回参数列表中指针arg_ptr所指的参数，返回类型为type，并使指针arg_ptr指向参数列表中下一个参数。 
　　va_copy(dest, src)：dest，src的类型都是va_list，va_copy()用于复制参数列表指针，将dest初始化为src。 
　　va_end(arg_ptr)：清空参数列表，并置参数指针arg_ptr无效。说明：指针arg_ptr被置无效后，可以通过调用 va_start()、va_copy()恢复arg_ptr。每次调用va_start() / va_copy()后，必须得有相应的va_end()与之匹配。参数指针可以在参数列表中随意地来回移动，但必须在va_start() … va_end()之内。
==========================================================
**使用示例：**
实现源码：
**[cpp]**[view plain](http://blog.csdn.net/sunboy_2050/article/details/6189072#)[copy](http://blog.csdn.net/sunboy_2050/article/details/6189072#)[print](http://blog.csdn.net/sunboy_2050/article/details/6189072#)[?](http://blog.csdn.net/sunboy_2050/article/details/6189072#)
- #include <stdio.h>  
- #include <stdlib.h>  
- #include <stdarg.h>  
- char *  
- make_message(const char *fmt, ...)  
- {  
-     int n, size=100;  
-     char *buff;      // 保存字符串缓存  
-     va_list ap;      // typedef char* va_list  
-     if((buff=(char *)malloc(size))==NULL)  
-         return NULL;  
-     while(1){  
-         //尝试在申请的空间中进行打印操作  
-         va_start(ap, fmt);  
-         n = vsnprintf(buff, size, fmt, ap);  
-         va_end(ap);  
-         //如果vsnprintf调用成功，则返回该字符串  
-         if(n>-1 && n<size)  
-             return buff;  
-         //申请原来2倍大小缓存  
-         size *=2;  
-         if((buff = (char *)realloc(buff, size)) == NULL)  
-             return NULL;  
-     }  
- }  
- int  
- main(void)  
- {  
-     char *format = "%d, %d, %d, %d";  
-     char *str = make_message(format, 5, 6, 7, 8);  
-     printf("%s/n", str);  
-     return 0;  
- }  
运行结果：
[work]$ gcc -W -o stdarg stdarg.c 
[work]$ ./stdarg    
5, 6, 7, 8
==========================================================
**参考拓展：**
[va_start va_arg va_end 的使用和原理](http://tieba.baidu.com/f?kz=329987122)
[va_start 探究与思考](http://www.programfan.com/club/showpost.asp?id=141694&t=o)
==========================================================
**va_startva_end 的使用和原理**
**1:当无法列出传递函数的所有实参的类型和数目时,可用省略号指定参数表**
void foo(...);
void foo(parm_list,...);
**2:函数参数的传递原理**
函数参数是以数据结构:栈的形式存取,从右至左入栈.eg:
**先介绍一下可变参数表的调用形式以及原理：**
首先是参数的内存存放格式：参数存放在内存的堆栈段中，在执行函数的时候，从最后一个开始入栈。因此栈底高地址，栈顶低地址，举个例子如下：
void func(int x, float y, char z);
那么，调用函数的时候，实参 char z 先进栈，然后是float y，最后是 int x，因此在内存中变量的存放次序是 x->y->z，因此，从理论上说，我们只要探测到任意一个变量的地址，并且知道其他变量的类型，通过指针移位运算，则总可以顺藤摸瓜找到其他的输入变量。
下面是<stdarg.h> 里面重要的几个宏定义如下：
typedef char* va_list;
void va_start ( va_list ap, prev_param ); /* ANSI version */
type va_arg ( va_list ap, type ); 
void va_end ( va_list ap ); 
va_list 是一个字符指针，可以理解为指向当前参数的一个指针，取参必须通过这个指针进行。
<Step 1> 在调用参数表之前，定义一个 va_list 类型的变量，(假设va_list 类型变量被定义为ap)；
<Step 2> 然后应该对ap 进行初始化，让它指向可变参数表里面的第一个参数，这是通过 va_start 来实现的，第一个参数是 ap 本身，第二个参数是在变参表前面紧挨着的一个变量,即“...”之前的那个参数；
<Step 3> 然后是获取参数，调用va_arg，它的第一个参数是ap，第二个参数是要获取的参数的指定类型，然后返回这个指定类型的值，并且把 ap 的位置指向变参表的下一个变量位置；
<Step 4> 获取所有的参数之后，我们有必要将这个 ap 指针关掉，以免发生危险，方法是调用 va_end，他是输入的参数 ap 置为 NULL，应该养成获取完参数表之后关闭指针的习惯。
例如 int max(int n, ...); 其函数内部应该如此实现：
#include<iostream> 
void fun(int a, ...) 
{ 
int *temp = &a; 
temp++; 
for (int i = 0; i < a; ++i) 
{ 
cout << *temp << endl; 
temp++; 
} 
}
intmain() 
{ 
int a = 1; 
int b = 2; 
int c = 3; 
int d = 4; 
fun(4, a, b, c, d); 
system("pause"); 
return 0; 
} 
Output:: 
1 
2 
3 
4
3:获取省略号指定的参数
在函数体中声明一个va_list，然后用va_start函数来获取参数列表中的参数，使用完毕后调用va_end()结束。像这段代码： 
void TestFun(char* pszDest, int DestLen, const char* pszFormat, ...) 
{ 
va_list args; 
va_start(args, pszFormat); //一定要“...”之前的那个参数
_vsnprintf(pszDest, DestLen, pszFormat, args); 
va_end(args); 
}
4.va_start使argp指向第一个可选参数。va_arg返回参数列表中的当前参数并使argp指向参数列表中的下一个参数。va_end把argp指针清为NULL。函数体内可以多次遍历这些参数，但是都必须以va_start开始，并以va_end结尾。
　　1).演示如何使用参数个数可变的函数，采用ANSI标准形式 
#include 〈stdio.h〉 
#include 〈string.h〉 
#include 〈stdarg.h〉 
/*函数原型声明，至少需要一个确定的参数，注意括号内的省略号*/ 
int demo( char, ... ); 
void main( void ) 
{ 
   demo("DEMO", "This", "is","a", "demo!", ""); 
} 
/*ANSI标准形式的声明方式，括号内的省略号表示可选参数*/ 
int demo( char msg, ... ) 
{ 
       /*定义保存函数参数的结构*/
   va_list argp; 
   int argno = 0; 
   char para; 
　　  /*argp指向传入的第一个可选参数，msg是最后一个确定的参数*/ 
   va_start( argp, msg ); 
   while (1) 
       { 
    para = va_arg( argp, char); 
       if ( strcmp( para, "") == 0 ) 
       break; 
       printf("Parameter #%d is: %s/n",argno, para); 
       argno++; 
} 
va_end( argp ); 
/*将argp置为NULL*/
return 0; 
} 
2)//示例代码1：可变参数函数的使用
#include "stdio.h"
#include "stdarg.h"
void simple_va_fun(int start, ...) 
{ 
    va_list arg_ptr; 
   int nArgValue =start;
    int nArgCout=0;     //可变参数的数目
    va_start(arg_ptr,start); //以固定参数的地址为起点确定变参的内存起始地址。
    do
    {
        ++nArgCout;
        printf("the %d th arg:%d/n",nArgCout,nArgValue);     //输出各参数的值
        nArgValue =va_arg(arg_ptr,int);                     //得到下一个可变参数的值
    } while(nArgValue !=-1);               
    return; 
}
int main(int argc, char* argv[])
{
    simple_va_fun(100,-1); 
    simple_va_fun(100,200,-1); 
    return 0;
}
3)//示例代码2:扩展——自己实现简单的可变参数的函数。
下面是一个简单的printf函数的实现，参考了<TheC Programming Language>中的例子
#include "stdio.h"
#include "stdlib.h"
void myprintf(char* fmt, ...)        //一个简单的类似于printf的实现，//参数必须都是int类型
{ 
    char*pArg=NULL;              //等价于原来的va_list 
    char c;
    pArg = (char*)&fmt;          //注意不要写成p = fmt !!因为这里要对//参数取址，而不是取值
    pArg +=sizeof(fmt);         //等价于原来的va_start          
    do
    {
        c =*fmt;
        if (c != '%')
        {
           putchar(c);           //照原样输出字符
        }
        else
        {
           //按格式字符输出数据
           switch(*++fmt) 
           {
            case'd':
               printf("%d",*((int*)pArg));          
               break;
            case'x':
               printf("%#x",*((int*)pArg));
               break;
            default:
               break;
            } 
            pArg +=sizeof(int);              //等价于原来的va_arg
        }
        ++fmt;
    }while (*fmt != '/0'); 
    pArg =NULL;                              //等价于va_end
    return; 
}
int main(int argc, char* argv[])
{
    int i = 1234;
    int j = 5678;
    myprintf("the first test:i=%d/n",i,j); 
    myprintf("the secend test:i=%d;%x;j=%d;/n",i,0xabcd,j); 
    system("pause");
    return 0;
}
int max(int n, ...) {               // 定参 n 表示后面变参数量，定界用，输入时切勿搞错
　va_list ap;                        // 定义一个 va_list 指针来访问参数表
     va_start(ap, n);                      // 初始化 ap，让它指向第一个变参，n之后的参数
    int maximum = -0x7FFFFFFF;         // 这是一个最小的整数
    int temp;
     for(int i = 0; i < n; i++) {
    temp = va_arg(ap, int);         // 获取一个 int 型参数，并且 ap 指向下一个参数
    if(maximum < temp) maximum = temp;
     }
    va_end(ap);                        // 善后工作，关闭 ap
    return max;
}
// 在主函数中测试 max 函数的行为(C++ 格式)
int main() {
   cout << max(3, 10, 20, 30) << endl;
   cout << max(6, 20, 40, 10, 50, 30, 40) << endl;
}
基本用法阐述至此，可以看到，这个方法存在两处极严重的漏洞：其一，输入参数的类型随意性，使得参数很容易以一个不正确的类型获取一个值(譬如输入一个float，却以int型去获取他)，这样做会出现莫名其妙的运行结果；其二，变参表的大小并不能在运行时获取，这样就存在一个访问越界的可能性，导致后果严重的 RUNTIME ERROR。
