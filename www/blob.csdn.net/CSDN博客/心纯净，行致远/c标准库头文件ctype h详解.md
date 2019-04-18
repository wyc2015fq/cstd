# c标准库头文件ctype.h详解 - 心纯净，行致远 - CSDN博客





2018年11月15日 15:19:56[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：84








最近实践了一些编程，越来越感觉库函数的重要性。掌握越来越多的函数才会站在巨人的肩膀上思考问题，而不是自己去写代码去实现函数，自己又能写几个函数呢……

ctype.h是C[标准](https://baike.baidu.com/item/%E6%A0%87%E5%87%86)函数库中的[头文件](https://baike.baidu.com/item/%E5%A4%B4%E6%96%87%E4%BB%B6/10978258)，定义了一批C语言[字符](https://baike.baidu.com/item/%E5%AD%97%E7%AC%A6/4768913)分类函数（C character classification functions），用于测试字符是否属于特定的字符类别，如字母字符、控制字符等等。既支持单[字节](https://baike.baidu.com/item/%E5%AD%97%E8%8A%82/1096318)（Byte)字符，也支持宽字符。

头文件包含两类函数：

**1****、[字符](https://baike.baidu.com/item/%E5%AD%97%E7%AC%A6)测试函数**

1> 函数原型均为int isxxxx(int)

2> 参数为int, 任何[实参](https://baike.baidu.com/item/%E5%AE%9E%E5%8F%82)均被提升成整型

3> 只能正确处理处于[0, 127]之间的值

**2****、字符映射函数**

1> 函数原型为int toxxxx(int)

2> 对参数进行检测, 若符合范围则转换, 否则不变

int tolower(int); 'A'~'Z' ==> 'a'~'z'

int [toupper](https://baike.baidu.com/item/toupper)(int); 'a'~'z' ==> 'A'~'Z'

具体常用函数如下：

**1****、isalpha**

函数名称: isalpha

函数原型: int isalpha(char ch);

函数功能: 检查ch是否是字母.

函数返回: 是字母返回非0（在vs2015中为2） ,否则返回 0.

参数说明:

所属文件 <ctype.h>

```cpp
//1031test

#include<stdio.h>

#include<ctype.h>

int main()

{

    char ch1='*';

    char ch2='a';

    if(isalpha(ch1)!=0)

        printf("%c is the ASCII alphabet\n",ch1);

    else

        printf("%c is not the ASCII alphabet\n",ch1);

    if(isalpha(ch2)!=0)

        printf("%c is the ASCII alphabet\n",ch2);

    else

        printf("%c is not the ASCII alphabet\n",ch2);

    return 0;

}
```

**2****、iscntrl**

函数名称: [iscntrl](https://baike.baidu.com/item/iscntrl)

函数原型: int iscntrl(int ch);

函数功能: 检查ch是否[控制字符](https://baike.baidu.com/item/%E6%8E%A7%E5%88%B6%E5%AD%97%E7%AC%A6)(其ASCII码在0和0x1F之间,数值为 0-31).

函数返回: 是返回非0,否则返回 0.

参数说明:

所属文件: <ctype.h>

```cpp
#include<stdio.h>

#include<ctype.h>

char chars[]={'A',0x09,'Z'};

#define SIZE sizeof(chars)/sizeof(char)

int main()

{

    int i;

    for(i=0;i<SIZE;i++)

    {

        printf("Char %c is%s a Control character\n",

        chars[i],(iscntrl(chars[i]))?" ":" not");

    }

    return 0;

}
```

**3****、isdigit**

函数名称: isdigit

函数原型: int isdigit(char ch);

函数功能: 检查ch是否是数字(0-9)

函数返回: 是返回非0,否则返回0

参数说明:

所属文件: <ctype.h>

```cpp
#include<stdio.h>

#include<ctype.h>

int main()

{

        char ch1='1';

        char ch2='a';

        printf("%c %s the ASCII number\n",ch1,isdigit(ch1)?"is":"is not");

        printf("%c %s the ASCII number\n",ch2,isdigit(ch2)?"is":"is not");

//      if(isdigit(ch1))

//             printf("%c is the ASCII number\n",ch1);

//      else

//             printf("%c is not the ASCII number\n",ch1);

//      if(isdigit(ch2))

//              printf("%c is the ASCII number\n",ch2);

//      else

//             printf("%c is not the ASCII number\n",ch2);

        return 0;

}
```

上面的代码显示，**使用条件表达式，可以很大程度上简化代码。**从下面的"if-else"的8行代码简化到只有2行，效果很好。另外说明，条件表达式本质上就是类似于“if-else”的选择结构。

**4****、isgraph**

函数名称: isgraph

函数原型: int isgraph(int ch);

函数功能: 检查ch是否可显示[字符](https://baike.baidu.com/item/%E5%AD%97%E7%AC%A6)(其ASCII码在0x21到0x7E之间),不包括空格

函数返回: 是返回非0,否则返回0

参数说明:

所属文件: <ctype.h>

```cpp
#include<stdio.h>

#include<ctype.h>

int main()

{

char ch1=' ';

char ch2='a';

if(isgraph(ch1)!=0)

printf("%cistheASCIIprintablecharacter\n",ch1);

else

printf("%cisnottheASCIIprintablecharacter\n",ch1);

if(isgraph(ch2)!=0)

printf("%cistheASCIIprintablecharacter\n",ch2);

else

printf("%cisnottheASCIIprintablecharacter\n",ch2);

return 0;

}
```

**5****、islower**

函数名称: islower

函数原型: int islower(int ch);

函数功能: 检查ch是否小写字母(a-z)

函数返回: 是返回非0,否则返回0

参数说明:

所属文件: <ctype.h>

**6****、isupper**

函数名称: [isupper](https://baike.baidu.com/item/isupper)

函数原型: int isupper(int ch);

函数功能: 检查ch是否是大写字母(A-Z)

函数返回: 是返回非0,否则返回0

参数说明:

所属文件: <ctype.h>

**7****、tolower**

函数名称: tolower

函数原型: int tolower(int ch);

函数功能: 将ch[字符](https://baike.baidu.com/item/%E5%AD%97%E7%AC%A6)转换为小写字母

函数返回: 返回ch所代表的字符的小写字母

参数说明:

所属文件: <ctype.h>

**8****、toupper**

函数名称: toupper

函数原型: int toupper(int ch);

函数功能: 将ch[字符](https://baike.baidu.com/item/%E5%AD%97%E7%AC%A6)转换成大写字母

函数返回: 与ch相应的大写字母

参数说明:

所属文件: <ctype.h>

**9****、isalnum**

函数名称: isalnum

函数原型: int isalnum(int ch);

函数功能: 检查ch是否是字母或数字

函数返回: 是字母或数字返回非0,否则返回0

参数说明:

所属文件: <ctype.h>

**10****、isprint**

函数名称: isprint

函数原型: int isprint(int ch);

函数功能: 检查ch是否是可打印[字符](https://baike.baidu.com/item/%E5%AD%97%E7%AC%A6)(包括空格),其ASCII码在0x20到0x7E之间

函数返回: 是返回非0,否则返回0

参数说明:

所属文件: <ctype.h>

**11****、ispunct**

函数名称: ispunct

函数原型: int [ispunct](https://baike.baidu.com/item/ispunct)(int ch);

函数功能: 检查ch是否是标点[字符](https://baike.baidu.com/item/%E5%AD%97%E7%AC%A6)(不包括空格),即除字母,数字和空格以外的所有可打印字符

函数返回: 是返回非0,否则返回0

参数说明:

所属文件: <ctype.h>



