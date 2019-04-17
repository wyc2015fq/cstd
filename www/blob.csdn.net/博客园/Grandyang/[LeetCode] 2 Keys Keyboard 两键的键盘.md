# [LeetCode] 2 Keys Keyboard 两键的键盘 - Grandyang - 博客园







# [[LeetCode] 2 Keys Keyboard 两键的键盘](https://www.cnblogs.com/grandyang/p/7439616.html)








Initially on a notepad only one character 'A' is present. You can perform two operations on this notepad for each step:
- `Copy All`: You can copy all the characters present on the notepad (partial copy is not allowed).
- `Paste`: You can paste the characters which are copied last time.



Given a number `n`. You have to get exactly `n` 'A' on the notepad by performing the minimum number of steps permitted. Output the minimum number of steps to get `n` 'A'.

Example 1:
Input: 3
Output: 3
Explanation:
Intitally, we have one character 'A'.
In step 1, we use Copy All operation.
In step 2, we use Paste operation to get 'AA'.
In step 3, we use Paste operation to get 'AAA'.



Note:
- The `n` will be in the range [1, 1000].



这道题只给了我们两个按键，如果只能选择两个按键，那么博主一定会要复制和粘贴，此二键在手，天下我有！！！果然，这道题就是给了我们复制和粘贴这两个按键，然后给了我们了一个A，我们的目标时利用这两个键来打印出n个A，注意复制的时候时全部复制，不能选择部分来复制，然后复制和粘贴都算操作步骤，问我们打印出n个A需要多少步操作。对于这种有明显的递推特征的题，我们要有隐约的感觉，一定要尝试递归和DP。递归解法一般接近于暴力搜索，但是有时候是可以优化的，从而能够通过OJ。而一旦递归不行的话，那么一般来说DP这个大杀器都能解的。还有一点，对于这种题，找规律最重要，DP要找出递推公式，而如果无法发现内在的联系，那么递推公式就比较难写出来了。所以，我们需要从简单的例子开始分析，试图找规律：

当n = 1时，已经有一个A了，我们不需要其他操作，返回0

当n = 2时，我们需要复制一次，粘贴一次，返回2

当n = 3时，我们需要复制一次，粘贴两次，返回3

当n = 4时，这就有两种做法，一种是我们需要复制一次，粘贴三次，共4步，另一种是先复制一次，粘贴一次，得到AA，然后再复制一次，粘贴一次，得到AAAA，两种方法都是返回4

当n = 5时，我们需要复制一次，粘贴四次，返回5

当n = 6时，我们需要复制一次，粘贴两次，得到AAA，再复制一次，粘贴一次，得到AAAAAA，共5步，返回5

通过分析上面这6个简单的例子，我想我们已经可以总结出一些规律了，首先对于任意一个n(除了1以外)，我们最差的情况就是用n步，不会再多于n步，但是有可能是会小于n步的，比如n=6时，就只用了5步，仔细分析一下，发现时先拼成了AAA，再复制粘贴成了AAAAAA。那么什么情况下可以利用这种方法来减少步骤呢，分析发现，小模块的长度必须要能整除n，这样才能拆分。对于n=6，我们其实还可先拼出AA，然后再复制一次，粘贴两次，得到的还是5。分析到这里，我想解题的思路应该比较清晰了，我们要找出n的所有因子，然后这个因子可以当作模块的个数，我们再算出模块的长度n/i，调用递归，加上模块的个数i来更新结果res即可，参见代码如下：



解法一：

```
class Solution {
public:
    int minSteps(int n) {
        if (n == 1) return 0;
        int res = n;
        for (int i = n - 1; i > 1; --i) {
            if (n % i == 0) {
                res = min(res, minSteps(n / i) + i);
            }
        }
        return res;
    }
};
```



下面这种方法是用DP来做的，我们可以看出来，其实就是上面递归解法的迭代形式，思路没有任何区别，参见代码如下：



解法二：

```
class Solution {
public:
    int minSteps(int n) {
        vector<int> dp(n + 1, 0);
        for (int i = 2; i <= n; ++i) {
            dp[i] = i;
            for (int j = i - 1; j > 1; --j) {
                if (i % j == 0) {
                    dp[i] = min(dp[i], dp[j] + i / j);
                }
            }
        }
        return dp[n];
    }
};
```



下面我们来看一种省空间的方法，我们不需要记录每一个中间值，而是通过改变n的值来实时累加结果res，参见代码如下：



解法三：

```
class Solution {
public:
    int minSteps(int n) {
        int res = 0;
        for (int i = 2; i <= n; ++i) {
            while (n % i == 0) {
                res += i;
                n /= i;
            }
        }
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/97590/java-dp-solution](https://discuss.leetcode.com/topic/97590/java-dp-solution)

[https://discuss.leetcode.com/topic/97623/loop-best-case-log-n-no-dp-no-extra-space-no-recursion-with-explanation](https://discuss.leetcode.com/topic/97623/loop-best-case-log-n-no-dp-no-extra-space-no-recursion-with-explanation)





[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













