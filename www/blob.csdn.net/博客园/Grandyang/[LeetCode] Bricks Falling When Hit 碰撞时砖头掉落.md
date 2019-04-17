# [LeetCode] Bricks Falling When Hit 碰撞时砖头掉落 - Grandyang - 博客园







# [[LeetCode] Bricks Falling When Hit 碰撞时砖头掉落](https://www.cnblogs.com/grandyang/p/9362777.html)







We have a grid of 1s and 0s; the 1s in a cell represent bricks.  A brick will not drop if and only if it is directly connected to the top of the grid, or at least one of its (4-way) adjacent bricks will not drop.

We will do some erasures sequentially. Each time we want to do the erasure at the location (i, j), the brick (if it exists) on that location will disappear, and then some other bricks may drop because of that erasure.

Return an array representing the number of bricks that will drop after each erasure in sequence.
Example 1:
Input: 
grid = [[1,0,0,0],[1,1,1,0]]
hits = [[1,0]]
Output: [2]
Explanation: 
If we erase the brick at (1, 0), the brick at (1, 1) and (1, 2) will drop. So we should return 2.Example 2:
Input: 
grid = [[1,0,0,0],[1,1,0,0]]
hits = [[1,1],[1,0]]
Output: [0,0]
Explanation: 
When we erase the brick at (1, 0), the brick at (1, 1) has already disappeared due to the last move. So each erasure will cause no bricks dropping.  Note that the erased brick (1, 0) will not be counted as a dropped brick.


Note:
- The number of rows and columns in the grid will be in the range [1, 200].
- The number of erasures will not exceed the area of the grid.
- It is guaranteed that each erasure will be different from any other erasure, and located inside the grid.
- An erasure may refer to a location with no brick - if it does, no bricks drop.



这道题给了我们一个由0和1组成的grid，说是1代表砖头，当砖头连着顶端的时候，就不会掉落，当某个砖头连着不会掉落的砖头时，其本身也不会掉落。然后我们要去掉一些砖头，当去掉某个砖头时，与其相连的砖头可能也会同时掉落。所以这里让我们求同时掉落的砖头个数。博主书读的不少，不会受骗，这尼玛不就是泡泡龙游戏么。其中泡泡龙的一大技巧就是挂葡萄，当关键节点处的泡泡被打掉后，这整个一串都可以直接掉下来。这里也是一样啊，grid的顶端就是游戏界面的顶端，然后砖头就是泡泡，去掉砖头就是消掉某个地方的泡泡，然后掉落的砖头就是掉下的泡泡啦。游戏玩的6，不代表题会做，其实这道题还是蛮有难度的，花了博主很长的时间。

首先我们来想，我们肯定要统计出当前没有掉落的砖头数量，当去掉某个砖头后，我们可以统计当前还连着的砖头数量，二者做差值就是掉落的砖头数量。那么如何来统计不会掉落的砖头数量呢，由于顶层的砖头时不会掉落的，那么跟顶层相连的所有砖头肯定也不会掉落，我们就可以使用DFS来遍历，我们可以把不会掉落的砖头位置存入一个HashSet中，这样通过比较不同状态下HashSet中元素的个数，我们就知道掉落了多少砖头。然后我们再来想一个问题，在没有去除任何砖头的时候，我们DFS查找会遍历所有的砖头，当某个砖头去除后，可能没有连带其他的砖头，那么如果我们再来遍历一遍所有相连的砖头，相当于又把整个数组搜索了一遍，这样并不是很高效。我们可以试着换一个思路，如果我们先把要去掉的所有砖头都先去掉，这样我们遍历所有相连的砖头就是最终还剩下的砖头，然后我们从最后一个砖头开始往回加，每加一个砖头，我们就以这个砖头为起点，DFS遍历其周围相连的砖头，加入HashSet中，那么只会遍历那些会掉的砖头，那么增加的这些砖头就是会掉的砖头数量了，然后再不停的在增加前面的砖头，直到把hits中所有的砖头都添加回来了，那么我们也就计算出了每次会掉的砖头的个数。

