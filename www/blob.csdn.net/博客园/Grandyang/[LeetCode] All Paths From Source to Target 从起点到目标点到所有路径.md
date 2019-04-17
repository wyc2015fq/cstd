# [LeetCode] All Paths From Source to Target 从起点到目标点到所有路径 - Grandyang - 博客园







# [[LeetCode] All Paths From Source to Target 从起点到目标点到所有路径](https://www.cnblogs.com/grandyang/p/9262159.html)







Given a directed, acyclic graph of `N` nodes.  Find all possible paths from node `0` to node `N-1`, and return them in any order.

The graph is given as follows:  the nodes are 0, 1, ..., graph.length - 1.  graph[i] is a list of all nodes j for which the edge (i, j) exists.
Example:
Input: [[1,2], [3], [3], []] 
Output: [[0,1,3],[0,2,3]] 
Explanation: The graph looks like this:
0--->1
|    |
v    v
2--->3
There are two paths: 0 -> 1 -> 3 and 0 -> 2 -> 3.

Note:
- The number of nodes in the graph will be in the range `[2, 15]`.
- You can print different paths in any order, but you should keep the order of nodes inside one path.



这道题给了我们一个无回路有向图，包含N个结点，然后让我们找出所有可能的从结点0到结点N-1的路径。这个图的数据是通过一个类似邻接链表的二维数组给的，最开始的时候博主没看懂输入数据的意思，其实很简单，我们来看例子中的input，[[1,2], [3], [3], []]，这是一个二维数组，最外层的数组里面有四个小数组，每个小数组其实就是和当前结点相通的邻结点，由于是有向图，所以只能是当前结点到邻结点，反过来不一定行。那么结点0的邻结点就是结点1和2，结点1的邻结点就是结点3，结点2的邻结点也是3，结点3没有邻结点。那么其实这道题的本质就是遍历邻接链表，由于要列出所有路径情况，那么递归就是不二之选了。我们用cur来表示当前遍历到的结点，初始化为0，然后在递归函数中，先将其加入路径path，如果cur等于N-1了，那么说明到达结点N-1了，将path加入结果res。否则我们再遍历cur的邻接结点，调用递归函数即可，参见代码如下：



解法一：

```
class Solution {
public:
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        vector<vector<int>> res;
        helper(graph, 0, {}, res);
        return res;
    }
    void helper(vector<vector<int>>& graph, int cur, vector<int> path, vector<vector<int>>& res) {
        path.push_back(cur);
        if (cur == graph.size() - 1) res.push_back(path);
        else for (int neigh : graph[cur]) helper(graph, neigh, path, res);
    }
};
```



下面这种解法也是递归，不过写法稍有不同，递归函数直接返回结果，这样参数就少了许多，但是思路还是一样的，如果cur等于N-1了，直接将cur先装入数组，再装入结果res中返回。否则就遍历cur的邻接结点，对于每个邻接结点，先调用递归函数，然后遍历其返回的结果，对于每个遍历到的path，将cur加到数组首位置，然后将path加入结果res中即可，这有点像是回溯的思路，路径是从后往前组成的，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        return helper(graph, 0);
    }
    vector<vector<int>> helper(vector<vector<int>>& graph, int cur) {
        if (cur == graph.size() - 1) {
            return {{graph.size() - 1}};
        }
        vector<vector<int>> res;
        for (int neigh : graph[cur]) {
            for (auto path : helper(graph, neigh)) {
                path.insert(path.begin(), cur);
                res.push_back(path);
            }
        }
        return res;
    }
};
```



类似题目：

[https://leetcode.com/problems/all-paths-from-source-to-target/solution/](https://leetcode.com/problems/all-paths-from-source-to-target/solution/)

[https://leetcode.com/problems/all-paths-from-source-to-target/discuss/121135/6-lines-C++-dfs](https://leetcode.com/problems/all-paths-from-source-to-target/discuss/121135/6-lines-C++-dfs)

[https://leetcode.com/problems/all-paths-from-source-to-target/discuss/118691/Easy-and-Concise-DFS-Solution-C++-2-line-Python](https://leetcode.com/problems/all-paths-from-source-to-target/discuss/118691/Easy-and-Concise-DFS-Solution-C++-2-line-Python)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












