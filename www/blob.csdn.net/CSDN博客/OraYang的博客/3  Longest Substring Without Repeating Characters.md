
# 3. Longest Substring Without Repeating Characters - OraYang的博客 - CSDN博客

2017年09月01日 11:05:06[OraYang](https://me.csdn.net/u010665216)阅读数：93标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a string, find the length of thelongest substringwithout repeating characters.
Examples:
Given`"abcabcbb"`, the answer
 is`"abc"`, which the length is 3.
Given`"bbbbb"`, the answer
 is`"b"`, with the length of 1.
Given`"pwwkew"`, the answer
 is`"wke"`, with the length of 3. Note that the answer must be asubstring,`"pwke"`is
 asubsequenceand not a substring.
思路：
本题思路比较清晰，首先建立map容器，遍历字符串，存储字符串当前字符与当前索引，维护一个变量max，一旦发现当前字符在前面出现过，就更新max,count清0，map清空，从前面出现的字符的下一个位置继续遍历，直到结束，最后再更新max,并返回输出
代码：

```python
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_map<char,int> mp;
        int count = 0,max = 0;
        for(int i=0;i<s.size();i++)
        {
            if(mp.find(s[i])==mp.end())
            {
                count++;
                mp[s[i]] = i;
            }  
            else
            {
                max = max>count?max:count;
                count = 0;
                i = mp.find(s[i])->second;
                mp.clear();
            }
        }
        max = max>count?max:count;
        return max;
    }
};
```



