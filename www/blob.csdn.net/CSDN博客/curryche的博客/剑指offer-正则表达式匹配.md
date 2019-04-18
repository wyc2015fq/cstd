# 剑指offer - 正则表达式匹配 - curryche的博客 - CSDN博客





2018年08月25日 19:38:28[curryche](https://me.csdn.net/whwan11)阅读数：53








## 题目

正则表达式匹配 

时间限制：1秒 空间限制：32768K 热度指数：115087 

本题知识点： 字符串 

题目描述 

请实现一个函数用来匹配包括’.’和’*‘的正则表达式。模式中的字符’.’表示任意一个字符，而’*‘表示它前面的字符可以出现任意次（包含0次）。 在本题中，匹配是指字符串的所有字符匹配整个模式。例如，字符串”aaa”与模式”a.a”和”ab*ac*a”匹配，但是与”aa.a”和”ab*a”均不匹配
## 解法

代码

```
class Solution {
public:
    bool match(char* str, char* pattern)
    {
        if(str==NULL||pattern==NULL)
            return false;
        return match_recursive(str,pattern);
    }

    bool match_recursive(char* str,char* pattern)
    {
        if(*str=='\0'&&*pattern=='\0')
            return true;
        if(*str!='\0'&&*pattern=='\0')
            return false;

        if(*(pattern+1)=='*')
        {
            if(*str==*pattern||(*pattern=='.'&&*str!='\0'))
            {
                return match_recursive(str,pattern+2)||match_recursive(str+1,pattern)||
                       match_recursive(str+1,pattern+2);
            }
            else
            {
                return match_recursive(str,pattern+2);
            }
        }
        else if(*str==*pattern||(*pattern=='.'&&*str!='\0'))
            return match_recursive(str+1,pattern+1);
        else
            return false;
    }
};
```




