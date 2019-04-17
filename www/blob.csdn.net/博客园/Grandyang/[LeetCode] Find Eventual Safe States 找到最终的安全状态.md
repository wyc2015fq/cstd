# [LeetCode] Find Eventual Safe States 找到最终的安全状态 - Grandyang - 博客园







# [[LeetCode] Find Eventual Safe States 找到最终的安全状态](https://www.cnblogs.com/grandyang/p/9319966.html)







In a directed graph, we start at some node and every turn, walk along a directed edge of the graph.  If we reach a node that is terminal (that is, it has no outgoing directed edges), we stop.

Now, say our starting node is *eventually safe *if and only if we must eventually walk to a terminal node.  More specifically, there exists a natural number `K` so that for any choice of where to walk, we must have stopped at a terminal node in less than `K` steps.

Which nodes are eventually safe?  Return them as an array in sorted order.

The directed graph has `N` nodes with labels `0, 1, ..., N-1`, where `N` is the length of `graph`.  The graph is given in the following form: `graph[i]` is a list of labels `j` such that `(i, j)` is a directed edge of the graph.
Example:
Input: graph = [[1,2],[2,3],[5],[0],[5],[],[]]
Output: [2,4,5,6]
Here is a diagram of the above graph.


![Illustration of graph](https://s3-lc-upload.s3.amazonaws.com/uploads/2018/03/17/picture1.png)

Note:
- `graph` will have length at most `10000`.
- The number of edges in the graph will not exceed `32000`.
- Each `graph[i]` will be a sorted list of different integers, chosen within the range `[0, graph.length - 1]`.



这道题给了我们一个有向图，然后定义了一种最终安全状态的结点，就是说该结点要在自然数K步内停止，所谓停止的意思，就是再没有向外的边，即没有出度，像上面例子中的结点5和6就是出度为0，因为graph[5]和graph[6]均为空。那么我们分析题目中的例子，除了没有出度的结点5和6之外，结点2和4也是安全状态结点，为啥呢，我们发现结点2和4都只能到达结点5，而结点5本身就是安全状态点，所以2和4也就是安全状态点了，所以我们可以得出的结论是，若某结点唯一能到达的结点是安全状态结点的话，那么该结点也同样是安全状态结点。那么我们就可以从没有出度的安全状态往回推，比如结点5，往回推可以到达结点4和2，先看结点4，此时我们先回推到结点4，然后将这条边断开，那么此时结点4出度为0，则标记结点4也为安全状态结点，同理，回推到结点2，断开边，此时结点2虽然入度仍为2，但是出度为0了，标记结点2也为安全状态结点。

分析到这里，思路应该比较明朗了，由于我们需要回推边，所以需要建立逆向边，用一个集合数组来存，由于题目要求返回的结点有序，我们可以利用集合TreeSet的自动排序的特性，由于需要断开边，为了不修改输入数据，所以我们干脆再建一个顺向边得了，即跟输入数据相同。还需要一个safe数组，布尔型的，来标记哪些结点是安全状态结点。在遍历结点的时候，直接先将出度为0的安全状态结点找出来，排入一个队列queue中，方便后续的处理。后续的处理就有些类似BFS的操作了，我们循环非空queue，取出队首元素，标记safe中该结点为安全状态结点，然后遍历其逆向边的结点，即可以到达当前队首结点的所有结点，我们在正向边集合中删除对应的边，如果此时结点出度为0了，将其加入队列queue中等待下一步处理，这样while循环退出后，所有的安全状态结点都已经标记好了，我们直接遍历safe数组，将其存入结果res中即可，参见代码如下：



解法一：

```
class Solution {
public:
    vector<int> eventualSafeNodes(vector<vector<int>>& graph) {
        vector<int> res;
        int n = graph.size();
        vector<bool> safe(n, false);
        vector<set<int>> g(n, set<int>()), revg = g;
        queue<int> q;
        for (int i = 0; i < n; ++i) {
            if (graph[i].empty()) q.push(i);
            for (int j : graph[i]) {
                g[i].insert(j);
                revg[j].insert(i);
            }
        }
        while (!q.empty()) {
            auto t = q.front(); q.pop();
            safe[t] = true;
            for (int i : revg[t]) {
                g[i].erase(t);
                if (g[i].empty()) q.push(i);
            }
        }
        for (int i = 0; i < n; ++i) {
            if (safe[i]) res.push_back(i);
        }
        return res;
    }
};
```



我们再来看一种DFS遍历有向图的解法。仔细分析题目中的例子，不难发现，之所以某些结点不是安全状态，因为有环的存在，而环经过的所有结点，一定不是安全状态结点，所以我们可以通过DFS遍历有向图来找出环即可。在大多数的算法中，经典的DFS遍历法对于结点都有三种状态标记，white，gray，和black，其中white表示结点还未遍历，gray表示正在遍历邻结点，black表示已经结束该结点的遍历。那么我们可以对每个结点都调用递归函数，在递归函数中，如果当前结点不是white，表示该结点已经访问过了，那么如果当前结点是black，直接返回true，如果是gray，直接返回false，因为遇到gray的结点，表示一定有环存在。否则我们给结点标记gray，然后开始遍历所有邻接结点，如果某个邻结点是black，直接跳过该结点。如果某个邻结点是gray，或者对该邻结点调用递归返回false了，说明当前结点是环结点，返回false。如果循环结束了，当前结点标记为black，并且返回true，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> eventualSafeNodes(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<int> res, color(n); // 0 white, 1 gray, 2 black
        for (int i = 0; i < n; ++i) {
            if (helper(graph, i, color)) res.push_back(i);
        }
        return res;
    }
    bool helper(vector<vector<int>>& graph, int cur, vector<int>& color) {
        if (color[cur] > 0) return color[cur] == 2;
        color[cur] = 1;
        for (int i : graph[cur]) {
            if (color[i] == 2) continue;
            if (color[i] == 1 || !helper(graph, i, color)) {
                return false;
            }
        }
        color[cur] = 2;
        return true;
    }
};
```



参考资料：

[https://leetcode.com/problems/find-eventual-safe-states/solution/](https://leetcode.com/problems/find-eventual-safe-states/solution/)

[https://leetcode.com/problems/find-eventual-safe-states/discuss/119871/Straightforward-Java-solution-easy-to-understand!](https://leetcode.com/problems/find-eventual-safe-states/discuss/119871/Straightforward-Java-solution-easy-to-understand!)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












