# [LeetCode] Pyramid Transition Matrix 金字塔转变矩阵 - Grandyang - 博客园







# [[LeetCode] Pyramid Transition Matrix 金字塔转变矩阵](https://www.cnblogs.com/grandyang/p/8476646.html)







We are stacking blocks to form a pyramid. Each block has a color which is a one letter string, like `'Z'`.

For every block of color `C` we place not in the bottom row, we are placing it on top of a left block of color `A` and right block of color `B`. We are allowed to place the block there only if `(A, B, C)` is an allowed triple.

We start with a bottom row of `bottom`, represented as a single string. We also start with a list of allowed triples `allowed`. Each allowed triple is represented as a string of length 3.

Return true if we can build the pyramid all the way to the top, otherwise false.

Example 1:
Input: bottom = "XYZ", allowed = ["XYD", "YZE", "DEA", "FFF"]
Output: true
Explanation:
We can stack the pyramid like this:
    A
   / \
  D   E
 / \ / \
X   Y   Z

This works because ('X', 'Y', 'D'), ('Y', 'Z', 'E'), and ('D', 'E', 'A') are allowed triples.



Example 2:
Input: bottom = "XXYX", allowed = ["XXX", "XXY", "XYX", "XYY", "YXZ"]
Output: false
Explanation:
We can't stack the pyramid to the top.
Note that there could be allowed triples (A, B, C) and (A, B, D) with C != D.



Note:
- `bottom` will be a string with length in range `[2, 8]`.
- `allowed` will have length in range `[0, 200]`.
- Letters in all strings will be chosen from the set `{'A', 'B', 'C', 'D', 'E', 'F', 'G'}`.



这道题让我们累一个金字塔，用字母来代表每块砖的颜色，给了一个allowed数组，里面都是长度为三的字符串，比如“ABC”表示C可以放在A和B的上方，注意AB上面也可以放其他的，比如“ABD”也可以同时出现，不过搭积木的时候只能选择一种。给了我们一个bottom字符串，是金字塔的底层，问我们能不能搭出一个完整的金字塔。那么实际上我们就是从底层开始，一层一层的向上来累加，直到搭出整个金字塔。我们先来看递归的解法，首先由于我们想快速知道两个字母上方可以放的字母，需要建立基座字符串和上方字符集合之间的映射，由于上方字符可以不唯一，所以用个HashSet来放字符。我们的递归函数有三个参数，当前层字符串cur，上层字符串above，还有我们的HashMap。如果cur的大小为2，above的大小为1，那么说明当前已经达到金字塔的顶端了，已经搭出来了，直接返回true。否则看，如果上一层的长度比当前层的长度正好小一个，说明上一层也搭好了，我们现在去搭上上层，于是调用递归函数，将above当作当前层，空字符串为上一层，将调用的递归函数结果直接返回。否则表示我们还需要继续去搭above层，我们先算出above层的长度pos，然后从当前层的pos位置开始取两个字符，就是above层接下来需要搭的字符的基座字符，举个例子如下：
  D   
 / \ / \
A   B   C
我们看到现在above层只有一个D，那么pos为1，在cur层1位置开始取两个字符，得到"BC"，即是D的下一个位置的字符的基座字符串base。取出了base后，如果HashMap中有映射，则我们遍历其映射的字符集合中的所有字符，对每个字符都调用递归函数，此时above字符串需要加上这个遍历到的字符，因为我们在尝试填充这个位置，如果有返回true的，那么当前递归函数就返回true了，否则最终返回false，参见代码如下：



解法一：

```
class Solution {
public:
    bool pyramidTransition(string bottom, vector<string>& allowed) {   
        unordered_map<string, unordered_set<char>> m;
        for (string str : allowed) {
            m[str.substr(0, 2)].insert(str[2]);
        }
        return helper(bottom, "", m);
    }
    bool helper(string cur, string above, unordered_map<string, unordered_set<char>>& m) {
        if (cur.size() == 2 && above.size() == 1) return true;
        if (above.size() == cur.size() - 1) return helper(above, "", m);
        int pos = above.size();
        string base = cur.substr(pos, 2);
        if (m.count(base)) {
            for (char ch : m[base]) {
                if (helper(cur, above + string(1, ch), m)) {
                    return true;
                }
            }
        }
        return false;
    }
};
```



下面来看一种迭代的写法，这是一种DP的解法，建立一个三维的dp数组，其中dp[i][j][ch]表示在金字塔(i, j)位置上是否可以放字符ch，金字塔的宽和高已经确定了，都是n。每个位置对应着nxn的数组的左半边，如下所示：

F _ _
D E _
A B C

除了底层，每个位置可能可以放多个字符，所以这里dp数组是一个三维数组，第三维的长度为7，因为题目中限定了字母只有A到G共7个，如果dp值为true，表示该位置放该字母，我们根据bottom字符串来初始化dp数组的底层。这里还是需要一个HashMap，不过跟上面的解法略有不同的是，我们建立上方字母跟其能放的基座字符串集合的映射，因为一个字母可能可以放多个位置，所以用个集合来表示。然后我们就开始从倒数第二层开始往顶部更新啦，对于金字塔的每个位置，我们都遍历A到G中所有的字母，如果当前字母在HashMap中有映射，则我们遍历对应的基座字符串集合中的所有字符串，基座字符串共有两个字母，左边的字母对应的金字塔中的位置是(i + 1, j)，右边的字母对应的位置是(i + 1, j + 1)，我们只要在这两个位置上分别查询对应的字母的dp值是否为true，是的话，说明当前位置有字母可以放，我们将当前位置的字母对应的dp值赋值为true。这样，当我们整个更新完成了之后，我们只要看金字塔顶端位置(0, 0)是否有字母可以放，有的话，说明可以搭出金字塔，返回true，否则返回false，参见代码如下：



解法二：

```
class Solution {
public:
    bool pyramidTransition(string bottom, vector<string>& allowed) {
        int n = bottom.size();
        vector<vector<vector<bool>>> dp(n, vector<vector<bool>>(n, vector<bool>(7, false)));
        unordered_map<char, unordered_set<string>> m;
        for (string str : allowed) {
            m[str[2]].insert(str.substr(0, 2));
        }
        for (int i = 0; i < n; ++i) {
            dp[n - 1][i][bottom[i] - 'A'] = true;
        }
        for (int i = n - 2; i >= 0; --i) {
            for (int j = 0; j <= i; ++j) {
                for (char ch = 'A'; ch <= 'G'; ++ch) {
                    if (!m.count(ch)) continue;
                    for (string str : m[ch]) {
                        if (dp[i + 1][j][str[0] - 'A'] && dp[i + 1][j + 1][str[1] - 'A']) {
                            dp[i][j][ch - 'A'] = true;
                        }
                    }
                }
            }
        }
        for (int i = 0; i < 7; ++i) {
            if (dp[0][0][i]) return true;
        }
        return false;
    }
};
```



参考资料：

[https://leetcode.com/problems/pyramid-transition-matrix/discuss/113075/DP-O(n2-*-m)](https://leetcode.com/problems/pyramid-transition-matrix/discuss/113075/DP-O(n2-*-m))

[https://leetcode.com/problems/pyramid-transition-matrix/discuss/113036/A-map-based-solution-based-on-memoization](https://leetcode.com/problems/pyramid-transition-matrix/discuss/113036/A-map-based-solution-based-on-memoization)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












