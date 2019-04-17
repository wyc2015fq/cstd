# [LeetCode] Longest Uncommon Subsequence II 最长非共同子序列之二 - Grandyang - 博客园







# [[LeetCode] Longest Uncommon Subsequence II 最长非共同子序列之二](https://www.cnblogs.com/grandyang/p/6680548.html)







Given a list of strings, you need to find the longest uncommon subsequence among them. The longest uncommon subsequence is defined as the longest subsequence of one of these strings and this subsequence should not be any subsequence of the other strings.

A subsequence is a sequence that can be derived from one sequence by deleting some characters without changing the order of the remaining elements. Trivially, any string is a subsequence of itself and an empty string is a subsequence of any string.

The input will be a list of strings, and the output needs to be the length of the longest uncommon subsequence. If the longest uncommon subsequence doesn't exist, return -1.

Example 1:
Input: "aba", "cdc", "eae"
Output: 3



Note:
- All the given strings' lengths will not exceed 10.
- The length of the given list will be in the range of [2, 50].



这道题是之前那道[Longest Uncommon Subsequence I](http://www.cnblogs.com/grandyang/p/6666839.html)的拓展，那道题因为只有两个字符串为大家所不屑。那么这道题有多个字符串，这次大家满足了吧。令我吃惊的是，这次的OJ异常的大度，连暴力搜索的解法也让过，那么还等什么，无脑暴力破解吧。遍历所有的字符串，对于每个遍历到的字符串，再和所有的其他的字符串比较，看是不是某一个字符串的子序列，如果都不是的话，那么当前字符串就是一个非共同子序列，用其长度来更新结果res，参见代码如下：



解法一：

```
class Solution {
public:
    int findLUSlength(vector<string>& strs) {
        int res = -1, j = 0, n = strs.size();
        for (int i = 0; i < n; ++i) {
            for (j = 0; j < n; ++j) {
                if (i == j) continue;
                if (checkSubs(strs[i], strs[j])) break;
            }
            if (j == n) res = max(res, (int)strs[i].size());
        }
        return res;
    }
    int checkSubs(string subs, string str) {
        int i = 0;
        for (char c : str) {
            if (c == subs[i]) ++i;
            if (i == subs.size()) break;
        } 
        return i == subs.size();
    }
};
```



下面这种解法使用一些博主能想到的优化手段，首先我们给字符串按长度来排序，将长度大的放到前面，这样我们如果找到了非共同子序列，那么直接返回其长度即可，因为当前找到的肯定是最长的。然后我们用一个集合来记录已经遍历过的字符串，利用集合的去重复特性，这样在有大量的重复字符串的时候可以提高效率，然后我们开始遍历字符串，对于当前遍历到的字符串，我们和集合中的所有字符串相比，看其是否是某个的子序列，如果都不是，说明当前的就是最长的非共同子序列。注意如果当前的字符串是集合中某个字符串的子序列，那么直接break出来，不用再和其他的比较了，这样在集合中有大量的字符串时可以提高效率，最后别忘了将遍历过的字符串加入集合中，参见代码如下：



解法二：

```
class Solution {
public:
    int findLUSlength(vector<string>& strs) {
        int n = strs.size();
        unordered_set<string> s;
        sort(strs.begin(), strs.end(), [](string a, string b){
            if (a.size() == b.size()) return a > b;
            return a.size() > b.size();
        });
        for (int i = 0; i < n; ++i) {
            if (i == n - 1 || strs[i] != strs[i + 1]) {
                bool found = true;
                for (auto a : s) {
                    int j = 0;
                    for (char c : a) {
                        if (c == strs[i][j]) ++j;
                        if (j == strs[i].size()) break;
                    }
                    if (j == strs[i].size()) {
                        found = false;
                        break;
                    }
                }
                if (found) return strs[i].size();
            }
            s.insert(strs[i]);
        }
        return -1;
    }
};
```



类似题目：

[Longest Uncommon Subsequence I](http://www.cnblogs.com/grandyang/p/6666839.html)



参考资料：

[https://discuss.leetcode.com/topic/85033/checking-subsequence-without-hashing](https://discuss.leetcode.com/topic/85033/checking-subsequence-without-hashing)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












