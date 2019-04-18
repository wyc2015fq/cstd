# 剑指offer - 翻转单词顺序列 - curryche的博客 - CSDN博客





2018年09月11日 08:17:27[curryche](https://me.csdn.net/whwan11)阅读数：22标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

翻转单词顺序列 

时间限制：1秒 空间限制：32768K 热度指数：215622 

本题知识点： 字符串
题目描述 

牛客最近来了一个新员工Fish，每天早晨总是会拿着一本英文杂志，写些句子在本子上。同事Cat对Fish写的内容颇感兴趣，有一天他向Fish借来翻看，但却读不懂它的意思。例如，“student. a am I”。后来才意识到，这家伙原来把句子单词的顺序翻转了，正确的句子应该是“I am a student.”。Cat对一一的翻转这些单词顺序可不在行，你能帮助他么？

## 解法

代码

```
class Solution {
public:
    string ReverseSentence(string str) {
        string result;
        if(str.size()==0)
            return result;

        string current;
        for(int i=str.size()-1;i>=0;i--)
        {
            if(str[i]==' ')
            {
                reverse(current.begin(),current.end());
                current+=' ';
                result+=current;
                current.clear();
            }
            else
                current+=str[i];
        }
        reverse(current.begin(),current.end());
        result+=current;
        return result;
    }
};
```





