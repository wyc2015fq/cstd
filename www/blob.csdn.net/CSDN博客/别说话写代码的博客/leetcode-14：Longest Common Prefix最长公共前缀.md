# leetcode-14：Longest Common Prefix最长公共前缀 - 别说话写代码的博客 - CSDN博客





2018年12月18日 10:51:20[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：13标签：[leetcode 14																[Longest Common Prefix																[最长公共前缀](https://so.csdn.net/so/search/s.do?q=最长公共前缀&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Longest Common Prefix&t=blog)




题目：
|Write a function to find the longest common prefix string amongst an array of strings.If there is no common prefix, return an empty string `""`.**Example 1:****Input: **["flower","flow","flight"]**Output:** "fl"**Example 2:****Input: **["dog","racecar","car"]**Output:** ""**Explanation:** There is no common prefix among the input strings.**Note:**All given inputs are in lowercase letters `a-z`.|编写一个函数来查找字符串数组中的最长公共前缀。如果不存在公共前缀，返回空字符串 `""`。**示例 1:****输入: **["flower","flow","flight"]**输出:** "fl"**示例 2:****输入: **["dog","racecar","car"]**输出:** ""**解释:** 输入不存在公共前缀。**说明:**所有输入只包含小写字母 `a-z` 。|
|----|----|

思路：先对字符串序列排序，排完后只需要比较第一个和最后一个字符串的前缀即可，若他俩相等其他肯定相等。还需注意字符串长度len应选择最短那个字符。

```cpp
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if(strs.empty()) return "";
        sort(strs.begin(),strs.end());
        int i=0,len=min(strs[0].size(),strs.back().size());
        while(i<len && strs[0][i]==strs.back()[i]) i++;
        return strs[0].substr(0,i);
    }
};
```](https://so.csdn.net/so/search/s.do?q=leetcode 14&t=blog)




