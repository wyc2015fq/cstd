
# 557. Reverse Words in a String III - OraYang的博客 - CSDN博客

2017年08月26日 09:47:04[OraYang](https://me.csdn.net/u010665216)阅读数：261所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a string, you need to reverse the order of characters in each word within a sentence while still preserving whitespace and initial word order.
Example 1:
Input:"Let's take LeetCode contest"Output:"s'teL ekat edoCteeL tsetnoc"
Note:In the string, each word is separated by single space and there will not be any extra space in the string.
思路：本题思路比较简单，遍历数组，遇到空格，便将此次空格与上次的空格间的字符串反转，本题要注意的是起始单词与末尾单词的处理
代码：

```python
class Solution {
public:
    string reverseWords(string s) {
        int z = -1;
        for(int i=0;i<s.size();i++)
        {
            if(s[i]==' ')
            {
                
                reverse(s.begin()+z+1,s.begin()+i);
                z=i;
            }
            
        }
        reverse(s.begin()+z+1,s.end());
        return s;
    }
};
```


