
# 171. Excel Sheet Column Number - OraYang的博客 - CSDN博客

2017年07月27日 10:12:03[OraYang](https://me.csdn.net/u010665216)阅读数：258所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Related to question[Excel Sheet Column Title](https://leetcode.com/problems/excel-sheet-column-title/)
Given a column title as appear in an Excel sheet, return its corresponding column number.
For example:
A -> 1
    B -> 2
    C -> 3
    ...
    Z -> 26
    AA -> 27
    AB -> 28思路：本题与前面一篇博客[168. Excel Sheet Column Title](http://blog.csdn.net/u010665216/article/details/76126964)很类似，Excel Sheet Column Title是做十进制转26进制，而本题是26进制转十进制。
代码：

```python
class Solution {
public:
    int titleToNumber(string s) {
        int res = 0;
        string::iterator itr = s.begin();
        while(itr!=s.end())  
        {  
            res = (*itr-'A'+1) + res;
            itr++;
            if(itr==s.end())
                break;
            res*=26;
              
        }  
        return res;  
        
    }
};
```


