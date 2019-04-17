# [CareerCup] 9.2 Robot Moving 机器人移动 - Grandyang - 博客园







# [[CareerCup] 9.2 Robot Moving 机器人移动](https://www.cnblogs.com/grandyang/p/4817962.html)







9.2 Imagine a robot sitting on the upper left corner of an X by Y grid. The robot can only move in two directions: right and down. How many possible paths are there for the robot to go from (0,0) to (X,Y)? 
 FOLLOW UP 
 Imagine certain spots are "off limits," such that the robot cannot step on them. Design an algorithm to find a path for the robot from the top left to the bottom right.



LeetCode上的原题，请参见我之前的博客[Unique Paths 不同的路径](http://www.cnblogs.com/grandyang/p/4353555.html)和[Unique Paths II 不同的路径之二](http://www.cnblogs.com/grandyang/p/4353680.html)。



解法一：

```
class Solution {
public:
    int getPath(int x, int y) {
        vector<int> dp(y + 1, 1);
        for (int i = 1; i <= x; ++i) {
            for (int j = 1; j <= y; ++j) {
                dp[j] += dp[j - 1];
            }
        }
        return dp[y];
    }
};
```



解法二：

```
class Solution {
public:
    int getPath(int x, int y) {
        double num = 1, denom = 1;
        int small = x < y ? x : y;
        for (int i = 1; i <= small; ++i) {
            num *= x + y - i + 1;
            denom *= i;
        }
        return (int)(num / denom);
    }
};
```



这道题的Follow up说格子中可能有障碍物，即不能到达的位子，让我们找到一条从左上点到右下点的路径，只需一条而已，不用将所有的都找出来，那么我们使用递归来做，我们反方向走，从右下点往左上点找，我们用哈希表来记录某个位置是否访问过，当递归到原点的时候，我们把原点加入结果中，然后逐层递归返回，把路径中的点依次加入结果中，这样结果中保存的顺序就是正确的，参见代码如下：



```
class Point {
public:
    int _x, _y;
    Point(int x, int y): _x(x), _y(y) {}
};

class Solution {
public:
    vector<Point*> getPath(vector<vector<int> > &grid, int x, int y) {
        vector<Point*> res;
        unordered_map<Point*, bool> m;
        getPathDFS(grid, x, y, m, res);
        return res;
    }
    bool getPathDFS(vector<vector<int> > &grid, int x, int y, unordered_map<Point*, bool> &m, vector<Point*> &res) {
        if (x < 0 || y < 0 || grid[x][y] == 1) return false;
        Point *p = new Point(x, y);
        if (m.find(p) != m.end()) return m[p];
        bool isAtOrigin = (x == 0) && (y == 0), success = false;
        if (isAtOrigin || getPathDFS(grid, x, y - 1, m, res) || getPathDFS(grid, x - 1, y, m, res)) {
            res.push_back(p);
            success = true;
        }
        m[p] = success;
        return success;
    }
};
```














