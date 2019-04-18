# 字符数组的处理—C语言 - 硬曲奇小屋 - CSDN博客





2017年09月27日 12:03:58[HardCookies](https://me.csdn.net/james_616)阅读数：104








继续学习Kernighan和Ritchie的The C programming Language。在1.9 字符数组中，通过下面的一个程序，来说明字符数组以及操作字符数组的函数的用法。该程序读入一组文本行，并把最长的文本行打印出来。 

首先，编写一个独立的函数getline，它读取输入的下一行，能够返回该行的长度，而在遇见文件结束符时返回0。当发现某个读入的行比以前读入的最长行还要长时，就把该行保存起来，所以需要另外一个copy函数。最后，我们需要在主函数main中控制getline和copy这两个函数。以下是编写的程序：

```
#include <stdio.h>
#define MAXLINE 50              /* 允许的输入行的最大长度 */

int getlinea(char line[], int maxline);
void copy(char to[], char from[]);

/* 打印最长的输入行 */
int main(int argc, char const *argv[])
{
    int len;
    int max;
    char line[MAXLINE];
    char longest[MAXLINE];

    max = 0;
    while ((len = getlinea(line, MAXLINE)) > 0)
        if (len > max)
        {
            max = len;
            copy(longest, line);
        }
    if (max > 0)
        printf("%s\n", longest);
    return 0;
}

/* getline函数：将一行读入到s中并返回其长度 */
int getlinea(char s[], int lim)
{
    int c, i;

    for (i = 0; i < lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
        s[i] = c;
    if (c == '\n')
    {
        s[i] = c;
        ++i;
    }
    s[i] = '\0';
    return i;
}
// 前面这一函数写错了，for循环里面也有一个int i，相当于每次都初始化，变量传不下来。

/* copy函数：将from复制到to；这里假定to足够大 */
void copy(char to[], char from[])
{
    int i;

    i = 0;
    while ((to[i] = from[i]) != '\0')
        ++i;
}
```

昨天写完这个程序始终是在输入端，键入EOF也不管用。今天又仔细看了一看，始终在输入肯定是因为判断条件`(len = getlinea(line, MAXLINE)) > 0`而导致的。问题所在就因该是getlinea函数存在问题，最后发现原因是因为在getlinea内定义了int i，而在for循环里又重新定义了 i。 
`for (int i = 0; i < lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)`

现在我们可以写一个程序来验证一下这个结果：
```
#include <stdio.h>

/* 测试一下关于刚刚出现的错误 */
int main(int argc, char const *argv[])
{
    int i;
    for (int i = 5; i < 10; ++i)
    {
        printf("%d\n", i);
    }
    printf("The last i is: %d\n", i);
    return 0;
}
```

输出的结果如下：

> 
5 

  6 

  7 

  8 

  9 

  The last i is: 0
从上面的结果可以看到，i是定义在for循环外，虽然i是作为for的循环变量但是结束循环后i的结果仍然是初始化的值0。 

这也就是我之前那个程序始终在循环里面的问题了！






