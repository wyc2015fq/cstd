
# 387. First Unique Character in a String - OraYang的博客 - CSDN博客

2017年08月09日 09:14:36[OraYang](https://me.csdn.net/u010665216)阅读数：90标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a string, find the first non-repeating character in it and return it's index. If it doesn't exist, return -1.
Examples:
s = "leetcode"
return 0.
s = "loveleetcode",
return 2.
Note:You may assume the string contain only lowercase letters.
思路：step1:求出字符串中重复项存入数组A
step2:便利字符串并查找A中是否存在，不存在则返回索引
代码：

```python
class Solution {
public:
    int firstUniqChar(string s) {
        unordered_map<char,int> mp;
        vector<int> res;
        for(int i=0;i<s.size();i++)
        {
            if(mp.find(s[i])!=mp.end())
                res.push_back(s[i]);
            else
                mp[s[i]] = i;         
        }
        vector<int>::iterator itr;
        for(int j=0;j<s.size();j++)
        {
            itr = find(res.begin(),res.end(),s[j]);
             if(itr==res.end())
                 return j;              
        }
    return -1;
    }
};
```


