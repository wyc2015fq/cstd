# 剑指Offer - 数值的整数次方 - curryche的博客 - CSDN博客





2018年08月10日 15:24:50[curryche](https://me.csdn.net/whwan11)阅读数：24








## 题目

数值的整数次方 

时间限制：1秒 空间限制：32768K 热度指数：264066 

题目描述 

给定一个double类型的浮点数base和int类型的整数exponent。求base的exponent次方。
## 解法

代码

```
class Solution {
public:
    double Power(double base, int exponent) {
    if (base == 0.0)
        return 0.0;

    if (exponent == 0)
        return 1.0;
    if (exponent == 1)
        return base;

    bool judge_minus = false;
    if (exponent < 0)
    {
        judge_minus = true;
        exponent = -exponent;

    }

    double result = Power(base, exponent >> 1);
    result *= result;

    if (exponent & 1)
        result *= base;

    if (judge_minus)
        result = 1 / result;

    return result;

}
};
```




