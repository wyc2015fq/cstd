# C语言：判断回文字符串的两种简单方法 - Machine Learning with Peppa - CSDN博客





2018年01月12日 20:34:33[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：11247
所属专栏：[C语言入门](https://blog.csdn.net/column/details/18792.html)









之前写过逆排序的数组实现，对于经典的回文问题却还没有深入研究过。今天抽空看了下，总结了两种比较常用的回文法。

一种是字符串（当然也可以叫数组法），此方法可以用来判断字符串输入以及INT类型的输入

另一种是数字输入，因为追求简便性所以此方法只能判断回文数，不能判断回文字符串。要想都判断也很简单，将变量类型调整即可实现

代码以及讲解如下：

```cpp
/***判断回文数***/

//情况1.利用字符串判断回文
//实现方法：利用字符串指针从头尾分别判断

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ctype.h>

//typedef char  Pre_;     方便调试回文时更改类型

bool Judge_char(const char *p); //声明一个布尔型变量的函数原型

int main(int argc, char *argv[])
{

    printf("Please enter the chars to judge:\n");

    bool re_value = 0;  //初始化逻辑变量
    char *k;
    scanf("%s", k);   //声明一个字符串指针，并将STDIN传入

    re_value = Judge_char(k);

    if (re_value){
        printf("This charset is Palindrom");
    }
    else {
        printf("this charset is not Palindrom");     //判断命题真假并输出结果
    }
    return 0;
}

bool Judge_char(const char *p)
{
    register int i = 0;      //计数变量初始化
    int len = strlen(p); //使用STRLEN函数取字符串数组的字符位数

    //注意此处STRLEN与SIZEOF的使用方法区别，后者返回参数的所占空间大小并包含空字符'\0'大小
    //在STRLEN原型中传入的参数是const指针而不是*p对象

    for (i = 0; i <= len; i++)
    {
        if (p[i] == p[len - 1])
        {
            len--; //若首尾两个字符等值，分别向字符串中心移动一位，并判断
        }
        else
        return false;
    }

    return true;
}
```

```cpp

```

```cpp
//情况2.判断数字回文
//利用数字的数值方法进行回文判断


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//此方法可以直接嵌入main函数中，不需要声明自定义函数
int main(int argc, char *argv[])
{

    printf("Please enter the number to judge:\n");

    int Inp;   //将INP作为一个保存初始变量的整型，用于循环后的判断
    scanf("%d", &Inp);      //将STDIN格式化传入INP中，举个例子Inp = 1234

    int sum  = 0;    
    int Calcul;
    Calcul = Inp;   //声明另一个CALCUL变量并将初始值赋给它，其在循环中将会用以判断并发生改变

    while(Calcul != false)
    {
        sum = sum * 10 + Calcul % 10;//第一次等式为sum=0*10+1234%10(其实值就是4)=4，
                    //第二次等式为sum=4*10+123%10(其实值为3)=43，第三次等式为sum=43*10+12%10=432
                   //第四次sum=432*10+1%10(其实为1)=4321
        Calcul /= 10;
//第一次等式为Calcul=1234/10=123第二次Calcul=123/10=12第三次Calcul=12/10=1,第四次Calcul=1/10=0;
    }

    if (sum == Inp)
    {
        printf("this is a palindrome");
    }

    else{printf("this is not a palindrome");
    }

    return 0;
}
```





