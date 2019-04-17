# 剑指offer- 整数中1出现的次数（从1到n的整数中1出现的次数） - curryche的博客 - CSDN博客





2018年10月24日 20:41:51[curryche](https://me.csdn.net/whwan11)阅读数：37







- 题目

剑指offer- 整数中1出现的次数（从1到n的整数中1出现的次数）

时间限制：1秒 空间限制：32768K 热度指数：136952

算法知识视频讲解

题目描述

求出1-13的整数中1出现的次数,并算出100-1300的整数中1出现的次数？为此他特别数了一下1~13中包含1的数字有1、10、11、12、13因此共出现6次,但是对于后面问题他就没辙了。ACMer希望你们帮帮他,并把问题更加普遍化,可以很快的求出任意非负整数区间中1出现的次数（从1 到 n 中1出现的次数）。- 解法

代码

```
class Solution {
public:
    int NumberOf1Between1AndN_Solution(int n)
    {
        if(n==0)
            return 0;
        if(n<10)
            return 1;
        
        int result,len,first_sum,base;
        len=length_num(n);
        base=(int)pow(10.0,len*1.0-1);
        first_sum=(n/base==1?n%base+1:base);
        return first_sum+n/base*(len-1)*base/10+NumberOf1Between1AndN_Solution(n%base);
    }
    int length_num(int num)
    {
        int result=0;
        while(num!=0)
        {
            result++;
            num/=10;
        }
        return result;
    }
};
```




