# [LeetCode] Escape The Ghosts 逃离鬼魂 - Grandyang - 博客园







# [[LeetCode] Escape The Ghosts 逃离鬼魂](https://www.cnblogs.com/grandyang/p/9165361.html)









You are playing a simplified Pacman game. You start at the point `(0, 0)`, and your destination is` (target[0], target[1])`. There are several ghosts on the map, the i-th ghost starts at` (ghosts[i][0], ghosts[i][1])`.

Each turn, you and all ghosts simultaneously *may* move in one of 4 cardinal directions: north, east, west, or south, going from the previous point to a new point 1 unit of distance away.

You escape if and only if you can reach the target before any ghost reaches you (for any given moves the ghosts may take.)  If you reach any square (including the target) at the same time as a ghost, it doesn't count as an escape.

Return True if and only if it is possible to escape.
Example 1:
Input: 
ghosts = [[1, 0], [0, 3]]
target = [0, 1]
Output: true
Explanation: 
You can directly reach the destination (0, 1) at time 1, while the ghosts located at (1, 0) or (0, 3) have no way to catch up with you.
Example 2:
Input: 
ghosts = [[1, 0]]
target = [2, 0]
Output: false
Explanation: 
You need to reach the destination (2, 0), but the ghost at (1, 0) lies between you and the destination.
Example 3:
Input: 
ghosts = [[2, 0]]
target = [1, 0]
Output: false
Explanation: 
The ghost can reach the target at the same time as you.

Note:
- All points have coordinates with absolute value <= `10000`.
- The number of ghosts will not exceed `100`.





这道题就是经典的吃豆人游戏啦，不过是简化版，小人只能躲开鬼魂，并不能吃大力丸，反干鬼魂。小人在原点，有若干个鬼魂在不同的位置，给了一个目标点，问小人能不能安全到达目标点。这里的鬼魂的设定跟游戏中的一样，都是很智能的，会朝着你移动，而且这里设定了如果跟鬼魂同时到达目标点也算输。那么实际上这道题就是要求出小人到目标点的最短距离，注意这里的距离不是两点之间的Euclidean距离，而应该是曼哈顿距离，即横纵坐标分别求差的绝对值再相加。求出小人到目标点到最短距离后，我们还要求每个鬼魂到目标点的最短距离，如果有一个鬼魂到目标带你的最短距离小于等于小人到目标点到最短距的话，那么就返回false，否则返回true，参见代码如下：



解法一：

```
class Solution {
public:
    bool escapeGhosts(vector<vector<int>>& ghosts, vector<int>& target) {
        int dist = abs(target[0]) + abs(target[1]), mn = INT_MAX;
        for (auto ghost : ghosts) {
            int t = abs(ghost[0] - target[0]) + abs(ghost[1] - target[1]);
            mn = min(mn, t);
        }
        return dist < mn;
    }
};
```



我们可以对上面的解法进行一个小优化，就是我们其实并不需要算完每一个鬼魂到目标点到最短距离，而是每算一个就进行比较，只要小于等于小人到目标点的最短距离了，就直接返回false。循环退出后返回true，参见代码如下：



解法二：

```
class Solution {
public:
    bool escapeGhosts(vector<vector<int>>& ghosts, vector<int>& target) {
        int dist = abs(target[0]) + abs(target[1]);
        for (auto ghost : ghosts) {
            int t = abs(ghost[0] - target[0]) + abs(ghost[1] - target[1]);
            if (t <= dist) return false;
        }
        return true;
    }
};
```



参考资料：

[https://leetcode.com/problems/escape-the-ghosts/discuss/116507/Java-5-liner](https://leetcode.com/problems/escape-the-ghosts/discuss/116507/Java-5-liner)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












