# [LeetCode] Perfect Squares 完全平方数 - Grandyang - 博客园







# [[LeetCode] Perfect Squares 完全平方数](https://www.cnblogs.com/grandyang/p/4800552.html)








Given a positive integer *n*, find the least number of perfect square numbers (for example, `1, 4, 9, 16, ...`) which sum to *n*.

Example 1:
Input: *n* = `12`
Output: 3 
Explanation: `12 = 4 + 4 + 4.`
Example 2:
Input: *n* = `13`
Output: 2
Explanation: `13 = 4 + 9.`

**Credits:**
Special thanks to [@jianchao.li.fighter](https://leetcode.com/discuss/user/jianchao.li.fighter) for adding this problem and creating all test cases.



又是超哥一个人辛苦的更新题目，一个人托起LeetCode免费题的一片天空啊，赞一个~ 这道题说是给我们一个正整数，求它最少能由几个完全平方数组成。这道题是考察[四平方和定理](https://zh.wikipedia.org/wiki/%E5%9B%9B%E5%B9%B3%E6%96%B9%E5%92%8C%E5%AE%9A%E7%90%86)，to be honest, 这是我第一次听说这个定理，天啦撸，我的数学是语文老师教的么?! 闲话不多扯，回来做题。先来看第一种很高效的方法，根据四平方和定理，任意一个正整数均可表示为4个整数的平方和，其实是可以表示为4个以内的平方数之和，那么就是说返回结果只有1,2,3或4其中的一个，首先我们将数字化简一下，由于一个数如果含有因子4，那么我们可以把4都去掉，并不影响结果，比如2和8,3和12等等，返回的结果都相同，读者可自行举更多的栗子。还有一个可以化简的地方就是，如果一个数除以8余7的话，那么肯定是由4个完全平方数组成，这里就不证明了，因为我也不会证明，读者可自行举例验证。那么做完两步后，一个很大的数有可能就会变得很小了，大大减少了运算时间，下面我们就来尝试的将其拆为两个平方数之和，如果拆成功了那么就会返回1或2，因为其中一个平方数可能为0. (注：由于输入的n是正整数，所以不存在两个平方数均为0的情况)。注意下面的!!a + !!b这个表达式，可能很多人不太理解这个的意思，其实很简单，感叹号!表示逻辑取反，那么一个正整数逻辑取反为0，再取反为1，所以用两个感叹号!!的作用就是看a和b是否为正整数，都为正整数的话返回2，只有一个是正整数的话返回1，参见代码如下：



解法一：

```
class Solution {
public:
    int numSquares(int n) {
        while (n % 4 == 0) n /= 4;
        if (n % 8 == 7) return 4;
        for (int a = 0; a * a <= n; ++a) {
            int b = sqrt(n - a * a);
            if (a * a + b * b == n) {
                return !!a + !!b;
            }
        }
        return 3;
    }
};
```



这道题远不止这一种解法，我们还可以用动态规划Dynamic Programming来做，我们建立一个长度为n+1的一维dp数组，将第一个值初始化为0，其余值都初始化为INT_MAX, i从0循环到n，j从1循环到i+j*j <= n的位置，然后每次更新dp[i+j*j]的值，动态更新dp数组，其中dp[i]表示正整数i能少能由多个完全平方数组成，那么我们求n，就是返回dp[n]即可，也就是dp数组的最后一个数字。需要注意的是这里的写法，i必须从0开始，j必须从1开始，因为我们的初衷是想用 dp[i] 来更新 dp[i + j * j]，如果 i=0, j=1 了，那么 dp[i] 和 dp[i + j * j] 就相等了，怎么能用本身dp值加1来更新自身呢，参见代码如下：



解法二：

```
class Solution {
public:
    int numSquares(int n) {
        vector<int> dp(n + 1, INT_MAX);
        dp[0] = 0;
        for (int i = 0; i <= n; ++i) {
            for (int j = 1; i + j * j <= n; ++j) {
                dp[i + j * j] = min(dp[i + j * j], dp[i] + 1);
            }
        }
        return dp.back();
    }
};
```



下面再来看一种DP解法，这种解法跟上面有些不同，上面那种解法是初始化了整个长度为n+1的dp数字，但是初始化的顺序不定的，而这个种方法只初始化了第一个值为0，那么在循环里计算，每次增加一个dp数组的长度，里面那个for循环一次循环结束就算好下一个数由几个完全平方数组成，直到增加到第n+1个，返回即可，想更直观的看这两种DP方法的区别，建议每次循环后都打印出dp数字的值来观察其更新的顺序，参见代码如下：



解法三：

```
class Solution {
public:
    int numSquares(int n) {
        vector<int> dp(1, 0);
        while (dp.size() <= n) {
            int m = dp.size(), val = INT_MAX;
            for (int i = 1; i * i <= m; ++i) {
                val = min(val, dp[m - i * i] + 1);
            }
            dp.push_back(val);
        }
        return dp.back();
    }
};
```



最后我们来介绍一种递归Recursion的解法，这种方法的好处是写法简洁，但是运算效率不敢恭维。我们的目的是遍历所有比n小的完全平方数，然后对n与完全平方数的差值递归调用函数，目的是不断更新最终结果，知道找到最小的那个，参见代码如下：



解法四：

```
class Solution {
public:
    int numSquares(int n) {
        int res = n, num = 2;
        while (num * num <= n) {
            int a = n / (num * num), b = n % (num * num);
            res = min(res, a + numSquares(b));
            ++num;
        }
        return res;
    }
};
```



讨论：解法二三四的运算效率真的不高，强推解法一，高效又易懂，如果想强行优化后三个算法，可以将解法一的前两个if判断加到后三个的算法的开头，能很大的提高运算效率。



类似题目：

[Count Primes](http://www.cnblogs.com/grandyang/p/4462810.html)

[Ugly Number II](http://www.cnblogs.com/grandyang/p/4743837.html)



参考资料：

[https://leetcode.com/problems/perfect-squares/](https://leetcode.com/problems/perfect-squares/)

[http://bookshadow.com/weblog/2015/09/09/leetcode-perfect-squares/](http://bookshadow.com/weblog/2015/09/09/leetcode-perfect-squares/)

[https://leetcode.com/problems/perfect-squares/discuss/71505/Simple-Java-DP-Solution](https://leetcode.com/problems/perfect-squares/discuss/71505/Simple-Java-DP-Solution)

[https://leetcode.com/problems/perfect-squares/discuss/71512/Static-DP-C%2B%2B-12-ms-Python-172-ms-Ruby-384-ms](https://leetcode.com/problems/perfect-squares/discuss/71512/Static-DP-C%2B%2B-12-ms-Python-172-ms-Ruby-384-ms)

[https://leetcode.com/problems/perfect-squares/discuss/71488/Summary-of-4-different-solutions-(BFS-DP-static-DP-and-mathematics)](https://leetcode.com/problems/perfect-squares/discuss/71488/Summary-of-4-different-solutions-(BFS-DP-static-DP-and-mathematics))



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












