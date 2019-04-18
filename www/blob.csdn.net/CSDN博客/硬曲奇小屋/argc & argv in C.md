# argc & argv in C - 硬曲奇小屋 - CSDN博客





2017年09月28日 22:53:59[HardCookies](https://me.csdn.net/james_616)阅读数：819








虽说输入／输出不是C语言本身的组成部分，但是它是程序与数据间交互中不可缺少的一部分。下边我们编写一个访问文件的程序cat，它把一批命名文件串联后输出到标准输出（stdout）上。例如，下列命令行： 
`cat x.c y.c`

将在标准输出上打印文件x.c和y.c的内容。 

读写一个文件前，需要通过库函数fopen打开该文件，并返回用于文件读写操作的文件指针。因为<stdio.h>中已经定义了结构FILE，在程序中只需按照下列方式声明一个文件指针即可：
```
FILE *fp;
FILE *fopen(char *name, char *mode);
```

有了这些预备知识后，我们就可以编写cat这个程序了。如果有命令行参数，参数将被解释为文件名，并按顺序逐个处理。如果没有参数，则处理标准输入。

```
#include <stdio.h>

int main(int argc, char const *argv[])
{
    FILE *fp;
    void filecopy(FILE *, FILE *);

    if (argc == 1)
        filecopy(stdin, stdout);
    else
        while (--argc > 0)
            if ((fp = fopen(*++argv, "r")) == NULL)
            {
                printf("cat: can't open %s\n", *argv);
                return 1;
            }
            else
            {
                filecopy(fp, stdout);
                fclose(fp);
            }
    return 0;
}

void filecopy(FILE *ifp, FILE *ofp)
{
    int c;

    while ((c = getc(ifp)) != EOF)
        putc(c, ofp);
}
```

一直以来，在写完main函数之后都会带有参数argc和argv，之前虽有了解但是对于如何使用还是不知道。在这个程序中，才真正搞明白其具体的含义。arg表示的是参数（argument），argc和argv分别代表argument counter和argument vector。argc是整型类，用来统计参数个数。argv是字符类型指针数组，指向字符串参数。 

很多程序是带有参数的，argc是指参数的个数，而argv[]就是指向这些参数的地址，其中argv[0]指向编译后的可执行文件本身，argv[1]、argv[2]…则指向程序之后带的参数。 

比如，我们编写的这个程序，先编译`gcc -o cat cat.c`
直接运行cat，参数只有1个即cat本身，所以执行从标准输入到标准输出，即将输入的内容输出。 

现在我们再编辑两个文件cat1.c和cat2.c里面的内容分别为cat1和cat2，运行cat并带有两个参数
`cat cat1.c cat2.c`
则输出结果就是两个文件串联在一起的内容：cat1cat2 

这里文件名cat1.c和cat2.c分别作为参数argv[1]和argv[2]输入到我们的程序中。





