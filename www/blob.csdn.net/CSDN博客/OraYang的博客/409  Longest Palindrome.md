
# 409. Longest Palindrome - OraYang的博客 - CSDN博客

2017年08月11日 09:07:00[OraYang](https://me.csdn.net/u010665216)阅读数：86标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a string which consists of lowercase or uppercase letters, find the length of the longest palindromes that can be built with those letters.
This is case sensitive, for example`"Aa"`is
 not considered a palindrome here.
Note:
Assume the length of given string will not exceed 1,010.
Example:
Input:
"abccccdd"
Output:
7
Explanation:
One longest palindrome that can be built is "dccaccd", whose length is 7.
思路：
观察回文的结构可以发现，回文个数=偶数个回文个数（奇数个减一） + 1（如果存在奇数个回文）
代码：

```python
class Solution {
public:
    int longestPalindrome(string s) {
        map<char,int> mp;
        if(!s.size())
            return 0;
        for(int i=0;i<s.size();i++)
        {
            if(mp.find(s[i])==mp.end())
                mp[s[i]] = 1;
            else
                mp[s[i]]++;
        }
        int len=0,flag=0;
        map<char,int>::iterator itr = mp.begin();
        itr = mp.begin();
        while(itr!=mp.end())
        {
            if(itr->second%2==0)
                len+=itr->second;
            else if(itr->second%2)
            {
                len+=itr->second-1;
                flag =1;
            }
            itr++;
        }
        if(!flag) {
            return len;
        } 
        else
            return len+1;
    }
};
```



