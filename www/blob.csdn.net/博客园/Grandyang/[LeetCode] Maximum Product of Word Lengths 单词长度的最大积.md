# [LeetCode] Maximum Product of Word Lengths 单词长度的最大积 - Grandyang - 博客园







# [[LeetCode] Maximum Product of Word Lengths 单词长度的最大积](https://www.cnblogs.com/grandyang/p/5090058.html)







Given a string array `words`, find the maximum value of `length(word[i]) * length(word[j])` where the two words do not share common letters. You may assume that each word will contain only lower case letters. If no such two words exist, return 0.

Example 1:

Given `["abcw", "baz", "foo", "bar", "xtfn", "abcdef"]`
Return `16`
The two words can be `"abcw", "xtfn"`.

Example 2:

Given `["a", "ab", "abc", "d", "cd", "bcd", "abcd"]`
Return `4`
The two words can be `"ab", "cd"`.

Example 3:

Given `["a", "aa", "aaa", "aaaa"]`
Return `0`
No such pair of words.



这道题给我们了一个单词数组，让我们求两个没有相同字母的单词的长度之积的最大值。我开始想的方法是每两个单词先比较，如果没有相同字母，则计算其长度之积，然后每次更新结果就能找到最大值。但是我开始想的两个单词比较的方法是利用哈希表先将一个单词的所有出现的字母存入哈希表，然后检查另一个单词的各个字母是否在哈希表出现过，若都没出现过，则说明两个单词没有相同字母，则计算两个单词长度之积并更新结果。但是这种判断方法无法通过OJ的大数据集，上网搜大神们的解法，都是用了mask，因为题目中说都是小写字母，那么只有26位，一个整型数int有32位，我们可以用后26位来对应26个字母，若为1，说明该对应位置的字母出现过，那么每个单词的都可由一个int数字表示，两个单词没有共同字母的条件是这两个int数想与为0，用这个判断方法可以通过OJ，参见代码如下：



解法一：

```
class Solution {
public:
    int maxProduct(vector<string>& words) {
        int res = 0;
        vector<int> mask(words.size(), 0);
        for (int i = 0; i < words.size(); ++i) {
            for (char c : words[i]) {
                mask[i] |= 1 << (c - 'a');
            }
            for (int j = 0; j < i; ++j) {
                if (!(mask[i] & mask[j])) {
                    res = max(res, int(words[i].size() * words[j].size()));
                }
            }
        }
        return res;
    }
};
```



还有一种写法，借助哈希表，映射每个mask的值和其单词的长度，每算出一个单词的mask，遍历哈希表里的值，如果和其中的mask值相与为0，则将当前单词的长度和哈希表中存的单词长度相乘并更新结果，参见代码如下：



解法二：

```
class Solution {
public:
    int maxProduct(vector<string>& words) {
        int res = 0;
        unordered_map<int, int> m;
        for (string word : words) {
            int mask = 0;
            for (char c : word) {
                mask |= 1 << (c - 'a');
            }
            m[mask] = max(m[mask], int(word.size()));
            for (auto a : m) {
                if (!(mask & a.first)) {
                    res = max(res, (int)word.size() * a.second);
                }
            }
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/discuss/74580/bit-shorter-c](https://leetcode.com/discuss/74580/bit-shorter-c)

[https://leetcode.com/discuss/75034/clear-and-easily-understanding](https://leetcode.com/discuss/75034/clear-and-easily-understanding)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)














