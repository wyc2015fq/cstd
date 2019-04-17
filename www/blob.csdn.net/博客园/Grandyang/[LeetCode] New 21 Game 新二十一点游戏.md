# [LeetCode] New 21 Game 新二十一点游戏 - Grandyang - 博客园







# [[LeetCode] New 21 Game 新二十一点游戏](https://www.cnblogs.com/grandyang/p/10386525.html)









Alice plays the following game, loosely based on the card game "21".

Alice starts with `0` points, and draws numbers while she has less than `K` points.  During each draw, she gains an integer number of points randomly from the range `[1, W]`, where `W` is an integer.  Each draw is independent and the outcomes have equal probabilities.

Alice stops drawing numbers when she gets `K` or more points.  What is the probability that she has `N` or less points?

Example 1:
Input: N = 10, K = 1, W = 10
Output: 1.00000
Explanation:  Alice gets a single card, then stops.

Example 2:
Input: N = 6, K = 1, W = 10
Output: 0.60000
Explanation:  Alice gets a single card, then stops.
In 6 out of W = 10 possibilities, she is at or below N = 6 points.

Example 3:
Input: N = 21, K = 17, W = 10
Output: 0.73278
Note:
- `0 <= K <= N <= 10000`
- `1 <= W <= 10000`
- Answers will be accepted as correct if they are within `10^-5` of the correct answer.
- The judging time limit has been reduced for this question.





这道题就是赌桌上经典的21点游戏了，想起了当年实习的游轮活动，就有21点游戏的赌桌。我当时还纳闷为啥庄家到了17点以后就不再要牌了，原来里面大有学问啊，因为再多拿牌，增大了爆的概率，而如果小于17就不拿牌的话，会增大玩家赢的概率，估计是经过精心计算，用这个阈值庄家赢的概率最大吧。但当时有一位同期实习的大神，可以根据分析台面上已经出现的牌，来推出最合理的策略，因为庄家的规则是不变的，只要过了17就坚决不拿牌，但是大神却可以根据已出现的牌来制定自己的最优策略，经常能赢庄家。据大神称他去赌场经常都能赢上个小二百刀，给跪了有木有？！

好，来解题吧。这道题说的是有 [1, W] 范围内的牌，问我们当拿到不少于K点的时候就停止摸牌，最终点数能不超过的N点的概率。那么我们先来分析下，拿到 [1, W] 范围内的任意一张牌的概率是 1/W，因为是随机取的，所以拿到任意张牌的概率都是相等的。那么点数大于W的时候，概率怎么算呢，比如 W = 10, 我们拿到15点的概率是多少呢？这时候肯定不止拿一张牌了，那么我们分析最后一张牌，可以取1到10，那么能拿到15点就有十种情况，之前共拿5点且最后一张拿10，之前共拿6点且最后一张拿9，之前拿共7点且最后一张拿8，...，之前共拿14点且最后一张拿1。那么拿15点的概率就是把这十种的概率都加起来。这道题给的假设是每次取牌都是等概率的，不管什么时候拿到 [1, 10] 内的任意张牌的概率都是十分之一，但是现实情况肯定不是这样的，已经取出了的牌，不会放回了，所以现实情况要更加复杂。不用管它，反正我们拿最后一张牌的概率都是 1/W，由于是‘且’的关系，所以是概率相乘，可以将 1/W 提取出来，那么对于拿到x点的概率就可以归纳出下面的等式：

P(x) = 1/W * (P(x-1) + P(x-2) + P(x-W))

       = 1/W * sumP(x-W, x-1)

