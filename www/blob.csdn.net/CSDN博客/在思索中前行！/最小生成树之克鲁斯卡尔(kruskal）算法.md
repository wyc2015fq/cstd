# 最小生成树之克鲁斯卡尔(kruskal）算法 - 在思索中前行！ - CSDN博客





2015年04月10日 16:18:44[_Tham](https://me.csdn.net/txl16211)阅读数：604








```cpp
#include <iostream>  
#include <string>  
using namespace std;  
  
typedef struct MGraph{  
    string vexs[10];//顶点信息  
    int arcs[10][10];//邻接矩阵  
    int vexnum, arcnum;//顶点数和边数  
}MGraph;  
  
int LocateVex(MGraph G, string u)//返回顶点u在图中的位置  
{  
    for(int i=0; i<G.vexnum; i++)  
        if(G.vexs[i]==u)  
            return i;  
    return -1;  
}  
  
void CreateUDN(MGraph &G)//构造无向网  
{  
    string v1, v2;  
    int w;  
    int i, j, k;  
    cout<<"请输入顶点数和边数：";  
    cin>>G.vexnum>>G.arcnum;  
  
    cout<<"请输入顶点：";  
    for(i=0; i<G.vexnum; i++)  
        cin>>G.vexs[i];  
  
    for(i=0; i<G.vexnum; i++)  
        for(j=0; j<G.vexnum; j++)  
            G.arcs[i][j]=1000;//初始化权值  
  
    cout<<"请输入边和权值："<<endl;  
    for(k=0; k<G.arcnum; k++)  
    {  
        cin>>v1>>v2>>w;  
        i=LocateVex(G, v1);  
        j=LocateVex(G, v2);  
        G.arcs[i][j]=G.arcs[j][i]=w;  
    }  
}  
  
void Kruskal(MGraph G)//克鲁斯卡尔算法  
{  
    int set[10], i, j;  
    int k=0, a=0, b=0, min=G.arcs[a][b];  
  
    for(i=0; i<G.vexnum; i++)  
        set[i]=i;//初态，各顶点分别属于各个集合  
  
    cout<<"最小生成树的各条边为："<<endl;  
  
    while(k < G.vexnum-1)//最小生成树的边数等于顶点数-1  
    {  
        for(i=0; i<G.vexnum; i++)//寻找最小权值的边，无向网，只在上三角形中查找  
            for(j=i+1; j<G.vexnum; j++)  
                if(G.arcs[i][j] < min)  
                {  
                    min=G.arcs[i][j];//最小权值  
                    a=i;//边的一个顶点  
                    b=j;//边的另一个顶点  
                }  
  
        min=G.arcs[a][b]=1000;//避免下次查找  
  
        if(set[a]!=set[b])//边的两个顶点不属于同一集合  
        {  
            cout<<G.vexs[a]<<"-"<<G.vexs[b]<<endl;  
            k++;//边数加1  
            for(i=0; i<G.vexnum; i++)  
                if(set[i]==set[b])//将顶点b所在集合并入顶点a集合  
                    set[i]=set[a];  
        }  
    }  
}  
  
void main()  
{  
    MGraph G;  
    CreateUDN(G);  
    Kruskal(G);  
}
```


![](https://img-my.csdn.net/uploads/201205/26/1337999777_8284.jpg)



![](https://img-my.csdn.net/uploads/201205/26/1337999813_3165.jpg)




