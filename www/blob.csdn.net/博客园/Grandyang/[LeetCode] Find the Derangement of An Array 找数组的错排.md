# [LeetCode] Find the Derangement of An Array 找数组的错排 - Grandyang - 博客园







# [[LeetCode] Find the Derangement of An Array 找数组的错排](https://www.cnblogs.com/grandyang/p/7210929.html)







In combinatorial mathematics, a derangement is a permutation of the elements of a set, such that no element appears in its original position.

There's originally an array consisting of `n` integers from 1 to `n` in ascending order, you need to find the number of derangement it can generate.

Also, since the answer may be very large, you should return the output mod 109 + 7.

Example 1:
Input: 3
Output: 2
Explanation: The original array is [1,2,3]. The two derangements are [2,3,1] and [3,1,2].



Note:
`n` is in the range of [1, 106].



这道题给了我们一个数组，让我们求其错排的个数，所谓错排就是1到n中的每个数字都不在其原有的位置上，全部打乱了，问能有多少种错排的方式。博主注意到了这道题又让对一个很大的数取余，而且每次那个很大的数都是109 + 7，为啥大家都偏爱这个数呢，有啥特别之处吗？根据博主之前的经验，这种结果很大很大的题十有八九都是用dp来做的，那么就建一个一维的dp数组吧，其中dp[i]表示1到i中的错位排列的个数。那么难点就是找递推公式啦，先从最简单的情况来看:

n = 1 时有 0 种错排

n = 2 时有 1 种错排 [2, 1]

n = 3 时有 2 种错排 [3, 1, 2], [2, 3, 1]

然后博主就在想知道了dp[2]，能求出dp[3]吗，又在考虑是不是算加入数字3的情况的个数。结果左看右看发现没有啥特别的规律，又在想是不是有啥隐含的信息需要挖掘，还是没想出来。于是看了一眼标签，发现是Math，我的天，难道又是小学奥数的题？挣扎了半天最后还是放弃了，上网去搜大神们的解法。其实这道题是组合数学种的[错排问题](https://zh.wikipedia.org/wiki/%E9%94%99%E6%8E%92%E9%97%AE%E9%A2%98)，是有专门的递归公式的。

我们来想n = 4时该怎么求，我们假设把4排在了第k位，这里我们就让k = 3吧，那么我们就把4放到了3的位置，变成了：

x x 4 x

我们看被4占了位置的3，应该放到哪里，这里分两种情况，如果3放到了4的位置，那么有：

x x 4 3

那么此时4和3的位置都确定了，实际上只用排1和2了，那么就相当于只排1和2，就是dp[2]的值，是已知的。那么再来看第二种情况，3不在4的位置，那么此时我们把4去掉的话，就又变成了：

x x x

这里3不能放在第3个x的位置，在去掉4之前，这里是移动4之前的4的位置，那么实际上这又变成了排1，2，3的情况了，就是dp[3]的值。

再回到最开始我们选k的时候，我们当时选了k = 3，其实k可以等于1,2,3，也就是有三种情况，所以dp[4] = 3 * (dp[3] + dp[2])。

那么递推公式也就出来了：

dp[i] = (i - 1) * (dp[i - 1] + dp[i - 2])

有了递推公式，代码就不难写了吧，参见代码如下：



解法一：

```
class Solution {
public:
    int findDerangement(int n) {
        if (n < 2) return 0;
        vector<long long> dp(n + 1, 0);
        dp[1] = 0; dp[2] = 1;
        for (int i = 3; i <= n; ++i) {
            dp[i] = (dp[i - 1] + dp[i - 2]) * (i - 1) % 1000000007;
        }
        return dp[n];
    }
};
```



下面这种解法精简了空间，因为当前值只跟前两个值有关系，所以没必要保留整个数组，只用两个变量来记录前两个值，并每次更新一下就好了，参见代码如下：



解法二：

```
class Solution {
public:
    int findDerangement(int n) {
        long long a = 0, b = 1, res = 1;
        for (int i = 3; i <= n; ++i) {
            res = (i - 1) * (a + b) % 1000000007;
            a = b;
            b = res;
        }
        return (n == 1) ? 0 : res;
    }
};
```



下面这种方法是对之前的递推公式进行了推导变形，使其只跟前一个数有关，具体的推导步骤是这样的：

我们假设 e[i] = dp[i] - i * dp[i - 1]

递推公式为:  dp[i] = (i - 1) * (dp[i - 1] + dp[i - 2])

将递推公式带入假设，得到：

e[i] = -dp[i - 1] + (n - 1) * dp[i - 2] = -e[i - 1]

从而得到 e[i] = (-1)^n

那么带回假设公式，可得: dp[i] = i * dp[i - 1] + (-1)^n

根据这个新的递推公式，可以写出代码如下：



解法三：

```
class Solution {
public:
    int findDerangement(int n) {
        long long res = 1;
        for (int i = 1; i <= n; ++i) {
            res = (i * res + (i % 2 == 0 ? 1 : -1)) % 1000000007; 
        }
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/94429/o-n-short-java-code](https://discuss.leetcode.com/topic/94429/o-n-short-java-code)

[https://discuss.leetcode.com/topic/94767/java-dp-solution-with-explanation](https://discuss.leetcode.com/topic/94767/java-dp-solution-with-explanation)

[https://discuss.leetcode.com/topic/94421/java-solution-with-explanation-by-using-staggered-formula](https://discuss.leetcode.com/topic/94421/java-solution-with-explanation-by-using-staggered-formula)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












