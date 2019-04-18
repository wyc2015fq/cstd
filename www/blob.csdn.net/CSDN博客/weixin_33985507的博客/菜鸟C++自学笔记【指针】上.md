# 菜鸟C++自学笔记【指针】上 - weixin_33985507的博客 - CSDN博客
2010年10月14日 16:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
指针（pointer）就含有变量和函数地址的变量。是C++语言的重要组成部分。C++程序可以声明指向任何数据类型的指针，包括结构和联合数据类型。程序可以使用表达式中任何变量的地址，只有用存储类register 声明的变量除外。程序可以将变量的地址赋予指针变量。此外，程序可以把变量的地址作为实参传递给含有指针变量形参的函数。程序可以在赋值或初始化中使用函数地址。
涉及的大纲如下，将分两个章节，本章为第一章：
- 指向内部类型的指针
- 指针的算术运算符
- 指针和数组
- 指向结构的指针
- 指针和地址作为函数实参
- 从函数返回地址
- 指向函数的指针
- 指向指针的指针
- 指向指针数组的指针
- 指向const 变量的指针
- const 指针变量
- void 指针
# 1.指向内部类型的指针 
声明一个指针时，需要指明该指针的数据类型、带有几个星号以及指针本身的名字。如下有四种声明指针的方式：
```
int *temp;    //此种写法大部分为c程序员的写法
    int* temp;    //此种写法c++程序的写法
    int*temp;    //由于空白可选，因此以下这两种方法也是正确的
    int * temp;
```
Tip：指针本身就是变量。不管指针所指向的是什么？它们通常都具有相同的大小。可以将任何数值强制赋予一个指针同，并把该数值当做一个指针类型的变量地址来提取它指向的内容。
下面这段代码示范了一些指向C++内部类型的指针：
```
#include "stdafx.h"
#include <iostream>
int main(int argc, char* argv[])
{ 
    char c='A';
    int i=123;
    long l=54321;
    float f=3.45;
    char* cp;
    int* ip;
    long* lp;
    float* fp;
    cp=&c;
    ip=&i;
    lp=&l;
    fp=&f;
    std::cout<<*cp<<"\n";
    std::cout<<*ip<<"\n";
    std::cout<<*lp<<"\n";
    std::cout<<*fp<<"\n";
 return 0;
}
```
运行效果如下：
![](https://images.cnblogs.com/cnblogs_com/terryblog/20101014.jpg)
地址运算符“&”返回它后面标识的地址。把一个变量的地址赋给一个指针，就使该指针指向了这个变量。用指针运算符“*”引用这个指针时，通过返回该指针指向变量的值提领指针。
# 2.指针的算术运算
指针与整数变量类似。它们含有可以作为存储器的数值，数值的格式取决于硬件平台。我们可以给指针加上整数值，也可从指针中减去整数值。指针与普通整数的差别在于：当给指针加上或减去偏移量时，编绎器通过指针指向目标的数据类型大小来调节偏移量。如果给指针加1或减1 ，实际上是加上或减去指针所指向的数据类型大小。当给指针加上一个整数值或减去一个整数值时，表达式返回一个新地址。相同类型的两个指针可以相减，减后返回的整数代表两个地址间该类型的实例个数。只有这此算术运算，还有它们极相似的运算符“++”、“--”、“+=”、“-=”才能在指针上进行操作。
如下代码演示了一个C++指针算术运算的简单示例：
```
#include "stdafx.h"
#include <iostream>
int counDown[]={
    10,9,8,7,6,5,4,3,2,1,0
};
int main(int argc, char* argv[])
{ 
    int* cdp=&counDown[0];
    do 
    {
        std::cout<<*cdp<<"\n";
        cdp++;
    } while (*cdp);
 return 0;
}
```
运行效果如下：
![](https://images.cnblogs.com/cnblogs_com/terryblog/201010141.jpg)
如上程序将数组第1 个元素的地址赋给指针cdp。cdp 递增的语句并不是给指针地址加上整数1。因为指针被声明为int 类型，该语句实际上给指针地址加上整数类型的大小。语句还可以这样简洁的写法：
```
do  
        std::cout<<*cdp++<<"\n"; 
     while (*cdp);
```
自动增量运算符“++”的优先级比指针运算符“*”优先级低，所以上面给出的表达式先检索cdp指针所指内容，然后递增cdp指针 。如果要在检索指针所指内容前递增指针，可以用下面的语句：
```
std::cout<<*++cdp<<"\n";
```
得到结果如下：
![](https://images.cnblogs.com/cnblogs_com/terryblog/201010142.jpg)
下面我们使用一个变量来循环访问指针的数组元素，代码如下：
```
#include "stdafx.h"
#include <iostream>
float dues[]={
    30.00,
    55.00,
    100.00
};
int main(int argc, char* argv[])
{ 
     float* dp=&dues[0];
     for (int i=0;i<3;i++)
     {
         std::cout<<*(dp+i)<<"\n";
     }
 return 0;
}
```
运行效果如下：
![](https://images.cnblogs.com/cnblogs_com/terryblog/201010143.jpg)
# 3.指针和数组
指针和数组之间有一种常常令人困惑的特殊关系，当我们知道存在两种获得数组或数组某个元素地址的方法时，往往产生困惑。So far ，以上代码都用地址运算符“&”取出数值第1个元素的地址。另一种表示法只使用数组的名称。在表达式中，使用数组的名称与取数组第1个元素的地址效果一样。
我们可以引用上面的代码来分析两种写法：
```
float* dp=&dues[0];
     float* dp1=dues;
     std::cout<<"dp的值："<<*dp<<"\ndp1的值："<<*dp1<<"\n";
```
看看运行的效果如何：
![](https://images.cnblogs.com/cnblogs_com/terryblog/201010144.jpg)
可以看出两种写法的效果是一样的，如果你要取出数组的其他的值该如何做呢？比如你想取数组下标为2的值 。很简单看如何代码：
```
float* dp=&dues[2];
     float* dp1=dues+2;
```
运行的效果也是一样的。
如果数组的多维的，即使没有给出所有维数的下标，这种地址表示法仍然适用，如下DEMO所示：
```
#include "stdafx.h"
#include <iostream>
int temp[3][4]={
    {
        1,2,3,4
    },
    {
         5,6,7,8
    },
    {
         9,10,11,12
    }
};
int main(int argc, char* argv[])
{ 
      
 int* cp1=&temp[0][1];
 int* cp2=temp[1]+1;
 int* cp3=temp[0];
 int* cp4=temp[2];
 std::cout<<"cp1的值是："<<*cp1<<"\n";
 std::cout<<"cp2的值是："<<*cp2<<"\n";
 std::cout<<"cp3的值是："<<*cp3<<"\n";
 std::cout<<"cp4的值是："<<*cp4<<"\n";
 return 0;
}
```
运行效果如下：
![](https://images.cnblogs.com/cnblogs_com/terryblog/201010145.jpg)
Tip：未始化的值编绎器都默认赋0；
我们可以遵循几种基本的用法约定，减轻对C++指针的困惑，直到对指针和数组的互换习以为常。下面示范了如何使用指针和数组：
```
#include "stdafx.h"
#include <iostream>
char msg[]="Now is the time \n";
int main(int argc, char* argv[])
{
    char* cp;
    int i;
    for (cp=msg;*cp;cp++)
    {
        std::cout<<*cp;
    }
    for (i=0;msg[i];i++)
    {
        std::cout<<msg[i];
    }
    for (cp=msg;cp[0];cp++)
    {
        std::cout<<cp[0];
    }
    for (i=0;*(msg+i);i++)
    {
        std::cout<<*(msg+i);
    }
    for (i=0,cp=msg;*(cp+i);i++)
    {
        std::cout<<*(cp+i);
    }
    for (i=0,cp=msg;cp[i];i++)
    {
        std::cout<<cp[i];
    }
    return 0;
}
```
运行效果如下：
![](https://images.cnblogs.com/cnblogs_com/terryblog/201010146.jpg)
我们可以把指针和数组下标两种表示法组合使用，以上代码给出的6种方法能得到相同结果。
# 4.指向结构的指针
指向结构的指针与其他指针的工作方式相同。结构指针指向结构类型的一个实例。对指针的递增和递减都会以结构大小的倍数改变指针地址，结构的大小就是结构成员的大小加上编译器给结构的填充或对齐大小。可以使用成员指针运算符“->”，通过指针来访问结构成员，如下代码使用结构指针循环访问一个结构数组：
```
#include "stdafx.h"
#include <iostream>
struct Emploee{
    int emplno;
    char* name;
};
Emploee emplos[]={
    {
        1,"terry"
    },
    {
        2,"aa"
    },
    {
        3,"bb"
    },
    {
        -1,"cc"
    }
};
int main(int argc, char* argv[])
{
 
    Emploee* ep=emplos;
    while(ep->emplno!=-1)
    {
        std::cout<<ep->emplno<<"\n";
        std::cout<<ep->name<<"\n";
        ep++;
    }
    return 0;
}
```
运行效果如下：
![](https://images.cnblogs.com/cnblogs_com/terryblog/201010147.jpg)
Tip：“.”和“->”是有区别的，前者对己命名结构进行直接成员访问，后者通过结构的指针间接访问结构成员。
以上程序给数组增加了一个终止元素-1，以便while 循环可以判断ep 指针何时指向数组末尾。下面是另一种循环访问数组的方法，它不用给数组增加终止元素就可以找到数组末尾：
```
const int elments=sizeof emplos+sizeof(Emploee);
    while(ep< emplos+ elments)
    {
        std::cout<<ep->emplno<<"\n";
        std::cout<<ep->name<<"\n";
        ep++;
    }
```
# 5.指针和地址作为函数实参
当函数的原型声明一个指针形参时，函数的调用者应传一个实参，这个实参可以是指针变量或是指针地址。声明指针形参有两种约定表示法：
```
void  ErrorMessage(char* msg);
void ErrorMessage(char msg[]);
```
这两个原型是相同的，我们不能将数组作为函数实参传递。传递给函数的实参可以是指针变量，也可以是指针变量的地址。下面这段代码示范了对类函数的调用：
```
#include "stdafx.h"
#include <iostream>
inline void ErrorMessage(char* msg){
    std::cout<<"Error:"<<msg<<"\n";
} 
int main(int argc, char* argv[])
{
    char* ep="Invalid input";
    ErrorMessage(ep);
    char msg[]="Failure";
    ErrorMessage(msg);
    ErrorMessage("Timeout");
 
    return 0;
}
```
输出结果也是一样的：
![](https://images.cnblogs.com/cnblogs_com/terryblog/201010148.jpg)
如果函数要循环访问一个多维数组，指向多维数组的指针形参必须指明外层的维数。该声明要告诉编译器外层数组的宽度。如下代码：
```
#include "stdafx.h"
#include <iostream>
void DisplayCalendar(int cal[][3]);
int main(int argc, char* argv[])
{
 
    static int calendar[2][3]={
        {
            1,2,3
        },
        {
            4,5,6
        } 
    };
 DisplayCalendar(calendar);
    return 0;
}
void DisplayCalendar(int cal[][3]){
    std::cout<<"输出的值为： \n";
    for (int week=0;week<2;week++)
    {
        for (int day=0;day<3;day++)
        {
            int date=cal[week][day];
            if(date)
                std::cout<<date<<"\n";
        }
    }
}
```
输出结果：
![](https://images.cnblogs.com/cnblogs_com/terryblog/201010149.jpg)
# 6.从函数返回地址
当一个函数被声明为返回指针的函数时，该函数的返回地址可以被它的调函数在某个表达式中使用，如下代码：
```
#include "stdafx.h"
#include <iostream>
int* GetDate(int wk,int dy);
int main(int argc, char* argv[])
{
    int wk,dy;
    do 
    {
        std::cout<<"Enter week(1-5) day(1-7)";
        std::cin>>wk>>dy;
    } while (wk<1||wk>6||dy<1||dy>7);
    std::cout<<*GetDate(wk,dy);
    return 0;
}
 
int* GetDate(int wk,int dy){
    static int calendar[5][7]={
        {
            1,2,3,4,5,6,7
        },
        {
            8,9,10,11,12,13,14
        },
        {
            15,16,17,18,19,20,21
        },
        {
             22,23,24,25,26,27,28
        },
        {
            29,30,31,32,33,34,-1
        }
    };
    return &calendar[wk][dy];
}
```
输入2和4得到效果如下：
![](https://images.cnblogs.com/cnblogs_com/terryblog/201010149.jpg)
如上，通过指针运算符“*”调用GetDate。这种写法提领函数返回的地址，将将返回地址值指向整数打印出来。
这是第一章指针的相关内容，目前工作学习娱乐一起抓，有点晕菜~！
