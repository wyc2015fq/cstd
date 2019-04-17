# [CareerCup] 4.2 Route between Two Nodes in Directed Graph 有向图中两点的路径 - Grandyang - 博客园







# [[CareerCup] 4.2 Route between Two Nodes in Directed Graph 有向图中两点的路径](https://www.cnblogs.com/grandyang/p/4691004.html)







4.2 Given a directed graph, design an algorithm to find out whether there is a route between two nodes.



LeetCode和CareerCup中关于图的题都不是很多，LeetCode中只有三道，分别是[Clone Graph 无向图的复制](http://www.cnblogs.com/grandyang/p/4267628.html)，[Course Schedule 课程清单](http://www.cnblogs.com/grandyang/p/4484571.html) 和 [Course Schedule II 课程清单之二](http://www.cnblogs.com/grandyang/p/4504793.html)。目前看来CareerCup中有关图的题在第四章中仅此一道，这是一道关于有向图的题，书中是用java来做的，我们用c++来做时要定义图和节点，这里参考了之前那道[Clone Graph 无向图的复制](http://www.cnblogs.com/grandyang/p/4267628.html)中关于无向图的定义，并且在里面加入了一个枚举类变量state，来帮助我们遍历。这种找两点之间路径的题就是遍历的问题，可以用BFS或DFS来解，先来看BFS的解法，如下所示：



```
//Definition for directed graph.
enum State {
    Unvisited, Visited, Visiting
};

struct DirectedGraphNode {
   int label;
   State state;
   vector<DirectedGraphNode *> neighbors;
   DirectedGraphNode(int x) : label(x) {};
};

struct DirectedGraph {
    vector<DirectedGraphNode*> nodes;
};

class Solution {
public:
    bool search(DirectedGraph *g, DirectedGraphNode *start, DirectedGraphNode *end) {
        queue<DirectedGraphNode*> q;
        for (auto a : g->nodes) a->state = Unvisited;
        start->state = Visiting;
        q.push(start);
        while (!q.empty()) {
            DirectedGraphNode *node = q.front(); q.pop();
            for (auto a : node->neighbors) {
                if (a->state == Unvisited) {
                    if (a == end) return true;
                    else {
                        a->state = Visiting;
                        q.push(a);
                    }
                }
            }    
            node->state = Visited;
        }
        return false;
    }
};
```














