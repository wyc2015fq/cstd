# 剑指offer 面试题64：求1+2+3+...+n  c++ - 别说话写代码的博客 - CSDN博客





2018年11月27日 13:23:23[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：62
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：求1+2+3+...+n，要求不能使用乘除法、for、while、if、else、switch、case等关键字及条件判断语句（A?B:C）。

解析：多种方法（1）利用构造函数求解  （2）利用虚函数求解   （3）利用函数指针求解  （4）利用模板类型求解

这里可以通过递归的方式进行计算，但是很疑问的地方在于如何结束递归。这里可以有多种的方式，基本方向是采用逻辑与或的方式来计算，与的时候通过n>0来短路，这样在n=0的时候不需要计算递归的值，或的时候通过n==0来短路，在n=0的时候可以短路逻辑或运算。

```cpp
class Solution {
public:
    int Sum_Solution(int n) {
        int sum = n;
        bool b = (n==0) || (sum+=Sum_Solution(n-1)) > 0;
        return sum;
    }
};
```





