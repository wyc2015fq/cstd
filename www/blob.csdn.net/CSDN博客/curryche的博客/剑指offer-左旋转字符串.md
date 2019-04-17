# 剑指offer - 左旋转字符串 - curryche的博客 - CSDN博客





2018年09月11日 10:49:02[curryche](https://me.csdn.net/whwan11)阅读数：24标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

左旋转字符串 

时间限制：1秒 空间限制：32768K 热度指数：140073 

本题知识点： 字符串
题目描述 

汇编语言中有一种移位指令叫做循环左移（ROL），现在有个简单的任务，就是用字符串模拟这个指令的运算结果。对于一个给定的字符序列S，请你把其循环左移K位后的序列输出。例如，字符序列S=”abcXYZdef”,要求输出循环左移3位后的结果，即“XYZdefabc”。是不是很简单？OK，搞定它！

## 解法

代码

```
class Solution {
public:
    string LeftRotateString(string str, int n) {
        string result;

        if(str.size()==0)
            return str;

        int k=n%str.size();
        if(k==0)
            return str;

        result=str;
        string temp;
        for(int i=0;i<k;i++)
        {
            temp+=str[i];
        }
        for(int i=k;i<str.size();i++)
        {
            result[i-k]=str[i];
        }
        for(int i=str.size()-k;i<str.size();i++)
        {
            result[i]=temp[i-str.size()+k];
        }
        return result;

    }
};
```





