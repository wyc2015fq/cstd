# 实现常用的配置文件/初始化文件读取的一个C程序 - 在思索中前行！ - CSDN博客





2014年11月16日 19:23:37[_Tham](https://me.csdn.net/txl16211)阅读数：852








      在编程中，我们经常会遇到一些配置文件或初始化文件。这些文件通常后缀名为.ini或者.conf，可以直接用记事本打开。里面会存储一些程序参数，在程序中直接读取使用。例如，计算机与服务器通信，服务器的ip地址，段口号可以存储于ini文件中。这样如果我想换另外一台服务器时，直接将ini文件中的ip地址改变即可，程序源代码不需要做任何修改。

本文将分享一段常用代码，用于读取配置文件中的信息。本文中的代码为C语言编写，在ubuntu 12.04 linux系统中调试没有问题。具体操作如下：

1. 首先用记事本创建一个config.ini文件（文件名可以随便取），并假设该文件是我们要读取的配置文件。文件内容如下：

```
information1: 1234567890
information2: this is test information
information3: `~!@#$%^&*()_+{}-[]\|:"/.,<>
```

假设我们读取的初始化文件每一行都是  <属性名称>: <属性值>  的格式。在上述例子中，文件共有三行，分别代表三个属性的信息。

2. 然后就是我们的代码文件了，如下（将以下代码存在ReadFile.cpp中）：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

#include <string.h>
#include <stdio.h>

const size_t MAX_LEN = 128;

typedef struct{
    char firstline[MAX_LEN];
    char secondline[MAX_LEN];
    char thirdline[MAX_LEN];
} Data;

void readfile(Data *d){
    const char *FileName = "config.ini";
    char LineBuf[MAX_LEN]={0};
    FILE *configFile = fopen(FileName, "r");
    memset(d,0,sizeof(Data));
    while(NULL != fgets(LineBuf, sizeof(LineBuf), configFile))
    {
        size_t bufLen = strlen(LineBuf);
        if('\r' == LineBuf[bufLen-1] || '\n' == LineBuf[bufLen-1])
        {
            LineBuf[bufLen-1] = '\0';
        }
        char *pos = strchr(LineBuf,':');
        if(NULL != pos)
        {
            *pos = '\0';
            pos++;
            if(0 == strcmp(LineBuf, "information1"))
            {
                for(; *pos == ' '; pos++){}
                
                strcpy(d->firstline, pos);
            }
            else if(0 == strcmp(LineBuf, "information2"))
            {
                for(; *pos == ' '; pos++){}
            
                strcpy(d->secondline, pos);
            }
            else if(0 == strcmp(LineBuf, "information3"))
            {
                for(; *pos == ' '; pos++){}

                strcpy(d->thirdline, pos);
            }
            else
            {
                printf("Failed to read information from the file.");
                break;
            }
        }
    }
    fclose(configFile);
    configFile = NULL;
    return;
}

int main(int argc, char *argv[])
{
    Data *d = new Data;
    readfile(d);
    printf("d->firstline is \"%s\"\n", d->firstline);
    printf("d->secondline is \"%s\"\n", d->secondline);
    printf("d->thirdline is \"%s\"\n", d->thirdline);
    delete d;
    return 0;
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

其中，struct Data是用于存储要读取的信息的结构体，readfile函数也就是实现我们读取功能的函数，其中的值均存在struct Data中。最后我们写了一个简单的main函数用来测试结果。需要注意的是，在struct Data中，我们设置了char数组长度，最大不超过128。因此如果要读取的信息超过128字节可能会出错。如果有需要读取更长的话可以将MAX_LEN设置为一个更大的值。

3. 最后就是我们的调试结果了，在命令行中运行如下命令

```
$ g++ -o test.out ReadFile.cpp
$ ./test.out
```

然后就是运行结果：

```
d->firstline is "1234567890"
d->secondline is "this is test information"
d->thirdline is "`!@#$%^&*()_+{}-[]\|:"/.,<>"
```

这种读取文件的代码应该非常常用，要掌握。



