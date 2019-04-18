# C 语言结构体的对齐原则 - 文章 - 伯乐在线
原文出处： [阿凡卢](http://www.cnblogs.com/luxiaoxun/archive/2012/11/09/2762438.html)
Q：关于结构体的对齐，到底遵循什么原则？
A：首先先不讨论结构体按多少字节对齐，先看看只以1字节对齐的情况：

```
#include <stdio.h>
#include <string.h>
#define PRINT_D(intValue)     printf(#intValue" is %d\n", (intValue));
#define OFFSET(struct,member)  ((char *)&((struct *)0)->member - (char *)0)
#pragma pack(1)
typedef struct
{
    char    sex;
    short   score;
    int     age;
}student;
int main()
{
    PRINT_D(sizeof(student))
    PRINT_D(OFFSET(student,sex))
    PRINT_D(OFFSET(student,score))
    PRINT_D(OFFSET(student,age))
    return 0;
}
```
输出：
sizeof(student) is 7
OFFSET(student,sex) is 0
OFFSET(student,score) is 1
OFFSET(student,age) is 3
可以看到，如果按1字节对齐，那么结构体内部的成员紧密排列，sizeof(char) == 1, sizeof(short) == 2, sizeof(int) == 4.
修改上面的代码， 去掉#pragma pack语句，代码如下：

```
#include <stdio.h>
#include <string.h>
#define PRINT_D(intValue)     printf(#intValue" is %d\n", (intValue));
#define OFFSET(struct,member)  ((char *)&((struct *)0)->member - (char *)0)
typedef struct
{
    char    sex;
    short   score;
    int     age;
}student;
int main()
{
    PRINT_D(sizeof(student))
    PRINT_D(OFFSET(student,sex))
    PRINT_D(OFFSET(student,score))
    PRINT_D(OFFSET(student,age))
    return 0;
}
```
运行结果：
sizeof(student) is 8
OFFSET(student,sex) is 0
OFFSET(student,score) is 2
OFFSET(student,age) is 4
此时，各个成员之间就不像之前那样紧密排列了，而是有一些缝隙。这里需要介绍下对齐原则：
此原则是在没有#pragma pack语句作用时的原则（不同平台可能会有不同）：
原则A：struct或者union的成员，第一个成员在偏移0的位置，之后的每个成员的起始位置必须是当前成员大小的整数倍；
原则B：如果结构体A含有结构体成员B，那么B的起始位置必须是B中最大元素大小整数倍地址；
原则C：结构体的总大小，必须是内部最大成员的整数倍；
依据上面3个原则，我们来具体分析下： sex在偏移0处，占1字节；score是short类型，占2字节，score必须以2的整数倍为起始位置，所以它的起始位置为2； age为int类型，大小为4字节，它必须以4的整数倍为起始位置，因为前面有sex占1字节，填充的1字节和score占2字节，地址4已经是4的整数倍，所以age的位置为4.最后，总大小为4的倍数，不用继续填充。
继续修改上面的代码，增加#pragma pack语句：

```
#include <stdio.h>
#include <string.h>
#define PRINT_D(intValue)     printf(#intValue" is %d\n", (intValue));
#define OFFSET(struct, member)  ((char *)&((struct *)0)->member - (char *)0)
#pragma pack(4)
typedef struct
{
    char    sex;
    short   score;
    int     age;
}student;
int main()
{
    PRINT_D(sizeof(student))
    PRINT_D(OFFSET(student,sex))
    PRINT_D(OFFSET(student,score))
    PRINT_D(OFFSET(student,age))
    return 0;
}
```
运行结果：
sizeof(student) is 8
OFFSET(student,sex) is 0
OFFSET(student,score) is 2
OFFSET(student,age) is 4
具体分析下：
有了#pragma pack(4)语句后，之前说的原则A和C就不适用了。实际对齐原则是自身对齐值(成员sizeof大小)和指定对齐值(#pragma pack指定的对齐大小)的较小者。依次原则，sex依然偏移为0， 自身对齐值为1，指定对齐值为4，所以实际对齐为1； score成员自身对齐值为2，指定对齐值为4，实际对齐为2；所以前面的sex后面将填充一个1字节，然后是score的位置，它的偏移为2；age自身对齐值为4，指定对齐为4，所以实际对齐值为4；前面的sex和score正好占用4字节，所以age接着存放；它的偏移为4.
Q：关于位域的问题，空域到底表示什么？
A：它表示之后的位域从新空间开始。

```
#include <stdio.h>
#include <string.h>
#define PRINT_D(intValue)     printf(#intValue" is %d\n", (intValue));
#define OFFSET(struct, member)  ((char *)&((struct *)0)->member - (char *)0)
typedef struct 
{
    int a : 1;
    int b : 3;
    int : 0;
    int d : 2;
}bit_info;
int main()
{
    PRINT_D(sizeof(bit_info))
    return 0;
}
```
运行结果：
sizeof(bit_info) is 8
bit_info中的a, b占用4个字节的前4位，到int:0; 时表示此时将填充余下所有没有填充的位，即刚刚的4个字节的余下28位；int d:2; 将从第四个字节开始填充，又会占用4个字节，所以总大小为8.
