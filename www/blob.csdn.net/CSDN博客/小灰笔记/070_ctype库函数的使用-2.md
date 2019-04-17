# 070_ctype库函数的使用-2 - 小灰笔记 - CSDN博客





2019年02月22日 23:12:01[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：21








1.判断一个字符是否是可以打印的字符判断：

```cpp
void isgraph_test(void)
{
    unsigned char s[] = "abcdef\t\n\rddd+^";
    int i = 0;

    while(s[i] != '\0')
    {
        if(isgraph(s[i]))
        {
            printf("%c OK\n",s[i]);
        }
        else
        {
            printf("%c NOT OK\n",s[i]);
        }
        i++;
    }

}
```

执行结果：

![](https://img-blog.csdnimg.cn/20190222230925903.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         很简单了，能够看得见的就是true，看不见的就是False。

2.大小写的判断，这两个其实只要记住一个即可，除非检测对象不是字母。以前感觉Python有这些功能很强大，看起来C也照样强大。

```cpp
void islower_test(void)
{
    unsigned char s[] = "ABCFxcdefSSK123x";
    int i = 0;

    while(s[i] != '\0')
    {
        if(islower(s[i]))
        {
            printf("%c is lower.\n",s[i]);
        }
        else
        {
            printf("%c is not lower.\n",s[i]);
        }
        i++;
    }
}
```

         执行结果：

![](https://img-blog.csdnimg.cn/20190222230925934.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)



3.是否是打印字符

```cpp
void isprint_test(void)
{
    unsigned char s[] = "ando423!@#$%^&*()\1\2\3\n\r\t";
    int i = 0;

    while(s[i] != '\0')
    {
        if(isprint(s[i]))
        {
            printf("%c is print.\n",s[i]);
        }
        else
        {
            printf("%c is not print.\n",s[i]);
        }
        i++;
    }
}
```

执行结果：

![](https://img-blog.csdnimg.cn/20190222230925932.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

4.是否可以处理为空白

```cpp
void isspace_test(void)
{
    unsigned char s[] = " \t\n\b\0";
    int i = 0;

    while(s[i] != '\0')
    {
        if(isspace(s[i]))
        {
            printf("%c is space.\n",s[i]);
        }
        else
        {
            printf("%c is not space.\n",s[i]);
        }
        i++;
    }
}
```

执行结果：

![](https://img-blog.csdnimg.cn/20190222230925935.png)

5.十六进制字符检查

```cpp
void isxdigit_test(void)
{
    unsigned char s[] = "1234567890abcdefABCDEFXSWF";
    int i = 0;

    while(s[i] != '\0')
    {
        if(isxdigit(s[i]))
        {
            printf("%c is x.\n",s[i]);
        }
        else
        {
            printf("%c is not x.\n",s[i]);
        }
        i++;
    }
}
```

执行结果：

![](https://img-blog.csdnimg.cn/20190222230925934.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

6.大小写的转换

```cpp
void convert_test(void)
{
    unsigned char s[] = "abcdeDDKKKSxxxFFF1123xxx";
    unsigned char s1[sizeof(s)];
    int i = 0;

    while(s[i] != '\0')
    {
        printf("raw: %s\n",s);
        while(s[i] != 0)
        {
            s1[i] = tolower(s[i]);
            i++;
        }
        i = 0;
        printf("lower: %s\n",s1);
        while(s[i] != 0)
        {
            s1[i] = toupper(s[i]);
            i++;
        }
        printf("upper isL %s\n",s1);
    }
}
```

![](https://img-blog.csdnimg.cn/20190222230925962.png)

         这个也让我想到了Python，看起来这个功能上Python是要简单一点。因为Python处理的是字符串，而C处理的是一个字符。



