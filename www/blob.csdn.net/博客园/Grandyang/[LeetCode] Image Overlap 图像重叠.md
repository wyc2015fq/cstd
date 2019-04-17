# [LeetCode] Image Overlap 图像重叠 - Grandyang - 博客园







# [[LeetCode] Image Overlap 图像重叠](https://www.cnblogs.com/grandyang/p/10355589.html)









Two images `A` and `B` are given, represented as binary, square matrices of the same size.  (A binary matrix has only 0s and 1s as values.)

We translate one image however we choose (sliding it left, right, up, or down any number of units), and place it on top of the other image.  After, the *overlap* of this translation is the number of positions that have a 1 in both images.

(Note also that a translation does not include any kind of rotation.)

What is the largest possible overlap?

Example 1:
Input: A = [[1,1,0],
            [0,1,0],
            [0,1,0]]
       B = [[0,0,0],
            [0,1,1],
            [0,0,1]]
Output: 3
Explanation: We slide A to right by 1 unit and down by 1 unit.
Notes: 
- `1 <= A.length = A[0].length = B.length = B[0].length <= 30`
- `0 <= A[i][j], B[i][j] <= 1`





这道题给了我们两个用大小相同的二维数组表示的图像，里面只有0或1，问我们经过任意平移后，能产生的最大重叠是多少，这里只计算值为1的重叠。给的例子中，我们只要将图像A向右和向下平移一位，就能得到3个重叠。那么首先来思考brute force的方法，对于一个nxn大小的数组，其实其能平移的情况是有限的，水平和竖直方向分别有n种移动方式，那么总共有nxn种移动方法，那么我们只要对于每种移动方式后，都计算一下重叠的个数，那么就一定可以找出最大值来。需要注意的是，A和B分别都需要移动nxn次，我们可以使用一个子函数来专门统计重叠个数，需要传入横向纵向的平移量rowOffset和colOffset，那么只需让其中一个数组减去偏移量后跟另一个数组对应位置的值相乘，由于只有0和1，若相乘为1的话，就说明有重叠，直接累加即可，参见代码如下：



解法一：

```
class Solution {
public:
    int largestOverlap(vector<vector<int>>& A, vector<vector<int>>& B) {
        int res = 0, n = A.size();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                res = max(res, max(count(A, B, i, j), count(B, A, i, j)));
            }
        }
        return res;
    }
    int count(vector<vector<int>>& A, vector<vector<int>>& B, int rowOffset, int colOffset) {
        int sum = 0, n = A.size();
        for (int i = rowOffset; i < n; ++i) {
            for (int j = colOffset; j < n; ++j) {
                sum += A[i][j] * B[i - rowOffset][j - colOffset];
            }
        }
        return sum;
    }
};
```



我们还可以换一种思路，由于只有值为1的地方才有可能重叠，所以我们只关心A和B中值为1的地方，将其坐标位置分别存入两个数组listA和listB中。由于对于A和B中的任意两个1的位置，肯定有一种方法能将A平移到B，平移的方法就是横向平移其横坐标之差，竖向平移其纵坐标之差。由于其是一一对应关系，所以只要是横纵坐标差相同的两对儿位置，一定是在同一次平移上。那么我们就需要一个HashMap来建立坐标差值和其出现次数之间的映射，为了降维，将横纵坐标之差转为字符串，然后中加上个横杠分隔开，这样只要组成了相同的字符串，那么一定就是在同一个平移上，计数器自增1。最后在HashMap中找到最大的值即可，参见代码如下：



解法二：

```
class Solution {
public:
    int largestOverlap(vector<vector<int>>& A, vector<vector<int>>& B) {
        int res = 0, n = A.size();
        vector<vector<int>> listA, listB;
        unordered_map<string, int> diffCnt;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (A[i][j] == 1) listA.push_back({i, j});
                if (B[i][j] == 1) listB.push_back({i, j});
            }
        }
        for (auto a : listA) {
            for (auto b : listB) {
                ++diffCnt[to_string(a[0] - b[0]) + "-" + to_string(a[1] - b[1])];
            }
        }
        for (auto diff : diffCnt) {
            res = max(res, diff.second);
        }
        return res;
    }
};
```



我们可以优化一下空间，可以将二维坐标加码成一个数字，一般的做法都是将 (i, j) 变成 i*n + j，但是这道题却不行，因为我们算横纵坐标的差值时想直接相减，这种加码方式会使得横纵坐标之间互相干扰。由于题目中给了n的范围，不会超过30，所以我们可以给横坐标乘以100，再加上纵坐标，即 i*100 + j，这种加码方式万无一失。然后还是要用HashMap来建立坐标差值和其出现次数之间的映射，不过这次就简单多了，不用转字符串了，直接用数字相减即可，最后返回HashMap中最大的统计数，参见代码如下：



解法三：

```
class Solution {
public:
    int largestOverlap(vector<vector<int>>& A, vector<vector<int>>& B) {
        int res = 0, n = A.size();
        vector<int> listA, listB;
        unordered_map<int, int> diffCnt;
        for (int i = 0; i < n * n; ++i) {
            if (A[i / n][i % n] == 1) listA.push_back(i / n * 100 + i % n);
            if (B[i / n][i % n] == 1) listB.push_back(i / n * 100 + i % n);
        }
        for (int a : listA) {
            for (int b : listB) {
                ++diffCnt[a - b];
            }
        }
        for (auto diff : diffCnt) {
            res = max(res, diff.second);
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/image-overlap/](https://leetcode.com/problems/image-overlap/)

[https://leetcode.com/problems/image-overlap/discuss/177485/Java-Easy-Logic](https://leetcode.com/problems/image-overlap/discuss/177485/Java-Easy-Logic)

[https://leetcode.com/problems/image-overlap/discuss/130623/C%2B%2BJavaPython-Straight-Forward](https://leetcode.com/problems/image-overlap/discuss/130623/C%2B%2BJavaPython-Straight-Forward)

[https://leetcode.com/problems/image-overlap/discuss/138976/A-generic-and-easy-to-understand-method](https://leetcode.com/problems/image-overlap/discuss/138976/A-generic-and-easy-to-understand-method)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













