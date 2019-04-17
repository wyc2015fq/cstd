# [LeetCode] Group Anagrams 群组错位词 - Grandyang - 博客园







# [[LeetCode] Group Anagrams 群组错位词](https://www.cnblogs.com/grandyang/p/4385822.html)







Given an array of strings, group anagrams together.

Example:
Input: `["eat", "tea", "tan", "ate", "nat", "bat"]`,
Output:
[
  ["ate","eat","tea"],
  ["nat","tan"],
  ["bat"]
]
Note:
- All inputs will be in lowercase.
- The order of your output does not matter.



这道题让我们群组给定字符串集中所有的错位词，所谓的错位词就是两个字符串中字母出现的次数都一样，只是位置不同，比如abc，bac, cba等它们就互为错位词，那么我们如何判断两者是否是错位词呢，我们发现如果把错位词的字符顺序重新排列，那么会得到相同的结果，所以重新排序是判断是否互为错位词的方法，由于错位词重新排序后都会得到相同的字符串，我们以此作为key，将所有错位词都保存到字符串数组中，建立key和字符串数组之间的映射，最后再存入结果res中即可，擦巾代码如下：



解法一：

```
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> res;
        unordered_map<string, vector<string>> m;
        for (string str : strs) {
            string t = str;
            sort(t.begin(), t.end());
            m[t].push_back(str);
        }
        for (auto a : m) {
            res.push_back(a.second);
        }
        return res;
    }
};
```



下面这种解法没有用到排序，我们用一个大小为26的int数组来统计每个单词中字符出现的次数，然后将int数组转为一个唯一的字符串，跟字符串数组进行映射，这样我们就不用给字符串排序了，代码如下：



解法二：

```
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> res;
        unordered_map<string, vector<string>> m;
        for (string str : strs) {
            vector<int> cnt(26, 0);
            string t = "";
            for (char c : str) ++cnt[c - 'a'];
            for (int d : cnt) t += to_string(d) + "/";
            m[t].push_back(str);
        }
        for (auto a : m) {
            res.push_back(a.second);
        }
        return res;
    }
};
```



类似题目：

[Valid Anagram](http://www.cnblogs.com/grandyang/p/4694988.html)

[Group Shifted Strings](http://www.cnblogs.com/grandyang/p/5204770.html)



参考资料：

[https://leetcode.com/problems/group-anagrams/](https://leetcode.com/problems/group-anagrams/)

[https://leetcode.com/problems/group-anagrams/discuss/19176/share-my-short-java-solution](https://leetcode.com/problems/group-anagrams/discuss/19176/share-my-short-java-solution)

[https://leetcode.com/problems/group-anagrams/discuss/19200/10-lines-76ms-easy-c-solution-updated-function-signature](https://leetcode.com/problems/group-anagrams/discuss/19200/10-lines-76ms-easy-c-solution-updated-function-signature)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












