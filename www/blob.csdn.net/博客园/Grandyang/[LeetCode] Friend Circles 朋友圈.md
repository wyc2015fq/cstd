# [LeetCode] Friend Circles 朋友圈 - Grandyang - 博客园







# [[LeetCode] Friend Circles 朋友圈](https://www.cnblogs.com/grandyang/p/6686983.html)







There are N students in a class. Some of them are friends, while some are not. Their friendship is transitive in nature. For example, if A is a direct friend of B, and B is a direct friend of C, then A is an indirect friend of C. And we defined a friend circle is a group of students who are direct or indirect friends.

Given a N*N matrix M representing the friend relationship between students in the class. If M[i][j] = 1, then the ithand jth students are direct friends with each other, otherwise not. And you have to output the total number of friend circles among all the students.

Example 1:
Input: 
[[1,1,0],
 [1,1,0],
 [0,0,1]]
Output: 2
Explanation:The 0th and 1st students are direct friends, so they are in a friend circle. 
The 2nd student himself is in a friend circle. So return 2.



Example 2:
Input: 
[[1,1,0],
 [1,1,1],
 [0,1,1]]
Output: 1
Explanation:The 0th and 1st students are direct friends, the 1st and 2nd students are direct friends, 
so the 0th and 2nd students are indirect friends. All of them are in the same friend circle, so return 1.



Note:
- N is in range [1,200].
- M[i][i] = 1 for all students.
- If M[i][j] = 1, then M[j][i] = 1.



这道题让我们求朋友圈的个数，题目中对于朋友圈的定义是可以传递的，比如A和B是好友，B和C是好友，那么即使A和C不是好友，那么他们三人也属于一个朋友圈。那么比较直接的解法就是DFS搜索，对于某个人，遍历其好友，然后再遍历其好友的好友，那么我们就能把属于同一个朋友圈的人都遍历一遍，我们同时标记出已经遍历过的人，然后累积朋友圈的个数，再去对于没有遍历到的人在找其朋友圈的人，这样就能求出个数。其实这道题的本质是之前那道题[Number of Connected Components in an Undirected Graph](http://www.cnblogs.com/grandyang/p/5166356.html)，其实许多题目的本质都是一样的，就是看我们有没有一双慧眼能把它们识别出来：



解法一：

```
class Solution {
public:
    int findCircleNum(vector<vector<int>>& M) {
        int n = M.size(), res = 0;
        vector<bool> visited(n, false);
        for (int i = 0; i < n; ++i) {
            if (visited[i]) continue;
            helper(M, i, visited);
            ++res;
        }
        return res;
    }
    void helper(vector<vector<int>>& M, int k, vector<bool>& visited) {
        visited[k] = true;
        for (int i = 0; i < M.size(); ++i) {
            if (!M[k][i] || visited[i]) continue;
            helper(M, i, visited);
        }
    }
};
```



我们也可以用BFS来遍历朋友圈中的所有人，解题思路和上面大同小异，参见代码如下：



解法二：

```
class Solution {
public:
    int findCircleNum(vector<vector<int>>& M) {
        int n = M.size(), res = 0;
        vector<bool> visited(n, false);
        queue<int> q;
        for (int i = 0; i < n; ++i) {
            if (visited[i]) continue;
            q.push(i);
            while (!q.empty()) {
                int t = q.front(); q.pop();
                visited[t] = true;
                for (int j = 0; j < n; ++j) {
                    if (!M[t][j] || visited[j]) continue;
                    q.push(j);
                }
            }
            ++res;
        }
        return res;
    }
};
```



下面这种解法叫联合查找Union Find，也是一种很经典的解题思路，在之前的两道道题[Graph Valid Tree](http://www.cnblogs.com/grandyang/p/5257919.html)和[Number of Connected Components in an Undirected Graph](http://www.cnblogs.com/grandyang/p/5166356.html)中也有过应用，核心思想是初始时给每一个对象都赋上不同的标签，然后对于属于同一类的对象，在root中查找其标签，如果不同，那么将其中一个对象的标签赋值给另一个对象，注意root数组中的数字跟数字的坐标是有很大关系的，root存的是属于同一组的另一个对象的坐标，这样通过getRoot函数可以使同一个组的对象返回相同的值，参见代码如下：



解法三：

```
class Solution {
public:
    int findCircleNum(vector<vector<int>>& M) {
        int n = M.size(), res = n;
        vector<int> root(n);
        for (int i = 0; i < n; ++i) root[i] = i;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (M[i][j] == 1) {
                    int p1 = getRoot(root, i);
                    int p2 = getRoot(root, j);
                    if (p1 != p2) {
                        --res;
                        root[p2] = p1;
                    }
                }
            }   
        }
        return res;
    }
    int getRoot(vector<int>& root, int i) {
        while (i != root[i]) {
            root[i] = root[root[i]];
            i = root[i];
        }
        return i;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/85204/c-bfs](https://discuss.leetcode.com/topic/85204/c-bfs)

[https://discuss.leetcode.com/topic/85050/clean-java-union-find](https://discuss.leetcode.com/topic/85050/clean-java-union-find)

[https://discuss.leetcode.com/topic/85091/c-concise-dfs-solution](https://discuss.leetcode.com/topic/85091/c-concise-dfs-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












