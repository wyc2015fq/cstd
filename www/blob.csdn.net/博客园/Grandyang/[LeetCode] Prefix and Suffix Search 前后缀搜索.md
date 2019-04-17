# [LeetCode] Prefix and Suffix Search 前后缀搜索 - Grandyang - 博客园







# [[LeetCode] Prefix and Suffix Search 前后缀搜索](https://www.cnblogs.com/grandyang/p/8331660.html)







Given many `words`, `words[i]` has weight `i`.

Design a class `WordFilter` that supports one function, `WordFilter.f(String prefix, String suffix)`. It will return the word with given `prefix` and `suffix` with maximum weight. If no word exists, return -1.

**Examples:**

**Input:**
WordFilter(["apple"])
WordFilter.f("a", "e") // returns 0
WordFilter.f("b", "") // returns -1

**Note:**

- `words` has length in range `[1, 15000]`.
- For each test case, up to `words.length` queries `WordFilter.f` may be made.
- `words[i]` has length in range `[1, 10]`.
- `prefix, suffix` have lengths in range `[0, 10]`.
- `words[i]` and `prefix, suffix` queries consist of lowercase letters only.



这道题给了我们一些单词，让我们通过输入单词的前缀和后缀来查找单词的位置。单词的位置就是其权重值，如果给定的前后缀能对应到不只一个单词，那么返回最大的权重。首先，一个单词如果长度为n的话，那么其就有n个前缀，比如对于单词apple，其前缀即为"a", "ap", "app", "appl", "apple"，同理，后缀也有n个。那么其组成的情况就有n2个，所以最简单的方法就是把这n2个前后缀组成一个字符串，和当前权重建立映射。如果后面的单词有相同的前后缀，直接用后面的大权重来覆盖之前的权重即可。为了将前后缀encode成一个字符串，我们可以在中间加上一个非字母字符，比如'#'，然后在查找的时候，我们先拼出“前缀#后缀”字符串，直接去哈希map中找即可，这种解法的WordFilter函数时间复杂度为O(NL^2)，其中N是单词个数，L是单词长度。f函数时间复杂度为O(1)，空间复杂度为O(NL^2)，适合需要大量查找的情况下使用，参见代码如下：



```
class WordFilter {
public:
    WordFilter(vector<string> words) {
        for (int k = 0; k < words.size(); ++k) {
            for (int i = 0; i <= words[k].size(); ++i) {
                for (int j = 0; j <= words[k].size(); ++j) {
                    m[words[k].substr(0, i) + "#" + words[k].substr(words[k].size() - j)] = k;
                }
            }
        }
    }

    int f(string prefix, string suffix) {
        return (m.count(prefix + "#" + suffix)) ? m[prefix + "#" + suffix] : -1;
    }

private:
    unordered_map<string, int> m;
};
```



如果我们希望节省一些空间的话，可以使用下面的方法。使用两个哈希map，一个建立所有前缀和权重数组之间的映射，另一个建立所有后缀和权重数组之间的映射。在WordFilter函数中，我们遍历每个单词，然后先遍历其所有前缀，将遍历到的前缀的映射数组中加入当前权重，同理再遍历其所有后缀，将遍历到的后缀的映射数组中加入当前权重。在搜索函数f中，首先判断，如果前缀或后缀不存在的话，直接返回-1。否则我们分别把前缀和后缀的权重数组取出来，然后用两个指针i和j，分别指向数组的最后一个位置。当i和j不小于0时进行循环，如果两者的权重相等，直接返回，如果前缀的权重数组值大，则j自减1，反之i自减1，这种解法的WordFilter函数时间复杂度为O(NL)，其中N是单词个数，L是单词长度。f函数时间复杂度为O(N)，空间复杂度为O(NL)，参见代码如下：



解法二：

```
class WordFilter {
public:
    WordFilter(vector<string> words) {
        for (int k = 0; k < words.size(); ++k) {
            for (int i = 0; i <= words[k].size(); ++i) {
                mp[words[k].substr(0, i)].push_back(k);
            }
            for (int i = 0; i <= words[k].size(); ++i) {
                ms[words[k].substr(words[k].size() - i)].push_back(k);
            }
        }
    }

    int f(string prefix, string suffix) {
        if (!mp.count(prefix) || !ms.count(suffix)) return -1;
        vector<int> pre = mp[prefix], suf = ms[suffix];
        int i = pre.size() - 1, j = suf.size() - 1;
        while (i >= 0 && j >= 0) {
            if (pre[i] < suf[j]) --j;
            else if (pre[i] > suf[j]) --i;
            else return pre[i];
        }
        return -1;
    }

private:
    unordered_map<string, vector<int>> mp, ms;
};
```



[moto72大神的帖子](https://discuss.leetcode.com/topic/113547/three-ways-to-solve-this-problem-in-java)中还有第三种解法，但是C++中没有startsWith()和endsWith()函数，以至于无法写出C++版本的，还是Java比较叼啊。



类似题目：

[Add and Search Word - Data structure design](http://www.cnblogs.com/grandyang/p/4507286.html)



参考资料：

[https://discuss.leetcode.com/topic/113547/three-ways-to-solve-this-problem-in-java](https://discuss.leetcode.com/topic/113547/three-ways-to-solve-this-problem-in-java)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












