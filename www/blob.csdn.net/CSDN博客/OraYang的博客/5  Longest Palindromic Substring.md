
# 5. Longest Palindromic Substring - OraYang的博客 - CSDN博客

2017年09月02日 10:38:01[OraYang](https://me.csdn.net/u010665216)阅读数：144标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a strings, find the longest palindromic substring ins. You may assume that the maximum length ofsis 1000.
Example:
Input:"babad"Output:"bab"Note:"aba" is also a valid answer.
Example:
Input:"cbbd"Output:"bb"思路：本题一开始假设最大回文子字符串长度为Max=s.size()，然后判断是否符合要求，不符合Max--，继续判断，知道发现符合要求的子字符串就直接返回子字符串，但是TimeLimit,但是思路可以参考，算是一种贪心的算法
代码：

```python
class Solution {
public:
    string longestPalindrome(string s) {
        int max = s.size();
        string str = s.substr(0,1),sub_str;
        int i = 0;
        while(1)
        {
            
            for(int j = i;j<=s.size()-max;j++)
            {
                sub_str = s.substr(j,max);
                
                if(judge(sub_str))
                {
                     return sub_str;
                } 
            }
            max--;
            i=0;
            if(max==1)
                break;
        }
    return str;    
    }
private:
    bool judge(string s)
    {
        string::iterator begin = s.begin();    
        string::iterator end = s.end();  
        end--;  
        for(;begin<end;begin++,end--)  
        {  
            if(*begin!=*end)  
            {
                flag = end
                return false; 
            }
                 
        }  
          
        return true;  
    }
};
```
思路：第二种思路直接采用官方解答方法：[Expand
 Around Center](https://leetcode.com/problems/longest-palindromic-substring/solution/)
代码：

```python
class Solution {
public:
    string longestPalindrome(string s) {
    int start = 0, end = 0;
    for (int i = 0; i < s.length(); i++) {
        int len1 = expandAroundCenter(s, i, i);
        int len2 = expandAroundCenter(s, i, i + 1);
        int len = max(len1, len2);
        if (len > end - start) {
            start = i - (len - 1) / 2;
            end = i + len / 2;
        }
    }
    return s.substr(start, end-start+1);
        
    }
private:
    int expandAroundCenter(string s,int left, int right){
    int L = left, R = right;
    while (L >= 0 && R < s.length() && s[L] == s[R]) {
        L--;
        R++;
    }
    return R-L-1;
    }
};
```



