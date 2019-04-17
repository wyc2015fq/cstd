# [LeetCode] Rotated Digits 旋转数字 - Grandyang - 博客园







# [[LeetCode] Rotated Digits 旋转数字](https://www.cnblogs.com/grandyang/p/9154892.html)







X is a good number if after rotating each digit individually by 180 degrees, we get a valid number that is different from X.  Each digit must be rotated - we cannot choose to leave it alone.

A number is valid if each digit remains a digit after rotation. 0, 1, and 8 rotate to themselves; 2 and 5 rotate to each other; 6 and 9 rotate to each other, and the rest of the numbers do not rotate to any other number and become invalid.

Now given a positive number `N`, how many numbers X from `1` to `N` are good?
Example:
Input: 10
Output: 4
Explanation: 
There are four good numbers in the range [1, 10] : 2, 5, 6, 9.
Note that 1 and 10 are not good numbers, since they remain unchanged after rotating.

Note:
- N  will be in range `[1, 10000]`.



这道题定义了一种好数字，就是把每位上的数字翻转一下，能得到一个不同的数字。翻转规则定义为，0，1，和8翻转后还为其本身，2和5，6和9可以互相翻转。然后给了我们一个数字N，问我们 [1, N] 区间内共有多少个这样的好数字。这道题大家踩👎的个数远超顶👍的个数，貌似很多人不太喜欢这道给数字发好人卡的题，博主倒觉得这道题还不错，感觉没有太多的技巧，就是一个数字一个数字的验证呗，对于每个数字，再一位一位的验证呗。我们将验证某个数字是否Good的操作写到一个子函数中，遍历数字的每一位的方法，可以通过不停的除以10来获得，也可以像博主这样通过转变为字符串，再遍历字符即可。翻转规则中没有提到的数字有三个，3，4，和7，说明这三个数字无法翻转，若一旦被翻转，则无法形成valid的数字，所以我们只要一旦遇到这三个数字中的一个，直接返回false即可。还有要注意的是，0，1，和8这三个数字翻转后还是其本身，由于好数字的需要翻转一个不同的数字，所以若都是由这三个数字组成，翻转后不会产生不同的数字，也不符合题意。所以我们需要2，5，6，和9这四个数字中至少出现一个，我们用一个flag来标记出现过这些数字，最后只要check这个flag变量即可，参见代码如下：



解法一：

```
class Solution {
public:
    int rotatedDigits(int N) {
        int res = 0;
        for (int i = 1; i <= N; ++i) {
            if (check(i)) ++res;
        }
        return res;
    }
    bool check(int k) {
        string str = to_string(k);
        bool flag = false;
        for (char c : str) {
            if (c == '3' || c == '4' || c == '7') return false;
            if (c == '2' || c == '5' || c == '6' || c == '9') flag = true;;
        }
        return flag;
    }
};
```



其实这道题还有更好的解法呢，使用动态规划Dynamic Programming来做的，思路非常巧妙，博主深为叹服。定义了一个长度为N+1的一维布尔型DP数组，其中dp[i]表示数字i的三种状态，0表示数字i翻转后不合法，1表示数字i翻转后和原数相同，2表示数字i翻转后形成一个不同的数字。那么根据题目中的定义可知，只有当dp[i]=2的时候才是好数字。那么下面来看状态转移方程吧，我们知道如果数字只有1位的话，那么判断起来很简单，如果是0，1，和8中的一个，那么dp[i]=1，如果是2，5，6，和9中的一个，那么dp[i]=2，并且结果res自增1。如果是剩下的三个数字3，4，7中的一个不用更新，因为dp数组初始化就为0。下面来看数字i大于10的情况，非常的经典，dp[i] 的值其实可以从 dp[i/10] 和 dp[i%10] 这两个状态值转移而来，由于我们更新的顺序是从小到大，所以当要更新dp[i]的时候，dp[i/10] 和 dp[i%10] 这两个状态值已经算过了。为啥dp[i] 的值是由这两个状态值决定的呢？因为每个数字都是相互独立的翻转，比如四位数字abcd，可以拆分为三位数abc，和个位数d，如果abc翻转后仍是abc，d翻转后仍是d，说明abcd翻转后仍是abcd，所以dp[i]=1，只要其中有一个大于1了，另外一个至少是1的话，那么说明可以翻转成不同的数字，dp[i]=2，并且结果res自增1，参见代码如下：



解法二：

```
class Solution {
public:
    int rotatedDigits(int N) {
        int res = 0;
        vector<int> dp(N + 1);   
        for (int i = 0; i <= N; ++i) {
            if (i < 10) {
                if (i == 0 || i == 1 || i == 8) dp[i] = 1;
                else if (i == 2 || i == 5 || i == 6 || i == 9) {
                    dp[i] = 2; ++res;
                }
            } else {
                int a = dp[i / 10], b = dp[i % 10];
                if (a == 1 && b == 1) dp[i] = 1;
                else if (a >= 1 && b >= 1) {
                    dp[i] = 2; ++res;
                }
            }
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/rotated-digits/solution/](https://leetcode.com/problems/rotated-digits/solution/)

[https://leetcode.com/problems/rotated-digits/discuss/117975/Java-dp-solution-9ms](https://leetcode.com/problems/rotated-digits/discuss/117975/Java-dp-solution-9ms)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












