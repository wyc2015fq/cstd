# leetcode-5:Longest Palindromic Substring 最长回文子串 - 别说话写代码的博客 - CSDN博客





2018年12月09日 18:41:06[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：25标签：[最长回文子串																[Longest Palindromic Substring																[leetcode 5](https://so.csdn.net/so/search/s.do?q=leetcode 5&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Longest Palindromic Substring&t=blog)




题目：
|Given a string **s**, find the longest palindromic substring in **s**. You may assume that the maximum length of **s** is 1000.**Example 1:****Input:** "babad"**Output:** "bab"**Note:** "aba" is also a valid answer.**Example 2:****Input:** "cbbd"**Output:** "bb"|给定一个字符串 `s`，找到 `s` 中最长的回文子串。你可以假设 `s` 的最大长度为 1000。**示例 1：****输入:** "babad"**输出:** "bab"**注意:** "aba" 也是一个有效答案。**示例 2：****输入:** "cbbd"**输出:** "bb"|
|----|----|

解析：这道题让我们求[最长回文子串](http://en.wikipedia.org/wiki/Longest_palindromic_substring)，首先说下什么是回文串，就是正读反读都一样的字符串，比如 "bob", "level", "noon" 等等。那么最长回文子串就是在一个字符串中的那个最长的回文子串。LeetCode中关于回文串的题共有五道，除了这道，其他的四道为[Palindrome Number](http://www.cnblogs.com/grandyang/p/4125510.html)，[Validate Palindrome](http://www.cnblogs.com/grandyang/p/4030114.html)，[Palindrome Partitioning](http://www.cnblogs.com/grandyang/p/4270008.html)，[Palindrome Partitioning II](http://www.cnblogs.com/grandyang/p/4271456.html)，我们知道传统的验证回文串的方法就是两个两个的对称验证是否相等，那么对于找回文字串的问题，就要以每一个字符为中心，像两边扩散来寻找回文串，这个算法的时间复杂度是O(n*n)，可以通过OJ，就是要注意奇偶情况，由于回文串的长度可奇可偶，比如"bob"是奇数形式的回文，"noon"就是偶数形式的回文，两种形式的回文都要搜索，对于奇数形式的，我们就从遍历到的位置为中心，向两边进行扩散，对于偶数情况，我们就把当前位置和下一个位置当作偶数行回文的最中间两个字符，然后向两边进行搜索，参见代码如下：

```cpp
class Solution {
public:
    string longestPalindrome(string s) {
        if(s.size()<2) return s;
        int start =0, maxlen=0;
        for(int i=0;i<s.length()-1;++i)
        {
            sub(s,i,i,start,maxlen);
            sub(s,i,i+1,start,maxlen);
        }return s.substr(start,maxlen);
    }
    void sub(string s,int left,int right,int &start,int &maxlen)
    {
        while(left>=0&&right<s.size() && s[left]==s[right])
        {
            right++; left--;
        }
        if(maxlen < right-left-1)
        {
            maxlen = right-left-1;
            start = left+1;
        }
    }
};
```](https://so.csdn.net/so/search/s.do?q=最长回文子串&t=blog)




