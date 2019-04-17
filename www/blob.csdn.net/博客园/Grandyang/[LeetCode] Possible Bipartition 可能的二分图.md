# [LeetCode] Possible Bipartition 可能的二分图 - Grandyang - 博客园







# [[LeetCode] Possible Bipartition 可能的二分图](https://www.cnblogs.com/grandyang/p/10317141.html)







Given a set of `N` people (numbered `1, 2, ..., N`), we would like to split everyone into two groups of any size.

Each person may dislike some other people, and they should not go into the same group. 

Formally, if `dislikes[i] = [a, b]`, it means it is not allowed to put the people numbered `a` and `b` into the same group.

Return `true` if and only if it is possible to split everyone into two groups in this way.




Example 1:
Input: N = 4, dislikes = [[1,2],[1,3],[2,4]]
Output: true
Explanation: group1 [1,4], group2 [2,3]


Example 2:
Input: N = 3, dislikes = [[1,2],[1,3],[2,3]]
Output: false


Example 3:
Input: N = 5, dislikes = [[1,2],[2,3],[3,4],[4,5],[1,5]]
Output: false



Note:
- `1 <= N <= 2000`
- `0 <= dislikes.length <= 10000`
- `1 <= dislikes[i][j] <= N`
- `dislikes[i][0] < dislikes[i][1]`
- There does not exist `i != j` for which `dislikes[i] == dislikes[j]`.






