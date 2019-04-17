# 剑指Offer - 斐波那契数列 - curryche的博客 - CSDN博客





2018年08月01日 15:58:11[curryche](https://me.csdn.net/whwan11)阅读数：30








## 题目

斐波那契数列 

时间限制：1秒 空间限制：32768K 热度指数：354415

题目描述 

大家都知道斐波那契数列，现在要求输入一个整数n，请你输出斐波那契数列的第n项（从0开始，第0项为0）。 

n<=39
## 解法

代码

```
class Solution {
public:
    int Fibonacci(int n) {
        if(n==0)
            return 0;
        if(n==1)
            return 1;

        int fn_1,fn_2,f_n,i;
        fn_1=1;
        fn_2=0;
        i=2;

        while(i<=n)
        {
            f_n=fn_1+fn_2;
            fn_2=fn_1;
            fn_1=f_n;  
            i++;
        }
        return f_n;
    }
};
```




