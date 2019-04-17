# [LeetCode] Word Pattern 词语模式 - Grandyang - 博客园







# [[LeetCode] Word Pattern 词语模式](https://www.cnblogs.com/grandyang/p/4857022.html)







Given a `pattern` and a string `str`, find if `str` follows the same pattern.

Here follow means a full match, such that there is a bijection between a letter in `pattern` and a non-empty word in `str`.

Example 1:
Input: pattern = `"abba"`, str = `"dog cat cat dog"`
Output: true
Example 2:
Input:pattern = `"abba"`, str = `"dog cat cat fish"`
Output: false
Example 3:
Input: pattern = `"aaaa"`, str = `"dog cat cat dog"`
Output: false
Example 4:
Input: pattern = `"abba"`, str = `"dog dog dog dog"`
Output: false
Notes:
You may assume `pattern` contains only lowercase letters, and `str` contains lowercase letters separated by a single space.

**Credits:**
Special thanks to [@minglotus6](https://leetcode.com/discuss/user/minglotus6) for adding this problem and creating all test cases.



这道题给我们一个模式字符串，又给我们一个单词字符串，让我们求单词字符串中单词出现的规律是否符合模式字符串中的规律。那么首先想到就是用HashMap来做，建立模式字符串中每个字符和单词字符串每个单词之间的映射，而且这种映射必须是一对一关系的，不能 'a' 和 'b' 同时对应 'dog'，也不能 'a' 同时对应到 'dog' 和 'cat'，所以我们在碰到一个新字符时，首先检查其是否在HashMap中出现，若出现，其映射的单词若不是此时对应的单词，则返回false。如果没有在HashMap中出现，我们还要遍历一遍HashMap，看新遇到的单词是否已经是其中的映射，若已经有其他映射，直接返回false，如果没有，再跟新遇到的字符建立映射。最后循环退出后，要检查此时的 i 是否和 n 相同，这是检查一对一映射的最后一步，因为当str中的单词处理完了之后，pattern中就不能有多余的字符了，参见代码如下：



解法一：

```
class Solution {
public:
    bool wordPattern(string pattern, string str) {
        unordered_map<char, string> m;
        istringstream in(str);
        int i = 0, n = pattern.size();
        for (string word; in >> word; ++i) {
            if (m.count(pattern[i])) {
                if (m[pattern[i]] != word) return false;
            } else {
                for (auto a : m) {
                    if (a.second == word) return false;
                }
                m[pattern[i]] = word;
            }
        }
        return i == n;
    }
};
```



当然这道题也可以用两个HashMap来完成，分别将字符和单词都映射到当前的位置加1，注意这里需要加1就是为了避免默认映射值0，因为C++中的HashMap的机制是若访问一个不存在的key值，会默认建立一个映射值为0的映射。那么我们在遇到新字符和单词时，首先看 i 是否已经是 n 了，若相等了，说明此时pattern中的字符已经用完了，而str中还有多余的单词，这样是无法建立一对一映射的，直接返回false。还有当两个HashMap的映射值不相同时也返回false，否则我们同时建立单词和pattern字符和当前位置加1之间的映射，循环推出后还是要检测 i 是否和 n 相等，参见代码如下：



解法二：

```
class Solution {
public:
    bool wordPattern(string pattern, string str) {
        unordered_map<char, int> m1;
        unordered_map<string, int> m2;
        istringstream in(str);
        int i = 0, n = pattern.size();
        for (string word; in >> word; ++i) {
            if (i == n || m1[pattern[i]] != m2[word]) return false;
            m1[pattern[i]] = m2[word] = i + 1;
        }
        return i == n;
    }
};
```



参考资料：

[https://leetcode.com/problems/word-pattern/](https://leetcode.com/problems/word-pattern/)

[https://leetcode.com/problems/word-pattern/discuss/73402/8-lines-simple-Java](https://leetcode.com/problems/word-pattern/discuss/73402/8-lines-simple-Java)

[https://leetcode.com/problems/word-pattern/discuss/73409/Short-C%2B%2B-read-words-on-the-fly](https://leetcode.com/problems/word-pattern/discuss/73409/Short-C%2B%2B-read-words-on-the-fly)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












