# [LeetCode] Number Of Corner Rectangles 边角矩形的数量 - Grandyang - 博客园







# [[LeetCode] Number Of Corner Rectangles 边角矩形的数量](https://www.cnblogs.com/grandyang/p/8433813.html)







Given a grid where each entry is only 0 or 1, find the number of corner rectangles.

A *corner rectangle* is 4 distinct 1s on the grid that form an axis-aligned rectangle. Note that only the corners need to have the value 1. Also, all four 1s used must be distinct.



Example 1:
Input: grid = 
[[1, 0, 0, 1, 0],
 [0, 0, 1, 0, 1],
 [0, 0, 0, 1, 0],
 [1, 0, 1, 0, 1]]
Output: 1
Explanation: There is only one corner rectangle, with corners grid[1][2], grid[1][4], grid[3][2], grid[3][4].



Example 2:
Input: grid = 
[[1, 1, 1],
 [1, 1, 1],
 [1, 1, 1]]
Output: 9
Explanation: There are four 2x2 rectangles, four 2x3 and 3x2 rectangles, and one 3x3 rectangle.



Example 3:
Input: grid = 
[[1, 1, 1, 1]]
Output: 0
Explanation: Rectangles must have four distinct corners.



Note:
- The number of rows and columns of `grid` will each be in the range `[1, 200]`.
- Each `grid[i][j]` will be either `0` or `1`.
- The number of `1`s in the grid will be at most `6000`.



这道题给了我们一个由0和1组成的二维数组，这里定义了一种边角矩形，其四个顶点均为1，让我们求这个二维数组中有多少个不同的边角矩形。那么最简单直接的方法就是暴力破解啦，我们遍历所有的子矩形，并且检验其四个顶点是否为1即可。先确定左上顶点，每个顶点都可以当作左上顶点，所以需要两个for循环，然后我们直接跳过非1的左上顶点，接下来就是要确定右上顶点和左下顶点了，先用一个for循环确定左下顶点的位置，同理，如果左下顶点为0，直接跳过。再用一个for循环确定右上顶点的位置，如果右上顶点位置也确定了，那么此时四个顶点中确定了三个，右下顶点的位置也就确定了，此时如果右上和右下顶点均为1，则结果res自增1，参见代码如下：



解法一：

```
class Solution {
public:
    int countCornerRectangles(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size(), res = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == 0) continue;
                for (int h = 1; h < m - i; ++h) {
                    if (grid[i + h][j] == 0) continue;
                    for (int w = 1; w < n - j; ++w) {
                        if (grid[i][j + w] == 1 && grid[i + h][j + w] == 1) ++res;
                    }
                }
            }
        }
        return res;
    }
};
```



我们来看一种优化了时间复杂度的方法，这种方法的原理是两行同时遍历，如果两行中相同列位置的值都为1，则计数器cnt自增1，那么最后就相当于有了(cnt - 1)个相邻的格子，问题就转化为了求cnt-1个相邻的格子能组成多少个矩形，就变成了初中数学问题了，共有cnt*(cnt-1)/2个，参见代码如下：



解法二：

```
class Solution {
public:
    int countCornerRectangles(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size(), res = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = i + 1; j < m; ++j) {
                int cnt = 0;
                for (int k = 0; k < n; ++k) {
                    if (grid[i][k] == 1 && grid[j][k] == 1) ++cnt;
                }
                res += cnt * (cnt - 1) / 2;
            }
        }
        return res;
    }
};
```



下面这种解法由热心网友edyyy提供，最大亮点是将解法二的beat 65%提高到了beat 97%，速度杠杠的，要飞起来了的节奏。在遍历前一行的时候，将所有为1的位置存入到了一个数组ones中，然后在遍历其他行时，直接检测ones数组中的那些位置是否为1，这样省去了检查一些之前行为0的步骤，提高了运行速度，但是也牺牲了一些空间，比如需要ones数组，算是个trade off吧，参见代码如下：



解法三：

```
class Solution {
public:
    int countCornerRectangles(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size(), res = 0;
        for (int i = 0; i < m - 1; i++) { 
            vector<int> ones;
            for (int k = 0; k < n; k++) if (grid[i][k]) ones.push_back(k);
            for (int j = i + 1; j < m; j++) {
                int cnt = 0;
                for (int l = 0; l < ones.size(); l++) {
                    if (grid[j][ones[l]]) cnt++;
                }
                res += cnt * (cnt - 1) / 2;
            }           
        }
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/114177/short-java-ac-solution-o-m-2-n-with-explanation](https://discuss.leetcode.com/topic/114177/short-java-ac-solution-o-m-2-n-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












