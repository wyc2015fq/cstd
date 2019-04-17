# [LeetCode] Bold Words in String 字符串中的加粗单词 - Grandyang - 博客园







# [[LeetCode] Bold Words in String 字符串中的加粗单词](https://www.cnblogs.com/grandyang/p/8531642.html)







Given a set of keywords `words` and a string `S`, make all appearances of all keywords in `S` bold. Any letters between `<b>` and `</b>` tags become bold.

The returned string should use the least number of tags possible, and of course the tags should form a valid combination.

For example, given that `words = ["ab", "bc"]` and `S = "aabcd"`, we should return `"a<b>abc</b>d"`. Note that returning `"a<b>a<b>b</b>c</b>d"` would use more tags, so it is incorrect.

Note:
- `words` has length in range `[0, 50]`.
- `words[i]` has length in range `[1, 10]`.
- `S` has length in range `[0, 500]`.
- All characters in `words[i]` and `S` are lowercase letters.



这道题跟之前的那道[Add Bold Tag in String](http://www.cnblogs.com/grandyang/p/7043394.html)是一模一样的，之前还换个马甲，这次连场景都不换了，直接照搬啊？！我也是服气的～这道题应该没有太多的技巧，就是照题目意思来就行了，我们使用一个数组bold，标记所有需要加粗的位置为true，初始化所有为false。我们首先要判断每个单词word是否是S的子串，判断的方法就是逐个字符比较，遍历字符串S，找到和word首字符相等的位置，并且比较随后和word等长的子串，如果完全相同，则将子串所有的位置在bold上比较为true。等我们知道了所有需要加粗的位置后，我们就可以来生成结果res了，我们遍历bold数组，如果当前位置是true的话，表示需要加粗，那么我们首先看如果是第一个字符，或者其前面的字符不用加粗，我们加上一个左标签<b>，然后我们将当前字符加入结果res中，然后再判断，如果当前是末尾字符，或者后面一个字符不用加粗，则需要加上一个右标签</b>；如果当前位置是false，我们直接将字符加入结果res中即可，参见代码如下：



解法一：

```
class Solution {
public:
    string boldWords(vector<string>& words, string S) {
        int n = S.size();
        string res = "";
        vector<bool> bold(n, false);      
        for (string word : words) {
            int len = word.size();
            for (int i = 0; i <= n - len; ++i) {
                if (S[i] == word[0] && S.substr(i, len) == word) {
                    for (int j = i; j < i + len; ++j) bold[j] = true;
                }
            }
        }
        for (int i = 0; i < n; ++i) {
            if (bold[i]) {
                if (i == 0 || !bold[i - 1]) res += "<b>";
                res.push_back(S[i]);
                if (i == n - 1 || !bold[i + 1]) res += "</b>";
            } else {
                res.push_back(S[i]);
            }
        }
        return res;
    }
};
```



我们可以用HashSet来代替数组，只是将需要加粗的位置放入HashSet，然后我们在生成结果res的时候，先检测当前位置是否加粗，如果加粗了，并且前一个位置不在HashSet中，这样就不用判断是否是第一个元素了，因为i-1肯定不再HashSet中，也不像数组那样存在越界的可能，我们给结果res加上左标签，然后将当前的字符加入结果res中，然后再判断如果当前位置如果加粗了，并且下一个位置不在HashSet中，我们给结果res加上右标签，参见代码如下： 



解法二：

```
class Solution {
public:
    string boldWords(vector<string>& words, string S) {
        int n = S.size();
        string res = "";
        unordered_set<int> bold;      
        for (string word : words) {
            int len = word.size();
            for (int i = 0; i <= n - len; ++i) {
                if (S[i] == word[0] && S.substr(i, len) == word) {
                    for (int j = i; j < i + len; ++j) bold.insert(j);
                }
            }
        }
        for (int i = 0; i < n; ++i) {
            if (bold.count(i) && !bold.count(i - 1)) res += "<b>";
            res.push_back(S[i]);
            if (bold.count(i) && !bold.count(i + 1)) res += "</b>";
        }
        return res;
    }
};
```



前面提到了这道题跟[Add Bold Tag in String](http://www.cnblogs.com/grandyang/p/7043394.html)是完全一样，那么当然二者的解法是互通的，下面的解法是之前那道题中的解法，其实整体思路是一样的，只不过在构建的bold数组的时候，是先遍历的字符串S，而不是先遍历的单词。对于字符串S中的每个字符为起点，我们都遍历下所有单词，如果某个单词是以当前字符为起点的子串的话，那么我们用i+len来更新end，所以遍历完所有单词后，只要当前位置需要加粗，那么end一定大于i，通过这种方法同样也可以生成正确的bold数组。然后在创建结果res字符串的时候也跟上面的方法有些不同，首先判断，如果当前未被加粗，那么将当前字符存入结果res中并且continue，否则开始找相连的需要加粗的位置，用j来指向下一个不用加粗的位置，这样中间的子串就可以放入标签中整体加到res中，然后继续在后面查找连续加粗的子串，参见代码如下：



解法三：

```
class Solution {
public:
    string boldWords(vector<string>& words, string S) {
        int n = S.size(), end = 0;
        string res = "";
        vector<bool> bold(n, false);      
        for (int i = 0; i < n; ++i) {
            for (string word : words) {
                int len = word.size();
                if (i + len <= n && S.substr(i, len) == word) {
                    end = max(end, i + len);
                }
            }
            bold[i] = end > i;
        }
        for (int i = 0; i < n; ++i) {
            if (!bold[i]) {
                res.push_back(S[i]);
                continue;
            }
            int j = i;
            while (j < n && bold[j]) ++j;
            res += "<b>" + S.substr(i, j - i) + "</b>";
            i = j - 1;
        }
        return res;
    }
};
```



类似题目：

[Add Bold Tag in String](http://www.cnblogs.com/grandyang/p/7043394.html)



参考资料：

[https://leetcode.com/problems/bold-words-in-string/discuss/113107/Java-Solution-without-HashMap](https://leetcode.com/problems/bold-words-in-string/discuss/113107/Java-Solution-without-HashMap)

[https://leetcode.com/problems/bold-words-in-string/discuss/113093/Clean-Python-and-C++-solutions-with-hashset-to-store-bold-indices](https://leetcode.com/problems/bold-words-in-string/discuss/113093/Clean-Python-and-C++-solutions-with-hashset-to-store-bold-indices)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












