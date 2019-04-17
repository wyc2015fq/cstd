# lintcode - 图中两个点之间的路线 - guoziqing506的博客 - CSDN博客





2016年08月29日 09:50:22[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：971标签：[bfs																[lintcode																[图](https://so.csdn.net/so/search/s.do?q=图&t=blog)
个人分类：[lintcode](https://blog.csdn.net/guoziqing506/article/category/6289140)

所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)](https://so.csdn.net/so/search/s.do?q=lintcode&t=blog)




题目描述：给出一张有向图，设计一个算法判断两个点 s 与 t 之间是否存在路线。




样例：如下图，




![](https://img-blog.csdn.net/20160829095107250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



for s = B and t = E, return true

for s = D and t = C, return false




题中的数据结构时有向图，关于有向图的定义其实看下面的代码就很容易明白了：



```python
class DirectedGraphNode:
    def __init__(self, x):
        self.label = x
        self.neighbors = []
```

也就是说，每个图中的节点所指向的节点在此节点当中有一个列表neigbors负责存储。现在要做的事情是写一个算法判断图中的节点之间是否存在路径，那么可以考虑用广度优先搜索（BFS(breadth-first search)）





之前，我已经说过深度优先搜索（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51150899)），原则是每走到一步就尽可能地“深入”，直到不能再深入为止。那么，其实广度优先搜索于深度优先搜索是相对应的，广度优先搜索的原则是每到一个节点，都尽可能地“延伸”。拿在有向图中搜索为例，扫描到当前节点之后，应该依次先访问这个节点所指向的其他节点，也就是neigbors中存储的节点，再继续访问neigbors中存储节点的neigbors. 话有点绕，看题中的图就容易明白了：一开始访问A，其后访问B, D，再访问C, E. 




显然，依照这个逻辑，可以利用队列先进先出的性质。




代码如下：



```python
# Definition for a Directed graph node
# class DirectedGraphNode:
#     def __init__(self, x):
#         self.label = x
#         self.neighbors = []


class Solution:
    """
    @param graph: A list of Directed graph node
    @param s: the starting Directed graph node
    @param t: the terminal Directed graph node
    @return: a boolean value
    """
    def hasRoute(self, graph, s, t):
        
        # visited表示已经访问了的节点
        visited = [s]
        return self.bfs(graph, s, t, visited)
    
    
    def bfs(self, graph, s, t, visited):
        queue = [s]
        while len(queue) != 0:
            temp = queue.pop(0)
            if temp == t:
                return True
            visited.append(temp)
            for ele in temp.neighbors:
                if ele not in visited:
                    queue.append(ele)
        return False
```](https://so.csdn.net/so/search/s.do?q=bfs&t=blog)




