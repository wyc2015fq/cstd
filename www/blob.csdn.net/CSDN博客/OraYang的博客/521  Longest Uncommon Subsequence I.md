
# 521. Longest Uncommon Subsequence I - OraYang的博客 - CSDN博客

2017年08月23日 10:52:06[OraYang](https://me.csdn.net/u010665216)阅读数：135标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a group of two strings, you need to find the longest uncommon subsequence of this group of two strings. The longest uncommon subsequence is defined as the longest subsequence of one of these
 strings and this subsequence should not beanysubsequence of the other strings.
Asubsequenceis a sequence that can be derived from one sequence by deleting some characters without changing the order of the remaining elements. Trivially,
 any string is a subsequence of itself and an empty string is a subsequence of any string.
The input will be two strings, and the output needs to be the length of the longest uncommon subsequence. If the longest uncommon subsequence doesn't exist, return -1.
Example 1:
Input:"aba", "cdc"Output:3Explanation:The longest uncommon subsequence is "aba" (or "cdc"),
because "aba" is a subsequence of "aba",
but not a subsequence of any other strings in the group of two strings.
Note:
Both strings' lengths will not exceed 100.
Only letters from a ~ z will appear in input strings.
思路：
本题题意是有歧义的，实际考察的是判断一个两个字符串是否相等，不相等就返回长度最长的字符串，相等则返回-1(讲道理，这道题出的毫无道理）
代码：

```python
class Solution {
public:
    int findLUSlength(string a, string b) {
        int flag = 0,size;
        int a_size = a.size(),b_size = b.size();
        size = a_size<b_size?b_size:a_size;
        for(int i =0;i<size;i++){  
            if(a[i]!=b[i])  
                flag=1;                
            }  
        return flag==1?size:-1;         
    } 
};
```