好，我们使用一个HashSet来保存不会掉落的砖头，然后先遍历hits数组，把要掉落的砖头位置的值都减去一个1，这里有个需要注意的地方，hits里的掉落位置实际上在grid中不一定有砖头，就是说可能是本身为0的位置，那么我们减1后，数组中也可能会有-1，没有太大的影响，不过需要注意一下，这里不能使用 if (grid[i][j]) 来直接判断其是否为1，因为非0值-1也会返回true。然后我们对第一行的砖头都调用递归函数，因为顶端的砖头不会掉落，跟顶端的砖头相连的砖头也不会掉落，所以要遍历所有相连的砖头，将位置都存入noDrop。然后就是从最后一个位置往前加砖头，先记录noDrop当前的元素个数，然后grid中对应的值自增1，之后增加后的值为1了，才说明这块之前是有砖头的，然后我们看其上下左右位置，若有砖头，则对当前位置调用递归，还有一种情况是当前是顶层的话，还是要调用递归。递归调用完成后二者的差值再减去1就是掉落的砖头数，减1的原因是去掉的砖头不算掉落的砖头数中，参见代码如下：



解法一：

```
class Solution {
public:
    vector<int> hitBricks(vector<vector<int>>& grid, vector<vector<int>>& hits) {
        int m = grid.size(), n = grid[0].size(), k = hits.size();
        vector<int> res(k);
        unordered_set<int> noDrop;
        for (int i = 0; i < k; ++i) grid[hits[i][0]][hits[i][1]] -= 1;
        for (int i = 0; i < n; ++i) {
            if (grid[0][i] == 1) check(grid, 0, i, noDrop);
        }
        for (int i = k - 1; i >= 0; --i) {
            int oldSize = noDrop.size(), x = hits[i][0], y = hits[i][1];
            if (++grid[x][y] != 1) continue;
            if ((x - 1 >= 0 && noDrop.count((x - 1) * n + y)) 
                || (x + 1 < m && noDrop.count((x + 1) * n + y))
                || (y - 1 >= 0 && noDrop.count(x * n + y - 1))
                || (y + 1 < n && noDrop.count(x * n + y + 1))
                || x == 0) {
                check(grid, x, y, noDrop);
                res[i] = noDrop.size() - oldSize - 1;
            }
        }
        return res;
    }
    void check(vector<vector<int>>& grid, int i, int j, unordered_set<int>& noDrop) {
        int m = grid.size(), n = grid[0].size();
        if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] != 1 || noDrop.count(i * n + j)) return;
        noDrop.insert(i * n + j);
        check(grid, i - 1, j, noDrop);
        check(grid, i + 1, j, noDrop);
        check(grid, i, j - 1, noDrop);
        check(grid, i, j + 1, noDrop);
    }
};
```



