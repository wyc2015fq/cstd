# 剑指offer 面试题65：不用加减乘除做加法 c++ - 别说话写代码的博客 - CSDN博客





2018年11月26日 19:54:11[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：15
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：写一个函数求 两个整数之和， 要求在函数体内 不得使用“+”，“-”，“*”，“/”四则远算符号。

思路：首先想到用位运算代替。分为3步，第一步让两个数字加起来不进位，可以使用异或运算，相同为0 相异 为1，第二步计算两个数字加起来的进位，可以用与操作然后左移 一位，因为进位只能由两个1加起来产生，所以使用与，然后左移。第三步 重复前面两步 直到不产生进位为止。

```cpp
class Solution {
public:
    int Add(int num1, int num2)
    {
        int sum,carry;
        do{
            sum = num1 ^ num2;
            carry = (num1 & num2) <<1;
            num1 = sum;
            num2 = carry;
        }while(num2 != 0);
        return num1;
    }
};
```





