# 图的基本操作（基于邻接表）：图的构造，深搜（DFS），广搜（BFS） - 在思索中前行！ - CSDN博客





2014年12月02日 16:04:05[_Tham](https://me.csdn.net/txl16211)阅读数：924








```cpp
#include <iostream>  
#include <string>  
#include <queue>  
using namespace std;  
  
//表结点  
typedef struct ArcNode{  
    int adjvex;//该弧所指向的顶点的位置  
    ArcNode *nextarc;//指向下一条弧的指针  
}ArcNode;  
  
//头结点  
typedef struct VNode{  
    string data;//顶点信息  
    ArcNode* firstarc;//第一个表结点的地址，指向第一条依附该顶点的弧的指针  
}VNode, AdjList[10];  
  
typedef struct{  
    AdjList vertices;  
    int vexnum, arcnum;//图的顶点数和弧数  
}ALGraph;  
  
int LocateVex(ALGraph G, string u)//返回顶点u在图中的位置  
{  
    for(int i=0; i<G.vexnum; i++)  
        if(G.vertices[i].data==u)  
            return i;  
    return -1;  
}  
  
void CreateUDG(ALGraph &G)//构造无向图  
{  
    string v1, v2;  
    int i, j, k;  
    cout<<"请输入顶点数和边数：";  
    cin>>G.vexnum>>G.arcnum;  
  
    cout<<"请输入顶点：";  
    for(i=0; i<G.vexnum; i++)  
    {  
        cin>>G.vertices[i].data;  
        G.vertices[i].firstarc=NULL;  
    }  
  
    cout<<"请输入边：";  
    cout<<endl;  
    for(k=0; k<G.arcnum; k++)  
    {  
        cin>>v1>>v2;  
        i=LocateVex(G, v1);  
        j=LocateVex(G, v2);  
  
        //插入v1的邻接表,为了提高效率，总在表头插入结点。  
        ArcNode *arc=new ArcNode;  
        arc->adjvex=j;  
        arc->nextarc=G.vertices[i].firstarc;  
        G.vertices[i].firstarc=arc;  
  
        //插入v2的邻接表,为了提高效率，总在表头插入结点。  
        arc=new ArcNode;  
        arc->adjvex=i;  
        arc->nextarc=G.vertices[j].firstarc;  
        G.vertices[j].firstarc=arc;  
          
    }  
}  
  
void Print(ALGraph G)//打印邻接表  
{  
    cout<<"打印邻接表如下：";  
    cout<<endl;  
    for(int i=0; i<G.vexnum; i++)//遍历每个顶点的邻接表  
    {  
        cout<<G.vertices[i].data;  
        ArcNode *p=G.vertices[i].firstarc;  
          
        while(p)  
        {  
            cout<<"->"<<G.vertices[p->adjvex].data;  
            p=p->nextarc;  
        }  
        cout<<endl;  
          
    }  
      
}  
  
int FirstAdjVex(ALGraph G, int v)//返回顶点v的第一个邻接点序号  
{  
      
    ArcNode *p=G.vertices[v].firstarc;  
    if(p)  
        return p->adjvex;  
    else  
        return -1;  
}  
  
int NextAdjVex(ALGraph G, int v, int w)//返回顶点v的相对于w的下一个邻接点序号  
{  
  
    ArcNode* p=G.vertices[v].firstarc;  
    while(p)   
    {  
        if(p->adjvex==w)  
            break;  
        p=p->nextarc;  
    }  
  
    if(p->adjvex!=w || !p->nextarc)//如果没找到w或者w是最后一个邻接点  
        return -1;  
    return p->nextarc->adjvex;  
  
}  
bool visited[10];  
  
void DFS(ALGraph G, int v)  
{  
    visited[v]=true;  
    cout<<G.vertices[v].data<<" ";  
    for(int w=FirstAdjVex(G, v); w>=0; w=NextAdjVex(G, v, w) )  
        if(!visited[w])  
            DFS(G, w);  
  
}  
  
void DFSTraverse(ALGraph G)//深搜  
{  
    for(int i=0; i<G.vexnum; i++)  
        visited[i]=false;  
  
    for(i=0; i<G.vexnum; i++)  
        if(!visited[i])  
            DFS(G, i);  
  
}  
  
void BFSTraverse(ALGraph G)//广搜  
{  
    queue<int> q;  
    for(int i=0; i<G.vexnum; i++)  
        visited[i]=false;  
  
    for(i=0; i<G.vexnum; i++)  
    {  
        if(!visited[i])  
        {  
            q.push(i);  
            visited[i]=true;  
            while(!q.empty())  
            {  
                int v=q.front();  
                q.pop();  
                cout<<G.vertices[v].data<<" ";  
                for(int w=FirstAdjVex(G, v); w>=0; w=NextAdjVex(G, v, w))  
                {  
                    if (!visited[w])  
                    {  
                        q.push(w);  
                        visited[w]=true;  
                    }  
                }  
  
            }  
        }  
  
    }  
  
}  
  
void main()  
{  
    ALGraph G;  
    CreateUDG(G);  
    Print(G);  
  
    cout<<"深搜：";  
    DFSTraverse(G);  
    cout<<endl;  
  
    cout<<"广搜：";  
    BFSTraverse(G);  
    cout<<endl;  
}
```


![](https://img-my.csdn.net/uploads/201205/25/1337941299_1816.jpg)



![](https://img-my.csdn.net/uploads/201205/25/1337941324_4442.JPG)




