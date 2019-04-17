# leetcode-49. Group Anagrams 字母异位词分组 - 别说话写代码的博客 - CSDN博客





2019年01月09日 23:43:29[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：25标签：[字母异位词分组																[Group Anagrams																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Group Anagrams&t=blog)



|Given an array of strings, group anagrams together.**Example:****Input:**`["eat", "tea", "tan", "ate", "nat", "bat"]`,**Output:**[  ["ate","eat","tea"],  ["nat","tan"],  ["bat"]]**Note:**- All inputs will be in lowercase.- The order of your output does not matter.|给定一个字符串数组，将字母异位词组合在一起。字母异位词指字母相同，但排列不同的字符串。**示例:****输入:**`["eat", "tea", "tan", "ate", "nat", "bat"]`,**输出:**[  ["ate","eat","tea"],  ["nat","tan"],  ["bat"]]**说明：**- 所有输入均为小写字母。- 不考虑答案输出的顺序。|
|----|----|

思路：

**第一种**：对每个字符 串的字母排序，如果结果相等，那就是一样的，将原始 字符串加入哈希表中。

```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> res;
        unordered_map<string,vector<string>> mp;
        for(string s:strs)
        {
            string t=s;    //遍历strs中每一个字符串
            sort(t.begin(),t.end());    //对每个字符串的字符排序
            mp[t].push_back(s);    //将相等的字符串加入哈希表
        }
        for(auto m:mp)
            res.push_back(m.second);    //将哈希表的value给res
        return res;
    }
};
```

**第二种**： 不用对每个字符串排序，用一个a[26]的数组映射 每个字符串中每个字符出现的次数。然后a数组 相等的字符串 就是一组，加入 同一个哈希表中。比较a数组相等 可以遍历是否相等，或者将其转为一个特定字符串，比较字符串是否相等。速度慢

```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> res;
        unordered_map<string,vector<string>> mp;
        for(string s:strs)
        {
            vector<int> vec(26,0);
            for(char c:s) vec[c-'a']++;
            string t="";
            for(int i:vec) t+=to_string(i)+"/";   #将映射数组转为唯一的字符串
            mp[t].push_back(s);
        }
        for(auto m:mp)
            res.push_back(m.second);
        return res;
    }
};
```](https://so.csdn.net/so/search/s.do?q=字母异位词分组&t=blog)




