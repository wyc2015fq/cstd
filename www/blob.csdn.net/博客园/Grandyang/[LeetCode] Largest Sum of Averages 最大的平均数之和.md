# [LeetCode] Largest Sum of Averages 最大的平均数之和 - Grandyang - 博客园







# [[LeetCode] Largest Sum of Averages 最大的平均数之和](https://www.cnblogs.com/grandyang/p/9504413.html)







We partition a row of numbers `A` into at most `K` adjacent (non-empty) groups, then our score is the sum of the average of each group. What is the largest score we can achieve?

Note that our partition must use every number in A, and that scores are not necessarily integers.
Example:
Input: 
A = [9,1,2,3,9]
K = 3
Output: 20
Explanation: 
The best choice is to partition A into [9], [1, 2, 3], [9]. The answer is 9 + (1 + 2 + 3) / 3 + 9 = 20.
We could have also partitioned A into [9, 1], [2], [3, 9], for example.
That partition would lead to a score of 5 + 2 + 6 = 13, which is worse.



Note:
- `1 <= A.length <= 100`.
- `1 <= A[i] <= 10000`.
- `1 <= K <= A.length`.
- Answers within `10^-6` of the correct answer will be accepted as correct.



这道题给了我们一个数组，说是让我们将数组分成至多K个非空组，然后说需要统计的分数是各组的平均数之和，让我们求一个分割方法，使得这个分数值最大，当然这个分数值不一定是整型数。这道题限制了分割的组必须为非空组，那么就是说K值要小于等于数组的元素个数。但是实际上博主感觉这个必须为非空的限制有没有都一样，因为题目中说至多分成K组，也就是说可以根本不分组，那么比如你输入个A=[9,1], K=3，照样返回一个10，给人的感觉好像是分成了[9], [1], [] 这三组一样，但其实只是分成了两组[9] 和 [1]。但我们不必纠结这些，不是重点。没有啥思路的情况下我们就先想想brute force的解法呗，对于题目中给的那个例子，我们用最暴力的方法就是遍历所有的可能性，即遍历所有分割成三个组的情况，用三个for循环。貌似行的通，但问题来了，如果K大于3呢，每大一个，多加一个for循环么，总共K个for循环？如果K=100呢，100个for循环么？画面太美我不敢看！显然这道题用brute force是行不通的，那么换个方法呗！像这种求极值的题，又是玩数组的题，根据老夫行走江湖多年的经验，十有八九都是用Dynamic Programming来做的。玩子数组且跟极值有关的题天然适合用DP来做，想想为什么？DP的本质是什么，不就是状态转移方程，根据前面的状态来更新当前的状态。而子数组不就是整个数组的前一个状态，不停的更新的使得我们最终能得到极值。

好，下面进入正题。DP走起，首先来考虑dp数组的定义，我们如何定义dp数组有时候很关键，定义的不好，那么就无法写出正确的状态转移方程。对于这道题，我们很容易直接用一个一维数组dp，其中dp[i]表示范围为[0, i]的子数组分成三组能得到的最大分数。用这样定义的dp数组的话，状态转移方程将会非常难写，因为我们忽略了一个重要的信息，即K。dp数组不把K加进去的话就不知道当前要分几组，这个Hidden Information是解题的关键。这是DP中比较难的一类，有些DP题的隐藏信息藏的更深，不挖出来就无法解题。这道题的dp数组应该是个二维数组，其中dp[i][k]表示范围是[i, n-1]的子数组分成k组的最大得分。那么这里你就会纳闷了，为啥范围是[i, n-1]而不是[0, i]，为啥要取后半段呢，不着急，听博主慢慢道来。由于把[i, n-1]范围内的子数组分成k组，那么suppose我们已经知道了任意范围内分成k-1组的最大分数，这是此类型题目的破题关键所在，要求状态k，一定要先求出所有的状态k-1，那么问题就转换成了从k-1组变成k组，即多分出一组，那么在范围[i, n-1]多分出一组，实际上就是将其分成两部分，一部分是一组，另一部分是k-1组，怎么分，就用一个变量j，遍历范围(i, n-1)中的每一个位置，那么分成的这两部分的分数如何计算呢？第一部分[i, j)，由于是一组，那么直接求出平均值即可，另一部分由于是k-1组，由于我们已经知道了所有k-1的情况，可以直接从cache中读出来dp[j][k-1]，二者相加即可 avg(i, j) + dp[j][k-1]，所以我们可以得出状态转移方程如下：

