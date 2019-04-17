# [LeetCode] Construct Quad Tree 建立四叉树 - Grandyang - 博客园







# [[LeetCode] Construct Quad Tree 建立四叉树](https://www.cnblogs.com/grandyang/p/9649348.html)







We want to use quad trees to store an `N x N` boolean grid. Each cell in the grid can only be true or false. The root node represents the whole grid. For each node, it will be subdivided into four children nodes until the values in the region it represents are all the same.

Each node has another two boolean attributes : `isLeaf` and `val`. `isLeaf` is true if and only if the node is a leaf node. The `val` attribute for a leaf node contains the value of the region it represents.

Your task is to use a quad tree to represent a given grid. The following example may help you understand the problem better:

Given the `8 x 8` grid below, we want to construct the corresponding quad tree:

![](https://s3-lc-upload.s3.amazonaws.com/uploads/2018/02/01/962_grid.png)

It can be divided according to the definition above:

![](https://s3-lc-upload.s3.amazonaws.com/uploads/2018/02/01/962_grid_divided.png)



The corresponding quad tree should be as following, where each node is represented as a `(isLeaf, val)`pair.

For the non-leaf nodes, `val` can be arbitrary, so it is represented as `*`.

![](https://s3-lc-upload.s3.amazonaws.com/uploads/2018/02/01/962_quad_tree.png)

Note:
- `N` is less than `1000` and guaranteened to be a power of 2.
- If you want to know more about the quad tree, you can refer to its [wiki](https://en.wikipedia.org/wiki/Quadtree).



这道题让我们根据一个二维数组来建立一棵四叉树，关于四叉树的介绍题目中也了给了[wiki百科的链接](https://zh.wikipedia.org/wiki/%E5%9B%9B%E5%8F%89%E6%A0%91)。但是博主开始看到题目中给的那个例子，没怎么看懂。后来分析大神们的代码，才略微弄明白了一些。原来叶结点表示的是值相同的一片区域，比如我们看二维数组图示那行的第三个图，首先整个数组被分成了四等份，左上，左下，和右下部分内的值均相同，那么他们都是一个叶结点，而右上只有再四等分一下，才能使各自部分内的值相同，所以其就不是叶结点，而四等分后的每个区间才是叶结点。题目中限定了N的值一定是2的指数，就是说其如果可分的话，一定可以四等分，而之前说了，只有区间内的值不同时，才需要四等分，否则整体就当作一个叶结点。所以我们需要check四等分区间内的值是否相同，当然，我们可以将二维数组拆分为四个二维数组，但是那样可能不太高效，而且还占用额外空间，一个比较好的选择是用坐标变量来控制等分数组的范围，我们只需要一个起始点坐标，和区间的长度，就可以精确定位一个区间了。比如说对于例子中的整个二维数组数组来说，知道起始点坐标 (0, 0)，还有长度8，就知道表示的是哪个区间。我们可以遍历这个区间上的其他所有的点，跟起点对比，只要有任何点跟起点不相同，则说明该区间是可分的，因为我们前面说了，只有一个区间上所有的值均相同，才能当作一个叶结点。只要有不同，就表示可以四分，那么我们就新建一个结点，这里的左上，左下，右上，和右下四个子结点就需要用过调用递归函数来实现了，实现原理都一样，重要的地方就是确定每个四分区间的起点和长度，长度好确定，都是当前长度的一半，然后就是把各个区间的起点确定好，这个也不难，就是细心一点，不要写错了就可以了，另外，对于非叶结点，结点值可以是true或者false都没问题。如果某个区间上所有值均相同，那么就生成一个叶结点，结点值就跟区间值相同，isLeaf是true，四个子结点均为NULL即可，参见代码如下：



解法一：

```
class Solution {
public:
    Node* construct(vector<vector<int>>& grid) {
        return build(grid, 0, 0, grid.size());
    }
    Node* build(vector<vector<int>>& grid, int x, int y, int len) {
        if (len <= 0) return NULL;
        for (int i = x; i < x + len; ++i) {
            for (int j = y; j < y + len; ++j) {
                if (grid[i][j] != grid[x][y]) {
                    return new Node(true, false,
                           build(grid, x, y, len / 2),
                           build(grid, x, y + len / 2, len / 2),
                           build(grid, x + len/ 2, y, len / 2),
                           build(grid, x + len / 2, y + len / 2, len / 2));
                }
            }
        }
        return new Node(grid[x][y] == 1, true, NULL, NULL, NULL, NULL);
    }
};
```



还有一种写法，记录了区间的左上点和右下点，知道这两个点也可以确定一个区间的位置，整体思路和上面的方法并没有什么太大的区别，参见代码如下：



解法二：

```
class Solution {
public:
    Node* construct(vector<vector<int>>& grid) {
        return build(grid, 0, 0, grid.size() - 1, grid.size() - 1);
    }
    Node* build(vector<vector<int>>& grid, int r1, int c1, int r2, int c2) {
        if (r1 > r2 || c1 > c2) return NULL;
        bool isLeaf = true;
        int val = grid[r1][c1], rowMid = (r1 + r2) / 2, colMid = (c1 + c2) / 2;
        for (int i = r1; i <= r2; ++i) {
            for (int j = c1; j <= c2; ++j) {
                if (grid[i][j] != val) {
                    isLeaf = false;
                    break;
                }
            }
        }
        if (isLeaf) return new Node(val == 1, true, NULL, NULL, NULL, NULL);
        return new Node(false, false, 
               build(grid, r1, c1, rowMid, colMid),
               build(grid, r1, colMid + 1, rowMid, c2),
               build(grid, rowMid + 1, c1, r2, colMid),
               build(grid, rowMid + 1, colMid + 1, r2, c2));
    }
};
```



参考资料：

[https://leetcode.com/problems/construct-quad-tree/](https://leetcode.com/problems/construct-quad-tree/)

[https://leetcode.com/problems/construct-quad-tree/discuss/151j684/Recursive-Java-Solution](https://leetcode.com/problems/construct-quad-tree/discuss/151j684/Recursive-Java-Solution)

[https://leetcode.com/problems/construct-quad-tree/discuss/154420/My-Java-Recursive-Solution](https://leetcode.com/problems/construct-quad-tree/discuss/154420/My-Java-Recursive-Solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












