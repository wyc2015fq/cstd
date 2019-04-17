# [LeetCode] Dungeon Game 地牢游戏 - Grandyang - 博客园







# [[LeetCode] Dungeon Game 地牢游戏](https://www.cnblogs.com/grandyang/p/4233035.html)







The demons had captured the princess (P) and imprisoned her in the bottom-right corner of a dungeon. The dungeon consists of M x N rooms laid out in a 2D grid. Our valiant knight (K) was initially positioned in the top-left room and must fight his way through the dungeon to rescue the princess.

The knight has an initial health point represented by a positive integer. If at any point his health point drops to 0 or below, he dies immediately.

Some of the rooms are guarded by demons, so the knight loses health (*negative* integers) upon entering these rooms; other rooms are either empty (*0's*) or contain magic orbs that increase the knight's health (*positive* integers).

In order to reach the princess as quickly as possible, the knight decides to move only rightward or downward in each step.



Write a function to determine the knight's minimum initial health so that he is able to rescue the princess.

For example, given the dungeon below, the initial health of the knight must be at least 7 if he follows the optimal path `RIGHT-> RIGHT -> DOWN -> DOWN`.
|-2 (K)|-3|3|
|----|----|----|
|-5|-10|1|
|10|30|-5 (P)|



Notes:
- The knight's health has no upper bound.
- Any room can contain threats or power-ups, even the first room the knight enters and the bottom-right room where the princess is imprisoned.



Credits:
Special thanks to [@stellari](https://oj.leetcode.com/discuss/user/stellari) for adding this problem and creating all test cases.



这道王子救公主的题还是蛮新颖的，我最开始的想法是比较右边和下边的数字的大小，去大的那个，但是这个算法对某些情况不成立，比如下面的情况：
|1 (K)|-3|3|
|----|----|----|
|0|-2|0|
|-3|-3|-3 (P)|

如果按我的那种算法走的路径为 1 -> 0 -> -2 -> 0 -> -3, 这样的话骑士的起始血量要为5，而正确的路径应为 1 -> -3 -> 3 -> 0 -> -3, 这样骑士的骑士血量只需为3。无奈只好上网看大神的解法，发现统一都是用[动态规划Dynamic Programming](http://zh.wikipedia.org/wiki/%E5%8A%A8%E6%80%81%E8%A7%84%E5%88%92)来做，建立一个二维数组dp，其中dp[i][j]用来表示当前位置 (i, j) 出发的起始血量，最先处理的是公主所在的房间的起始生命值，然后慢慢向第一个房间扩散，不断的得到各个位置的最优的生命值。逆向推正是本题的精髓所在啊，仔细想想也是，如果从起始位置开始遍历，我们并不知道初始时应该初始化的血量，但是到达公主房间后，我们知道血量至少不能小于1，如果公主房间还需要掉血的话，那么掉血后剩1才能保证起始位置的血量最小。那么下面来推导状态转移方程，首先考虑每个位置的血量是由什么决定的，骑士会挂主要是因为去了下一个房间时，掉血量大于本身的血值，而能去的房间只有右边和下边，所以当前位置的血量是由右边和下边房间的可生存血量决定的，进一步来说，应该是由较小的可生存血量决定的，因为较我们需要起始血量尽可能的少，因为我们是逆着往回推，骑士逆向进入房间后PK后所剩的血量就是骑士正向进入房间时pk前的起始血量。所以用当前房间的右边和下边房间中骑士的较小血量减去当前房间的数字，如果是负数或着0，说明当前房间是正数，这样骑士进入当前房间后的生命值是1就行了，因为不会减血。而如果差是正数的话，当前房间的血量可能是正数也可能是负数，但是骑士进入当前房间后的生命值就一定要是这个差值。所以我们的状态转移方程是dp[i][j] = max(1, min(dp[i+1][j], dp[i][j+1]) - dungeon[i][j])。为了更好的处理边界情况，我们的二维dp数组比原数组的行数列数均多1个，先都初始化为整型数最大值INT_MAX，由于我们知道到达公主房间后，骑士火拼完的血量至少为1，那么此时公主房间的右边和下边房间里的数字我们就都设置为1，这样到达公主房间的生存血量就是1减去公主房间的数字和1相比较，取较大值，就没有问题了，代码如下：



解法一：

```
class Solution {
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        int m = dungeon.size(), n = dungeon[0].size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, INT_MAX));
        dp[m][n - 1] = 1; dp[m - 1][n] = 1;
        for (int i = m - 1; i >= 0; --i) {
            for (int j = n - 1; j >= 0; --j) {
                dp[i][j] = max(1, min(dp[i + 1][j], dp[i][j + 1]) - dungeon[i][j]);
            }
        }
        return dp[0][0];
    }
};
```



我们可以对空间进行优化，使用一个一维的dp数组，并且不停的覆盖原有的值，参见代码如下：



解法二：

```
class Solution {
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        int m = dungeon.size(), n = dungeon[0].size();
        vector<int> dp(n + 1, INT_MAX);
        dp[n - 1] = 1;
        for (int i = m - 1; i >= 0; --i) {
            for (int j = n - 1; j >= 0; --j) {
                dp[j] = max(1, min(dp[j], dp[j + 1]) - dungeon[i][j]);
            }
        }
        return dp[0];
    }
};
```



类似题目：

[Unique Paths](http://www.cnblogs.com/grandyang/p/4353555.html)

[Minimum Path Sum](http://www.cnblogs.com/grandyang/p/4353255.html)

[Cherry Pickup](http://www.cnblogs.com/grandyang/p/8215787.html)



[参考资料：](https://leetcode.com/problems/dungeon-game/)

[https://leetcode.com/problems/dungeon-game/](https://leetcode.com/problems/dungeon-game/)

[https://leetcode.com/problems/dungeon-game/discuss/52774/C++-DP-solution](https://leetcode.com/problems/dungeon-game/discuss/52774/C++-DP-solution)

[https://leetcode.com/problems/dungeon-game/discuss/52843/6-lines-16-ms-C++-O(mn)-Time-O(n)-Space](https://leetcode.com/problems/dungeon-game/discuss/52843/6-lines-16-ms-C++-O(mn)-Time-O(n)-Space)

[https://leetcode.com/problems/dungeon-game/discuss/52790/My-AC-Java-Version-Suggestions-are-welcome](https://leetcode.com/problems/dungeon-game/discuss/52790/My-AC-Java-Version-Suggestions-are-welcome)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












