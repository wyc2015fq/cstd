# [LeetCode] Cheapest Flights Within K Stops K次转机内的最便宜的航班 - Grandyang - 博客园







# [[LeetCode] Cheapest Flights Within K Stops K次转机内的最便宜的航班](https://www.cnblogs.com/grandyang/p/9109981.html)







There are `n` cities connected by `m` flights. Each fight starts from city `u `and arrives at `v` with a price `w`.

Now given all the cities and fights, together with starting city `src` and the destination `dst`, your task is to find the cheapest price from `src` to `dst` with up to `k` stops. If there is no such route, output `-1`.
Example 1:
Input: 
n = 3, edges = [[0,1,100],[1,2,100],[0,2,500]]
src = 0, dst = 2, k = 1
Output: 200
Explanation: 
The graph looks like this:

![](https://s3-lc-upload.s3.amazonaws.com/uploads/2018/02/16/995.png)
The cheapest price from city `0` to city `2` with at most 1 stop costs 200, as marked red in the picture.Example 2:
Input: 
n = 3, edges = [[0,1,100],[1,2,100],[0,2,500]]
src = 0, dst = 2, k = 0
Output: 500
Explanation: 
The graph looks like this:

![](https://s3-lc-upload.s3.amazonaws.com/uploads/2018/02/16/995.png)
The cheapest price from city `0` to city `2` with at most 0 stop costs 500, as marked blue in the picture.
Note:
- The number of nodes `n` will be in range `[1, 100]`, with nodes labeled from `0` to `n`` - 1`.
- The size of `flights` will be in range `[0, n * (n - 1) / 2]`.
- The format of each flight will be `(src, ``dst``, price)`.
- The price of each flight will be in the range `[1, 10000]`.
- `k` is in the range of `[0, n - 1]`.
- There will not be any duplicated flights or self cycles.



这道题给了我们一些航班信息，包括出发地，目的地，和价格，然后又给了我们起始位置和终止位置，说是最多能转K次机，让我们求出最便宜的航班价格。那么实际上这道题就是一个有序图的遍历问题，博主最先尝试的递归解法由于没有做优化，TLE了，实际上我们可以通过剪枝处理，从而压线过OJ。首先我们要建立这个图，选取的数据结构就是邻接链表的形式，具体来说就是建立每个结点和其所有能到达的结点的集合之间的映射，然后就是用DFS来遍历这个图了，用变量cur表示当前遍历到的结点序号，还是当前剩余的转机次数K，访问过的结点集合visited，当前累计的价格out，已经全局的最便宜价格res。在递归函数中，首先判断如果当前cur为目标结点dst，那么结果res赋值为out，并直接返回。你可能会纳闷为啥不是取二者中较小值更新结果res，而是直接赋值呢？原因是我们之后做了剪枝处理，使得out一定会小于结果res。然后判断如果K小于0，说明超过转机次数了，直接返回。然后就是遍历当前结点cur能到达的所有结点了，对于遍历到的结点，首先判断如果当前结点已经访问过了，直接跳过。或者是当前价格out加上到达这个结点需要的价格之和大于结果res的话，那么直接跳过。这个剪枝能极大的提高效率，是压线过OJ的首要功臣。之后就是标记结点访问，调用递归函数，以及还原结点状态的常规操作了，参见代码如下：



解法一：

```
class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int K) {
        int res = INT_MAX;
        unordered_map<int, vector<vector<int>>> m;
        unordered_set<int> visited{{src}};
        for (auto flight : flights) {
            m[flight[0]].push_back({flight[1], flight[2]});
        }
        helper(m, src, dst, K, visited, 0, res);
        return (res == INT_MAX) ? -1 : res;
    }
    void helper(unordered_map<int, vector<vector<int>>>& m, int cur, int dst, int K, unordered_set<int>& visited, int out, int& res) {
        if (cur == dst) {res = out; return;}
        if (K < 0) return;
        for (auto a : m[cur]) {
            if (visited.count(a[0]) || out + a[1] > res) continue;
            visited.insert(a[0]);
            helper(m, a[0], dst, K - 1, visited, out + a[1], res);
            visited.erase(a[0]);
        }
    }
};
```



下面这种解法是用BFS来做的，还是来遍历图，不过这次是一层一层的遍历，需要使用queue来辅助。前面建立图的数据结构的操作和之前相同，BFS的写法还是经典的写法，但需要注意的是这里也同样的做了剪枝优化，当当前价格加上新到达位置的价格之和大于结果res的话直接跳过。最后注意如果超过了转机次数就直接break，参见代码如下： 



解法二：

```
class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int K) {
        int res = INT_MAX, cnt = 0;
        unordered_map<int, vector<vector<int>>> m;
        queue<vector<int>> q{{{src, 0}}};
        for (auto flight : flights) {
            m[flight[0]].push_back({flight[1], flight[2]});
        }
        while (!q.empty()) {
            for (int i = q.size(); i > 0; --i) {
                auto t = q.front(); q.pop();
                if (t[0] == dst) res = min(res, t[1]);
                for (auto a : m[t[0]]) {
                    if (t[1] + a[1] > res) continue;
                    q.push({a[0], t[1] + a[1]});
                }
            }
            if (cnt++ > K) break;
        }
        return (res == INT_MAX) ? -1 : res;
    }
};
```



再来看使用Bellman Ford算法的解法，关于此算法的detail可以上网搜帖子看看。核心思想还是用的动态规划Dynamic Programming，最核心的部分就是松弛操作Relaxation，也就是DP的状态转移方程。这里我们使用一个二维DP数组，其中dp[i][j]表示最多飞i次航班到达j位置时的最少价格，那么dp[0][src]初始化为0，因为飞0次航班的价格都为0，转机K次，其实就是飞K+1次航班，我们开始遍历，i从1到K+1，每次dp[i][src]都初始化为0，因为在起点的价格也为0，然后即使遍历所有的航班x，更新dp[i][x[1]]，表示最多飞i次航班到达航班x的目的地的最低价格，用最多飞i-1次航班，到达航班x的起点的价格加上航班x的价格之和，二者中取较小值更新即可，参见代码如下：



解法三：

```
class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int K) {
        vector<vector<int>> dp(K + 2, vector<int>(n, 1e9));
        dp[0][src] = 0;
        for (int i = 1; i <= K + 1; ++i) {
            dp[i][src] = 0;
            for (auto x : flights) {
                dp[i][x[1]] = min(dp[i][x[1]], dp[i - 1][x[0]] + x[2]);
            }
        }
        return (dp[K + 1][dst] >= 1e9) ? -1 : dp[K + 1][dst];
    }
};
```



我们可以稍稍优化下上面解法的空间复杂度，使用一个一维的DP数组即可，具体思路没有啥太大的区别，参见代码如下：



解法四：

```
class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int K) {
        vector<int> dp(n, 1e9);
        dp[src] = 0;
        for (int i = 0; i <= K; ++i) {
            vector<int> t = dp;
            for (auto x : flights) {
                t[x[1]] = min(t[x[1]], dp[x[0]] + x[2]);
            }
            dp = t;
        }
        return (dp[dst] >= 1e9) ? -1 : dp[dst];
    }
};
```



类似题目：

[Maximum Vacation Days](http://www.cnblogs.com/grandyang/p/6919389.html)



参考资料：

[https://leetcode.com/problems/cheapest-flights-within-k-stops/discuss/115596/c++-8-line-bellman-ford](https://leetcode.com/problems/cheapest-flights-within-k-stops/discuss/115596/c++-8-line-bellman-ford)

[https://leetcode.com/problems/cheapest-flights-within-k-stops/discuss/128217/Three-C++-solutions-BFS-DFS-and-BF](https://leetcode.com/problems/cheapest-flights-within-k-stops/discuss/128217/Three-C++-solutions-BFS-DFS-and-BF)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