我们再来看一种使用并查集Union Find的方法来做的，接触过并查集题目的童鞋应该有印象，是专门处理群组问题的一种算法。最典型的就是岛屿问题(例如[Number of Islands II](http://www.cnblogs.com/grandyang/p/5190419.html))，很适合使用Union Find来做，LeetCode中有很多道可以使用这个方法来做的题，比如[Friend Circles](http://www.cnblogs.com/grandyang/p/6686983.html)，[Graph Valid Tree](http://www.cnblogs.com/grandyang/p/5257919.html)，[Number of Connected Components in an Undirected Graph](http://www.cnblogs.com/grandyang/p/5166356.html)，和[Redundant Connection](http://www.cnblogs.com/grandyang/p/7628977.html)等等。都是要用一个root数组，每个点开始初始化为不同的值，如果两个点属于相同的组，就将其中一个点的root值赋值为另一个点的位置，这样只要是相同组里的两点，通过find函数得到相同的值。当然初始化的时候也不用都赋为不同的值，如果表示的是坐标的话，我们也可以都初始化为-1，在find函数稍稍改动一下，也是可以的，这里就把判断 root[x] == x 改为 root[x] == -1 即可。这道题要稍稍复杂一些，我们不光需要并查集数组root，还需要知道每个位置右方和下方跟其相连的砖头个数数组count，还有标记每个位置是否相连且不会坠落的状态数组t，第一行各个位置的t值初始化为1。跟上面的方法类似，我们还是从最后一个砖头开始往回加，那么我们还是要把hits中所有的位置在grid中对应的值减1。然后我们要建立并查集的关系，我们遍历每一个位置，如果是砖头，那么我们对其右边和下边的位置进行检测，如果是砖头，我们就进行经典的并查集的操作，分别对当前位置和右边位置调用find函数，如果两个值不同，说明目前属于两个不同的群组，我们要链接上这两个位置，这里有个小问题需要注意一下，一般来说，我们链接群组的时候，root[x] = y 或 root[y] = x 都是可以的，但是这里我们需要使用第二种，为了跟后面的 count[x] += count[y] 对应起来，因为这里的y是在x的右边，所以count[x]要大于count[y]，这里x和y我们都使用x的群组号，这样能保证后面加到正确的相连的砖头个数。还有我们的t[x] 和 t[y] 也需要更新，因为两个位置要相连，所以只有其中有一方是跟顶端相连的，那么二者的t值都应该为1。初始化完成后，我们就从hits数组末尾开始往回加砖头，跟之前的方法一样，首先要判断之前是有砖头的，然后遍历周围四个新位置，如果位置合法且有砖头的话，再调用并查集的经典操作，对老位置和新位置分别调用find函数，如果不在同一个群组的话，我们需要一个变量cnt来记录可以掉落的砖头个数，如果新位置的t值为0，说明其除了当前位置之外不跟其他位置相连，我们将其count值加入cnt。然后就是链接两个群组，通知更新老位置的count值，新老位置的t值等等。当周围位置遍历完成后，如果当前位置的t值为1，则将cnt值存入结果res的对应位置，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> hitBricks(vector<vector<int>>& grid, vector<vector<int>>& hits) {
        int m = grid.size(), n = grid[0].size(), k = hits.size();
        vector<int> res(k), root(m * n, -1), count(m * n, 1), t(m * n, 0);
        vector<vector<int>> dirs{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        for (int i = 0; i < k; ++i) grid[hits[i][0]][hits[i][1]] -= 1;
        for (int i = 0; i < n; ++i) t[i] = 1;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] != 1) continue;
                if (i + 1 < m && grid[i + 1][j] == 1) {
                    int x = find(root, i * n + j), y = find(root, (i + 1) * n + j);
                    if (x != y) {root[y] = x; count[x] += count[y]; t[x] = t[y] = (t[x] | t[y]);}
                }
                if (j + 1 < n && grid[i][j + 1] == 1) {
                    int x = find(root, i * n + j), y = find(root, i * n + j + 1);
                    if (x != y) {root[y] = x; count[x] += count[y]; t[x] = t[y] = (t[x] | t[y]);}
                }
            }
        }
        for (int i = k - 1; i >= 0; --i) {
            int x = hits[i][0], y = hits[i][1], a = find(root, x * n + y), cnt = 0;
            if (++grid[x][y] != 1) continue;
            for (auto dir : dirs) {
                int newX = x + dir[0], newY = y + dir[1];
                if (newX < 0 || newX >= m || newY < 0 || newY >= n || grid[newX][newY] != 1) continue;
                int b = find(root, newX * n + newY);
                if (a == b) continue;
                if (!t[b]) cnt += count[b];
                root[b] = a; count[a] += count[b]; t[a] = t[b] = (t[a] | t[b]);
            }
            if (t[a]) res[i] = cnt;
        }
        return res;
    }
    int find(vector<int>& root, int x) {
        return (root[x] == -1) ? x : find(root, root[x]);
    }
};
```



参考资料：

[https://leetcode.com/problems/bricks-falling-when-hit/](https://leetcode.com/problems/bricks-falling-when-hit/)

[https://leetcode.com/problems/bricks-falling-when-hit/discuss/173204/Java-DFS-solution-by-adding-bricks-reversely!!!](https://leetcode.com/problems/bricks-falling-when-hit/discuss/173204/Java-DFS-solution-by-adding-bricks-reversely!!!)

[https://leetcode.com/problems/bricks-falling-when-hit/discuss/120259/C%2B%2B-reverse-adding-brick-with-union-O(colume*row%2Bhits)-time-and-space](https://leetcode.com/problems/bricks-falling-when-hit/discuss/120259/C%2B%2B-reverse-adding-brick-with-union-O(colume*row%2Bhits)-time-and-space)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












