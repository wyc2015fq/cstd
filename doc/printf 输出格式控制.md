# printf 输出格式控制

 

**printf()用%f输出double型，而scanf却用%lf**

| 格式       | **printf**   | **scanf**      |
| ---------- | ------------ | -------------- |
| %c         | int          | char *         |
| %d, %i     | int          | int *          |
| %o, %u, %x | unsigned int | unsigned int * |

 （续） 

| 格式          | **printf**        | **scanf**            |
| ------------- | ----------------- | -------------------- |
| %ld, %li      | long int          | long int *           |
| %lo, %lu, %lx | unsinged long int | unsigned long int *  |
| %hd, %hi      | int               | short int *          |
| %ho, %hu, %hx | unsigned int      | unsigned short int * |
| %e, %f, %g    | double            | float *              |
| %le, %lf, %lg | n/a               | double *             |
| %s            | char *            | char *               |
| %[...]        | n/a               | char *               |
| %p            | void              | void **              |
| %n            | int *             | int *                |
| %%            | none              | none                 |

 （严格地讲，%lf在printf下是未定义的，但是很多系统可能会接受它。要确保可移植性，就要坚持使用%f。） 

 

## 1．转换说明符

​       %a(%A)     浮点数、十六进制数字和p-(P-)记数法(C99)
​       %c             字符
​       %d             有符号十进制整数
​       %f              浮点数(包括float和doulbe)
​       %e(%E)     浮点数指数输出[e-(E-)记数法]
​       %g(%G)     浮点数不显无意义的零"0"
​       %i              有符号十进制整数(与%d相同)
​       %u             无符号十进制整数
​       %o             八进制整数    e.g.     0123
​       %x(%X)      十六进制整数0f(0F)   e.g.   0x1234
​       %p             指针
​       %s             字符串
​       %%            "%"

## 2．标志

​       左对齐："-"   e.g.   "%-20s"
​       右对齐："+"  e.g.   "%+20s"
​       空格：若符号为正，则显示空格，负则显示"-"   e.g.   "%  6.2f"      
​       \#：对c,s,d,u类无影响；对o类，在输出时加前缀o；对x类，在输出时加前缀0x；
​            对e,g,f 类当结果有小数时才给出小数点。

## 3．格式字符串（格式）

​       ［标志］［输出最少宽度］［．精度］［长度］类型
​      "％-md" ：左对齐，若m比实际少时，按实际输出。
​      "%m.ns"：输出m位，取字符串(左起)n位，左补空格，当n>m or m省略时m=n
​                       e.g.    "%7.2s"   输入CHINA
​                                  　           输出"     CH"
​      "%m.nf"：输出浮点数，m为宽度，n为小数点右边数位
​                       e.g.    "%3.1f"    输入3852.99
​                                                输出3853.0 
​       长度：为ｈ短整形量,ｌ为长整形量





## printf输出指定长度字符串

  

 编程时往往存在这种情况：

 函数接口传给你一个没有“\0”结尾的字符串str和他的长度str_len，调试的时候你需要将其打印出来。

 一般人都会这么做：

```c
void print_str(const char* str, unsigned int str_len)  
{  
    unsigned int i=0;  
    for (; i < str_len; i++)  
        printf("%c", str[i]);  
    printf("\n");  
}  
```



但是其实一行代码就可以搞定：

```
printf("%.*s\n", str_len, str);   
```

