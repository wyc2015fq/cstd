# 剑指offer - 字符串的排列 - curryche的博客 - CSDN博客





2018年08月31日 16:51:20[curryche](https://me.csdn.net/whwan11)阅读数：40标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

字符串的排列 

时间限制：1秒 空间限制：32768K 热度指数：256734 

本题知识点： 字符串
题目描述 

输入一个字符串,按字典序打印出该字符串中字符的所有排列。例如输入字符串abc,则打印出由字符a,b,c所能排列出来的所有字符串abc,acb,bac,bca,cab和cba。 

输入描述: 

输入一个字符串,长度不超过9(可能有字符重复),字符只包括大小写字母。
## 解法

代码

```cpp
class Solution {
public:
    vector<string> Permutation(string str) {
         vector<string> result;
        if(str.empty())
            return result;
        if(str[0]=='\0')
            return result;
        if(str[1]=='\0')
        {
            result.push_back(str);
            return result;
        }
        result=Permutation_recursive(str);
        string exchange;
        for(int i=result.size()-1;i>0;i--)
        {
            for(int j=0;j<i;j++)
            {
                if(result[j]>result[j+1])
                {
                    exchange=result[j];
                    result[j]=result[j+1];
                    result[j+1]=exchange;
                }
            }
        }
        return result;
    }

    vector<string> Permutation_recursive(string str) {
        vector<string> result;
        if(str.empty())
            return result;
        if(str[0]=='\0')
            return result;
        if(str[1]=='\0')
        {
            result.push_back(str);
            return result;
        }

        char temp;
        vector<string> current_result;
        for(int i=0;i<str.size();i++)
        {
            if(i>0&&str[i]==str[0])
                continue;
            else
            {
                temp=str[0];
                str[0]=str[i];
                str[i]=temp;
                string current_str(str,1);
                current_result=Permutation_recursive(current_str);
                for(int j=0;j<current_result.size();j++)
                {
                    current_result[j]=str[0]+current_result[j];
                    result.push_back(current_result[j]);
                }
                current_result.clear();
                temp=str[0];
                str[0]=str[i];
                str[i]=temp;
            }
        }
        return result;
    }
};
```






