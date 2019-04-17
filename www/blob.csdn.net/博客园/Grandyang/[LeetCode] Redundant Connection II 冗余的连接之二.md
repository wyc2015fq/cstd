# [LeetCode] Redundant Connection II 冗余的连接之二 - Grandyang - 博客园







# [[LeetCode] Redundant Connection II 冗余的连接之二](https://www.cnblogs.com/grandyang/p/8445733.html)








In this problem, a rooted tree is a directed graph such that, there is exactly one node (the root) for which all other nodes are descendants of this node, plus every node has exactly one parent, except for the root node which has no parents.

The given input is a directed graph that started as a rooted tree with N nodes (with distinct values 1, 2, ..., N), with one additional directed edge added. The added edge has two different vertices chosen from 1 to N, and was not an edge that already existed.

The resulting graph is given as a 2D-array of `edges`. Each element of `edges` is a pair `[u, v]` that represents a directed edge connecting nodes `u` and `v`, where `u` is a parent of child `v`.

Return an edge that can be removed so that the resulting graph is a rooted tree of N nodes. If there are multiple answers, return the answer that occurs last in the given 2D-array.

Example 1:
Input: [[1,2], [1,3], [2,3]]
Output: [2,3]
Explanation: The given directed graph will be like this:
  1
 / \
v   v
2-->3



Example 2:
Input: [[1,2], [2,3], [3,4], [4,1], [1,5]]
Output: [4,1]
Explanation: The given directed graph will be like this:
5 <- 1 -> 2
     ^    |
     |    v
     4 <- 3



Note:
- The size of the input 2D-array will be between 3 and 1000.
- Every integer represented in the 2D-array will be between 1 and N, where N is the size of the input array.




这道题是之前那道[Redundant Connection](http://www.cnblogs.com/grandyang/p/7628977.html)的拓展，那道题给的是无向图，只需要删掉组成环的最后一条边即可，归根到底就是检测环就行了。而这道题给我们的是有向图，那么整个就复杂多了，因为有多种情况存在，比如给的例子1就是无环，但是有入度为2的结点3。再比如例子2就是有环，但是没有入度为2的结点。其实还有一种情况例子没有给出，就是既有环，又有入度为2的结点。好，我们现在就来总结一下这三种情况：

第一种：无环，但是有结点入度为2的结点（结点3）

[[1,2], [1,3], [2,3]]

```
1
 / \
v   v
2-->3
```



第二种：有环，没有入度为2的结点

[[1,2], [2,3], [3,4], [4,1], [1,5]]

```
5 <- 1 -> 2
     ^    |
     |    v
     4 <- 3
```



第三种：有环，且有入度为2的结点（结点1）

[[1,2],[2,3],[3,1],[1,4]]

```
4
    /
   v
   1
 /  ^
v    \
2 -->3
```



对于这三种情况的处理方法各不相同，首先对于第一种情况，我们返回的产生入度为2的后加入的那条边[2, 3]，而对于第二种情况，我们返回的是刚好组成环的最后加入的那条边[4, 1]，最后对于第三种情况我们返回的是组成环，且组成入度为2的那条边[3, 1]。

明白了这些，我们先来找入度为2的点，如果有的话，那么我们将当前产生入度为2的后加入的那条边标记为second，前一条边标记为first。然后我们来找环，为了方便起见，找环使用联合查找Union Find的方法，可参见[Redundant Connection](http://www.cnblogs.com/grandyang/p/7628977.html)中的解法三。当我们找到了环之后，如果first不存在，说明是第二种情况，我们返回刚好组成环的最后加入的那条边。如果first存在，说明是第三种情况，我们返回first。如果没有环存在，说明是第一种情况，我们返回second，参见代码如下：



```
class Solution {
public:
    vector<int> findRedundantDirectedConnection(vector<vector<int>>& edges) {
        int n = edges.size();
        vector<int> root(n + 1, 0), first, second;
        for (auto& edge : edges) {
            if (root[edge[1]] == 0) {
                root[edge[1]] = edge[0];
            } else {
                first = {root[edge[1]], edge[1]};
                second = edge;
                edge[1] = 0;
            }
        }
        for (int i = 0; i <= n; ++i) root[i] = i;
        for (auto& edge : edges) {
            if (edge[1] == 0) continue;
            int x = getRoot(root, edge[0]), y = getRoot(root, edge[1]);
            if (x == y) return first.empty() ? edge : first;
            root[x] = y;
        }
        return second;
    }
    int getRoot(vector<int>& root, int i) {
        return i == root[i] ? i : getRoot(root, root[i]);
    }
};
```



讨论：使用联合查找Union Find的方法一般都需要写个子函数，来查找祖宗结点，上面的解法getRoot()函数就是这个子函数，我们使用递归的形式来写的，其实还可以用迭代的方式来写，下面这两种写法都可以：



```
int getRoot(vector<int>& root, int i) {
    while (i != root[i]) {
        root[i] = root[root[i]];
        i = root[i];
    }
    return i;
}
```



```
int getRoot(vector<int>& root, int i) {
    while (i != root[i]) i = root[i];
    return i;
}
```



类似题目：

[Redundant Connection](http://www.cnblogs.com/grandyang/p/7628977.html)



参考资料：

[https://leetcode.com/problems/redundant-connection-ii/](https://leetcode.com/problems/redundant-connection-ii/)

[https://leetcode.com/problems/redundant-connection-ii/discuss/108045/C++Java-Union-Find-with-explanation-O(n)](https://leetcode.com/problems/redundant-connection-ii/discuss/108045/C++Java-Union-Find-with-explanation-O(n))

[https://leetcode.com/problems/redundant-connection-ii/discuss/108058/one-pass-disjoint-set-solution-with-explain](https://leetcode.com/problems/redundant-connection-ii/discuss/108058/one-pass-disjoint-set-solution-with-explain)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












