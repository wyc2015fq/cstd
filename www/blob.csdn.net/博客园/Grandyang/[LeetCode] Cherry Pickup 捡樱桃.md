# [LeetCode] Cherry Pickup 捡樱桃 - Grandyang - 博客园







# [[LeetCode] Cherry Pickup 捡樱桃](https://www.cnblogs.com/grandyang/p/8215787.html)







In a N x N `grid` representing a field of cherries, each cell is one of three possible integers.


- 0 means the cell is empty, so you can pass through;
- 1 means the cell contains a cherry, that you can pick up and pass through;
- -1 means the cell contains a thorn that blocks your way.



Your task is to collect maximum number of cherries possible by following the rules below:


- Starting at the position (0, 0) and reaching (N-1, N-1) by moving right or down through valid path cells (cells with value 0 or 1);
- After reaching (N-1, N-1), returning to (0, 0) by moving left or up through valid path cells;
- When passing through a path cell containing a cherry, you pick it up and the cell becomes an empty cell (0);
- If there is no valid path between (0, 0) and (N-1, N-1), then no cherries can be collected.





Example 1:
Input: grid =
[[0, 1, -1],
 [1, 0, -1],
 [1, 1,  1]]
Output: 5
Explanation: 
The player started at (0, 0) and went down, down, right right to reach (2, 2).
4 cherries were picked up during this single trip, and the matrix becomes [[0,1,-1],[0,0,-1],[0,0,0]].
Then, the player went left, up, up, left to return home, picking up one more cherry.
The total number of cherries picked up is 5, and this is the maximum possible.



Note:
- `grid` is an `N` by `N` 2D array, with `1 <= N <= 50`.
- Each `grid[i][j]` is an integer in the set `{-1, 0, 1}`.
- It is guaranteed that grid[0][0] and grid[N-1][N-1] are not -1.



这道题给了我们一个二维数组，每个数字只有三个数字，-1，0，和1，其中-1表示障碍物不能通过，1表示有樱桃并可以通过，0表示没有樱桃并可以通过，并设定左上角为起点，右下角为终点，让我们从起点走到终点，再从终点返回起点，求最多能捡的樱桃的个数，限定起点和终点都没有障碍物。博主开始想的是就用dp来做呗，先从起点走到终点，求最多能捡多个樱桃，然后将捡起樱桃后将grid值变为0，然后再走一遍，把两次得到的樱桃数相加即可，但是类似贪婪算法的dp解法却跪在了下面这个case：



```
1 1 1 1 0 0 0
0 0 0 1 0 0 0
0 0 0 1 0 0 1
1 0 0 1 0 0 0
0 0 0 1 0 0 0
0 0 0 1 0 0 0
0 0 0 1 1 1 1
```



我们可以看出，红色的轨迹是第一次dp解法走过的路径，共拿到了13个樱桃，但是回到起点的话，剩下的两个樱桃无论如何也不可能同时拿到，只能拿到1颗，所以总共只能捡到14颗樱桃，而实际上所有的樱桃都可以捡到，需要换个走法的话，比如下面这种走法：



```
1 1 1 1 0 0 0
0 0 0 1 0 0 0
0 0 0 1 0 0 1
1 0 0 1 0 0 0
0 0 0 1 0 0 0
0 0 0 1 0 0 0
0 0 0 1 1 1 1
```



