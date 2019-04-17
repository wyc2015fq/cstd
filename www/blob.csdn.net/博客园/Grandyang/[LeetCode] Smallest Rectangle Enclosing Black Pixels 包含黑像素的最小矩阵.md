# [LeetCode] Smallest Rectangle Enclosing Black Pixels 包含黑像素的最小矩阵 - Grandyang - 博客园







# [[LeetCode] Smallest Rectangle Enclosing Black Pixels 包含黑像素的最小矩阵](https://www.cnblogs.com/grandyang/p/5268775.html)







An image is represented by a binary matrix with `0` as a white pixel and `1` as a black pixel. The black pixels are connected, i.e., there is only one black region. Pixels are connected horizontally and vertically. Given the location `(x, y)` of one of the black pixels, return the area of the smallest (axis-aligned) rectangle that encloses all black pixels.

For example, given the following image:
[
  "0010",
  "0110",
  "0100"
]

and `x = 0`, `y = 2`,



Return `6`.



这道题给我们一个二维矩阵，表示一个图片的数据，其中1代表黑像素，0代表白像素，现在让我们找出一个最小的矩阵可以包括所有的黑像素，还给了我们一个黑像素的坐标，我们先来看Brute Force的方法，这种方法的效率不高，遍历了整个数组，如果遇到了1，就更新矩形的返回，参见代码如下：



解法一：

```
// Brute force
class Solution {
public:
    int minArea(vector<vector<char>>& image, int x, int y) {
        int left = y, right = y, up = x, down = x;
        for (int i = 0; i < image.size(); ++i) {
            for (int j = 0; j < image[i].size(); ++j) {
                if (image[i][j] == '1') {
                    left = min(left, j);
                    right = max(right, j);
                    up = min(up, i);
                    down = max(down, i);
                }
            }
        }
        return (right - left + 1) * (down - up + 1);
    }
};
```



下面这种解法是解法一的递归写法，本质上来说跟上面的解法没有啥区别，也没有任何的优化，所以仍然可以认为是暴力搜索法，参见代码如下：



解法二：

```
// DFS
class Solution {
public:
    int minArea(vector<vector<char>>& image, int x, int y) {
        int left = y, right = y, up = x, down = x;
        dfs(image, x, y, left, right, up, down);
        return (right - left + 1) * (down - up + 1);
    }
    void dfs(vector<vector<char>> &image, int x, int y, int &left, int &right, int &up, int &down) {
        if (x < 0 || x >= image.size() || y < 0 || y >= image[0].size() || image[x][y] != '1') return;
        left = min(left, y); 
        right = max(right, y);
        up = min(up, x);
        down = max(down, x);
        image[x][y] = '2';
        dfs(image, x + 1, y, left, right, up, down);
        dfs(image, x - 1, y, left, right, up, down);
        dfs(image, x, y + 1, left, right, up, down);
        dfs(image, x, y - 1, left, right, up, down);
    }
};
```



我们再来看一种优化了时间复杂度的解法，这是一种二分搜索法，以给定的一个黑像素(x, y)为中心，分别用二分法快速找到整个黑色区域的上下左右的临界点，然后直接算出面积。首先我们来看上边界怎么找，既然是以(x, y)为中心，而且上边界又是某个行数，那么其范围肯定在[0, x]之间，能成为上边界的条件是该行中至少有一个点是1，那么其列数的范围就在[0, n]之间，我们在进行二分搜索的时候，先根据i, j算出中间行mid，然后列数从0开始遍历，直到找到为1的点，或者越界位置，然后我们判断列数是否越界，越界的话，说明当前行没有1，此时更新i为mid+1，如果找到了1，那么更新j为mid。找下边界也是同样的道理，但是跟上边界稍微又些不同的地方是，如果当前行找到了1，我们应该再往下找，那么i应该更新为mid+1；如果没找到，就应该往上找，靠近(x, y)点；所以两种情况只是在二分法更新范围的地方正好想法，所以我们可以用一个bool型的变量opt来决定还如何更新行数。

下面我们来看如何确定左边界和右边界，其实跟确定上下边界大同小异。左边界是列数，若以(x, y)点为中心，那么其范围便是[0, y]，因为我们之前已经确定了上下边界up和down了，所以左边界点的行数范围就是[up, down]，那么同理，当我们通过i, j求出了中间列mid时，我们就要遍历该列，找到为1的点，所以此时我们是用image[k][mid]，而在找上下边界时，我们用的是image[mid][k]，还是顺序不一样，我们可以用另外一个bool型变量h来控制，h表示horizontal，就是水平遍历的意思。这样我们通过两个bool型变量就可以用一个函数来涵盖四种情况的二分搜索，是不是很叼？下面更新i或j的时候参考上下边界的分析，应该不难理解，参见代码如下：



解法三：

```
// Binary Search
class Solution {
public:
    int minArea(vector<vector<char>>& image, int x, int y) {
        int m = image.size(), n = image[0].size();
        int up = binary_search(image, true, 0, x, 0, n, true);
        int down = binary_search(image, true, x + 1, m, 0, n, false);
        int left = binary_search(image, false, 0, y, up, down, true);
        int right = binary_search(image, false, y + 1, n, up, down, false);
        return (right - left) * (down - up);
    }
    int binary_search(vector<vector<char>> &image, bool h, int i, int j, int low, int high, bool opt) {
        while (i < j) {
            int k = low, mid = (i + j) / 2;
            while (k < high && (h ? image[mid][k] : image[k][mid]) == '0') ++k;
            if (k < high == opt) j = mid;
            else i = mid + 1;
        }
        return i;
    }
};
```


参考资料：

[https://leetcode.com/discuss/68243/c-dfs-solution](https://leetcode.com/discuss/68243/c-dfs-solution)

[https://leetcode.com/discuss/71898/1ms-java-binary-search-dfs-is-4ms](https://leetcode.com/discuss/71898/1ms-java-binary-search-dfs-is-4ms)

[https://leetcode.com/discuss/68246/c-java-python-binary-search-solution-with-explanation](https://leetcode.com/discuss/68246/c-java-python-binary-search-solution-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












