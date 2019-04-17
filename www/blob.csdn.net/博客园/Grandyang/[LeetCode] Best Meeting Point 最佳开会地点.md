# [LeetCode] Best Meeting Point 最佳开会地点 - Grandyang - 博客园







# [[LeetCode] Best Meeting Point 最佳开会地点](https://www.cnblogs.com/grandyang/p/5291058.html)







A group of two or more people wants to meet and minimize the total travel distance. You are given a 2D grid of values 0 or 1, where each 1 marks the home of someone in the group. The distance is calculated using [Manhattan Distance](http://en.wikipedia.org/wiki/Taxicab_geometry), where distance(p1, p2) = `|p2.x - p1.x| + |p2.y - p1.y|`.

For example, given three people living at `(0,0)`, `(0,4)`, and `(2,2)`:
1 - 0 - 0 - 0 - 1
|   |   |   |   |
0 - 0 - 0 - 0 - 0
|   |   |   |   |
0 - 0 - 1 - 0 - 0
The point `(0,2)` is an ideal meeting point, as the total travel distance of 2+2+2=6 is minimal. So return 6.

**Hint:**
- Try to solve it in one dimension first. How can this solution apply to the two dimension case?



这道题让我们求最佳的开会地点，该地点需要到每个为1的点的曼哈顿距离之和最小，题目中给了我们提示，让我们先从一维的情况来分析，那么我们先看一维时有两个点A和B的情况,

______A_____P_______B_______

那么我们可以发现，只要开会为位置P在[A, B]区间内，不管在哪，距离之和都是A和B之间的距离，如果P不在[A, B]之间，那么距离之和就会大于A和B之间的距离，那么我们现在再加两个点C和D：

______C_____A_____P_______B______D______

我们通过分析可以得出，P点的最佳位置就是在[A, B]区间内，这样和四个点的距离之和为AB距离加上CD距离，在其他任意一点的距离都会大于这个距离，那么分析出来了上述规律，这题就变得很容易了，我们只要给位置排好序，然后用最后一个坐标减去第一个坐标，即CD距离，倒数第二个坐标减去第二个坐标，即AB距离，以此类推，直到最中间停止，那么一维的情况分析出来了，二维的情况就是两个一维相加即可，参见代码如下：



解法一：

```
class Solution {
public:
    int minTotalDistance(vector<vector<int>>& grid) {
        vector<int> rows, cols;
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[i].size(); ++j) {
                if (grid[i][j] == 1) {
                    rows.push_back(i);
                    cols.push_back(j);
                }
            }
        }
        return minTotalDistance(rows) + minTotalDistance(cols);
    }
    int minTotalDistance(vector<int> v) {
        int res = 0;
        sort(v.begin(), v.end());
        int i = 0, j = v.size() - 1;
        while (i < j) res += v[j--] - v[i++];
        return res;
    }
};
```



我们也可以不用多写一个函数，直接对rows和cols同时处理，稍稍能简化些代码：



解法二：

```
class Solution {
public:
    int minTotalDistance(vector<vector<int>>& grid) {
        vector<int> rows, cols;
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[i].size(); ++j) {
                if (grid[i][j] == 1) {
                    rows.push_back(i);
                    cols.push_back(j);
                }
            }
        }
        sort(cols.begin(), cols.end());
        int res = 0, i = 0, j = rows.size() - 1;
        while (i < j) res += rows[j] - rows[i] + cols[j--] - cols[i++];
        return res;
    }
};
```



参考资料：

[https://leetcode.com/discuss/65336/14ms-java-solution](https://leetcode.com/discuss/65336/14ms-java-solution)

[https://leetcode.com/discuss/85074/14-lines-concise-and-easy-understand-c-solution](https://leetcode.com/discuss/85074/14-lines-concise-and-easy-understand-c-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












