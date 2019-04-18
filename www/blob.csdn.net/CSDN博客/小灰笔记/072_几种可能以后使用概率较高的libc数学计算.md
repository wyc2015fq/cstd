# 072_几种可能以后使用概率较高的libc数学计算 - 小灰笔记 - CSDN博客





2019年02月23日 23:12:19[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：30








1.e的幂函数

代码

```cpp
void exp_test(void)
{
    printf("%f\n",exp(1.0));
    printf("%f\n",exp(1.5));
}
```

运行结果：

![](https://img-blog.csdnimg.cn/20190223230956416.png)

通过运行结果可以看出这个计算的是e的幂。

2.把一个浮点数分成整数和小数部分

代码

```cpp
void modf_test(void)
{
    double a = 1.2334;
    double int_value;

    printf("%f\n",modf(a,&int_value));
    printf("%f\n",int_value);
    printf("%d\n",(int)int_value);
}
```

运行结果：

![](https://img-blog.csdnimg.cn/20190223230956409.png)

3.x的y次幂

```cpp
void pow_test(void)
{
    printf("%f\n",pow(2, 10));
    printf("%f\n",pow(2, 0.5));
}
```

运行结果：

![](https://img-blog.csdnimg.cn/20190223230956413.png)

4.开平方

似乎有x的y次幂，这个函数的用途就不是很大了。

代码：

```cpp
void sqrt_test(void)
{
    printf("%f\n",sqrt(2));
    printf("%f\n",pow(2,0.5));
    printf("%f\n",sqrt(3));
    printf("%f\n",pow(3,0.5));
}
```

运行结果：

![](https://img-blog.csdnimg.cn/20190223230956442.png)

5.ceil,floor

这是两个类似的函数，代码如下：

```cpp
void ceil_floor_test(void)
{
    printf("%d\n",ceil(4.5));
    printf("%d\n",floor(4.5));
    printf("%d\n",(int)ceil(4.5));
    printf("%d\n",(int)floor(4.5));
}
```

运行结果：

![](https://img-blog.csdnimg.cn/20190223230956448.png)

值得注意的是，这个返回值是浮点数，需要进行强制数据类型转换，否则打印错误。

6.余数求取

```cpp
void fmode_test(void)
{
    printf("%f\n",fmod(3.14,1));
    printf("%f\n",fmod(-3.14,1));
}
```

运行结果：

![](https://img-blog.csdnimg.cn/20190223230956458.png)

这个函数接下里或许我很快就有用了，后面之所以使用 1就是我自己想要的目的，直接取得一个浮点数的小数部分。从运行结果看，这个还是有一个正负符号的。



