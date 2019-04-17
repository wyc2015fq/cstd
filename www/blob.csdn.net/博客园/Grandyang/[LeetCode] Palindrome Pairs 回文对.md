# [LeetCode] Palindrome Pairs 回文对 - Grandyang - 博客园







# [[LeetCode] Palindrome Pairs 回文对](https://www.cnblogs.com/grandyang/p/5272039.html)







Given a list of unique words. Find all pairs of *distinct* indices `(i, j)` in the given list, so that the concatenation of the two words, i.e. `words[i] + words[j]` is a palindrome.

Example 1:
Given `words` = `["bat", "tab", "cat"]`
Return `[[0, 1], [1, 0]]`
The palindromes are `["battab", "tabbat"]`

Example 2:
Given `words` = `["abcd", "dcba", "lls", "s", "sssll"]`
Return `[[0, 1], [1, 0], [3, 2], [2, 4]]`
The palindromes are `["dcbaabcd", "abcddcba", "slls", "llssssll"]`

Credits:
Special thanks to [@dietpepsi](https://leetcode.com/discuss/user/dietpepsi) for adding this problem and creating all test cases.



这道题给我们了许多单词，让我们找出回文对，就是两个单词拼起来是个回文字符串，我最开始尝试的是brute force的方法，每两个单词都拼接起来然后判断是否是回文字符串，但是通过不了OJ，会超时，可能这也是这道题标为Hard的原因之一吧，那么我们只能找别的方法来做，通过学习大神们的解法，发现如下两种方法比较好，其实两种方法的核心思想都一样，写法略有不同而已，那么我们先来看第一种方法吧，要用到哈希表来建立每个单词和其位置的映射，然后需要一个set来保存出现过的单词的长度，算法的思想是，遍历单词集，对于遍历到的单词，我们对其翻转一下，然后在哈希表查找翻转后的字符串是否存在，注意不能和原字符串的坐标位置相同，因为有可能一个单词翻转后和原单词相等，现在我们只是处理了bat和tab的情况，还存在abcd和cba，dcb和abcd这些情况需要考虑，这就是我们为啥需要用set，由于set是自动排序的，我们可以找到当前单词长度在set中的iterator，然后从开头开始遍历set，遍历比当前单词小的长度，比如abcdd翻转后为ddcba，我们发现set中有长度为3的单词，然后我们dd是否为回文串，若是，再看cba是否存在于哈希表，若存在，则说明abcdd和cba是回文对，存入结果中，对于dcb和aabcd这类的情况也是同样处理，我们要在set里找的字符串要在遍历到的字符串的左边和右边分别尝试，看是否是回文对，这样遍历完单词集，就能得到所有的回文对，参见代码如下：



解法一：

```
class Solution {
public:
    vector<vector<int>> palindromePairs(vector<string>& words) {
        vector<vector<int>> res;
        unordered_map<string, int> m;
        set<int> s;
        for (int i = 0; i < words.size(); ++i) {
            m[words[i]] = i;
            s.insert(words[i].size());
        }
        for (int i = 0; i < words.size(); ++i) {
            string t = words[i];
            int len = t.size();
            reverse(t.begin(), t.end());
            if (m.count(t) && m[t] != i) {
                res.push_back({i, m[t]});
            }
            auto a = s.find(len);
            for (auto it = s.begin(); it != a; ++it) {
                int d = *it;
                if (isValid(t, 0, len - d - 1) && m.count(t.substr(len - d))) {
                    res.push_back({i, m[t.substr(len - d)]});
                }
                if (isValid(t, d, len - 1) && m.count(t.substr(0, d))) {
                    res.push_back({m[t.substr(0, d)], i});
                }
            }
        }
        return res;
    }
    bool isValid(string t, int left, int right) {
        while (left < right) {
            if (t[left++] != t[right--]) return false;
        }
        return true;
    }
};
```



下面这种方法没有用到set，但实际上循环的次数要比上面多，因为这种方法对于遍历到的字符串，要验证其所有可能的子串，看其是否在哈希表里存在，并且能否组成回文对，anyway，既然能通过OJ，说明还是比brute force要快的，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> palindromePairs(vector<string>& words) {
        vector<vector<int>> res;
        unordered_map<string, int> m;
        for (int i = 0; i < words.size(); ++i) m[words[i]] = i;
        for (int i = 0; i < words.size(); ++i) {
            int l = 0, r = 0;
            while (l <= r) {
                string t = words[i].substr(l, r - l);
                reverse(t.begin(), t.end());
                if (m.count(t) && i != m[t] && isValid(words[i].substr(l == 0 ? r : 0, l == 0 ? words[i].size() - r: l))) {
                    if (l == 0) res.push_back({i, m[t]});
                    else res.push_back({m[t], i});
                }
                if (r < words[i].size()) ++r;
                else ++l;
            }
        }
        return res;
    }
    bool isValid(string t) {
        for (int i = 0; i < t.size() / 2; ++i) {
            if (t[i] != t[t.size() - 1 - i]) return false;
        }
        return true;
    }
};
```



参考资料：

[https://leetcode.com/discuss/91562/my-c-solution-275ms-worst-case-o-n-2](https://leetcode.com/discuss/91562/my-c-solution-275ms-worst-case-o-n-2)

[https://leetcode.com/discuss/91531/accepted-short-java-solution-using-hashmap](https://leetcode.com/discuss/91531/accepted-short-java-solution-using-hashmap)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












