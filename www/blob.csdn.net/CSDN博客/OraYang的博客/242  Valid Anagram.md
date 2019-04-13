
# 242. Valid Anagram - OraYang的博客 - CSDN博客

2017年08月02日 10:00:16[OraYang](https://me.csdn.net/u010665216)阅读数：118所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given two stringssandt, write a function to determine iftis an anagram ofs.
For example,
s= "anagram",t= "nagaram", return true.
s= "rat",t= "car", return false.
Note:
You may assume the string contains only lowercase alphabets.
思路：本题求有效哈希表，充分利用unorder_map即可
代码：

```python
class Solution {
public:
    bool isAnagram(string s, string t) {
        int n = s.size();
        if(n!=t.size())  
            return false;  
        unordered_map<char, int> mp;
        for(int i;i<n;i++)
        {
            mp[s[i]]++;
            mp[t[i]]--;
        }
        for(auto count:mp)
            if(count.second)
                return false;
        return true;
        
    }
};
```


