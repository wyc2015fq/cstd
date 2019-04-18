# 剑指offer - 表示数值的字符串 - curryche的博客 - CSDN博客





2018年08月25日 20:41:15[curryche](https://me.csdn.net/whwan11)阅读数：55








## 题目

表示数值的字符串 

时间限制：1秒 空间限制：32768K 热度指数：89570 

本题知识点： 字符串
题目描述 

请实现一个函数用来判断字符串是否表示数值（包括整数和小数）。例如，字符串”+100”,”5e2”,”-123”,”3.1416”和”-1E-16”都表示数值。 但是”12e”,”1a3.14”,”1.2.3”,”+-5”和”12e+4.3”都不是。

## 解法

代码

```
class Solution {
public:
    bool isNumeric(char* str)
    {
        if(str==NULL)
            return false;

        bool result;
        result=scanInt(&str);
        if(*str=='.')
        {
            str++;
            result=scanUnsignInt(&str)||result;
        }
        if(*str=='e'||*str=='E')
        {
            str++;
            result=scanInt(&str)&&result;
        }
        return result&&*str=='\0';
    }

    bool scanInt(char** str)
    {
        if(**str=='+'||**str=='-')
            (*str)++;
        return scanUnsignInt(str);
    }

    bool scanUnsignInt(char** str)
    {
        char* start=*str;
        while(**str!='\0'&&**str<='9'&&**str>='0')
        {
            (*str)++;
        }
        return (*str)!=start;
    }

};
```




