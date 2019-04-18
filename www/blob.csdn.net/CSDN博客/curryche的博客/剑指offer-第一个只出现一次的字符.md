# 剑指offer - 第一个只出现一次的字符 - curryche的博客 - CSDN博客





2018年09月04日 21:35:41[curryche](https://me.csdn.net/whwan11)阅读数：13标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

第一个只出现一次的字符 

时间限制：1秒 空间限制：32768K 热度指数：186567 

本题知识点： 字符串
题目描述 

在一个字符串(0<=字符串长度<=10000，全部由字母组成)中找到第一个只出现一次的字符,并返回它的位置, 如果没有则返回 -1（需要区分大小写）.

## 解法

代码

```
class Solution {
public:
    int FirstNotRepeatingChar(string str) {
        if(str.size()==0)
            return -1;

        int counts[256]={0};
        for(int i=0;i<str.size();i++)
            counts[str[i]]++;
        int i=0;
        while(i<str.size())
        {
            if(counts[str[i]]==1)
                return i;
            i++;
        }
        return -1;
    }
};
```





