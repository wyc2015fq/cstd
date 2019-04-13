
# 205. Isomorphic Strings - OraYang的博客 - CSDN博客

2017年07月30日 17:13:57[OraYang](https://me.csdn.net/u010665216)阅读数：207标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given two stringssandt, determine if they are isomorphic.
Two strings are isomorphic if the characters inscan be replaced to gett.
All occurrences of a character must be replaced with another character while preserving the order of characters. No two characters may map to the same character but a character may map to itself.
For example,
Given`"egg"`,`"add"`,
 return true.
Given`"foo"`,`"bar"`,
 return false.
Given`"paper"`,`"title"`,
 return true.
思路：本题通过哈希表映射的实现来实现，要s到t的映射及t到s的映射
代码：

```python
class Solution {
public:
    bool isIsomorphic(string s, string t) {
        if(s.size()!=t.size())
            return false;
        map<char,char> mp;
        for(int i =0;i<s.size();i++)
        {
            if(mp.find(s[i])==mp.end())
                mp[s[i]] = t[i];
            else if(mp[s[i]]!=t[i])
                return false;
        }
        mp.clear();
        for(int i =0;i<s.size();i++)
        {
            if(mp.find(t[i])==mp.end())
                mp[t[i]] = s[i];
            else if(mp[t[i]]!=s[i])
                return false;
        }
        return true;
    }
};
```