dp[i][k] = max(avg(i, n) + max_{j > i} (avg(i, j) + dp[j][k-1]))

这里的avg(i, n)是其可能出现的情况，由于是至多分为k组，所以我们可以不分组，所以直接计算范围[i, n-1]内的平均值，然后用j来遍历区间(i, n-1)中的每一个位置，最终得到的dp[i][k]就即为所求。注意这里我们通过建立累加和数组sums来快速计算某个区间之和。博主觉得这道题十分的经典，考察点非常的多，很具有代表性，标为Hard都不过分，前面提到了dp[i][k]表示的是范围[i, n-1]的子数组分成k组的最大得分，现在想想貌似定义为[0, i]范围内的子数组分成k组的最大得分应该也是可以的，那么此时j就是遍历(0, i)中的每个位置了，好像也没什么不妥的地方，有兴趣的童鞋可以尝试的写一下～



解法一：

```
class Solution {
public:
    double largestSumOfAverages(vector<int>& A, int K) {
        int n = A.size();
        vector<double> sums(n + 1);
        vector<vector<double>> dp(n, vector<double>(K));
        for (int i = 0; i < n; ++i) {
            sums[i + 1] = sums[i] + A[i];
        }
        for (int i = 0; i < n; ++i) {
            dp[i][0] = (sums[n] - sums[i]) / (n - i);
        }    
        for (int k = 1; k < K; ++k) {
            for (int i = 0; i < n - 1; ++i) {
                for (int j = i + 1; j < n; ++j) {
                    dp[i][k] = max(dp[i][k], (sums[j] - sums[i]) / (j - i) + dp[j][k - 1]);
                }
            }
        }
        return dp[0][K - 1];
    }
};
```



我们可以对空间进行优化，由于每次的状态k，只跟前一个状态k-1有关，所以我们不需要将所有的状态都保存起来，只需要保存前一个状态的值就行了，那么我们就用一个一维数组就可以了，不断的进行覆盖，从而达到了节省空间的目的，参见代码如下：



解法二：

```
class Solution {
public:
    double largestSumOfAverages(vector<int>& A, int K) {
        int n = A.size();
        vector<double> sums(n + 1);
        vector<double> dp(n);
        for (int i = 0; i < n; ++i) {
            sums[i + 1] = sums[i] + A[i];
        }
        for (int i = 0; i < n; ++i) {
            dp[i] = (sums[n] - sums[i]) / (n - i);
        }    
        for (int k = 1; k < K; ++k) {
            for (int i = 0; i < n - 1; ++i) {
                for (int j = i + 1; j < n; ++j) {
                    dp[i] = max(dp[i], (sums[j] - sums[i]) / (j - i) + dp[j]);
                }
            }
        }
        return dp[0];
    }
};
```



我们也可以是用递归加记忆数组的方式来实现，记忆数组的运作原理和DP十分类似，也是一种cache，将已经计算过的结果保存起来，用的时候直接取即可，避免了大量的重复计算，参见代码如下：



解法三：

```
class Solution {
public:
    double largestSumOfAverages(vector<int>& A, int K) {
        int n = A.size();
        vector<vector<double>> memo(101, vector<double>(101));
        double cur = 0;
        for (int i = 0; i < n; ++i) {
            cur += A[i];
            memo[i + 1][1] = cur / (i + 1);
        }
        return helper(A, K, n, memo);
    }
    double helper(vector<int>& A, int k, int j, vector<vector<double>>& memo) {
        if (memo[j][k] > 0) return memo[j][k];
        double cur = 0;
        for (int i = j - 1; i > 0; --i) {
            cur += A[i];
            memo[j][k] = max(memo[j][k], helper(A, k - 1, i, memo) + cur / (j - i));
        }
        return memo[j][k];
    }
};
```



参考资料：

[https://leetcode.com/problems/largest-sum-of-averages/description/](https://leetcode.com/problems/largest-sum-of-averages/description/)

[https://leetcode.com/problems/largest-sum-of-averages/solution/](https://leetcode.com/problems/largest-sum-of-averages/solution/)

[https://leetcode.com/problems/largest-sum-of-averages/discuss/122739/C++JavaPython-Easy-Understood-Solution-with-Explanation](https://leetcode.com/problems/largest-sum-of-averages/discuss/122739/C++JavaPython-Easy-Understood-Solution-with-Explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












