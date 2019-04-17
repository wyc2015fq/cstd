# 【剑指offer】题9：矩阵覆盖 - xiaxzhou的博客 - CSDN博客





2017年06月16日 14:34:01[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：120








> 
我们可以用2*1的小矩形横着或者竖着去覆盖更大的矩形。请问用n个2*1的小矩形无重叠地覆盖一个2*n的大矩形，总共有多少种方法？


使用$F(n)$表示n列的覆盖方法数



$F(n)=F(n-1)+F(n-2)$

```
int rectCover(int number) {
    if (number < 1)
        return 0;
    if (number == 1)
        return 1;
    if (number == 2)
        return 2;
    long long f_1 = 2, f_2 = 1;
    long long  f;
    for (auto i = 3; i <= number; ++i)
    {
        f = f_1 + f_2;
        f_2 = f_1;
        f_1 = f;
    }
    return f;
}
```



