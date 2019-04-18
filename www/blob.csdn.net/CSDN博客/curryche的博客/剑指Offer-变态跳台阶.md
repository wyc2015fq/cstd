# 剑指Offer - 变态跳台阶 - curryche的博客 - CSDN博客





2018年08月01日 16:50:40[curryche](https://me.csdn.net/whwan11)阅读数：32








## 题目

变态跳台阶 

时间限制：1秒 空间限制：32768K 热度指数：209737

题目描述 

一只青蛙一次可以跳上1级台阶，也可以跳上2级……它也可以跳上n级。求该青蛙跳上一个n级的台阶总共有多少种跳法。

## 解法

代码

```
class Solution {
public:
    int jumpFloorII(int number) {
        if(number==0)
            return 0;

        int result=1;
        for(int i=1;i<number;i++)
        {
            result*=2;
        }
        return result;
    }
};
```



