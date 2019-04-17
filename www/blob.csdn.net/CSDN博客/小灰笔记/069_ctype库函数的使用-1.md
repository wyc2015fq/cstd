# 069_ctype库函数的使用-1 - 小灰笔记 - CSDN博客





2019年02月20日 23:03:15[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：30
个人分类：[C](https://blog.csdn.net/grey_csdn/article/category/6639462)








- 判断一个字符是否是十进制数组

```cpp
#include "stdio.h"
#include "ctype.h"

unsigned char s[] = "1234567890abc";

int main(void)
{
    int i = 0;

    while(s[i] != '\0')
    {
        if(isdigit(s[i]))
        {
            printf("%c is a num.\n",s[i]);
        }
        else
        {
            printf("%c is not a num.\n",s[i]);
        }

        i++;
    }

    printf("%d\n",sizeof(s));

    return 0;
}
```

执行结果：

![](https://img-blog.csdnimg.cn/20190220230020933.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)
- .是否是字母测试

```cpp
void isalpha_test(void)
{
    unsigned char s[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123!@'";
    int i = 0;

    while(s[i] != '\0')
    {
        if(isalpha(s[i]))
        {
            printf("%c is a alpha.\n",s[i]);
        }
        else
        {
            printf("%c is a not alpha.\n",s[i]);
        }
        i++;
    }
    
}
```

测试结果：

![](https://img-blog.csdnimg.cn/201902202300210.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2019022023002191.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)
- 数字以及字母检测

```cpp
void isalnum_test(void)
{
    unsigned char s[] = "abcDFG1230x88_+-";
    int i = 0;

    while(s[i] != '\0')
    {
        if(isalnum(s[i]))
        {
            printf("%c is alnum.\n",s[i]);
        }
        else
        {
            printf("%c is not alnum.\n",s[i]);
        }
        i++;
    }
}
```

运行结果：

![](https://img-blog.csdnimg.cn/2019022023002191.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         时间有点晚了，今天总结到此。



