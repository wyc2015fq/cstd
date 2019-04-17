# 剑指Offer - 矩形覆盖 - curryche的博客 - CSDN博客





2018年08月01日 17:05:13[curryche](https://me.csdn.net/whwan11)阅读数：30








## 题目

矩形覆盖 

时间限制：1秒 空间限制：32768K 热度指数：186231

题目描述 

我们可以用2*1的小矩形横着或者竖着去覆盖更大的矩形。请问用n个2*1的小矩形无重叠地覆盖一个2*n的大矩形，总共有多少种方法？

## 解法

代码

```
class Solution {
public:
    int rectCover(int number) {
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



