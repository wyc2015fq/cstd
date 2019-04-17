# [LeetCode] Student Attendance Record II 学生出勤记录之二 - Grandyang - 博客园







# [[LeetCode] Student Attendance Record II 学生出勤记录之二](https://www.cnblogs.com/grandyang/p/6866756.html)







Given a positive integer **n**, return the number of all possible attendance records with length n, which will be regarded as rewardable. The answer may be very large, return it after mod 109 + 7.

A student attendance record is a string that only contains the following three characters:


- **'A'** : Absent.
- **'L'** : Late.
- **'P'** : Present.

A record is regarded as rewardable if it doesn't contain **more than one 'A' (absent)** or **more than two continuous 'L' (late)**.

**Example 1:**

**Input:** n = 2
**Output:** 8 
**Explanation:**
There are 8 records with length 2 will be regarded as rewardable:
"PP" , "AP", "PA", "LP", "PL", "AL", "LA", "LL"
Only "AA" won't be regarded as rewardable owing to more than one absent times. 

**Note:** The value of **n** won't exceed 100,000.



这道题是之前那道[Student Attendance Record I](http://www.cnblogs.com/grandyang/p/6736484.html)的拓展，但是比那道题难度要大的多。从题目中说结果要对一个很大的数取余，说明结果是一个很大很大的数。那么一般来说这种情况不能用递归来求解，可能会爆栈，所以要考虑利用数学方法或者DP来做。其实博主最先看到这题的时候，心想这不就是高中时候学的排列组合的题吗，于是又在想怎么写那些A几几，C几几的式子来求结果，可惜并没有做出来。现在想想怎么当初高中的自己这么生猛，感觉啥都会的样子，上知天文下知地理，数理化生样样精通的感觉，燃鹅随着时间的推移，所有的一切都还给了老师。总感觉这题用数学的方法应该也可以解，但是看网上的大神们都是用DP做的，没办法，那只能用DP来做了。下面这种做法来自[大神lixx2100的帖子](https://discuss.leetcode.com/topic/86526/improving-the-runtime-from-o-n-to-o-log-n)，我们定义一个三维的dp数组，其中dp[i][j][k] 表示数组前i个数字中，最多有j个A，最多有k个连续L的组合方式，那么我们最终要求的结果就保存在dp[n][1][2]中。然后我们来考虑如何求dp[i][j][k]，首先我们来取出前一个状态下的值，就是前i-1个数的值，dp[i-1][j][2]，即数组前i-1个数中，最多有j个A，最多有2个连续L的排列方式，然后如果j>0，那么再加上dp[i-1][j-1][2]，即加上了最多有j-1个A的情况，并对超大数取余；如果k>0，则再加上dp[i-1][j][k-1]，即加上了最多有j个A，最多有k-1个连续L的排列方式，其实博主并没有完全理解为什么要这么更新，如果有大神们理解了这么做的含义，请不吝赐教，在下方留言告知博主啊～



解法一：

```
class Solution {
public:
    int checkRecord(int n) {
        int M = 1000000007;
        int dp[n + 1][2][3] = {0};
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 3; ++k) {
                dp[0][j][k] = 1;
            }
        }
        for (int i = 1; i <= n; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 3; ++k) {
                    int val = dp[i - 1][j][2];
                    if (j > 0) val = (val + dp[i - 1][j - 1][2]) % M;
                    if (k > 0) val = (val + dp[i - 1][j][k - 1]) % M;
                    dp[i][j][k] = val;
                }
            }
        }
        return dp[n][1][2];
    }
};
```



下面这种方法来自[大神KJer的帖子](https://discuss.leetcode.com/topic/86696/share-my-o-n-c-dp-solution-with-thinking-process-and-explanation)，大神帖子里面的讲解写的很详细，很赞，也不难读懂。定义了三个DP数组P, L, A，其中P[i]表示数组[0,i]范围内以P结尾的所有排列方式，L[i]表示数组[0,i]范围内以L结尾的所有排列方式，A[i]表示数组[0,i]范围内以A结尾的所有排列方式。那么最终我们所求的就是P[n-1] + L[n-1] + A[n-1]了。那么难点就是分别求出P, L, A数组的递推公式了。

首先来看P数组的，P字符没有任何限制条件，可以跟在任何一个字符后面，所以有 P[i] = A[i-1] + P[i-1] + L[i-1]

再来看L数组的，L字符唯一的限制条件是不能有超过两个连续的L，那么在P和L字符后面可以加1一个L，如果前一个字符是L，我们要看再前面的一位是什么字符，如果是P或着A的话，可以加L，如果是L的话，就不能再加了，否则就连续3个了，所以有 L[i] = A[i-1] + P[i-1] + A[i-2] + P[i-2]

最后来看A数组的，这个比较麻烦，字符A的限制条件是整个字符串最多只能有1个A，那么当前一个字符是A的话，就不能再加A来，当前一个字符是P或者L的话，要确定之前从没有A出现过，才能加上A。那么实际上我们还需要定义两个数组P1, L1, 其中P1[i]表示数组[0,i]范围内以P结尾的不包含A的所有排列方式，L1[i]表示数组[0,i]范围内以L结尾的不包含A的所有排列方式，根据前两种情况我们不难推出P1和L1的递推公式，再加上A的递推公式如下：

A[i] = P1[i-1] + L1[i-1]

P1[i] = P1[i-1] + L1[i-1]

L1[i] = P1[i-1] + P1[i-2]

将第二第三个等式多次带入第一个等式，就可以将P1和L1消掉，可以化简为：

A[i] = A[i-1] + A[i-2] + A[i-3]

这样就可以少定义两个数组了，递推公式有了，代码也就不难写了：



解法二：

```
class Solution {
public:
    int checkRecord(int n) {
        int M = 1000000007;
        vector<int> P(n), L(n), A(n);
        P[0] = 1; L[0] = 1; L[1] = 3;
        A[0] = 1; A[1] = 2; A[2] = 4;
        for (int i = 1; i < n; ++i) {
            P[i] = ((P[i - 1] + L[i - 1]) % M + A[i - 1]) % M;
            if (i > 1) L[i] = ((A[i - 1] + P[i - 1]) % M + (A[i - 2] + P[i - 2]) % M) % M;
            if (i > 2) A[i] = ((A[i - 1] + A[i - 2]) % M + A[i - 3]) % M;
        }
        return ((A[n - 1] + P[n - 1]) % M + L[n - 1]) % M;
    }
};
```



下面这种方法来自[大神dettier的帖子](https://discuss.leetcode.com/topic/86507/simple-java-o-n-solution)，这里面定义了两个数组P和PorL，其中P[i]表示数组前i个数字中已P结尾的排列个数，PorL[i]表示数组前i个数字中已P或者L结尾的排列个数。这个解法的精髓是我们先不考虑字符A的情况，而是先把定义的这个数组先求出来，由于P字符可以再任意字符后面加上，所以 P[i] = PorL[i-1]；而PorL[i]由两部分组成，P[i] + L[i]，其中P[i]已经更新了，L[i]只能当前一个字符是P，或者前一个字符是L且再前一个字符是P的时候加上，即为P[i-1] + P[i-2]，所以PorL[i] = P[i] + P[i-1] + P[i-2]。

那么我们就已经把不包含A的情况求出来了，存在了PorL[n]中，下面就是要求包含一个A的情况，那么我们就得去除一个字符，从而给A留出位置。那么就相当于在数组的任意一个位置上加上A，那么数组就被分成左右两个部分了，而这两个部分当然就不能再有A了，实际上所有不包含A的情况都已经在数组PorL中计算过了，而分成的子数组的长度又不会大于原数组的长度，所以我们直接在PorL中取值就行了，两个子数组的排列个数相乘，然后再把所有分割的情况累加起来就是最终结果啦，参见代码如下：



解法三：

```
class Solution {
public:
    int checkRecord(int n) {
        int M = 1000000007;
        vector<long long> P(n + 1), PorL(n + 1);
        P[0] = 1; PorL[0] = 1; PorL[1] = 2;
        for (int i = 1; i <= n; ++i) {
            P[i] = PorL[i - 1];
            if (i > 1) PorL[i] = (P[i] + P[i - 1] + P[i - 2]) % M;
        }
        long long res = PorL[n];
        for (int i = 0; i < n; ++i) {
            long long t = (PorL[i] * PorL[n - 1 - i]) % M;
            res = (res + t) % M;
        }
        return res;
    }
};
```



类似题目：

[Student Attendance Record I](http://www.cnblogs.com/grandyang/p/6736484.html)



参考资料：

[https://discuss.leetcode.com/topic/86507/simple-java-o-n-solution](https://discuss.leetcode.com/topic/86507/simple-java-o-n-solution)

[https://discuss.leetcode.com/topic/86526/improving-the-runtime-from-o-n-to-o-log-n](https://discuss.leetcode.com/topic/86526/improving-the-runtime-from-o-n-to-o-log-n)

[https://discuss.leetcode.com/topic/86696/share-my-o-n-c-dp-solution-with-thinking-process-and-explanation/2](https://discuss.leetcode.com/topic/86696/share-my-o-n-c-dp-solution-with-thinking-process-and-explanation/2)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












