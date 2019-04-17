# [LeetCode] Letter Case Permutation 字母大小写全排列 - Grandyang - 博客园







# [[LeetCode] Letter Case Permutation 字母大小写全排列](https://www.cnblogs.com/grandyang/p/9065702.html)







Given a string S, we can transform every letter individually to be lowercase or uppercase to create another string.  Return a list of all possible strings we could create.
Examples:
Input: S = "a1b2"
Output: ["a1b2", "a1B2", "A1b2", "A1B2"]

Input: S = "3z4"
Output: ["3z4", "3Z4"]

Input: S = "12345"
Output: ["12345"]

Note:
- `S` will be a string with length at most `12`.
- `S` will consist only of letters or digits.



这道题给了我们一个只包含字母和数字的字符串，让我们将字母以大小写进行全排列，给的例子很好的说明了题意。博主认为这道题给Easy有点不合适，至少应该是Medium的水准。这题主要参考了[官方解答贴](https://leetcode.com/problems/letter-case-permutation/solution/)的解法，我们关心的是字母，数字的处理很简单，直接加上就可以了。比如说S = "abc"，那么先让 res = [""]，然后res中的每个字符串分别加上第一个字符a和A，得到 ["a", "A"]，然后res中的每个字符串分别加上第二个字符b和B，得到 ["ab", "Ab", "aB", "AB"]，然后res中的每个字符串分别加上第三个字符c和C，得到 ["abc", "Abc", "aBc", "ABc", "abC", "AbC", "aBC", "ABC"]，参见代码如下：



解法一：

```
class Solution {
public:
    vector<string> letterCasePermutation(string S) {
        vector<string> res{""};
        for (char c : S) {
            int len = res.size();
            if (c >= '0' && c <= '9') {
                for (string &str : res) str.push_back(c);
            } else {
                for (int i = 0; i < len; ++i) {
                    res.push_back(res[i]);
                    res[i].push_back(tolower(c));
                    res[len + i].push_back(toupper(c));
                }
            }
        }
        return res;
    }
};
```



下面这种解法跟上面的解法没有太大的区别，只不过没有用到tolower()和toupper()这两个built-in函数，而是使用了一个trick来flip大小写字母，通过亦或32实现，为什么呢？因为我们知道 'A' = 65, 'B' = 66, 和 'a' = 97, 'b' = 98, 小写字母的ASCII码比大写字母多32，刚好是(1 << 5)，那么我们只要flip第五位上的1，就可以实现大小写的交替了，参见代码如下：



解法二：

```
class Solution {
public:
    vector<string> letterCasePermutation(string S) {
        vector<string> res{""};
        for (char c : S) {
            int len = res.size();
            if (c >= '0' && c <= '9') {
                for (string &str : res) str.push_back(c);
            } else {
                for (int i = 0; i < len; ++i) {
                    res.push_back(res[i]);
                    res[i].push_back(c);
                    res[len + i].push_back(c ^ 32);
                }
            }
        }
        return res;
    }
};
```



我们再来看一种递归的写法，是一种回溯的思路，比如说S = "abc"，用一个pos指向当前处理的位置，初始化带入0，然后再递归函数中，如果pos等于s的长度了，那么将s存入结果res再返回；否则调用递归函数，此时带入pos+1，那么递归函数就会一直深入，直到pos等于s的长度了，那么此时就把"abc"存入结果res了，返回后此时pos=2，发现s[pos]是字母，则用上面解法中的flip方法来翻转字母，并且调用递归函数，这样"abC"就会存入结果res中，然后回溯到pos=1的位置，s[pos]是字符，可以flip，并且调用递归函数，这样"aBC"就会存入结果res中，然后pos继续往后遍历，这样"aBc"就会存入结果res中，然后回溯到pos=0的位置，s[pos]是字符，可以flip，并且调用递归函数，这样"ABc"就会存入结果res中，然后继续回溯，这样"ABC"就会存入结果res中，pos又回溯到1的位置，s[pos]是字符，可以flip，并且调用递归函数，这样"AbC"就会存入结果res中，然后pos继续往后遍历，这样"Abc"就会存入结果res中，整个的顺序为：[abc abC aBC aBc ABc ABC AbC Abc]，参见代码如下：



解法三：

```
class Solution {
public:
    vector<string> letterCasePermutation(string S) {
        vector<string> res;
        helper(S, 0, res);
        return res;
    }
    void helper(string& s, int pos, vector<string>& res) {
        if (pos == s.size()) {
            res.push_back(s);
            return;
        }
        helper(s, pos + 1, res);
        if (s[pos] > '9') {
            s[pos] ^= 32;
            helper(s, pos + 1, res);
        }
    }
};
```



下面这种解法是[官方解答贴](https://leetcode.com/problems/letter-case-permutation/solution/)的Binary Mask解法，感觉也很巧妙，如果我们仔细观察S = "abc"这个例子，结果会返回8个，为什么是8个呢，因为每个字母都有大小写两种可能，那么三个字母就有2x2x2=8，正好是2的三次方，那么不正好和二进制数相对应么，如果1对应大写字母，0对应小写字母，则有：

```
000 -> ABC
001 -> ABc
010 -> AbC
011 -> Abc
100 -> aBC
101 -> aBc
110 -> abC
111 -> abc
```

这样的话，我们只需要先统计出S中字母的个数cnt，然后遍历 [0, 2^cnt) 中的每个数字，对于每个数字，再遍历S中的每个字符，如果是字母，那么如果当前位是1，则加入小写字母，反之加入大写字母；如果是数字，则直接加入即可。我们用j指向位，每次处理完一位后j自增1，表示对下一位进行处理，参见代码如下：



解法四：

```
class Solution {
public:
    vector<string> letterCasePermutation(string S) {
        vector<string> res;
        int cnt = 0;
        for (char c : S) {
            if (c > '9') ++cnt;
        }
        for (int i = 0; i < (1 << cnt); ++i) {
            int j = 0;
            string word = "";
            for (char c : S) {
                if (c > '9') {
                    if (((i >> j++) & 1) == 1) {
                        word.push_back(tolower(c));
                    } else {
                        word.push_back(toupper(c));
                    }
                } else {
                    word.push_back(c);
                }
            }
            res.push_back(word);
        }
        return res;
    }
};
```



类似题目：

[Subsets](http://www.cnblogs.com/grandyang/p/4309345.html)



参考资料：

[https://leetcode.com/problems/letter-case-permutation/solution/](https://leetcode.com/problems/letter-case-permutation/solution/)

[https://leetcode.com/problems/letter-case-permutation/discuss/115515/C++-backtrack-solution-w-trick](https://leetcode.com/problems/letter-case-permutation/discuss/115515/C++-backtrack-solution-w-trick)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