红色为从起点到终点的走法，共拿到9颗樱桃，回去走蓝色的路径，可拿到6颗樱桃，所以总共15颗都能收入囊中。那这是怎么回事，原因出在了我们的dp递推式的设计上，博主之前设计式，当前位置的樱桃数跟上边和左边的樱桃数有关，取二者的较大值，如果只是从起点到终点走单程的话，这种设计是没有问题的，可以拿到最多的樱桃，但如果是round trip的话，那么就不行了。这里参考的还是[fun4LeetCode大神的帖子](https://discuss.leetcode.com/topic/113762/step-by-step-guidance-of-the-o-n-3-time-and-o-n-2-space-solution)，范佛利特扣德大神的帖子每次讲解都写的巨详细，总是让博主有种读paper的感觉。博主就挑选部分来讲讲，完整版可以自己去读一读大神的亲笔～

最开始时博主定义的dp[i][j]为单程的，即到达(i, j)位置能捡到的最大樱桃数，即：

```
T(i, j) = grid[i][j] + max{ T(i-1, j), T(i, j-1) }
```

但是定义单程就得改变grid的值，再进行一次dp计算时，就会陷入之前例子中的陷阱。所以我们的dp[i][j]还是需要定义为round trip的，即到达(i, j)位置并返回起点时能捡到的最大樱桃数，但是新的问题就来了，樱桃只有一个，只能捡一次，去程捡了，返程就不能再捡了，如何才能避免重复计算呢？我们只有i和j是不够的，其只能定义去程的位置，我们还需要pg，(不是pgone哈哈)，来定义返程的位置，那么重现关系Recurrence Relations就变成了 T(i, j, p, g)，我们有分别两种方式离开(i, j)和(p, g)，我们suppose时从终点往起点遍历，那么就有4种情况：

```
Case 1: (0, 0) ==> (i-1, j) ==> (i, j); (p, q) ==> (p-1, q) ==> (0, 0)
Case 2: (0, 0) ==> (i-1, j) ==> (i, j); (p, q) ==> (p, q-1) ==> (0, 0)
Case 3: (0, 0) ==> (i, j-1) ==> (i, j); (p, q) ==> (p-1, q) ==> (0, 0)
Case 4: (0, 0) ==> (i, j-1) ==> (i, j); (p, q) ==> (p, q-1) ==> (0, 0)
```

根据定义，我们有：

```
Case 1 is equivalent to T(i-1, j, p-1, q) + grid[i][j] + grid[p][q];
Case 2 is equivalent to T(i-1, j, p, q-1) + grid[i][j] + grid[p][q];
Case 3 is equivalent to T(i, j-1, p-1, q) + grid[i][j] + grid[p][q];
Case 4 is equivalent to T(i, j-1, p, q-1) + grid[i][j] + grid[p][q];
```

因此，我们的重现关系可以写作：

```
T(i, j, p, q) = grid[i][j] + grid[p][q] + max{T(i-1, j, p-1, q), T(i-1, j, p, q-1), T(i, j-1, p-1, q), T(i, j-1, p, q-1)}
```

为了避免重复计算，我们希望 grid[i][j] 和 grid[p][g] 不出现在T(i-1, j, p-1, q), T(i-1, j, p, q-1), T(i, j-1, p-1, q) 和 T(i, j-1, p, q-1)中的任意一个上。显而易见的是(i, j)不会出现在(0, 0) ==> (i-1, j) 或 (0, 0) ==> (i, j-1) 的路径上，同理，(p, g) 也不会出现在 (p-1, q) ==> (0, 0) 或 (p, q-1) ==> (0, 0) 的路径上。因此，我们需要保证(i, j) 不会出现在 (p-1, q) ==> (0, 0) 或 (p, q-1) ==> (0, 0) 的路径上，同时 (p, g)不会出现在(0, 0) ==> (i-1, j) 或 (0, 0) ==> (i, j-1) 的路径上，怎么做呢？

我们观察到(0, 0) ==> (i-1, j) 和 (0, 0) ==> (i, j-1) 的所有点都在矩形 [0, 0, i, j] 中（除了右下角点(i, j)点），所以只要 (p, g) 不在矩形 [0, 0, i, j] 中就行了，注意(p, g) 和 (i, j) 是有可能重合了，这种情况特殊处理一下就行了。同理， (i, j) 也不能在矩形 [0, 0, p, g] 中，那么以下三个条件中需要满足一个：

```
i < p && j > q
i == p && j == q
i > p && j < q
```

为了满足上述条件，我们希望当 i 或 p 增加的时候，j 或 q 减小，那么我们可以有这个等式:

```
k = i + j = p + q
```

其中k为从起点开始走的步数，所以我们可以用 T(k, i, p)  来代替 T(i, j, p, g)，那么我们的重现关系式就变成了：

```
T(k, i, p) = grid[i][k-i] + grid[p][k-p] + max{T(k-1, i-1, p-1), T(k-1, i-1, p), T(k-1, i, p-1), T(k-1, i, p)}.
```

当 i == p 时，grid[i][k-i] 和 grid[p][k-p] 就相等了，此时只能加一个。我们注意到 i, j, p, q 的范围是 [0, n)， 意味着k只能在范围 [0, 2n - 1) 中， 初始化时 T(0, 0, 0) = grid[0][0]。我们这里的重现关系T虽然是三维的，但是我们可以用二维dp数组来实现，因为第k步的值只依赖于第k-1步的情况，参见代码如下：



```
class Solution {
public:
    int cherryPickup(vector<vector<int>>& grid) {
        int n = grid.size(), mx = 2 * n - 1;
        vector<vector<int>> dp(n, vector<int>(n, -1));
        dp[0][0] = grid[0][0];
        for (int k = 1; k < mx; ++k) {
            for (int i = n - 1; i >= 0; --i) {
                for (int p = n - 1; p >= 0; --p) {
                    int j = k - i, q = k - p;
                    if (j < 0 || j >= n || q < 0 || q >= n || grid[i][j] < 0 || grid[p][q] < 0) {
                        dp[i][p] = -1;
                        continue;
                    }
                    if (i > 0) dp[i][p] = max(dp[i][p], dp[i - 1][p]);
                    if (p > 0) dp[i][p] = max(dp[i][p], dp[i][p - 1]);
                    if (i > 0 && p > 0) dp[i][p] = max(dp[i][p], dp[i - 1][p - 1]);
                    if (dp[i][p] >= 0) dp[i][p] += grid[i][j] + (i != p ? grid[p][q] : 0);
                }
            }
        }
        return max(dp[n - 1][n - 1], 0);
    }
};
```



类似题目：

[Minimum Path Sum](http://www.cnblogs.com/grandyang/p/4353255.html)

[Dungeon Game](http://www.cnblogs.com/grandyang/p/4233035.html)



参考资料：

[https://discuss.leetcode.com/topic/112877/annotated-c-dp-solution](https://discuss.leetcode.com/topic/112877/annotated-c-dp-solution)

[https://discuss.leetcode.com/topic/113762/step-by-step-guidance-of-the-o-n-3-time-and-o-n-2-space-solution](https://discuss.leetcode.com/topic/113762/step-by-step-guidance-of-the-o-n-3-time-and-o-n-2-space-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












