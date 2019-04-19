# C语言串 typedef  char  String[MaxLength + 1] - Koma Hub - CSDN博客
2017年12月05日 22:07:39[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：159
个人分类：[C/C++																[Algorithm](https://blog.csdn.net/Rong_Toa/article/category/7221428)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef char String[250];/*  0索引号单元存放串的长度 */
void setString(String str, char *chs)
{
    str[0] = strlen(chs);
    int i;
    for(i=1;i<=str[0];i++)
        str[i] = *(chs+i-1);
}
void copyString(String in, String out)
{
    int i;
    for(i=0;i<=in[0];i++)
        out[i] = in[i];
}
/*  字符串比较 */
int cmpString(String a, String b)
{
    int i;
    for(i=0;i<=a[0]&&i<=b[0];i++)
    {
        if(a[i] != b[i])
            return 0;
    }
    return i-1;
}
/* 用T返回S1和S2联接而成的新串*/
void catString(String a, String b, String c)
{
    c[0] = a[0]+b[0];
    int i;
    for(i = 1;i<=a[0];i++)
        c[i] = a[i];
    for(i = 1;i<=c[0];i++)
        c[i+a[0]] = b[i];
}
void showString(String a)
{
    printf("show:");
    int i;
    for (i=1;i<=a[0];i++)
        printf("%c",a[i]);
    printf("\n");
}   
/* 用Sub返回串S的第pos个字符起长度为len的子串。 */
void subString(String a, String b, int index, int length)
{
    if(index<1||index>a[0]||length<=0||length>index+a[0]){
        
        b[0] = 0;
        return;
    }else{
    
        b[0] = length;
        int i;
        for(i=1;i<=length;i++)
        {
            b[i] = a[index+i];
        }
    }
}
int main(int argc, char*argv[])
{
    String a;
    setString(a,"rong");
    showString(a);
    String b;
    setString(b,"tao12321v");
    showString(b);
    String c;
    catString(a,b,c);
    showString(c);
    String d;
    subString(c,d,3,3);
    showString(d);
	return 0;
}
```
结果：
```
-bash-4.1$ gcc -o a Demo.c
-bash-4.1$ ./a
show:rong
show:tao12321v
show:rongtao12321v
show:gta
-bash-4.1$
```
