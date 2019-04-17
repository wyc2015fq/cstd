# [LeetCode] Decode Ways II 解码方法之二 - Grandyang - 博客园







# [[LeetCode] Decode Ways II 解码方法之二](https://www.cnblogs.com/grandyang/p/7279152.html)







A message containing letters from `A-Z` is being encoded to numbers using the following mapping way:
'A' -> 1
'B' -> 2
...
'Z' -> 26

Beyond that, now the encoded string can also contain the character '*', which can be treated as one of the numbers from 1 to 9.

Given the encoded message containing digits and the character '*', return the total number of ways to decode it.

Also, since the answer may be very large, you should return the output mod 109 + 7.

Example 1:
Input: "*"
Output: 9
Explanation: The encoded message can be decoded to the string: "A", "B", "C", "D", "E", "F", "G", "H", "I".



Example 2:
Input: "1*"
Output: 9 + 9 = 18



Note:
- The length of the input string will fit in range [1, 105].
- The input string will only contain the character '*' and digits '0' - '9'.



这道解码的题是之前那道[Decode Ways](http://www.cnblogs.com/grandyang/p/4313384.html)的拓展，难度提高了不少，引入了星号，可以代表1到9之间的任意数字，是不是有点外卡匹配的感觉。有了星号以后，整个题就变得异常的复杂，所以结果才让我们对一个很大的数求余，避免溢出。这道题的难点就是要分情况种类太多，一定要全部理通顺才行。我们还是用DP来做，建立一个一维dp数组，其中dp[i]表示前i个字符的解码方法等个数，长度为字符串的长度加1。将dp[0]初始化为1，然后我们判断，如果字符串第一个字符是0，那么直接返回0，如果是*，则dp[1]初始化为9，否则初始化为1。下面就来计算一般情况下的dp[i]了，我们从i=2开始遍历，由于要分的情况种类太多，我们先选一个大分支，就是当前遍历到的字符s[i-1]，只有三种情况，要么是0，要么是1到9的数字，要么是星号。我们一个一个来分析：

首先来看s[i-1]为0的情况，这种情况相对来说比较简单，因为0不能单独拆开，只能跟前面的数字一起，而且前面的数字只能是1或2，其他的直接返回0即可。那么当前面的数字是1或2的时候，dp[i]的种类数就跟dp[i-2]相等，可以参见之前那道[Decode Ways](http://www.cnblogs.com/grandyang/p/4313384.html)的讲解，因为后两数无法单独拆分开，就无法产生新的解码方法，所以只保持住原来的拆分数量就不错了；如果前面的数是星号的时候，那么前面的数可以为1或者2，这样就相等于两倍的dp[i-2]；如果前面的数也为0，直接返回0即可。

再来看s[i-1]为1到9之间的数字的情况，首先搞清楚当前数字是可以单独拆分出来的，那么dp[i]至少是等于dp[i-1]的，不会拖后腿，还要看其能不能和前面的数字组成两位数进一步增加解码方法。那么就要分情况讨论前面一个数字的种类，如果当前数字可以跟前面的数字组成一个小于等于26的两位数的话，dp[i]还需要加上dp[i-2]；如果前面的数字为星号的话，那么要看当前的数字是否小于等于6，如果是小于等于6，那么前面的数字就可以是1或者2了，此时dp[i]需要加上两倍的dp[i-2]，如果大于6，那么前面的数字只能是1，所以dp[i]只能加上dp[i-2]。

最后来看s[i-1]为星号的情况，如果当前数字为星号，那么就创造9种可以单独拆分的方法，所以那么dp[i]至少是等于9倍的dp[i-1]，还要看其能不能和前面的数字组成两位数进一步增加解码方法。那么就要分情况讨论前面一个数字的种类，如果前面的数字是1，那么当前的9种情况都可以跟前面的数字组成两位数，所以dp[i]需要加上9倍的dp[i-2]；如果前面的数字是2，那么只有小于等于6的6种情况都可以跟前面的数字组成两位数，所以dp[i]需要加上6倍的dp[i-2]；如果前面的数字是星号，那么就是上面两种情况的总和，dp[i]需要加上15倍的dp[i-2]。

每次算完dp[i]别忘了对超大数取余，参见代码如下：



解法一：

```
class Solution {
public:
    int numDecodings(string s) {
        int n = s.size(), M = 1e9 + 7;
        vector<long> dp(n + 1, 0);
        dp[0] = 1;
        if (s[0] == '0') return 0;
        dp[1] = (s[0] == '*') ? 9 : 1;
        for (int i = 2; i <= n; ++i) {
            if (s[i - 1] == '0') {
                if (s[i - 2] == '1' || s[i - 2] == '2') {
                    dp[i] += dp[i - 2];
                } else if (s[i - 2] == '*') {
                    dp[i] += 2 * dp[i - 2];
                } else {
                    return 0;
                }
            } else if (s[i - 1] >= '1' && s[i - 1] <= '9') {
                dp[i] += dp[i - 1];
                if (s[i - 2] == '1' || (s[i - 2] == '2' && s[i - 1] <= '6')) {
                    dp[i] += dp[i - 2];
                } else if (s[i - 2] == '*') {
                    dp[i] += (s[i - 1] <= '6') ? (2 * dp[i - 2]) : dp[i - 2];
                }
            } else { // s[i - 1] == '*'
                dp[i] += 9 * dp[i - 1];
                if (s[i - 2] == '1') dp[i] += 9 * dp[i - 2];
                else if (s[i - 2] == '2') dp[i] += 6 * dp[i - 2];
                else if (s[i - 2] == '*') dp[i] += 15 * dp[i - 2];
            }
            dp[i] %= M;
        }
        return dp[n];
    }
};
```



下面这种解法是论坛上排名最高的解法，常数级的空间复杂度，写法非常简洁，思路也巨牛逼，博主是无论如何也想不出来的，只能继续当搬运工了。这里定义了一系列的变量e0, e1, e2, f0, f1, f2。其中：

e0表示当前可以获得的解码的次数，当前数字可以为任意数 (也就是上面解法中的dp[i])

e1表示当前可以获得的解码的次数，当前数字为1

e2表示当前可以获得的解码的次数，当前数字为2

f0, f1, f2分别为处理完当前字符c的e0, e1, e2的值

那么下面我们来进行分类讨论，当c为星号的时候，f0的值就是9*e0 + 9*e1 + 6*e2，这个应该不难理解了，可以参考上面解法中的讲解，这里的e0就相当于dp[i-1]，e1和e2相当于两种不同情况的dp[i-2]，此时f1和f2都赋值为e0，因为要和后面的数字组成两位数的话，不会增加新的解码方法，所以解码总数跟之前的一样，为e0, 即dp[i-1]。

当c不为星号的时候，如果c不为0，则f0首先应该加上e0。然后不管c为何值，e1都需要加上，总能和前面的1组成两位数；如果c小于等于6，可以和前面的2组成两位数，可以加上e2。然后我们更新f1和f2，如果c为1，则f1为e0；如果c为2，则f2为e0。

最后别忘了将f0，f1，f2赋值给e0，e1，e2，其中f0需要对超大数取余，参见代码如下：



解法二：

```
class Solution {
public:
    int numDecodings(string s) {
        long e0 = 1, e1 = 0, e2 = 0, f0, f1, f2, M = 1e9 + 7;
        for (char c : s) {
            if (c == '*') {
                f0 = 9 * e0 + 9 * e1 + 6 * e2;
                f1 = e0;
                f2 = e0;
            } else {
                f0 = (c > '0') * e0 + e1 + (c <= '6') * e2;
                f1 = (c == '1') * e0;
                f2 = (c == '2') * e0;
            }
            e0 = f0 % M;
            e1 = f1;
            e2 = f2;
        }
        return e0;
    }
};
```



下面这解法由热心网友edyyy提供，在解法二的基础上去掉了两个变量，节省了行数，很符合博主的极简风格，参见代码如下：



解法三：

```
class Solution {
public:
    int numDecodings(string s) {
        long e0 = 1, e1 = 0, e2 = 0, f0 = 0, M = 1e9 + 7;
        for (char c : s) {
            if (c == '*') {
                f0 = 9 * e0 + 9 * e1 + 6 * e2;
                e1 = e0;
                e2 = e0;
            } else {
                f0 = (c > '0') * e0 + e1 + (c <= '6') * e2;
                e1 = (c == '1') * e0;
                e2 = (c == '2') * e0;
            }
            e0 = f0 % M;
        }
        return e0;
    }
};
```



类似题目：

[Decode Ways](http://www.cnblogs.com/grandyang/p/4313384.html)



参考资料：

[https://discuss.leetcode.com/topic/95301/python-straightforward-with-explanation](https://discuss.leetcode.com/topic/95301/python-straightforward-with-explanation)

[https://discuss.leetcode.com/topic/95518/java-o-n-by-general-solution-for-all-dp-problems](https://discuss.leetcode.com/topic/95518/java-o-n-by-general-solution-for-all-dp-problems)

[https://discuss.leetcode.com/topic/95204/java-dp-solution-o-n-time-and-space-some-explanations](https://discuss.leetcode.com/topic/95204/java-dp-solution-o-n-time-and-space-some-explanations)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