这道题又是关于二分图的题，第一次接触的时候是 [Is Graph Bipartite?](http://www.cnblogs.com/grandyang/p/8519566.html)，那道题给的是建好的邻接链表（虽然是用数组实现的），但是本质上和这道题是一样的，同一条边上的两点是不能在同一个集合中的，那么这就相当于本题中的dislike的关系，，也可以把每个dislike看作是一条边，那么两端的两个人不能在同一个集合中。看透了题目的本质后，就不难做了，跟之前的题相比，这里唯一不同的就是邻接链表没有给我们建好，需要自己去建。不管是建邻接链表，还是邻接矩阵都行，反正是要先把图建起来才能遍历。那么这里我们先建立一个邻接矩阵好了，建一个大小为 (N+1) x (N+1) 的二维数组g，其中若 g[i][j] 为1，说明i和j互相不鸟。那么我们先根据dislikes的情况，把二维数组先赋上值，注意这里 g[i][j] 和 g[j][i] 都要更新，因为是互相不鸟，而并不是某一方热脸贴冷屁股。下面就要开始遍历了，还是使用染色法，使用一个一维的colors数组，大小为N+1，初始化是0，由于只有两组，我们可以用1和-1来区分。那么开始遍历图中的结点，对于每个遍历到的结点，如果其还未被染色，还是一张白纸的时候，我们调用递归函数对其用颜色1进行尝试染色。在递归函数中，现将该结点染色，然后就要遍历所有跟其合不来的人，这里就发现邻接矩阵的好处了吧，不然每次还得遍历dislikes数组。由于这里是邻接矩阵，所以我们只有在其值为1的时候才处理，当找到一个跟其合不来的人，首先检测其染色情况，如果此时两个人颜色相同了，说明已经在一个组里了，这就矛盾了，直接返回false。如果那个人还是白纸一张，我们尝试用相反的颜色去染他，如果无法成功染色，则返回false。循环顺序退出后，返回true，参见代码如下：



解法一：

```
class Solution {
public:
    bool possibleBipartition(int N, vector<vector<int>>& dislikes) {
        vector<vector<int>> g(N + 1, vector<int>(N + 1));
        for (auto dislike : dislikes) {
            g[dislike[0]][dislike[1]] = 1;
            g[dislike[1]][dislike[0]] = 1;
        }
        vector<int> colors(N + 1);
        for (int i = 1; i <= N; ++i) {
            if (colors[i] == 0 && !helper(g, i, 1, colors)) return false;
        }
        return true;
    }
    bool helper(vector<vector<int>>& g, int cur, int color, vector<int>& colors) {
        colors[cur] = color;
        for (int i = 0; i < g.size(); ++i) {
            if (g[cur][i] == 1) {
                if (colors[i] == color) return false;
                if (colors[i] == 0 && !helper(g, i, -color, colors)) return false;
            }
        }
        return true;
    }
};
```



我们还可以用迭代的写法，不实用递归函数，但是整个思路还是完全一样的。这里我们建立邻接链表，比邻接矩阵能省一些空间，只把跟其相邻的结点存入对应的数组内。还是要建立一个一维colors数组，并开始遍历结点，若某个结点已经染过色了，跳过，否则就先给其染为1。然后我们借助queue来进行BFS遍历，现将当前结点排入队列，然后开始循环队列，取出队首结点，然后遍历其所有相邻结点，如果两个颜色相同，直接返回false，否则若其为白纸，则赋相反颜色，并且排入队列。最终若顺序完成遍历，返回true，参见代码如下：



解法二：

```
class Solution {
public:
    bool possibleBipartition(int N, vector<vector<int>>& dislikes) {
        vector<vector<int>> g(N + 1);
        for (auto dislike : dislikes) {
            g[dislike[0]].push_back(dislike[1]);
            g[dislike[1]].push_back(dislike[0]);
        }
        vector<int> colors(N + 1);
        for (int i = 1; i <= N; ++i) {
            if (colors[i] != 0) continue;
            colors[i] = 1;
            queue<int> q{{i}};
            while (!q.empty()) {
                int t = q.front(); q.pop();
                for (int cur : g[t]) {
                    if (colors[cur] == colors[t]) return false;
                    if (colors[cur] == 0) {
                        colors[cur] = -colors[t];
                        q.push(cur);
                    }
                }
            }
        }
        return true;
    }
};
```



其实这道题还可以使用并查集Union Find来做，所谓的并查集，简单来说，就是归类，将同一集合的元素放在一起。那么如何在能验证两个元素是否属于同一个集合呢，这里就要使用一个root数组（有时候是使用HashMap），如果两个元素是同一个组的话，那么最终调用find函数返回的值应该是相同的，可以理解为老祖宗相同就是同一个组，两个点的root值不同，也可能是同一个组，因为find函数的运作机制是一直追根溯源到最原始的值。可以看到，这里博主的find函数写的是递归形式，一行搞定碉堡了，当然也有while循环式的迭代写法。好，回过头来继续说这道题，这里我们还是首先建图，这里建立邻接链表，跟上面的使用二维数组的方法不同，这里使用来HashMap，更加的节省空间。现在我们不需要用colors数组了，而是要使用并查集需要的root数组，给每个点都初始化为不同的值，因为在初始时将每个点都看作一个不同的组。然后我们开始遍历所有结点，若当前结点没有邻接结点，直接跳过。否则就要开始进行处理了，并查集方法的核心就两步，合并跟查询。我们首先进行查询操作，对当前结点和其第一个邻接结点分别调用find函数，如果其返回值相同，则意味着其属于同一个集合了，这是不合题意的，直接返回false。否则我们继续遍历其他的邻接结点，对于每一个新的邻接结点，我们都调用find函数，还是判断若返回值跟原结点的相同，return false。否则就要进行合并操作了，根据敌人的敌人就是朋友的原则，所有的邻接结点之间应该属于同一个组，因为就两个组，我所有不爽的人都不能跟我在一个组，那么他们所有人只能都在另一个组，所以需要将他们都合并起来，合并的时候不管是用 root[parent] = y 还是 root[g[i][j]] = y 都是可以，因为不管直接跟某个结点合并，或者跟其祖宗合并，最终经过find函数追踪溯源都会返回相同的值，参见代码如下：



解法三：

```
class Solution {
public:
    bool possibleBipartition(int N, vector<vector<int>>& dislikes) {
        unordered_map<int, vector<int>> g;
        for (auto dislike : dislikes) {
            g[dislike[0]].push_back(dislike[1]);
            g[dislike[1]].push_back(dislike[0]);
        }
        vector<int> root(N + 1);
        for (int i = 1; i <= N; ++i) root[i] = i;
        for (int i = 1; i <= N; ++i) {
            if (!g.count(i)) continue;
            int x = find(root, i), y = find(root, g[i][0]);
            if (x == y) return false;
            for (int j = 1; j < g[i].size(); ++j) {
                int parent = find(root, g[i][j]);
                if (x == parent) return false;
                root[parent] = y;
            }
        }
        return true;
    }
    int find(vector<int>& root, int i) {
        return root[i] == i ? i : find(root, root[i]);
    }
};
```



讨论：可以看到本文中的三种解法在建立图的时候，使用的数据结构都不同，解法一使用二维数组建立了邻接矩阵，解法二使用二维数组建立了邻接链表，解法三使用了HashMap建立了邻接链表。刻意使用不同的方法就是为了大家可以对比区别一下，这三种方法都比较常用，在不同的题目中选择最适合的方法即可。



类似题目：

[Is Graph Bipartite?](http://www.cnblogs.com/grandyang/p/8519566.html)



参考资料：

[https://leetcode.com/problems/possible-bipartition/](https://leetcode.com/problems/possible-bipartition/)

[https://leetcode.com/problems/possible-bipartition/discuss/159085/java-graph](https://leetcode.com/problems/possible-bipartition/discuss/159085/java-graph)

[https://leetcode.com/problems/possible-bipartition/discuss/195303/Java-Union-Find](https://leetcode.com/problems/possible-bipartition/discuss/195303/Java-Union-Find)

[https://leetcode.com/problems/possible-bipartition/discuss/158957/Java-DFS-solution](https://leetcode.com/problems/possible-bipartition/discuss/158957/Java-DFS-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












