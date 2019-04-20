# C语言day1(数据类型、变量本质) - 风雪夜归人 - CSDN博客
2017年08月25日 10:40:26[cdjccio](https://me.csdn.net/qq_34624951)阅读数：113
# **1、数据类型的用途及本质**
**数据类型的本质：固定大小内存块的别名**
```cpp
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
//1、数据类型的用途
//数据类型的本质：固定大小内存块的别名
//b 和 &b 数组数据类型 （学会定义 1数组数据类型  2数组指针 3数组类型和数组指针类型的关系）
int main01()
{
    int a;//告诉c编译器分配4个字节的内存
    int b[10];//告诉c编译器分配40个字节的内存
    printf("b:%d, b+1:%d, &d:%d, &b+1:%d \n",b,b+1,&b,&b+1);//
    //b+1  &b+1  结果不一样   b和&b所代表的数据类型不一样
    //b  代表的是数组首元素的地址
    //&d  代表的是整个数组的地址
    printf("sizeof(b):%d",sizeof(b));//40
    printf("sizeof(a):%d",sizeof(a));//4
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
# 2、怎样给数据类型取别名
```cpp
struct teacher
{
    char name[64];
    int age;
}teacher;
typedef struct teacher2
{
    char name[64];
    int age;
}teacher2;
//数据别名 typedef
typedef int u32;
int main02()
{
    int a;//告诉c编译器分配4个字节的内存
    int b[10];//告诉c编译器分配40个字节的内存
//    teacher t1;  error
    struct teacher t1; //ok
    teacher2 t2; //ok
    t1.age = 24;
    printf("sizeof(u32):%d\n",sizeof(u32));//4
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
# **3、void *的初探**
```cpp
//void *
int main03()
{
    {
        char *p2 = NULL;
        void *p1;
        p2 = (char *)malloc(100);//malloc()函数的返回数据类型是void *，所以需要做数据类型的强制类型转换
        p1 = &p2;  //可以把任何类型赋值给p1
    }
    {
//        void a;
    }
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
# 4、变量的本质是什么？
**一段连续内存空间的别名（一个门牌号）通过变量往内存读写数据，不是往变量读写数据**
```cpp
//变量的本质是什么？
//1、一段连续内存空间的别名（一个门牌号）
//2、通过变量往内存读写数据，不是往变量读写数据
int main()
{
    int a;//4
    a = 10;//直接赋值
    int b;//4
    char *p;//4
    printf("&a:%d\n",&a);//2686780
    printf("a:%d\n",a);
    //间接赋值 直接通过内存
    *((int *)2686780) = 200;
    printf("a:%d\n",a);//2686780
    {
        p = 2686780;//间接赋值 通过指针
        *p = 300;
    }
    //3、对内存空间取多个别名  引用
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
# 5、静态存储区的理解
**如果p1和p2指针指向的数据完全相同，则编译器会让p1和p2指向同一块内存空间。**
```cpp
//静态存储区的理解
char *getStr1()
{
//    char *p1 = "abcdefg";
    char *p1 = "abcdefgh";
    return p1;
}
char *getStr2()
{
    char *p2 = "abcdefgh";
    return p2;
}
int main()
{
    char *p1 = NULL;
    char *p2 = NULL;
    p1 = getStr1();
    p2 = getStr2();
    //打印p1 p2所指向内存空间的数据
    printf("p1=%s, p2=%s\n",p1,p2);
    //打印p1 p2的值
    printf("p1=%d, p2=%d\n",p1,p2);
    //4214989 4214997 char *p1 = "abcdefg";char *p2 = "abcdefgh";
    //4214989 4214989 char *p1 = "abcdefgh";char *p2 = "abcdefgh";
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
内存四区图如下：
![](https://img-blog.csdn.net/20170825171041246?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzQ2MjQ5NTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# **6、堆栈的理解**
```cpp
//堆
char *getMem(int num)
{
    char *p1 = NULL;
    p1 = (char *)malloc(sizeof(char) * num);
    if (p1 == NULL)
    {
        return NULL;
    }
    return p1;
}
//栈
char *getMem2()
{
    char buf[64];//栈区存放
    strcpy(buf,"123456789");
    printf("buf:%s\n",buf);
    return buf;//返回不了内存块
}
//堆栈的理解
int main()
{
    char *tmp = NULL;
    tmp = getMem(10);
    if(tmp == NULL)
    {
        return;
    }
    strcpy(tmp,"112233");//向tmp所指向的内存空间copy数据
    printf("tmp = %s\n",tmp);//112233
    tmp = getMem2();//123456789
    printf("tmp = %s\n",tmp);//tmp = null 因为栈区buf空间被析构 赋给tmp的指针为空
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
**![](https://img-blog.csdn.net/20170825221624191?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzQ2MjQ5NTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**![](https://img-blog.csdn.net/20170906104726318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzQ2MjQ5NTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170906104756323?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzQ2MjQ5NTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 7、内存的使用范围
![](https://img-blog.csdn.net/20170826135957415?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzQ2MjQ5NTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
一个程序不管有多少个函数组成，编译器都只分配一个内存四区。
# 8、指针规律一：指针也是一种数据类型
1）指针也是一种变量，占有内存空间，用来保存内存地址
2）*p操作内存
在指针声明时，*号表示所声明的变量为指针
在指针使用时，*号表示 操作 指针所指向的内存空间中的值
         *p相当于通过地址(p变量的值)找到一块内存；然后操作内存
         *p放在等号的左边赋值（给内存赋值）
         *p放在等号的右边取值（从内存获取值）
3）指针变量和它指向的内存块是两个不同的概念
含义1 给p赋值p=0x1111; 只会改变指针变量值，不会改变所指的内容；p = p +1; //p++
含义2 给*p赋值*p='a'; 不会改变指针变量的值，只会改变所指的内存块的值 
含义3 =左边*p 表示 给内存赋值， =右边*p 表示取值 含义不同切结！
含义4 =左边char *p
含义5保证所指的内存块能修改
4）指针是一种数据类型，是指它指向的内存空间的数据类型  
含义1：指针步长（p++），根据所指内存空间的数据类型来确定
p++相当于(unsigned char )p+sizeof(a);
结论：指针的步长，根据所指内存空间类型来定。
注意：指针指向谁，就把谁的地址赋值给指针。图和代码和二为一。    
                  不断的给指针变量赋值，就是不断的改变指针变量（和所指向内存空间没有任何关系）。
```cpp
//指针铁律一：指针是一种数据类型
int main08()
{
    int a = 10;
    char *p1 = 100;//分配4个字节的内存空间
    printf("p1:%d\n",p1);
    char ****p2 = 100;
    int *p3 = NULL;
    p3 = &a;
    *p3 = 20;//间接修改a的值
    {
        int c = 0;
        c = *p3;//c= 20
        //*p放在=左边  写内存
        //*p放在=右边  读内存
    }
    //不断的给指针赋值，相当于不停的改变指针的指向
    {
        char *p4 = NULL;
        p4 = (char *)malloc(100);
        p4 = (char *)malloc(200);
    }
    
    printf("Hello world!\n");
    system("pause");
    return 0;
}
////
char *getStr()
{
    char *tmp = NULL;
    tmp = "abcdefg";
    return tmp;
}
/*
//指针是一种数据类型，是指它指向的内存空间的数据类型  
int getABC1(char *p1);
int getABC2(char **p2);
int getABC3(char ***p3);
int getABC4(char (*p4)[30]);
int getABC5(char p5[10][30]);
//指针做函数参数 形参有多级指针的时候，
//站在编译器的角度，只需要分配4个字节的内存
//
*/
int main()
{
    char *p= getStr();
    printf("p= %s\n",p);
//    *(p+2) = 'r';//出现问题（常量区的值不能被修改）  保证所指向的内存空间可以被修改
//    printf("p= %s\n",p);
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
