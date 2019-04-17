# 【剑指offer】题11：数值的整数次方 - xiaxzhou的博客 - CSDN博客





2017年06月16日 16:37:51[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：106








> 
给定一个double类型的浮点数base和int类型的整数exponent。求base的exponent次方


**测试**：

0、0  

0、-1 

2.3 、0 

2.3 、-2 

2.3 、11
```
bool equal(double lhs, double rhs)
{
    if (abs(lhs-rhs)<0.00001)
    {
        return true;
    }
    else
    {
        return false;
    }
}

double Power_core(double base, int exponent)
{
    if (exponent == 0)
    {
        return 1;
    }
    double result = Power_core(base, exponent >> 1);
    result *= result;
    if (exponent&1)
    {
        result *= base;
    }
    return result;
}

double Power(double base, int exponent)
{
    if (equal(base,0)&&exponent<=0)
    {
        throw new exception("error");
    }
    if (exponent<0)
    {
        return 1.0 / Power_core(base, -exponent);
    }
    else
    {
        return Power_core(base, exponent);
    }
}
```




