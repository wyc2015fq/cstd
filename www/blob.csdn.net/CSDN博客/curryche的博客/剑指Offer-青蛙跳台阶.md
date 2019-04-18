# 剑指Offer - 青蛙跳台阶 - curryche的博客 - CSDN博客





2018年08月01日 16:33:06[curryche](https://me.csdn.net/whwan11)阅读数：25








## 题目

青蛙跳台阶 

时间限制：1秒 空间限制：32768K 热度指数：268747

题目描述 

一只青蛙一次可以跳上1级台阶，也可以跳上2级。求该青蛙跳上一个n级的台阶总共有多少种跳法（先后次序不同算不同的结果）。

## 解法

代码

```
class Solution {
public:
    int jumpFloor(int number) {
        if(number==0)
            return 0;
        if(number==1)
            return 1;
        if(number==2)
            return 2;
        int fn_1,fn_2,f_n,i;
        fn_1=2;
        fn_2=1;
        i=3;

        while(i<=number)
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



