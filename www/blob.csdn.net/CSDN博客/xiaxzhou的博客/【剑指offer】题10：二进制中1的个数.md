# 【剑指offer】题10：二进制中1的个数 - xiaxzhou的博客 - CSDN博客





2017年06月16日 15:24:29[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：107








> 
输入一个整数，输出该数二进制表示中1的个数。其中负数用补码表示


常规解法：

```cpp
int  NumberOf1(int n) 
{
    int m = sizeof(n);
    m *= 8;
    int k(0);
    for (auto i = 0; i < m;++i)
    {
        if ((n>>i)&1)
        {
            k++;
        }
    }
    return k;
}
```

利用**n&（n-1）**可去除n最右边的1：

```
int  NumberOf1(int n) 
{
    int count(0);
    while (n)
    {
        count++;
        n = n&(n - 1);
    }
    return count;
}
```



