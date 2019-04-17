# [LeetCode] Fibonacci Number 斐波那契数字 - Grandyang - 博客园







# [[LeetCode] Fibonacci Number 斐波那契数字](https://www.cnblogs.com/grandyang/p/10306787.html)







The Fibonacci numbers, commonly denoted `F(n)` form a sequence, called the Fibonacci sequence, such that each number is the sum of the two preceding ones, starting from `0` and `1`. That is,
F(0) = 0,   F(1) = 1
F(N) = F(N - 1) + F(N - 2), for N > 1.

Given `N`, calculate `F(N)`.



Example 1:
Input: 2
Output: 1
Explanation: F(2) = F(1) + F(0) = 1 + 0 = 1.

Example 2:
Input: 3
Output: 2
Explanation: F(3) = F(2) + F(1) = 1 + 1 = 2.

Example 3:
Input: 4
Output: 3
Explanation: F(4) = F(3) + F(2) = 2 + 1 = 3.



Note:

0 ≤ `N` ≤ 30.



这道题是关于斐波那契数列的，这个数列想必我们都听说过，简而言之，除了前两个数字之外，每个数字等于前两个数字之和。举个生动的例子，大学食堂里今天的汤是昨天的汤加上前天的汤。哈哈，是不是瞬间记牢了。题目没让返回整个数列，而是直接让返回位置为N的数字。那么还是要构建整个斐波那契数组，才能知道位置N上的数字。像这种有规律有pattern的数组，最简单的方法就是使用递归啦，先把不合规律的前两个数字处理了，然后直接对N-1和N-2调用递归，并相加返回即可，参见代码如下：



解法一：

```
class Solution {
public:
    int fib(int N) {
        if (N <= 1) return N;
        return fib(N - 1) + fib(N - 2);
    }
};
```



上面的写法虽然简单，但是并不高效，因为有大量的重复计算，我们希望每个值只计算一次，所以可以使用动态规划Dynamic Programming来做，建立一个N+1的dp数组，其中dp[i]为位置i上的数字，先初始化前两个分别为0和1，然后就可以开始更新整个数组了，状态转移方程就是斐波那契数组的性质，最后返回dp[N]即可，参见代码如下：



解法二：

```
class Solution {
public:
    int fib(int N) {
        vector<int> dp(N + 1);
        dp[0] = 0; dp[1] = 1;
        for (int i = 2; i <= N; ++i) {
            dp[i] = dp[i - 1] + dp[i - 2];
        }
        return dp[N];
    }
};
```



我们可以对上面解法进行空间上的进一步优化，由于当前数字只跟前两个数字有关，所以我们不需要保存整个数组，而是只需要保存前两个数字就行了，前一个数字用b表示，再前面的用a表示。a和b分别初始化为0和1，代表数组的前两个数字。然后从位置2开始更新，先算出a和b的和sum，然后a更新为b，b更新为sum。最后返回b即可，参见代码如下：



解法三：

```
class Solution {
public:
    int fib(int N) {
        if (N <= 1) return N;
        int a = 0, b = 1;
        for (int i = 2; i <= N; ++i) {
            int sum = a + b;
            a = b;
            b = sum;
        }
        return b;
    }
};
```



给下面的这种解法跪了，直接hardcode了所有N范围内的斐波那契数字，然后直接返回，这尼玛诸葛孔明的棺材板快压不住了。。。我从未见过如此。。。



解法四：

```
class Solution {
public:
    int fib(int N) {
        vector<int> fibs{0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040};
        return fibs[N];
    }
};
```



类似题目：

[Split Array into Fibonacci Sequence](https://www.cnblogs.com/grandyang/p/10434771.html)

Length of Longest Fibonacci Subsequence



参考资料：

[https://leetcode.com/problems/fibonacci-number/](https://leetcode.com/problems/fibonacci-number/)

[https://leetcode.com/problems/fibonacci-number/discuss/215992/Java-Solutions](https://leetcode.com/problems/fibonacci-number/discuss/215992/Java-Solutions)

[https://leetcode.com/problems/fibonacci-number/discuss/216245/Java-O(1)-time](https://leetcode.com/problems/fibonacci-number/discuss/216245/Java-O(1)-time)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












