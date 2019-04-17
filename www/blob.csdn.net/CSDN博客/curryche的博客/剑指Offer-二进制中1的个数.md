# 剑指Offer - 二进制中1的个数 - curryche的博客 - CSDN博客





2018年08月08日 20:38:47[curryche](https://me.csdn.net/whwan11)阅读数：38








## 题目

二进制中1的个数 

输入一个整数，输出该数二进制表示中1的个数。其中负数用补码表示。

## 解法

代码

```
class Solution {
public:
     int  NumberOf1(int n) {
         int count;
         count=0;
         while(n)
         {
             count++;
             n=n&(n-1);
         }
         return count;
     }
};
```



