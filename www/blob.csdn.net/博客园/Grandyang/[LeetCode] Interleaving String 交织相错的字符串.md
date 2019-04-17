# [LeetCode] Interleaving String 交织相错的字符串 - Grandyang - 博客园







# [[LeetCode] Interleaving String 交织相错的字符串](https://www.cnblogs.com/grandyang/p/4298664.html)







Given *s1*, *s2*, *s3*, find whether *s3* is formed by the interleaving of *s1* and *s2*.

 For example,

Given:
*s1* = `"aabcc"`,
*s2* = `"dbbca"`,



When *s3* = `"aadbbcbcac"`, return true.

When *s3* = `"aadbbbaccc"`, return false.




这道求交织相错的字符串和之前那道[Word Break 拆分词句](http://www.cnblogs.com/grandyang/p/4257740.html) 的题很类似，就想我之前说的只要是遇到字符串的子序列或是匹配问题直接就上动态规划Dynamic Programming，其他的都不要考虑，什么递归呀的都是浮云，千辛万苦的写了递归结果拿到OJ上妥妥Time Limit Exceeded，能把人气昏了，所以还是直接就考虑DP解法省事些。一般来说字符串匹配问题都是更新一个二维dp数组，核心就在于找出递推公式。那么我们还是从题目中给的例子出发吧，手动写出二维数组dp如下：



```
Ø d b b c a
Ø T F F F F F
a T F F F F F
a T T T T T F
b F T T F T F
c F F T T T T
c F F F T F T
```



首先，这道题的大前提是字符串s1和s2的长度和必须等于s3的长度，如果不等于，肯定返回false。那么当s1和s2是空串的时候，s3必然是空串，则返回true。所以直接给dp[0][0]赋值true，然后若s1和s2其中的一个为空串的话，那么另一个肯定和s3的长度相等，则按位比较，若相同且上一个位置为True，赋True，其余情况都赋False，这样的二维数组dp的边缘就初始化好了。下面只需要找出递推公式来更新整个数组即可，我们发现，在任意非边缘位置dp[i][j]时，它的左边或上边有可能为True或是False，两边都可以更新过来，只要有一条路通着，那么这个点就可以为True。那么我们得分别来看，如果左边的为True，那么我们去除当前对应的s2中的字符串s2[j - 1] 和 s3中对应的位置的字符相比（计算对应位置时还要考虑已匹配的s1中的字符），为s3[j - 1 + i], 如果相等，则赋True，反之赋False。 而上边为True的情况也类似，所以可以求出递推公式为：

dp[i][j] = (dp[i - 1][j] && s1[i - 1] == s3[i - 1 + j]) || (dp[i][j - 1] && s2[j - 1] == s3[j - 1 + i]);

其中dp[i][j] 表示的是 s2 的前 i 个字符和 s1 的前 j 个字符是否匹配 s3 的前 i+j 个字符，根据以上分析，可写出代码如下：



解法一：

```
class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        if (s1.size() + s2.size() != s3.size()) return false;
        int n1 = s1.size();
        int n2 = s2.size();
        vector<vector<bool> > dp(n1 + 1, vector<bool> (n2 + 1, false)); 
        dp[0][0] = true;
        for (int i = 1; i <= n1; ++i) {
            dp[i][0] = dp[i - 1][0] && (s1[i - 1] == s3[i - 1]);
        }
        for (int i = 1; i <= n2; ++i) {
            dp[0][i] = dp[0][i - 1] && (s2[i - 1] == s3[i - 1]);
        }
        for (int i = 1; i <= n1; ++i) {
            for (int j = 1; j <= n2; ++j) {
                dp[i][j] = (dp[i - 1][j] && s1[i - 1] == s3[i - 1 + j]) || (dp[i][j - 1] && s2[j - 1] == s3[j - 1 + i]);
            }
        }
        return dp[n1][n2];
    }
};
```



我们也可以把for循环合并到一起，用if条件来处理边界情况，整体思路和上面的解法没有太大的区别，参见代码如下：



解法二：

```
class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        if (s1.size() + s2.size() != s3.size()) return false;
        int n1 = s1.size(), n2 = s2.size();
        vector<vector<bool> > dp(n1 + 1, vector<bool> (n2 + 1, false)); 
        for (int i = 0; i <= n1; ++i) {
            for (int j = 0; j <= n2; ++j) {
                if (i == 0 && j == 0) {
                    dp[i][j] = true;
                } else if (i == 0) {
                    dp[i][j] = dp[i][j - 1] && s2[j - 1] == s3[i + j - 1];
                } else if (j == 0) {
                    dp[i][j] = dp[i - 1][j] && s1[i - 1] == s3[i + j - 1];
                } else {
                    dp[i][j] = (dp[i - 1][j] && s1[i - 1] == s3[i + j - 1]) || (dp[i][j - 1] && s2[j - 1] == s3[i + j - 1]);
                }
            }
        }
        return dp[n1][n2];
    }
};
```



这道题也可以使用带优化的DFS来做，我们使用一个哈希集合，用来保存匹配失败的情况，我们分别用变量i，j，和k来记录字符串s1，s2，和s3匹配到的位置，初始化的时候都传入0。在递归函数中，首先根据i和j，算出key值，由于我们的哈希集合中只能放一个数字，而我们要encode两个数字i和j，所以通过用i乘以s3的长度再加上j来得到key，此时我们看，如果key已经在集合中，直接返回false，因为集合中存的是无法匹配的情况。然后先来处理corner case的情况，如果i等于s1的长度了，说明s1的字符都匹配完了，此时s2剩下的字符和s3剩下的字符可以直接进行匹配了，所以我们直接返回两者是否能匹配的bool值。同理，如果j等于s2的长度了，说明s2的字符都匹配完了，此时s1剩下的字符和s3剩下的字符可以直接进行匹配了，所以我们直接返回两者是否能匹配的bool值。如果s1和s2都有剩余字符，那么当s1的当前字符等于s3的当前字符，那么调用递归函数，注意i和k都加上1，如果递归函数返回true，则当前函数也返回true；还有一种情况是，当s2的当前字符等于s3的当前字符，那么调用递归函数，注意j和k都加上1，如果递归函数返回true，那么当前函数也返回true。如果匹配失败了，则将key加入集合中，并返回false即可，参见代码如下：



解法三：

```
class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        if (s1.size() + s2.size() != s3.size()) return false;
        unordered_set<int> s;
        return helper(s1, 0, s2, 0, s3, 0, s);
    }
    bool helper(string& s1, int i, string& s2, int j, string& s3, int k, unordered_set<int>& s) {
        int key = i * s3.size() + j;
        if (s.count(key)) return false;
        if (i == s1.size()) return s2.substr(j) == s3.substr(k);
        if (j == s2.size()) return s1.substr(i) == s3.substr(k);
        if ((s1[i] == s3[k] && helper(s1, i + 1, s2, j, s3, k + 1, s)) || 
            (s2[j] == s3[k] && helper(s1, i, s2, j + 1, s3, k + 1, s))) return true;
        s.insert(key);
        return false;
    }
};
```



既然DFS可以，那么BFS也就坐不住了，也要出来浪一波。这里我们需要用队列queue来辅助运算，如果将解法一讲解中的那个二维dp数组列出来的TF图当作一个迷宫的话，那么BFS的目的就是要从(0, 0)位置找一条都是T的路径通到(n1, n2)位置，这里我们还要使用哈希集合，不过此时保存到是已经遍历过的位置，队列中还是存key值，key值的encode方法跟上面DFS解法的相同，初识时放个0进去。然后我们进行while循环，循环条件除了q不为空，还有一个是k小于n3，因为匹配完s3中所有的字符就结束了。然后由于是一层层的遍历，所以要直接循环queue中元素个数的次数，在for循环中，对队首元素进行解码，得到i和j值，如果i小于n1，说明s1还有剩余字符，如果s1当前字符等于s3当前字符，那么把s1的下一个位置i+1跟j一起加码算出key值，如果该key值不在于集合中，则加入集合，同时加入队列queue中；同理，如果j小于n2，说明s2还有剩余字符，如果s2当前字符等于s3当前字符，那么把s2的下一个位置j+1跟i一起加码算出key值，如果该key值不在于集合中，则加入集合，同时加入队列queue中。for循环结束后，k自增1。最后如果匹配成功的话，那么queue中应该只有一个(n1, n2)的key值，且k此时等于n3，所以当queue为空或者k不等于n3的时候都要返回false，参见代码如下：



解法四：

```
class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        if (s1.size() + s2.size() != s3.size()) return false;
        int n1 = s1.size(), n2 = s2.size(), n3 = s3.size(), k = 0;
        unordered_set<int> s;
        queue<int> q{{0}};
        while (!q.empty() && k < n3) {
            int len = q.size();
            for (int t = 0; t < len; ++t) {
                int i = q.front() / n3, j = q.front() % n3; q.pop();
                if (i < n1 && s1[i] == s3[k]) {
                    int key = (i + 1) * n3 + j;
                    if (!s.count(key)) {
                        s.insert(key);
                        q.push(key);
                    }
                }
                if (j < n2 && s2[j] == s3[k]) {
                    int key = i * n3 + j + 1;
                    if (!s.count(key)) {
                        s.insert(key);
                        q.push(key);
                    }
                }
            }
            ++k;
        }
        return !q.empty() && k == n3;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/7728/dp-solution-in-java](https://discuss.leetcode.com/topic/7728/dp-solution-in-java)

[https://discuss.leetcode.com/topic/3532/my-dp-solution-in-c](https://discuss.leetcode.com/topic/3532/my-dp-solution-in-c)

[https://discuss.leetcode.com/topic/30127/summary-of-solutions-bfs-dfs-dp](https://discuss.leetcode.com/topic/30127/summary-of-solutions-bfs-dfs-dp)

[https://discuss.leetcode.com/topic/3436/my-accepted-java-recursive-solution-for-interleaving-string](https://discuss.leetcode.com/topic/3436/my-accepted-java-recursive-solution-for-interleaving-string)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












