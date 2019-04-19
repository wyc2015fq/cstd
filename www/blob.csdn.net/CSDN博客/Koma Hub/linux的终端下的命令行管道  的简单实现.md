# linux的终端下的命令行管道"|"的简单实现 - Koma Hub - CSDN博客
2018年12月25日 22:19:50[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：63
个人分类：[C/C++																[IPC																[Linux](https://blog.csdn.net/Rong_Toa/article/category/7156254)](https://blog.csdn.net/Rong_Toa/article/category/7969461)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
两个文件：a.c和b.c
a.c内容：
```cpp
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
        if(argc < 2)
        {
                fprintf(stderr, "Usage: %s [string]\n", argv[0]);
                exit(0);
        }
        fprintf(stdout, "%s\n", argv[1]);
}
```
b.c内容如下：
```cpp
#include <stdio.h>
int main()
{
        char str[256];
        fscanf(stdin, "%s", str);
        printf("%s:%s\n", __func__, str);
}
```
分别编译生成a.exe 和 b.exe
然后简单添加一个环境变量：
`$ export PATH=$PATH:./`
最后就可以试试了：
```
$ a.exe | b.exe
Usage: a [string]
main:▒
$ a.exe  rongtao | b.exe
main:rongtao
```
