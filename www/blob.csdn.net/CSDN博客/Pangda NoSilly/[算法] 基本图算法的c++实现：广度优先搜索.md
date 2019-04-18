# [算法] 基本图算法的c++实现：广度优先搜索 - Pangda NoSilly - CSDN博客

2017年06月02日 20:31:56[hepangda](https://me.csdn.net/hepangda)阅读数：279


# 伪代码

```python
BFS(G, s):
    for each vertex u ∈ G.V - {s}:
        u.color = White
        u.d = ∞
        u.π = NIL
    s.color = Gray
    s.d = 0
    s.π = NIL

    Q = ∅
    Q.push(s)
    while Q ≠ ∅:
        u = Q.pop()
        for each v ∈ G.Adj[u]:
            if v.color == White then
                v.color = Gray
                v.d = u.d + 1
                v.π = u
                Q.push(v)
        u.color = Black
```

# C++的一般实现方式

## 第一种写法

首先，我们严格按照伪代码形式来将之写为C++形式。 

第一步就是先构造每一个结点，因为结点具有多种属性，因此我们可以使用以下的结构体构造形式。

```
enum vertexcolor {
    WHITE,
    GRAY,
    BLACK
};

struct vertex {
    vertexcolor color;
    int d;
    int pi;
};
```

其中我们将结点的颜色color使用一个枚举变量表示。 

第二步，构造完毕结点之后，我们还应当构造出图的各种属性，比如图的各个边以及图结点的集合，我们仍然可以用结构体构造出一个图：

```cpp
struct graph {
    vector<vertex> V;
    map<vertex *, vector<vertex *> > Adj;
};
```

在该结构体中，V是图中所有结点的集合，Adj则是一个映射，每给定一个结点的指针，返回一个该结点连接的所有结点的指针的集合。 

第三步，在定义了graph这个结构之后，显然我们已经满足了伪代码中所有需要的属性，可以直接利用伪代码执行了。这样一般的结构就是：

```cpp
void BFS(graph *G, vertex *s) {
    const int INF = 0x3f3f3f;
    for (vertex u : G->V) {
        u.color = WHITE;
        u.d = INF;
        u.pi = nullptr;
    }
    s->color = GRAY;
    s->d = 0;
    //s->pi = nullptr; 此句在上述循环中已经进行过

    queue<vertex *> Q;
    Q.push(s);

    vertex *u;
    while (!Q.empty()) {
        u = Q.front();
        Q.pop();

        for (vertex *v : G->Adj[u]) {
            if (v->color == WHITE) {
                v->color = GRAY;
                v->d = u->d + 1;
                v->pi = u;
                Q.push(v);
            }
            u->color = BLACK;
        }
    }
}
```

这样我们就完成了一个通用性质的c++广度优先算法模板，然而这个模板还不能直接使用，在特定的地方需要作特定的修改。我们以以下这个问题作为例子，来探讨这种写法如何特例化。

```
迷宫问题

用一个5×5的二维数组表示一个迷宫，其中的1表示墙壁，0表示路，只能横着走或竖着走，不能斜着走，要求输出从左上角到右下角的所需的最小步数。
比如，若输入：
0 1 0 0 0
0 1 0 1 0
0 0 0 0 0
0 1 1 1 0
0 0 0 1 0
则输出：
8
```

对于这个问题，我们发现我们可以将图设定为只能向右和向下发展，这样我们将图中的每一个结点都将与他右边和下边的结点存在边相连，如果某一个结点上是一堵墙，我们认为他不与任何结点有边相连。因此，我们可以写出以下代码（注释在代码中已给出）：

```cpp
#include<iostream>
#include<map>
#include<vector>
#include<queue>
#define FIND(Row, Col) ((Row) * 5 + Col)
#define INIT(Row, Col) (Row >= 0 && Row < 5 && Col >= 0 && Col < 5)
//FIND宏用于查找处于i行j列的结点的下标
//INIT宏用于判定给定的i行j列是否在迷宫当中

using namespace std;

enum vertexcolor {
    WHITE,
    GRAY,
    BLACK
};

struct vertex {
    vertexcolor color;
    int d;
    vertex *pi;
};

struct graph {
    vector<vertex> V;
    map<vertex *, vector<vertex *> > Adj;
};

void BFS(graph &G, vertex *s) {
    const int INF = 0x3f3f3f;
    for (vertex &u : G.V) {
        u.color = WHITE;
        u.d = INF;
        u.pi = nullptr;
    }
    s->color = GRAY;
    s->d = 0;

    queue<vertex *> Q;
    Q.push(s);

    vertex *u;
    while (!Q.empty()) {
        u = Q.front();
        Q.pop();

        for (vertex *v : G.Adj[u]) {
            if (v->color == WHITE) {
                v->color = GRAY;
                v->d = u->d + 1;
                v->pi = u;
                Q.push(v);
            }
        }
        u->color = BLACK;
    }
}

int main() {
    graph it;
    it.V.resize(25); //将结点个数设为25个

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int t;
            cin >> t; 
            if (t == 0) { //若此处不是一堵墙
                if (INIT(i + 1, j)) //若该结点下方有一个结点
                    it.Adj[&it.V[FIND(i, j)]].push_back(&it.V[FIND(i + 1, j)]); 
                if (INIT(i, j + 1)) //若该结点右方有一个结点
                    it.Adj[&it.V[FIND(i, j)]].push_back(&it.V[FIND(i, j + 1)]);
            }
        }
    }

    BFS(it, &it.V[0]);
    cout << it.V[FIND(4, 4)].d << endl; //输出右下角到左上角的距离
    return 0;
}
```

## 第二种写法

上面一种写法严格按照了伪代码的格式，然而由于C++语言的特性，使得实现形式较为繁琐，而且使用了很多对具体问题而言没有意义的设定。于是第二种写法便是我们联系问题的实际情况，用全局数组来存放图的各个属性，若符合条件还可以将图的结构用邻接矩阵的方法来表示。 

以下我们以上例中的迷宫问题来探讨。

```
迷宫问题

用一个5×5的二维数组表示一个迷宫，其中的1表示墙壁，0表示路，只能横着走或竖着走，不能斜着走，要求输出从左上角到右下角的所需的最小步数。
比如，若输入：
0 1 0 0 0
0 1 0 1 0
0 0 0 0 0
0 1 1 1 0
0 0 0 1 0
则输出：
8
```

首先，在广度优先搜索中，我们需要表示图的结构。而我们发现在这个问题中，图是一个稀疏图，一共有25个结点，然而一个结点最多有两条边，所以此例子中不适合使用邻接矩阵法来表示图。第二，结点数目是固定的——25个。第三，每个结点的边都是具有相同特性的，即连向他正下方和右方的结点。所以我们不妨直接读入输入的数据存入一个二维数组，使用时只需要检测他下方和右方结点在不在5×5范围内并检测是否是墙，即可知道两结点之间是否存在边。除此之外，我们发现图中每一个点只有0和1两种属性，所以一个bool类型的变量即足够。于是我们可以定义图的结构为以下形式：
`bool graph_struct[5][5]; //true表示路径，false表示墙壁`
第二，我们发现在搜索算法中，颜色是一个十分重要的属性，但实际上研究代码可以发现，灰色和黑色没有什么不同，也就是说实际上只存在两种颜色：白色、非白色。这样我们可以将一个结点的颜色表示为true或者false，分别来表示白色和非白色。即我们可以定义颜色为：
`bool graph_color[5][5];  //true表示白色，false表示非白色`
第三，在搜索算法中所用到的前驱属性对这个问题而言没有意义，所以我们并不表示这个属性。 

第四，搜索算法中还有每一个点到起始点的距离，结合图的存储形式，我们直接将距离与图的结构一一对应即可，即同样使用一个5×5的数组来表示结构中每一个元素到左上角的距离。这样，有：
`int dist[5][5] { 0 };  //显然[0][0]所表示的左上角点到自身的距离为0`
最后需要解决的问题就是，在每次搜索中从队列中拿出元素的时候如何知道这个元素所表示的是什么点。我们用一个结构来表示：

```
struct pos {
    int x, y; //表示该点是存储在graph_struct[x][y]的点
};
```

现在在算法中所有需要的内容都已经被表示完毕，可以直接将之组织为代码了：

```cpp
#include<iostream>
#include<queue>
#define INIT(Row, Col) (Row >= 0 && Row < 5 && Col >= 0 && Col < 5)

using namespace std;

bool graph_struct[5][5]; //true表示路径，false表示墙壁
bool graph_color[5][5];  //true表示白色，false表示非白色
int dist[5][5];  //显然[0][0]所表示的左上角点到自身的距离为0

struct pos {
    int x, y;
};

void BFS() {
    const int INF = 0x3f3f3f;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            graph_color[i][j] = true;
            dist[i][j] = INF;
        }
    }
    dist[0][0] = 0;
    graph_color[0][0] = false;

    queue<pos> Q;
    Q.push(pos{0, 0}); //将起点放入队列

    pos u, v;
    while (!Q.empty()) {
        u = Q.front();
        Q.pop();

        if (INIT(u.x + 1, u.y) && graph_struct[u.x + 1][u.y]) {
            v.x = u.x + 1; v.y = u.y;
            graph_color[v.x][v.y] = false; //设置颜色
            dist[v.x][v.y] = dist[u.x][u.y] + 1; //设置距离
            Q.push(v);
        }

        if (INIT(u.x, u.y +  1) && graph_struct[u.x][u.y + 1]) {
            v.x = u.x; v.y = u.y + 1;
            graph_color[v.x][v.y] = false; 
            dist[v.x][v.y] = dist[u.x][u.y] + 1;
            Q.push(v);
        }
/* 此处是因为只有两种情况，故选择使用两个if。
 * 若有多个方向，可以考虑以下形式：(以下以此题做一个该种写法的例子）：
        const int opr[2][2] = { 1, 0, 0, 1 };
        for (int i = 0; i < 2; i++) {
            v.x = u.x + opr[i][0];
            v.y = u.y + opr[i][1];

            if (INIT(v.x, v.y) && graph_struct[v.x][v.y]) {
                graph_color[v.x][v.y] = false;
                dist[v.x][v.y] = dist[u.x][u.y] + 1;
                Q.push(v);
            }
        }
 */
    }
}

int main() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            cin >> graph_struct[i][j];
            graph_struct[i][j] = !graph_struct[i][j];
        }
    }

    BFS();
    cout << dist[4][4] << endl;

    return 0;
}
```

由于广度优先的性质，若在队列中第一次取出目标点，那么此时所经过的路程即为最后搜索的结果，在复杂的图中可以减少循环进行的次数。具体做法是将dist作为pos结构体的一个属性，在每次循环时更新。每次从队列中取出元素时判断是否为目标点，若为目标点则直接返回结果。可以参照这种做法：

```cpp
#include<iostream>
#include<queue>
#define INIT(Row, Col) (Row >= 0 && Row < 5 && Col >= 0 && Col < 5)

using namespace std;

bool graph_struct[5][5]; //true表示路径，false表示墙壁
bool graph_color[5][5];  //true表示白色，false表示非白色

struct pos {
    int x, y, dist; //将dist作为pos的一个属性
};

int BFS() { //将返回值作为算法的结果
    const int INF = 0x3f3f3f;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            graph_color[i][j] = true;
        }
    }
    graph_color[0][0] = false;

    queue<pos> Q;
    Q.push(pos{0, 0, 0});

    pos u, v;
    while (!Q.empty()) {
        u = Q.front();
        Q.pop();
        //判断是否到达终点
        if (u.x == 4 && u.y == 4)
            return u.dist; 
        if (INIT(u.x + 1, u.y) && graph_struct[u.x + 1][u.y]) {
            v.x = u.x + 1; v.y = u.y;
            graph_color[v.x][v.y] = false; //设置颜色
            v.dist = u.dist + 1; //###设置距离###
            Q.push(v);
        }

        if (INIT(u.x, u.y +  1) && graph_struct[u.x][u.y + 1]) {
            v.x = u.x; v.y = u.y + 1;
            graph_color[v.x][v.y] = false; 
            v.dist = u.dist + 1;
            Q.push(v);
        }
    }
}

int main() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            cin >> graph_struct[i][j];
            graph_struct[i][j] = !graph_struct[i][j];
        }
    }

    cout << BFS() << endl;

    return 0;
}
```

## 总结

一般而言，在解决具体问题时一般**参照**第二种写法，根据需求写出内容。第一种写法也可以进行很多改进，但这里没有叙述。可以参照第二种写法的一些思路进行修改。

