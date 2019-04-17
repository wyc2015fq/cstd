# [CareerCup] 12.1 Find Mistakes 找程序错误 - Grandyang - 博客园







# [[CareerCup] 12.1 Find Mistakes 找程序错误](https://www.cnblogs.com/grandyang/p/4903350.html)







12.1 Find the mistake(s) in the following code:

```
unsigned int i;
for (i = 190; i >= 0; --i)
printf("%d\n", i);
```



这道题让我们找出给定的简短的程序的错误，这里共有两处错误：

1. 无符号整型永远大于0，所以i >= 0没有意义

2. 打印无符号整型要用%u

正确代码参见下方：



```
unsigned int i;
for (i = 190; i > 0; --i)
printf("%u\n", i);
```














