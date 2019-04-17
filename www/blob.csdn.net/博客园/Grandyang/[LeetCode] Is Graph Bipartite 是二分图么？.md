# [LeetCode] Is Graph Bipartite? 是二分图么？ - Grandyang - 博客园







# [[LeetCode] Is Graph Bipartite? 是二分图么？](https://www.cnblogs.com/grandyang/p/8519566.html)







Given an undirected `graph`, return `true` if and only if it is bipartite.

Recall that a graph is *bipartite* if we can split it's set of nodes into two independent subsets A and B such that every edge in the graph has one node in A and another node in B.

The graph is given in the following form: `graph[i]` is a list of indexes `j` for which the edge between nodes `i` and `j` exists.  Each node is an integer between `0` and `graph.length - 1`.  There are no self edges or parallel edges: `graph[i]` does not contain `i`, and it doesn't contain any element twice.
Example 1:
Input: [[1,3], [0,2], [1,3], [0,2]]
Output: true
Explanation: 
The graph looks like this:
0----1
|    |
|    |
3----2
We can divide the vertices into two groups: {0, 2} and {1, 3}.
Example 2:
Input: [[1,2,3], [0,2], [0,1,3], [0,2]]
Output: false
Explanation: 
The graph looks like this:
0----1
| \  |
|  \ |
3----2
We cannot find a way to divide the set of nodes into two independent subsets.



Note:
- `graph` will have length in range `[1, 100]`.
- `graph[i]` will contain integers in range `[0, graph.length - 1]`.
- `graph[i]` will not contain `i` or duplicate values.
- The graph is undirected: if any element `j` is in `graph[i]`, then `i` will be in `graph[j]`.



这道题博主在最开始做的时候，看了半天，愣是没弄懂输出数据的意思，博主开始以为给的是边，后来发现跟图对应不上，就懵逼了，后来是通过研究论坛上大神们的解法，才总算搞懂了题目的意思，原来输入数组中的graph[i]，表示顶点i所有相邻的顶点，比如对于例子1来说，顶点0和顶点1，3相连，顶点1和顶点0，2相连，顶点2和结点1，3相连，顶点3和顶点0，2相连。这道题让我们验证给定的图是否是二分图，所谓[二分图](https://zh.wikipedia.org/wiki/%E4%BA%8C%E5%88%86%E5%9B%BE)，就是可以将图中的所有顶点分成两个不相交的集合，使得同一个集合的顶点不相连。为了验证是否有这样的两个不相交的集合存在，我们采用一种很机智的染色法，大体上的思路是要将相连的两个顶点染成不同的颜色，一旦在染的过程中发现有两连的两个顶点已经被染成相同的颜色，说明不是二分图。这里我们使用两种颜色，分别用1和-1来表示，初始时每个顶点用0表示未染色，然后遍历每一个顶点，如果该顶点未被访问过，则调用递归函数，如果返回false，那么说明不是二分图，则直接返回false。如果循环退出后没有返回false，则返回true。在递归函数中，如果当前顶点已经染色，如果该顶点的颜色和将要染的颜色相同，则返回true，否则返回false。如果没被染色，则将当前顶点染色，然后再遍历与该顶点相连的所有的顶点，调用递归函数，如果返回false了，则当前递归函数的返回false，循环结束返回true，参见代码如下：



解法一：

```
class Solution {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        vector<int> colors(graph.size());
        for (int i = 0; i < graph.size(); ++i) {
            if (colors[i] == 0 && !valid(graph, 1, i, colors)) {
                return false;
            }
        }
        return true;
    }
    bool valid(vector<vector<int>>& graph, int color, int cur, vector<int>& colors) {
        if (colors[cur] != 0) return colors[cur] == color;
        colors[cur] = color;
        for (int i : graph[cur]) {
            if (!valid(graph, -1 * color, i, colors)) {
                return false;
            }
        }
        return true;
    }
};
```



我们再来看一种迭代的解法，整体思路还是一样的，还是遍历整个顶点，如果未被染色，则先染色为1，然后使用BFS进行遍历，将当前顶点放入队列queue中，然后while循环queue不为空，取出队首元素，遍历其所有相邻的顶点，如果相邻顶点未被染色，则染成和当前顶点相反的颜色，然后把相邻顶点加入queue中，否则如果当前顶点和相邻顶点颜色相同，直接返回false，循环退出后返回true，参见代码如下：



解法二：

```
class Solution {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        vector<int> colors(graph.size());
        for (int i = 0; i < graph.size(); ++i) {
            if (colors[i] != 0) continue;
            colors[i] = 1;
            queue<int> q{{i}};
            while (!q.empty()) {
                int t = q.front(); q.pop();
                for (auto a : graph[t]) {
                    if (colors[a] == colors[t]) return false;
                    if (colors[a] == 0) {
                        colors[a] = -1 * colors[t];
                        q.push(a);
                    }
                }
            }
        }
        return true;
    }
};
```



其实这道题还可以使用并查集 Union Find 来做，所谓的并查集，简单来说，就是归类，将同一集合的元素放在一起。我们开始遍历所有结点，若当前结点没有邻接结点，直接跳过。否则就要开始进行处理了，并查集方法的核心就两步，合并跟查询。我们首先进行查询操作，对当前结点和其第一个邻接结点分别调用find函数，如果其返回值相同，则意味着其属于同一个集合了，这是不合题意的，直接返回false。否则我们继续遍历其他的邻接结点，对于每一个新的邻接结点，我们都调用find函数，还是判断若返回值跟原结点的相同，return false。否则就要进行合并操作了，根据敌人的敌人就是朋友的原则，所有的邻接结点之间应该属于同一个组，因为就两个组，我所有不爽的人都不能跟我在一个组，那么他们所有人只能都在另一个组，所以需要将他们都合并起来，合并的时候不管是用 root[parent] = y 还是 root[g[i][j]] = y 都是可以，因为不管直接跟某个结点合并，或者跟其祖宗合并，最终经过find函数追踪溯源都会返回相同的值，参见代码如下：



解法三：

```
class Solution {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        vector<int> root(graph.size());
        for (int i = 0; i < graph.size(); ++i) root[i] = i;
        for (int i = 0; i < graph.size(); ++i) {
            if (graph[i].empty()) continue;
            int x = find(root, i), y = find(root, graph[i][0]);
            if (x == y) return false;
            for (int j = 1; j < graph[i].size(); ++j) {
                int parent = find(root, graph[i][j]);
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



类似题目：

[Possible Bipartition](https://www.cnblogs.com/grandyang/p/10317141.html)



参考资料：

[https://leetcode.com/problems/is-graph-bipartite/](https://leetcode.com/problems/is-graph-bipartite/)

[https://leetcode.com/problems/is-graph-bipartite/discuss/115487/Java-Clean-DFS-solution-with-Explanation](https://leetcode.com/problems/is-graph-bipartite/discuss/115487/Java-Clean-DFS-solution-with-Explanation)

[https://leetcode.com/problems/is-graph-bipartite/discuss/115723/C++-short-iterative-solution-with-comments](https://leetcode.com/problems/is-graph-bipartite/discuss/115723/C++-short-iterative-solution-with-comments)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












