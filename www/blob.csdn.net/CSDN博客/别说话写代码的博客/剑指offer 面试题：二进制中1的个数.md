# 剑指offer 面试题：二进制中1的个数 - 别说话写代码的博客 - CSDN博客





2019年03月09日 09:52:41[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：11标签：[剑指offer																[二进制中1的个数](https://so.csdn.net/so/search/s.do?q=二进制中1的个数&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)








题目：输入一个整数，输出该数二进制表示中1的个数。其中负数用补码表示。

思路：用位运算，n&(n-1)意思就是讲n的最后一个位置的1变为0

```cpp
class Solution {
public:
     int  NumberOf1(int n) {
         int count=0;
         while(n)
         {
             count++;
             n=n&(n-1);
         }return count;
     }
};
```






