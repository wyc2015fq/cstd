# DFS和BFS（搜索的方向是一种可能性，不重不漏就好） - PeterBishop - CSDN博客





2018年08月08日 23:34:57[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：62








### 图的深度优先搜索(Depth First Search)，和树的先序遍历比较类似。它的思想：假设初始状态是图中所有顶点均未被访问，则从某个顶点v出发，首先访问该顶点，然后依次从它的各个未被访问的邻接点出发深度优先搜索遍历图，直至图中所有和v有路径相通的顶点都被访问到。 若此时尚有其他顶点未被访问到，则另选一个未被访问的顶点作起始点，重复上述过程，直至图中所有顶点都被访问到为止。显然，深度优先搜索是一个递归的过程，适合去寻找多少种组合。

```cpp
DFS:/*该DFS 框架以2D 坐标范围为例，来体现DFS 算法的实现思想。*/
#include<cstdio>
#include<cstring>
#include<cstdlib>
using namespace std;
const int maxn=100;
bool vstmaxn; // 访问标记
int mapmaxn;// 坐标范围
int dir4={0,1,0,-1,1,0,-1,0}; // 方向向量，(x,y)周围的四个方向

bool CheckEdge(int x,int y) // 边界条件和约束条件的判断
{ 
    if(!vstx && ...) // 满足条件 
        return 1; 
    else // 与约束条件冲突 
        return 0;
}

void dfs(int x,int y)
{ 
    vstx=1; // 标记该节点被访问过 
    if(mapx==G) // 出现目标态G
    { 
        ...... // 做相应处理
        return; 
    } 
    for(int i=0;i<4;i++) 
    { 
        if(CheckEdge(x+diri,y+diri)) // 按照规则生成下一个节点 
        {

​             dfs(x+diri,y+diri);

​             //回溯恢复修改状态……
​        }
        return; // 没有下层搜索节点，回溯
     }

}
```

### 广度优先搜索算法(Breadth First Search)，又称为"宽度优先搜索"或"横向优先搜索"，简称BFS。它的思想是：从图中某顶点v出发，在访问了v之后依次访问v的各个未曾访问过的邻接点，然后分别从这些邻接点出发依次访问它们的邻接点，并使得先被访问的顶点的邻接点先于后被访问的顶点的邻接点被访问，直至图中所有已被访问的顶点的邻接点都被访问到。如果此时图中尚有顶点未被访问，则需要另选一个未曾被访问过的顶点作为新的起始点，重复上述过程，直至图中所有顶点都被访问到为止。换句话说，广度优先搜索遍历图的过程是以v为起点，由近至远，依次访问和v有路径相通且路径长度为1,2...的顶点，适合去找最短路

```cpp
#include<cstdio>
#include<cstring>
#include<queue>
#include<algorithm>
using namespace std;
const int maxn=100;
bool vstmaxn; // 访问标记
int dir4={0,1,0,-1,1,0,-1,0}; // 方向向量

struct State // BFS 队列中的状态数据结构
{ 
    int x,y; // 坐标位置 
    int Step_Counter; // 搜索步数统计器
};

State a[maxn];

boolCheckState(State s) // 约束条件检验
{ 
    if(!vsts.x && ...) // 满足条件 
        return 1; 
    else // 约束条件冲突 
        return 0;
}

void bfs(State st)
{ 
    queue <State> q; // BFS 队列 
    State now,next; // 定义2 个状态，当前和下一个 
    st.Step_Counter=0; // 计数器清零 
    q.push(st); // 入队 
    vstst.x=1; // 访问标记 
    while(!q.empty()) 
    { 
        now=q.front(); // 取队首元素进行扩展 
        if(now==G) // 出现目标态，此时为Step_Counter 的最小值，可以退出即可 
        { 
            ...... // 做相关处理 
            return; 
        } 
        for(int i=0;i<4;i++) 
        { 
            next.x=now.x+diri; // 按照规则生成 下一个状态 
            next.y=now.y+diri; 
            next.Step_Counter=now.Step_Counter+1; // 计数器加1 
            if(CheckState(next)) // 如果状态满足约束条件则入队 
            { 
                q.push(next); 
                vstnext.x=1; //访问标记 
            } 
        } 
        q.pop(); // 队首元素出队 
    } 
    return;
}
}
```





