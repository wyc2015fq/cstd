# [LeetCode] Word Pattern II 词语模式之二 - Grandyang - 博客园







# [[LeetCode] Word Pattern II 词语模式之二](https://www.cnblogs.com/grandyang/p/5325761.html)







Given a `pattern` and a string `str`, find if `str`follows the same pattern.

Here follow means a full match, such that there is a bijection between a letter in `pattern` and a non-empty substring in `str`.

Example 1:
Input: pattern = `"abab"`, str = `"redblueredblue"`
Output: true
Example 2:
Input: pattern = pattern = `"aaaa"`, str = `"asdasdasdasd"`
Output: true
Example 3:
Input: pattern = `"aabb"`, str = `"xyzabcxzyabc"`
Output: false

Notes:
You may assume both `pattern` and `str` contains only lowercase letters.



这道题是之前那道 [Word Pattern](http://www.cnblogs.com/grandyang/p/4857022.html) 的拓展，之前那道题词语之间都有空格隔开，这样我们可以一个单词一个单词的读入，然后来判断是否符合给定的特征，而这道题没有空格了，那么难度就大大的增加了，因为我们不知道对应的单词是什么，所以得自行分开，那么我们可以用回溯法来生成每一种情况来判断，我们还是需要用哈希表来建立模式字符和单词之间的映射，我们还需要用变量p和r来记录当前递归到的模式字符和单词串的位置，在递归函数中，如果p和r分别等于模式字符串和单词字符串的长度，说明此时匹配成功结束了，返回ture，反之如果一个达到了而另一个没有，说明匹配失败了，返回false。如果都不满足上述条件的话，我们取出当前位置的模式字符，然后从单词串的r位置开始往后遍历，每次取出一个单词，如果模式字符已经存在哈希表中，而且对应的单词和取出的单词也相等，那么我们再次调用递归函数在下一个位置，如果返回true，那么我们就返回true。反之如果该模式字符不在哈希表中，我们要看有没有别的模式字符已经映射了当前取出的单词，如果没有的话，我们建立新的映射，并且调用递归函数，注意如果递归函数返回false了，我们要在哈希表中删去这个映射，参见代码如下：



解法一：

```
class Solution {
public:
    bool wordPatternMatch(string pattern, string str) {
        unordered_map<char, string> m;
        return helper(pattern, 0, str, 0, m);
    }
    bool helper(string pattern, int p, string str, int r, unordered_map<char, string> &m) {
        if (p == pattern.size() && r == str.size()) return true;
        if (p == pattern.size() || r == str.size()) return false;
        char c = pattern[p];
        for (int i = r; i < str.size(); ++i) {
            string t = str.substr(r, i - r + 1);
            if (m.count(c) && m[c] == t) {
                if (helper(pattern, p + 1, str, i + 1, m)) return true;
            } else if (!m.count(c)) {
                bool b = false;
                for (auto it : m) {
                    if (it.second == t) b = true;
                } 
                if (!b) {
                    m[c] = t;
                    if (helper(pattern, p + 1, str, i + 1, m)) return true;
                    m.erase(c);
                }
            }
        }
        return false;
    }
};
```



下面这种方法和上面那种方法很类似，不同点在于使用了set，而使用其的原因也是为了记录所有和模式字符建立过映射的单词，这样我们就不用每次遍历哈希表了，只要在set中查找取出的单词是否存在，如果存在了则跳过后面的处理，反之则进行和上面相同的处理，注意还要在set中插入新的单词，最后也要同时删除掉，参见代码如下：



解法二：

```
class Solution {
public:
    bool wordPatternMatch(string pattern, string str) {
        unordered_map<char, string> m;
        set<string> s;
        return helper(pattern, 0, str, 0, m, s);
    }
    bool helper(string pattern, int p, string str, int r, unordered_map<char, string> &m, set<string> &s) {
        if (p == pattern.size() && r == str.size()) return true;
        if (p == pattern.size() || r == str.size()) return false;
        char c = pattern[p];
        for (int i = r; i < str.size(); ++i) {
            string t = str.substr(r, i - r + 1);
            if (m.count(c) && m[c] == t) {
                if (helper(pattern, p + 1, str, i + 1, m, s)) return true;
            } else if (!m.count(c)) {
                if (s.count(t)) continue;
                m[c] = t;
                s.insert(t);
                if (helper(pattern, p + 1, str, i + 1, m, s)) return true;
                m.erase(c);
                s.erase(t);
            }
        }
        return false;
    }
};
```



再来看一种不写helper函数的解法，可以调用自身，思路和上面的方法完全相同，参见代码如下：



解法三：

```
class Solution {
public:
    bool wordPatternMatch(string pattern, string str) {
        if (pattern.empty()) return str.empty();
        if (m.count(pattern[0])) {
            string t = m[pattern[0]];
            if (t.size() > str.size() || str.substr(0, t.size()) != t) return false;
            if (wordPatternMatch(pattern.substr(1), str.substr(t.size()))) return true;
        } else {
            for (int i = 1; i <= str.size(); ++i) {
                if (s.count(str.substr(0, i))) continue;
                m[pattern[0]] = str.substr(0, i);
                s.insert(str.substr(0, i));
                if (wordPatternMatch(pattern.substr(1), str.substr(i))) return true;
                m.erase(pattern[0]);
                s.erase(str.substr(0, i));
            }
        }
        return false;
    }
    unordered_map<char, string> m;
    unordered_set<string> s;
};
```



类似题目：

[Word Pattern](http://www.cnblogs.com/grandyang/p/4857022.html)



参考资料：

[https://leetcode.com/problems/word-pattern-ii/](https://leetcode.com/problems/word-pattern-ii/)

[https://leetcode.com/problems/word-pattern-ii/discuss/73721/My-simplified-java-version](https://leetcode.com/problems/word-pattern-ii/discuss/73721/My-simplified-java-version)

[https://leetcode.com/problems/word-pattern-ii/discuss/73664/Share-my-Java-backtracking-solution](https://leetcode.com/problems/word-pattern-ii/discuss/73664/Share-my-Java-backtracking-solution)[](https://leetcode.com/discuss/81648/my-simplified-java-version)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












