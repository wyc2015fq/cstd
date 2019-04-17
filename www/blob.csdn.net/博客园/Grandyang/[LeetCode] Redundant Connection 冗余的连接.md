# [LeetCode] Redundant Connection 冗余的连接 - Grandyang - 博客园







# [[LeetCode] Redundant Connection 冗余的连接](https://www.cnblogs.com/grandyang/p/7628977.html)







In this problem, a tree is an undirected graph that is connected and has no cycles.

The given input is a graph that started as a tree with N nodes (with distinct values 1, 2, ..., N), with one additional edge added. The added edge has two different vertices chosen from 1 to N, and was not an edge that already existed.

The resulting graph is given as a 2D-array of `edges`. Each element of `edges` is a pair `[u, v]` with `u < v`, that represents an undirected edge connecting nodes `u` and `v`.

Return an edge that can be removed so that the resulting graph is a tree of N nodes. If there are multiple answers, return the answer that occurs last in the given 2D-array. The answer edge `[u, v]` should be in the same format, with `u < v`.

Example 1:
Input: [[1,2], [1,3], [2,3]]
Output: [2,3]
Explanation: The given undirected graph will be like this:
  1
 / \
2 - 3



Example 2:
Input: [[1,2], [2,3], [3,4], [1,4], [1,5]]
Output: [1,4]
Explanation: The given undirected graph will be like this:
5 - 1 - 2
    |   |
    4 - 3



Note:
- The size of the input 2D-array will be between 3 and 1000.
- Every integer represented in the 2D-array will be between 1 and N, where N is the size of the input array.



Update (2017-09-26):
We have overhauled the problem description + test cases and specified clearly the graph is an *undirected* graph. For the *directed*graph follow up please see [Redundant Connection II](https://leetcode.com/problems/redundant-connection-ii/description/)). We apologize for any inconvenience caused.



这道题给我们了一个无向图，让我们删掉组成环的最后一条边，其实这道题跟之前那道[Graph Valid Tree](http://www.cnblogs.com/grandyang/p/5257919.html)基本没什么区别，三种解法都基本相同。博主觉得老题稍微变一下就是一道新题，而onsite遇到原题的概率很小，大多情况下都会稍稍变一下，所以举一反三的能力真的很重要，要完全吃透一道题也不太容易，需要多下功夫。我们首先来看递归的解法，这种解法的思路是，每加入一条边，就进行环检测，一旦发现了环，就返回当前边。对于无向图，我们还是用邻接表来保存，建立每个结点和其所有邻接点的映射，由于两个结点之间不算有环，所以我们要避免这种情况 1->{2}, 2->{1}的死循环，所以我们用一个变量pre记录上一次递归的结点，比如上一次遍历的是结点1，那么在遍历结点2的邻接表时，就不会再次进入结点1了，这样有效的避免了死循环，使其能返回正确的结果，参见代码如下：



解法一：

```
class Solution {
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        unordered_map<int, unordered_set<int>> m;
        for (auto edge : edges) {
            if (hasCycle(edge[0], edge[1], m, -1)) return edge;
            m[edge[0]].insert(edge[1]);
            m[edge[1]].insert(edge[0]);
        }
        return {};
    }
    bool hasCycle(int cur, int target, unordered_map<int, unordered_set<int>>& m, int pre) {
        if (m[cur].count(target)) return true;
        for (int num : m[cur]) {
            if (num == pre) continue;
            if (hasCycle(num, target, m, cur)) return true;
        }
        return false;
    }
};
```



既然递归能做，一般来说迭代也木有问题。但是由于BFS的遍历机制和DFS不同，所以没法采用利用变量pre来避免上面所说的死循环(不是很确定，可能是博主没想出来，有做出来的请在评论区贴上代码)，所以我们采用一个集合来记录遍历过的结点，如果该结点已经遍历过了，那么直接跳过即可，否则我们就把该结点加入queue和集合，继续循环，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        unordered_map<int, unordered_set<int>> m;
        for (auto edge : edges) {
            queue<int> q{{edge[0]}};
            unordered_set<int> s{{edge[0]}};
            while (!q.empty()) {
                auto t = q.front(); q.pop();
                if (m[t].count(edge[1])) return edge;
                for (int num : m[t]) {
                    if (s.count(num)) continue;
                    q.push(num);
                    s.insert(num);
                }
            }
            m[edge[0]].insert(edge[1]);
            m[edge[1]].insert(edge[0]);
        }
        return {};
    }
};
```



其实这道题最好的解法使用Union Find来做，论坛上清一色的都是用这种解法来做的，像博主用DFS和BFS这么清新脱俗的方法还真不多:) 其实Union Find的核心思想并不是很难理解，首先我们建立一个长度为(n+1)的数组root，由于这道题并没有明确的说明n是多少，只是说了输入的二位数组的长度不超过1000，那么n绝对不会超过2000，我们加1的原因是由于结点值是从1开始的，而数组是从0开始的，我们懒得转换了，就多加一位得了。我们将这个数组都初始化为-1，有些人喜欢初始化为i，都可以。开始表示每个结点都是一个单独的组，所谓的Union Find就是要让结点之间建立关联，比如若root[1] = 2，就表示结点1和结点2是相连的，root[2] = 3表示结点2和结点3是相连的，如果我们此时新加一条边[1, 3]的话，我们通过root[1]得到2，再通过root[2]得到3，说明结点1有另一条路径能到结点3，这样就说明环是存在的；如果没有这条路径，那么我们要将结点1和结点3关联起来，让root[1] = 3即可，参见代码如下：



解法三：

```
class Solution {
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        vector<int> root(2001, -1);
        for (auto edge : edges) {
            int x = find(root, edge[0]), y = find(root, edge[1]);
            if (x == y) return edge;
            root[x] = y;
        }
        return {};
    }
    int find(vector<int>& root, int i) {
        while (root[i] != -1) {
            i = root[i];
        }
        return i;
    }
};
```



类似题目：

[Graph Valid Tree](http://www.cnblogs.com/grandyang/p/5257919.html)



参考资料：

[https://discuss.leetcode.com/topic/104729/10-line-java-solution-union-find](https://discuss.leetcode.com/topic/104729/10-line-java-solution-union-find)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