这里的x是有范围限制的，必须在 [W, K] 之间，因为小于等于W的点数概率都是 1/W，而大于等于K的时候，就不会再拿牌了。现在回过头来看看这道题要我们求什么，要求的是拿到不少于K点的时候就停止摸牌，最终点数能不超过的N点的概率，即 P(<=N | >= K)。那么现在我们就要引入强大的[条件概率公式](https://zh.wikipedia.org/wiki/%E6%9D%A1%E4%BB%B6%E6%A6%82%E7%8E%87)了，传说中的[贝叶斯公式](https://zh.wikipedia.org/wiki/%E8%B4%9D%E5%8F%B6%E6%96%AF%E5%AE%9A%E7%90%86)就是由其推导出来的：

P(A | B) = P(AB) / P(B)

意思就是在事件B发生的条件下发生事件A的概率，等于事件A和B同时发生的概率除以事件B单独发生的概率。那么带入本题的环境，就可以得到下列等式：

P(<=N | >=K) = P(<=N && >=K) / P(>=K)

就是说拿到不小于K点的前提下，还能不超过N点的概率，等于拿到不小于K点且不超过N点的概率除以拿到不小于K点的概率。这样，我们只要分别求出 P(<=N && >=K) 和 P(>=K) 就可以了：

P(<=N && >=K) = P(K) + P(K+1) + ... + P(N) = sumP(K, N)

P(>=K) = sumP(K, +∞) = sumP(K, K+W-1)

需要注意的是，一旦大于等于K+W了，那么概率就为0了，所以边界就从正无穷降到 K+W-1 了。既然说到了边界，那么就来处理一下corner case吧，当K=0时，由于题目中说当前点数大于等于K，不能摸牌，那么一开始就不能摸牌了，而 K <= N，所以永远不会超过N，概率返回1。还有就是当 N >= K+W 的时候，当我们大于等于K的时候，不能摸牌，此时不会超过N。当刚好为K-1的时候，此时还有一次摸牌机会，但最大也就摸个W，总共为K-1+W，还是小于N，所以返回概率为1。

根据上面的条件概率公式推导，P(>=K)的边界降到了 K+W-1, 所以我们只要更新到这个边界就都用了，因为 P(<=N && >=K) 的范围是 [K, N]，而 N 是要小于 K+W的。我们新建一个大小为 K+W 的一维数组sums，其中 sum[i] 表示获得范围 [0, i] 内的点数的概率综合，初始化 sum[0] 为1.0。下面来推导状态转移方程吧 ，通常来说，我们要更新 sum[i]，那么只要知道了 sum[i-1]，就只要算出 P[i]，就行了，因为 sum[i] = sum[i-1] + P[i]。但这道题的更新其实比较复杂，要考虑两个关键的位置，K和W，我们还是用经典的21点游戏来举例说明吧，N=21, K=17, W=10。先来说一下当点数不超过10的更新方法，这个其实比较简单，比如拿到七点的概率P[7]，根据我们上面对于P(x)的求法，我们知道可以拆分为下列多种情况：先拿到六点的概率(P[6])乘以再拿一个1点的概率(1/W)，先拿到五点的概率(P[5])乘以再拿一个2点的概率(1/W)，...，先拿到一点的概率(P[1])乘以再拿一个六点的概率(1/W)，直接拿个七点的概率(1/W)，那么统统加起来，就是:

P[7] = 1/W * (P[6] + p[5] + ... + P[1] + P[0]) = 1/W * sum[6]

那么归纳一下，对于 i <= W 的情况下：

P[i] = 1/W * sum[i-1]

sum[i] = sum[i-1] + P[i] = sum[i-1] + sum[i-1] / W     (when i <= W)

那么当 i > W 的时候，情况是不一样的，比如要求得到15点的概率P[15]，那么还是根据上面求P(x)的方法，拆分为下面多种情况：先拿到14点的概率(P[14])乘以再拿一个1点的概率(1/W)，先拿到13点的概率(P[13])乘以再拿一个2点的概率(1/W)，...，先拿到五点的概率(P[5])乘以再拿一个10点的概率(1/W)，那么统统加起来就是：

P[15] = 1/W * (P[14] + P[13] + ... + P[5]) = 1/W * (sum[14] - sum[4])

那么归纳一下，对于 i > W 的情况下：

P[i] = 1/W * (sum[i-1] - sum[i-W-1])

sum[i] = sum[i-1] + P[i] = sum[i-1] + (sum[i-1] - sum[i-W-1]) / W     (when i > W)

到这里，你以为就大功告成了吗？图样图森破，嘛哒得斯。还有一个K呢，更新K以内的P值，和更新大于K的P值是稍有不同的，比如当K=17时，我们要更新P[15]，那么跟上面分析的一样，同时还得考虑W的情况，归纳一下：

**P[i] = 1/W * sum[i-1]     (when i <= K && i <= W)**

**P[i] = 1/W * (sum[i-1] - sum[i-W-1])    (when i <= K && i > W)**

但是对于大于K的值，比如P[20]的更新方法就有所不同了，为啥呢？这要分析20点是怎么得来的，由于超过了17点就不能再摸牌了，所以20点只能由下列情况组成：先拿到16点的概率(P[16])再拿到一个4点的概率(1/W)，先拿到15点的概率(P[15])再拿到一个5点的概率(1/W)，...，先拿到10点的概率(P[10])再拿到一个10点的概率(1/W)，那么统统加起来就是：

P[20] = 1/W * (P[16] + P[15] + P[14] + ... + P[10]) = 1/W * (sum[16] - sum[9])

那么我们归纳一下，就有：

**P[i] = 1/W * sum[K-1]     (when i > K && i <= W)**

**P[i] = 1/W * (sum[K-1] - sum[i-W-1])    (when i > K && i > W)**

讲到这里，是不是头晕又眼花，哈哈，博主也快绕晕了，最重要的四个式子已经加粗显示了，K和W的大小关系其实是不知道的，不过我们可以把二者揉在一起，我们每次使用 i-1 和 K-1 中的较小值来算 P[i] 即可，这样就完美把K融到了W的分类情况中，当sum数组计算完成之后，我们就直接按照上面的条件概率公式来算 P(<=N | >=K) = P(<=N && >=K) / P(>=K) = sumP(K, N) / sumP(K, K+W-1) 就行了，写的累s博主了，听个青鸟缓解一下吧，跟博主一起唱～阿欧伊，阿欧伊，阿弄嗖啦～



解法一：

```
class Solution {
public:
    double new21Game(int N, int K, int W) {
        if (K == 0 || N >= K + W) return 1.0;
        vector<double> sum(K + W);
        sum[0] = 1.0;
        for (int i = 1; i < K + W; ++i) {
            int t = min(i - 1, K - 1);
            if (i <= W) sum[i] = sum[i - 1] + sum[t] / W;
            else sum[i] = sum[i - 1] + (sum[t] - sum[i - W - 1]) / W;
        }
        return (sum[N] - sum[K - 1]) / (sum[K + W - 1] - sum[K - 1]);
    }
};
```



下面这种解法跟上面的解法没有啥本质的区别，这里的dp数组跟上面的sum数组表达的意思是完全一样的，dp[i] 表示获得范围 [0, i] 内的点数的概率综合，初始化 dp[0] 为1.0。希望博主在上面已经解释清楚了，我们可以看到，这里并没有将K融合到W的分类中，而是多加了 (K, i] 区间的部分，所以当 i > K 时就要将这部分多加的减去，从而符合题意。还有一点让博主惊奇的地方是，这道题的条件概率和联合概率是相同的，根据之前的条件概率公式：

P(<=N | >=K) = P(<=N && >=K) / P(>=K)

就是说拿到不小于K点的前提下，还能不超过N点的概率，等于拿到不小于K点且不超过N点的概率除以拿到不小于K点的概率。但是实际上这道题P(<=N | >=K) = P(<=N && >=K)，即拿到不小于K点的前提下，还能不超过N点的概率，等于拿到不小于K点且不超过N点的概率。那么就是说拿到不小于K点的概率的总是为1，想想也是阿，只有在拿到不少K点的时候才停止摸牌，这样肯定点数不少于K点阿，单独计算这个概率简直是多此一举啊，参见代码如下：



解法二：

```
class Solution {
public:
    double new21Game(int N, int K, int W) {
        if (K == 0 || N >= K + W) return 1.0;
        vector<double> dp(K + W);
        dp[0] = 1.0;
        for (int i = 1; i < K + W; ++i) {
            dp[i] = dp[i - 1];
            if (i <= W) dp[i] += dp[i - 1] / W;
            else dp[i] += (dp[i - 1] - dp[i - W - 1]) / W;
            if (i > K) dp[i] -= (dp[i - 1] - dp[K - 1]) / W;
        }
        return dp[N] - dp[K - 1];
    }
};
```



下面这种解法还是大同小异，吃透了解法一的讲解，看这些变形基本都比较好理解。这里的dp数组意义跟上面的一样，但是并没有初始化大小为K+W，而是只初始化为了N+1，为啥呢，根据解法二的讲解，我们知道了这道题的条件概率和联合概率是相等的，所以只要求出P(<=N && >=K)，即 dp[N] - dp[K-1]，而这题不是更新完整个dp数组后再求联合概率，而是在更新的过程中就累加到了结果res，当 i >= K 的时候，正好可以将概率加入到结果res，而且此时不用再累加sumW，这里的sumW是保存的到目前为止的概率和，相当于之前的 dp[i-1]，还需要判断的是当 i >= W 时，要减去多加的概率 dp[i-W]，参见代码如下：



解法三：

```
class Solution {
public:
    double new21Game(int N, int K, int W) {
        if (K == 0 || N >= K + W) return 1.0;
        vector<double> dp(N + 1);
        dp[0] = 1.0;
        double sumW = 1.0, res = 0.0;
        for (int i = 1; i <= N; ++i) {
            dp[i] = sumW / W;
            if (i < K) sumW += dp[i];
            else res += dp[i];
            if (i - W >= 0) sumW -= dp[i - W];
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/new-21-game/](https://leetcode.com/problems/new-21-game/)

[https://leetcode.com/problems/new-21-game/discuss/132334/One-Pass-DP-O(N)](https://leetcode.com/problems/new-21-game/discuss/132334/One-Pass-DP-O(N))

[https://leetcode.com/problems/new-21-game/discuss/132478/C%2B%2B-12ms-O(K%2BW)-solution-with-explanation](https://leetcode.com/problems/new-21-game/discuss/132478/C%2B%2B-12ms-O(K%2BW)-solution-with-explanation)

[https://leetcode.com/problems/new-21-game/discuss/132358/Java-O(K-%2B-W)-DP-solution-with-explanation](https://leetcode.com/problems/new-21-game/discuss/132358/Java-O(K-%2B-W)-DP-solution-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












