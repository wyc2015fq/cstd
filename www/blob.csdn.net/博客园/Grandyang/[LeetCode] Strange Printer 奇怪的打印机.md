# [LeetCode] Strange Printer 奇怪的打印机 - Grandyang - 博客园







# [[LeetCode] Strange Printer 奇怪的打印机](https://www.cnblogs.com/grandyang/p/8319913.html)








There is a strange printer with the following two special requirements:
- The printer can only print a sequence of the same character each time.
- At each turn, the printer can print new characters starting from and ending at any places, and will cover the original existing characters.



Given a string consists of lower English letters only, your job is to count the minimum number of turns the printer needed in order to print it.

Example 1:
Input: "aaabbb"
Output: 2
Explanation: Print "aaa" first and then print "bbb".



Example 2:
Input: "aba"
Output: 2
Explanation: Print "aaa" first and then print "b" from the second place of the string, which will cover the existing character 'a'.



Hint: Length of the given string will not exceed 100.



这道题说有一种奇怪的打印机每次只能打印一排相同的字符，然后可以在任意起点和终点位置之间打印新的字符，用来覆盖原有的字符。现在给了我们一个新的字符串，问我们需要几次可以正确的打印出来。题目中给了两个非常简单的例子，主要是帮助我们理解的。博主最开始想的方法是一种类似贪婪算法，先是找出出现次数最多的字符，然后算需要多少次变换能将所有其他字符都变成那个出现最多次的字符，结果fail了。然后又试了一种类似剥洋葱的方法，从首尾都分别找连续相同的字符，如果首尾字符相同，则两部分一起移去，否则就移去连续相同个数多的子序列，这种基于贪婪算法的解法还是fail了，所以这道题是典型的只能动态规划Dynamic Programming，而不能用贪婪算法Greedy Algorithm的题。这道题的解题思路跟之前那道[Remove Boxes](http://www.cnblogs.com/grandyang/p/6850657.html)很相似，博主在那个帖子中做了详细的讲解，是根据fun4leetcode大神的帖子写的，大神的思路对解这道题也相当有帮助。其实这道题并没有之前那道[Remove Boxes](http://www.cnblogs.com/grandyang/p/6850657.html)难，移除盒子的题有隐含的条件需要加到重现关系中，大大地增加了题目的难度，非常地难想出来，这道题没有隐含条件都是个Hard题，那道题妥妥应该是Super Hard。

好，话不多说，来分析这道题吧。思考的线索和思路很重要，不理解核心精髓，当背题侠是没用的，稍微变个形式又不会了，博主就经常是这样的-.-!!!。既然说了要用DP来做，先整个二维dp数组呗，其中dp[i][j]表示打印出字符串[i, j]范围内字符的最小步数，难点就是找递推公式啦。遇到乍看去没啥思路的题，博主一般会先从简单的例子开始，看能不能分析出规律，从而找到解题的线索。首先如果只有一个字符，比如字符串是"a"的话，那么直接一次打印出来就行了。如果字符串是"ab"的话，那么我们要么先打印出"aa"，再改成"ab"，或者先打印出"bb"，再改成"ab"。同理，如果字符串是"abc"的话，就需要三次打印。那么一个很明显的特征是，如果没有重复的字符，打印的次数就是字符的个数。燃鹅这题的难点就是要处理有相同字符的情况，比如字符串是"aba"的时候，我们先打"aaa"的话，两步就搞定了，如果先打"bbb"的话，就需要三步。我们再来看一个字符串"abcb"，我们知道需要需要三步，我们看如果把这个字符串分成两个部分"a"和"bcb"，它们分别的步数是1和2，加起来的3是整个的步数。而对于字符串"abba"，如果分成"a"和"bba"，它们分别的步数也是1和2，但是总步数却是2。这是因为分出的"a"和"bba"中的最后一个字符相同。对于字符串"abbac"，因为位置0上的a和位置3上的a相同，那么整个字符串的步数相当于"bb"和"ac"的步数之和，为3。那么分析到这，是不是有点眉目了？我们关心的是字符相等的地方，对于[i, j]范围的字符，我们从i+1位置上的字符开始遍历到j，如果和i位置上的字符相等，我们就以此位置为界，将[i+1, j]范围内的字符拆为两个部分，将二者的dp值加起来，和原dp值相比，取较小的那个。所以我们的递推式如下:

```
dp[i][j] = min(dp[i][j], dp[i + 1][k - 1] + dp[k][j]       (s[k] == s[i] and i + 1 <= k <= j)
```

要注意一些初始化的值，dp[i][i]是1，因为一个字符嘛，打印1次，还是就是在遍历k之前，dp[i][j]初始化为 1 + dp[i + 1][j]，为啥呢，可以看成在[i + 1, j]的范围上多加了一个s[i]字符，最坏的情况就是加上的是一个不曾出现过的字符，步数顶多加1步，注意我们的i是从后往前遍历的，当然你可以从前往后遍历，参数对应好就行了，参见代码如下：



解法一：

```
class Solution {
public:
    int strangePrinter(string s) {
        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n, 0));
        for (int i = n - 1; i >= 0; --i) {
            for (int j = i; j < n; ++j) {
                dp[i][j] = (i == j) ? 1 : (1 + dp[i + 1][j]);
                for (int k = i + 1; k <= j; ++k) {
                    if (s[k] == s[i]) dp[i][j] = min(dp[i][j], dp[i + 1][k - 1] + dp[k][j]);
                }
            }
        }
        return (n == 0) ? 0 : dp[0][n - 1];
    }
};
```



理解了上面的DP的方法，那么也可以用递归的形式来写，记忆数组memo就相当于dp数组，整个思路完全一样，参见代码如下： 



解法二：

```
class Solution {
public:
    int strangePrinter(string s) {
        int n = s.size();
        vector<vector<int>> memo(n, vector<int>(n, 0));
        return helper(s, 0, n - 1, memo);
    }
    int helper(string s, int i, int j, vector<vector<int>>& memo) {
        if (i > j) return 0;
        if (memo[i][j]) return memo[i][j];
        memo[i][j] = helper(s, i + 1, j, memo) + 1;
        for (int k = i + 1; k <= j; ++k) {
            if (s[k] == s[i]) {
                memo[i][j] = min(memo[i][j], helper(s, i + 1, k - 1, memo) + helper(s, k, j, memo));
            }
        }
        return memo[i][j];
    }
};
```



类似题目：

[Remove Boxes](http://www.cnblogs.com/grandyang/p/6850657.html)

[Burst Balloons](http://www.cnblogs.com/grandyang/p/5006441.html)

[Zuma Game](http://www.cnblogs.com/grandyang/p/6759881.html)



参考资料：

[https://discuss.leetcode.com/topic/100137/java-solution-dp](https://discuss.leetcode.com/topic/100137/java-solution-dp)

[https://discuss.leetcode.com/topic/100212/c-29ms-dp-solution](https://discuss.leetcode.com/topic/100212/c-29ms-dp-solution)

[https://discuss.leetcode.com/topic/100135/java-o-n-3-short-dp-solution](https://discuss.leetcode.com/topic/100135/java-o-n-3-short-dp-solution)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












