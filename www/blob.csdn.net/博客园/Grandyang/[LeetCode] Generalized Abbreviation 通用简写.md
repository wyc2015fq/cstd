# [LeetCode] Generalized Abbreviation 通用简写 - Grandyang - 博客园







# [[LeetCode] Generalized Abbreviation 通用简写](https://www.cnblogs.com/grandyang/p/5261569.html)







Write a function to generate the generalized abbreviations of a word.

Example:

Given word = `"word"`, return the following list (order does not matter):
["word", "1ord", "w1rd", "wo1d", "wor1", "2rd", "w2d", "wo2", "1o1d", "1or1", "w1r1", "1o2", "2r1", "3d", "w3", "4"]


这道题让我们对一个单词进行部分简写，简写的规则是若干个字母可以用数字来表示，但是不能有两个相邻的数字，具体可以参考题目中给的例子，根据我以往的经验，这种列举所有情况的必定是要用DFS来写的，但是我一时半会又没想到该咋递归，后来我数了一下题目中给的例子的所有情况的个数，是16个，而word有4个字母，刚好是2的4次方，这是巧合吗，当然不是，后来我又发现如果把0到15的二进制写出来，每一个可以对应一种情况，如下所示：

0000 word
0001 wor1
0010 wo1d
0011 wo2
0100 w1rd
0101 w1r1
0110 w2d
0111 w3
1000 1ord
1001 1or1
1010 1o1d
1011 1o2
1100 2rd
1101 2r1
1110 3d
1111 4



那么我们就可以观察出规律，凡是0的地方都是原来的字母，单独的1还是1，如果是若干个1连在一起的话，就要求出1的个数，用这个数字来替换对应的字母，既然规律找出来了，那么代码就很好写了，如下所示：



解法一：

```
class Solution {
public:
    vector<string> generateAbbreviations(string word) {
        vector<string> res;
        for (int i = 0; i < pow(2, word.size()); ++i) {
            string out = "";
            int cnt = 0, t = i;
            for (int j = 0; j < word.size(); ++j) {
                if (t & 1 == 1) {
                    ++cnt;
                    if (j == word.size() - 1) out += to_string(cnt);
                } else {
                    if (cnt != 0) {
                        out += to_string(cnt);
                        cnt = 0;
                    }
                    out += word[j];
                }
                t >>= 1;
            }
            res.push_back(out);
        }
        return res;
    }
};
```



上述方法返回结果的顺序为：

["word","1ord","w1rd","2rd","wo1d","1o1d","w2d","3d","wor1","1or1","w1r1","2r1","wo2","1o2","w3","4"]

我们可以对上面代码稍稍改写一下，变的稍微简洁一点： 



解法二：

```
class Solution {
public:
    vector<string> generateAbbreviations(string word) {
        vector<string> res;
        for (int i = 0; i < pow(2, word.size()); ++i) {
            string out = "";
            int cnt = 0;
            for (int j = 0; j < word.size(); ++j) {
                if ((i >> j) & 1) ++cnt;
                else {
                    if (cnt != 0) {
                        out += to_string(cnt);
                        cnt = 0;
                    }
                    out += word[j];
                }
            }
            if (cnt > 0) out += to_string(cnt);
            res.push_back(out);
        }
        return res;
    }
};
```



那么迭代的写法看完了，来考虑一些递归的写法吧，上网搜了一下，发现下面三种写法比较容易理解， 



解法三：

```
class Solution {
public:
    vector<string> generateAbbreviations(string word) {
        vector<string> res{word};
        helper(word, 0, res);
        return res;
    }
    void helper(string word, int pos, vector<string> &res) {
        for (int i = pos; i < word.size(); ++i) {
            for (int j = 1; i + j <= word.size(); ++j) {
                string t = word.substr(0, i);
                t += to_string(j) + word.substr(i + j);
                res.push_back(t);
                helper(t, i + 1 + to_string(j).size(), res);
            }
        }
    }
};
```



上述方法返回结果的顺序为：

["word","1ord","1o1d","1o2","1or1","2rd","2r1","3d","4","w1rd","w1r1","w2d","w3","wo1d","wo2","wor1"]



解法四：

```
class Solution {
public:
    vector<string> generateAbbreviations(string word) {
        vector<string> res;
        helper(word, 0, 0, "", res);
        return res;
    }
    void helper(string word, int pos, int cnt, string out, vector<string> &res) {
        if (pos == word.size()) {
            if (cnt > 0) out += to_string(cnt);
            res.push_back(out);
        } else {
            helper(word, pos + 1, cnt + 1, out, res);
            helper(word, pos + 1, 0, out + (cnt > 0 ? to_string(cnt) : "") + word[pos], res);
        }
    }
};
```



上述方法返回结果的顺序为：

 ["4","3d","2r1","2rd","1o2","1o1d","1or1","1ord","w3","w2d","w1r1","w1rd","wo2","wo1d","wor1","word"]



解法五：

```
class Solution {
public:
    vector<string> generateAbbreviations(string word) {
        vector<string> res;
        res.push_back(word.size() == 0 ? "" : to_string(word.size()));
        for (int i = 0; i < word.size(); ++i) {
            for (auto a : generateAbbreviations(word.substr(i + 1))) {
                string left = i > 0 ? to_string(i) : "";
                res.push_back(left + word.substr(i, 1) + a);
            }
        }
        return res;
    }
};
```



上述方法返回结果的顺序为：

["4","w3","wo2","wor1","word","wo1d","w1r1","w1rd","w2d","1o2","1or1","1ord","1o1d","2r1","2rd","3d"]



参考资料：

[https://leetcode.com/discuss/75528/9-line-easy-java-solution](https://leetcode.com/discuss/75528/9-line-easy-java-solution)

[https://leetcode.com/discuss/75754/java-backtracking-solution](https://leetcode.com/discuss/75754/java-backtracking-solution)

[https://leetcode.com/discuss/77280/c-straightforward-recursive-solution](https://leetcode.com/discuss/77280/c-straightforward-recursive-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












