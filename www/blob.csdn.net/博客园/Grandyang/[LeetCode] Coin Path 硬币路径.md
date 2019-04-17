# [LeetCode] Coin Path 硬币路径 - Grandyang - 博客园







# [[LeetCode] Coin Path 硬币路径](https://www.cnblogs.com/grandyang/p/8183477.html)







Given an array `A` (index starts at `1`) consisting of N integers: A1, A2, ..., AN and an integer `B`. The integer `B`denotes that from any place (suppose the index is `i`) in the array `A`, you can jump to any one of the place in the array `A` indexed `i+1`, `i+2`, …, `i+B` if this place can be jumped to. Also, if you step on the index `i`, you have to pay Ai coins. If Ai is -1, it means you can’t jump to the place indexed `i` in the array.

Now, you start from the place indexed `1` in the array `A`, and your aim is to reach the place indexed `N` using the minimum coins. You need to return the path of indexes (starting from 1 to N) in the array you should take to get to the place indexed `N` using minimum coins.

If there are multiple paths with the same cost, return the lexicographically smallest such path.

If it's not possible to reach the place indexed N then you need to return an empty array.

Example 1:
Input: [1,2,4,-1,2], 2
Output: [1,3,5]



Example 2:
Input: [1,2,4,-1,2], 1
Output: []



Note:
- Path Pa1, Pa2, ..., Pan is lexicographically smaller than Pb1, Pb2, ..., Pbm, if and only if at the first `i` where Pai and Pbi differ, Pai < Pbi; when no such `i` exists, then `n` < `m`.
- A1 >= 0. A2, ..., AN (if exist) will in the range of [-1, 100].
- Length of A is in the range of [1, 1000].
- B is in the range of [1, 100].



这道题给了我们一个数组A，又给了我们一个整数B，表示能走的最大步数，数组上的每个数字都是cost值，如果到达某个位置，就要加上该位置上的数字，其实位置是在第一个数字上，目标是到达末尾位置，我们需要让总cost值最小，并输入路径，如果cos相同的话，输出字母顺序小的那个路径。还有就是如果数组上的某个位置为-1的话，表示到达该位置后不能再去下一个位置，而且数组末位置不能为-1。博主最开始写了一个递归的解法，结果MLE了，看来这道题对内存使用的管控极为苛刻。所以我们不能将所有的候选路径都存在内存中，而是应该建立祖先数组，即数组上每个位置放其父结点的位置，有点像联合查找Union Find中的root数组，再最后根据这个祖先数组来找出正确的路径。由于需要找出cost最小的路径，所以我们可以考虑用dp数组，其中dp[i]表示从开头到位置i的最小cost值，但是如果我们从后往前跳，那么dp[i]就是从末尾到位置i的最小cost值。

我们首先判断数组A的末尾数字是否为-1，是的话直接返回空集。否则就新建结果res数组，dp数组，和pos数组，其中dp数组都初始化为整型最大值，pos数组都初始化为-1。然后将dp数组的最后一个数字赋值为数组A的尾元素。因为我们要从后往前跳，那我们从后往前遍历，如果遇到数字-1，说明不能往前跳了，直接continue继续循环，然后对于每个遍历到的数字，我们都要遍历其上一步可能的位置的dp[j]值来更新当前dp[i]值，由于限制了步数B，所以最多能到i+B，为了防止越界，要取i+B和n-1中的较小值为界限，如果上一步dp[j]值为INT_MAX，说明上一个位置无法跳过来，直接continue，否则看上一个位置dp[j]值加上当前cost值A[i]，如果小于dp[i]，说明dp[i]需要更新，并且建立祖先数组的映射pos[i] = j。最后在循环结束后，我们判断dp[0]的值，如果是INT_MAX，说明没有跳到首位置，直接返回空集，否则我们就通过pos数组来取路径。我们从前往后遍历pos数组来取位置，直到遇到-1停止。另外要说明的就是，这种从后往前遍历的模式得到的路径一定是字母顺序最小的，[zestypanda大神的帖子](https://discuss.leetcode.com/topic/98399/c-dp-o-nb-time-o-n-space)中有证明，不过博主没太看懂-.-|||，可以带这个例子尝试：

A = [0, 0, 0], B = 2

上面这个例子得到的结果是[1, 2, 3]，是字母顺序最小的路径，而相同的cost路径[1, 3]，就不是字母顺序最小的路径，参见代码如下：



解法一：

```
class Solution {
public:
   vector<int> cheapestJump(vector<int>& A, int B) {
       if (A.back() == -1) return {};
        int n = A.size();
        vector<int> res, dp(n, INT_MAX), pos(n, -1);
        dp[n - 1] = A[n - 1];
        for (int i = n - 2; i >= 0; --i) {
            if (A[i] == -1) continue;
            for (int j = i + 1; j <= min(i + B, n - 1); ++j) {
                if (dp[j] == INT_MAX) continue;
                if (A[i] + dp[j] < dp[i]) {
                    dp[i] = A[i] + dp[j];
                    pos[i] = j;
                }
            }
        }
        if (dp[0] == INT_MAX) return res;
        for (int cur = 0; cur != -1; cur = pos[cur]) {
            res.push_back(cur + 1);
        }
        return res;
   }
};
```



下面这种方法是正向遍历的解法，正向跳的话就需要另一个数组len，len[i]表示从开头到达位置i的路径的长度，如果两个路径的cost相同，那么一定是路径长度大的字母顺序小，可以参见例子 A = [0, 0, 0], B = 2。

具体的写法就不讲了，跟上面十分类似，参考上面的讲解，需要注意的就是更新的判定条件中多了一个t == dp[i] && len[i] < len[j] + 1，就是判断当cost相同时，我们取长度大路径当作结果保存。还有就是最后查找路径时要从末尾往前遍历，只要遇到-1时停止，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> cheapestJump(vector<int>& A, int B) {
        if (A.back() == -1) return {};
        int n = A.size();
        vector<int> res, dp(n, INT_MAX), pos(n, -1), len(n, 0);
        dp[0] = 0;
        for (int i = 0; i < n; ++i) {
            if (A[i] == -1) continue;
            for (int j = max(0, i - B); j < i; ++j) {
                if (dp[j] == INT_MAX) continue;
                int t = A[i] + dp[j];
                if (t < dp[i] || (t == dp[i] && len[i] < len[j] + 1)) {
                    dp[i] = t;
                    pos[i] = j;
                    len[i] = len[j] + 1;
                }
            }
        }
        if (dp[n - 1] == INT_MAX) return res;
        for (int cur = n - 1; cur != -1; cur = pos[cur]) {
            res.insert(res.begin(), cur + 1);
        }
        return res;
    }
};
```



类似题目：

[House Robber II](http://www.cnblogs.com/grandyang/p/4518674.html)

[House Robber](http://www.cnblogs.com/grandyang/p/4383632.html)

[Frog Jump](http://www.cnblogs.com/grandyang/p/5888439.html)

[Jump Game](http://www.cnblogs.com/grandyang/p/4371526.html)

[Jump Game II](http://www.cnblogs.com/grandyang/p/4373533.html)



参考资料：

[https://discuss.leetcode.com/topic/98399/c-dp-o-nb-time-o-n-space](https://discuss.leetcode.com/topic/98399/c-dp-o-nb-time-o-n-space)

[https://discuss.leetcode.com/topic/98491/java-22-lines-solution-with-proof](https://discuss.leetcode.com/topic/98491/java-22-lines-solution-with-proof)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












