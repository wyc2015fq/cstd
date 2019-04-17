# 剑指offer 面试题：数值的整数次方 - 别说话写代码的博客 - CSDN博客





2019年03月03日 13:05:27[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：22标签：[数值的整数次方																[剑指offer																[某数字的平方根																[某数字的立方根](https://so.csdn.net/so/search/s.do?q=某数字的立方根&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)](https://so.csdn.net/so/search/s.do?q=某数字的平方根&t=blog)




题目：实现函数double power(double base,int exponent)，求base的exponent次方。不能使用库函数，同时不需要考虑大数问题。

思路：这个题目可以求某个数字的 平方根，立方根。思路就是先判断不合法输入，也就是0分之1，base=0且exponent<0的情况。然后判断exponent是否<0，如果小于0其实可以先算-exponent的 情况（也就是正数），然后统一计算base的正数exponent的次方，其实就是将exponent个base  乘起来。然后返回（若exponent<0，就返回1除以返回值）

```cpp
class Solution {
public:
    double Power(double base, int exponent) {
        if(base==0 && exponent<0) return 0;
        int absexp = exponent;
        if(exponent<0) absexp=-absexp;
        double res = cal(base,absexp);
        if(exponent<0) return 1.0/res;
        return res;
    }
    double cal(double base,int exp)   //用位运算来操作，o(logn)复杂度
    {
        if(exp==0) return 1;
        if(exp==1) return base;
        double res=cal(base,exp>>1);   //每次计算除以2的，然后将他俩相乘
        res*=res;
        if(exp&1) res*=base;   //如果exponent是奇数，那就多乘以个base就行
        return res;
    }
    double cal0(double base,int exp)   //一步一步乘
    {
        double res=1.0;
        for(int i=1;i<=exp;++i) res*=base;
        return res;
    }
};
```](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)](https://so.csdn.net/so/search/s.do?q=数值的整数次方&t=blog)




