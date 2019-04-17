# leetcode-207. Course Schedule - 别说话写代码的博客 - CSDN博客





2019年04月15日 23:20:02[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：4标签：[207. Course Schedule																[leetcode																[leetcode 207																[课程表](https://so.csdn.net/so/search/s.do?q=课程表&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)




|There are a total of *n* courses you have to take, labeled from `0` to `n-1`.Some courses may have prerequisites, for example to take course 0 you have to first take course 1, which is expressed as a pair: `[0,1]`Given the total number of courses and a list of prerequisite **pairs**, is it possible for you to finish all courses?**Example 1:****Input:** 2, [[1,0]] **Output: **true**Explanation:** There are a total of 2 courses to take.              To take course 1 you should have finished course 0. So it is possible.**Example 2:****Input:** 2, [[1,0],[0,1]]**Output: **false**Explanation:** There are a total of 2 courses to take.              To take course 1 you should have finished course 0, and to take course 0 you should             also have finished course 1. So it is impossible.**Note:**- The input prerequisites is a graph represented by **a list of edges**, not adjacency matrices. Read more about [how a graph is represented](https://www.khanacademy.org/computing/computer-science/algorithms/graph-representation/a/representing-graphs).- You may assume that there are no duplicate edges in the input prerequisites.|现在你总共有 *n* 门课需要选，记为 `0` 到 `n-1`。在选修某些课程之前需要一些先修课程。 例如，想要学习课程 0 ，你需要先完成课程 1 ，我们用一个匹配来表示他们: `[0,1]`给定课程总量以及它们的先决条件，判断是否可能完成所有课程的学习？**示例 1:****输入:** 2, [[1,0]] **输出: **true**解释:** 总共有 2 门课程。学习课程 1 之前，你需要完成课程 0。所以这是可能的。**示例 2:****输入:** 2, [[1,0],[0,1]]**输出: **false**解释:** 总共有 2 门课程。学习课程 1 之前，你需要先完成​课程 0；并且学习课程 0 之前，你还应先完成课程 1。这是不可能的。**说明:**- 输入的先决条件是由**边缘列表**表示的图形，而不是邻接矩阵。详情请参见[图的表示法](http://blog.csdn.net/woaidapaopao/article/details/51732947)。- 你可以假定输入的先决条件中没有重复的边。**提示:**- 这个问题相当于查找一个循环是否存在于有向图中。如果存在循环，则不存在拓扑排序，因此不可能选取所有课程进行学习。- [通过 DFS 进行拓扑排序](https://www.coursera.org/specializations/algorithms) - 一个关于Coursera的精彩视频教程（21分钟），介绍拓扑排序的基本概念。- 拓扑排序也可以通过 [BFS](https://baike.baidu.com/item/%E5%AE%BD%E5%BA%A6%E4%BC%98%E5%85%88%E6%90%9C%E7%B4%A2/5224802?fr=aladdin&fromid=2148012&fromtitle=%E5%B9%BF%E5%BA%A6%E4%BC%98%E5%85%88%E6%90%9C%E7%B4%A2) 完成。|
|----|----|

思路：

方法一：BFS.这个题目就是检测有向图中有没有环。做法就是先建立一个邻接表，然后建一个课程入度的表in，定义一个队列，将所有入度为0的节点放入，然后按照队列中度为0的节点遍历其邻接表，每到一个节点，将in对应的值减一，这就相当于进行拓扑排序，然后从前往后去掉了边。最后只需要判断是不是所有的节点入度都为0.

```
class Solution {
public:
    bool canFinish(int numCourses, vector<pair<int, int>>& prerequisites) {
        vector<vector<int>> graph(numCourses,vector<int>(0));
        vector<int> in(numCourses,0);
        for(auto a:prerequisites)
        {
            graph[a.second].push_back(a.first);
            in[a.first]++;
        }
        queue<int> q;
        for(int i=0;i<numCourses;++i)
        {
            if(in[i]==0) q.push(i);
        }
        while(!q.empty())
        {
            int t=q.front();
            q.pop();
            for(auto a:graph[t])
            {
                in[a]--;
                if(in[a]==0) q.push(a);
            }
        }
        for(int i=0;i<numCourses;++i)
            if(in[i]!=0) return false;
        return true;
    }
};
```

方法二：DFS.

```
class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int> >& prerequisites) {
        vector<vector<int> > graph(numCourses, vector<int>(0));
        vector<int> visit(numCourses, 0);
        for (auto a : prerequisites) {
            graph[a[1]].push_back(a[0]);
        }
        for (int i = 0; i < numCourses; ++i) {
            if (!canFinishDFS(graph, visit, i)) return false;
        }
        return true;
    }
    bool canFinishDFS(vector<vector<int> > &graph, vector<int> &visit, int i) {
        if (visit[i] == -1) return false;
        if (visit[i] == 1) return true;
        visit[i] = -1;
        for (auto a : graph[i]) {
            if (!canFinishDFS(graph, visit, a)) return false;
        }
        visit[i] = 1;
        return true;
    }
};
```](https://so.csdn.net/so/search/s.do?q=leetcode 207&t=blog)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)](https://so.csdn.net/so/search/s.do?q=207. Course Schedule&t=blog)




