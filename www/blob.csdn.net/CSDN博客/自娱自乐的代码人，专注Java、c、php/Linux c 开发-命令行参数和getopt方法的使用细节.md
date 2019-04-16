# Linux c 开发 - 命令行参数和getopt方法的使用细节 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年03月10日 21:52:47[initphp](https://me.csdn.net/initphp)阅读数：2938
所属专栏：[Linux c 开发系列](https://blog.csdn.net/column/details/mylinuxc.html)









### 命令行参数

在c语言的入口函数main方法中，可以添加两个参数：**argc**和**argv**。通过这两个参数，我们就可以在可执行文件执行的时候将自定义的参数传递到程序中。

看一个例子：



```cpp
#include <stdio.h>
#include <unistd.h>

int main (int argc, char *argv[])
{

    printf("argc:%d\r\n", argc);
    printf("argv:%s argv1:%s argv2:%s argv3:%s\r\n\r\n", argv[0], argv[1], argv[2], argv[3]);
   return 0;
}
```
**argc：**这个参数主要告诉程序有多少个参数，每个参数在命令行上是通过空格分隔开的。

**argv：**用于存储命令行参数的字符指针数组。

执行结果：



```
[admin@localhost ~]$ ./main -p1024 -n10 -c
argc:4
argv:./main argv1:-p1024 argv2:-n10 argv3:-c
```




### getopt方法

头文件：



```cpp
#include<unistd.h>
```
函数定义：



```cpp
int getopt(int argc,char * const argv[ ],const char * optstring);
```

使用了getopt方法后，命令行参数就可以像这样使用短参数模式：**./main -p1024 -n10 -c**

**optstring参数定义规则：**

1. 如果使用**-p1024**或者**-p 1024**这样的短参数方式，则在optstring中需要定义：**p:**。p为短参数的名称。**单个冒号**为-p后面会带上参数，参数可以是有空格分隔，也可以不分隔。

2. 如果使用**-n10**这样的短参数方式，其中n和10之间没有空格，则optstring中需要带**双冒号：n::**

3. 如果使用**-c**这样的短参数方式，并且没有参数，则optstring中直接写这个字母就行了：**c**

**变量：**

**char *optarg**：参数值

**int optind**：argv的索引值，第几个参数。

具体看详细列子：



```cpp
#include <stdio.h>
#include <unistd.h>

int main (int argc, char *argv[])
{

    printf("argc:%d\r\n", argc);
    printf("argv:%s argv1:%s argv2:%s argv3:%s\r\n\r\n", argv[0], argv[1], argv[2], argv[3]);

    int oc;
    char ec;
    //通过while循环获取
    while((oc = getopt(argc, argv, "p:n::c")) != -1)
    {
        printf("optind:%d\r\n",optind); //参数索引值
        printf("optarg:%s\r\n",optarg); //参数值
        switch(oc)
        {
            case 'p': //参数p   optarg为参数值，-p 1024或者-p1024使用
                printf("Port:%s\n\n", optarg);
                break;
            case 'n': //参数n  需要-n10 这样使用
                printf("Thread Num:%s\n\n", optarg);
                break;
            case 'c': //参数c，需要-c使用，不用带参数
                printf("This is c\n\n");
                break;
            case '?': //如果设置了未定义的参数，则返回这边的错误信息
                ec = (char)optopt;
                printf("未定义的参数:%c\r\n", ec);
                break;

            case ':': //缺少选项
                printf("缺少选项参数\r\n");
                break;
        }
    }
   return 0;
}
```
运行命令行：



```cpp
[admin@localhost ~]$ ./main -p1024 -n10 -c  -a
argc:5
argv:./main argv1:-p1024 argv2:-n10 argv3:-c

optind:2
optarg:1024
Port:1024

optind:3
optarg:10
Thread Num:10

optind:4
optarg:(null)
This is c

./main: invalid option -- 'a'
optind:5
optarg:(null)
未定义的参数:a
```






