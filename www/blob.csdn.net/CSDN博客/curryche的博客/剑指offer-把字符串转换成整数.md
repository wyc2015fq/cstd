# 剑指offer - 把字符串转换成整数 - curryche的博客 - CSDN博客





2018年09月23日 16:43:44[curryche](https://me.csdn.net/whwan11)阅读数：114







- 题目

把字符串转换成整数

时间限制：1秒 空间限制：32768K 热度指数：135656

本题知识点： 字符串

算法知识视频讲解

题目描述

将一个字符串转换成一个整数(实现Integer.valueOf(string)的功能，但是string不符合数字要求时返回0)，要求不能使用字符串转换整数的库函数。 数值为0或者字符串不是一个合法的数值则返回0。

输入描述:

输入一个字符串,包括数字字母符号,可以为空

输出描述:

如果是合法的数值表达则返回该数字，否则返回0

示例1

输入

复制

+2147483647

1a33

输出

复制

2147483647

0- 解法

代码

```
class Solution {
public:
    int StrToInt(string str) {
        if(!IsValid(str))
            return 0;
        int result,i;
        
        int p=INT_MIN/10;
        int r=INT_MIN%10;
        bool flag;

        if(str[0]=='-')
            flag=false;
        else
            flag=true;
        
        if(str[0]=='+'||str[0]=='-')
            i=1;
        else
            i=0;
        
        result=0;
        for(;i<str.size();i++)
        {
            if(result<p)
                return 0;
            if(result==p&&(str[i]-'0')<r)
                return 0;
            result=10*result-(str[i]-'0');
        }
        if(flag)
            return -result;
        else
            return result;
        
    }
    bool IsValid(string str)
    {
        if(str.size()==0)
            return false;
        
        if(str[0]=='0'&&str.size()>1)
            return false;
        
        if((str[0]=='+'||str[0]=='-')&&str.size()==1)
            return false;
        
        int i;
        if(str[0]=='+'||str[0]=='-')
            i=1;
        else
            i=0;
        
        for(;i<str.size();i++)
        {
            if(str[i]<'0'||str[i]>'9')
                return false;
        }
        return true;
    }
    
};
```




