# 华为软件精英挑战赛及最短路径dijkstra源代码 - 李鑫o_O - CSDN博客

2016年03月23日 09:06:15[hustlx](https://me.csdn.net/HUSTLX)阅读数：1052


前言

赛题源自“未来网络”业务发放中的路由计算问题。算路问题属于基础算法问题，在图论、网络、交通等各个方面均有着广泛的研究与运用，里面不乏一些经典的算法，例如最短路中的广度优先搜索，Dijkstra算法等。网络算路问题的更优算法实现对于网络资源高效配置具有重要价值。

1 问题定义

给定一个带权重的有向图G=(V,E)，V为顶点集，E为有向边集，每一条有向边均有一个权重。对于给定的顶点s、t，以及V的子集V'，寻找从s到t的不成环有向路径P，使得P经过V'中所有的顶点(对经过V'中节点的顺序不做要求)。

若不存在这样的有向路径P，则输出无解，程序运行时间越短，则视为结果越优；若存在这样的有向路径P，则输出所得到的路径，路径的权重越小，则视为结果越优，在输出路径权重一样的前提下，程序运行时间越短，则视为结果越优。

说明：

1）图中所有权重均为[1，20]内的整数；

2）任一有向边的起点不等于终点；

3）连接顶点A至顶点B的有向边可能超过一条，其权重可能一样，也可能不一样；

4）该有向图的顶点不会超过600个，每个顶点出度(以该点为起点的有向边的数量)不超过8；

5）V'中元素个数不超过50；

6）从s到t的不成环有向路径P是指，P为由一系列有向边组成的从s至t的有向连通路径，且不允许重复经过任一节点；

7）路径的权重是指所有组成该路径的所有有向边的权重之和。

2 输入与输出

输入文件格式

以两个.csv 文件(csv 是以逗号为分隔符的文本文件)给出输入数据，一个为图的数据(G)，一个为需要计算的路径信息(s,t,V')。文件每行以换行符(ASCII'\n'即0x0a)为结尾。

1）图的数据中，每一行包含如下的信息：

LinkID,SourceID,DestinationID,Cost

其中，LinkID 为该有向边的索引，SourceID 为该有向边的起始顶点的索引，DestinationID为该有向边的终止顶点的索引，Cost 为该有向边的权重。顶点与有向边的索引均从0 开始 编号(不一定连续，但用例保证索引不重复)。

2）路径信息中，只有一行如下数据：

SourceID,DestinationID,IncludingSet

其中，SourceID 为该路径的起点，DestinationID 为该路径的终点，IncludingSet 表示必须经过的顶点集合V'，其中不同的顶点索引之间用'|'分割。

输出文件格式

输出文件同样为一个.csv 文件。

1）如果该测试用例存在满足要求的有向路径P，则按P 经过的有向边顺序，依次输出有向边的索引，索引之间用'|'分割；

2）如果该测试用例不存在满足要求的有向路径P，则输出两个字符NA；

3）只允许输出最多一条有向路径。

这个题有点类似TSP问题即著名的旅行商问题，这是一个NP难问题，下面给出基于本题的单源最短路径dijkstra算法，作为抛砖引玉，但完整解决此题需要更深一步考虑必须经过给定节点和所有节点最多一次，完全的TSP问题有多种解法，常见的有动态规划、遗传算法等

```cpp
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
using namespace std;

struct edge 
{ 
    int DestinationID,Cost,LinkID;
};
vector< vector<edge>> initialize_single_rource() {
    vector<vector<int>> row;
    vector< vector<edge>> graph;
    ifstream in("topo.csv");
    if (in.fail()) {
        cout << "File not found" << endl;
        return graph;
    }
    string temp;
    while (getline(in, temp))
    {
        vector<int> line;
        int i = 0;
        int begin = 0;
        while (i < temp.size()) {
            if (temp[i] == ',') {
                line.push_back(stoi(temp.substr(begin, i - begin)));
                begin = i + 1;
            }
            i++;
        }
        line.push_back(stoi(temp.substr(begin, temp.size() - begin)));
        row.push_back(line);
    }
    in.close();
    
    for (int i = 0; i < row.size(); i++) {
        while (graph.size() <= row[i][1]) {
            graph.push_back(vector<edge>());
        }
        graph[row[i][1]].push_back({ row[i][2],row[i][3],row[i][0] });
    }
    return graph;
}
int dijkstra(const vector< vector<edge>> &graph, int source, int target) {
    vector<int> min_distance(graph.size(), INT_MAX);
    min_distance[source] = 0;
    set< pair<int, int> > active_vertices;
    active_vertices.insert({ 0,source });
    while (!active_vertices.empty()) {
        int where = active_vertices.begin()->second;
        if (where == target) return min_distance[where];
        active_vertices.erase(active_vertices.begin());
        for (auto edge : graph[where])
            if (min_distance[edge.DestinationID] > min_distance[where] + edge.Cost) {
                active_vertices.erase({ min_distance[edge.DestinationID], edge.DestinationID });
                min_distance[edge.DestinationID] = min_distance[where] + edge.Cost;
                active_vertices.insert({ min_distance[edge.DestinationID], edge.DestinationID });
            }
    }
    return INT_MAX;
}
int main() {
    vector< vector<edge>> graph = initialize_single_rource();
    int mindist = dijkstra(graph, 0, 18);
}
```

