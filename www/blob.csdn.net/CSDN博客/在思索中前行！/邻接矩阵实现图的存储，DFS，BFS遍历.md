# 邻接矩阵实现图的存储，DFS，BFS遍历 - 在思索中前行！ - CSDN博客





2014年09月02日 17:14:45[_Tham](https://me.csdn.net/txl16211)阅读数：2790








 图的遍历一般由两者方式：深度优先搜索(DFS)，广度优先搜索(BFS)，深度优先就是先访问完最深层次的数据元素，而BFS其实就是层次遍历，每一层每一层的遍历。

##  1.深度优先搜索(DFS)

 我一贯习惯有举例的方法来讲，示例如下：红色代表的是正搜索，蓝色代表回溯，最下面为标志数组。

![](https://img-blog.csdn.net/20150504221222290?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHhsMTk5MTA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 注意：DFS的搜索出来的序列不是每个人都是一样的，根据具体的程序可能出现不同的顺序。

    程序设计：由对深度优先搜索的理解，我们可以知道我们从根节点的开始向下搜索，注意题目中给出的是连通的图，在实际情况下可能有非连通的图，图中根节点v1的子节点为v2，v3，我们可以访问它的子节点按照从左到右也可以从右到左，我们这里选择从左到右的方向，先访问v2，访问v2后又以v2作为子节点开始从左到右访问，但是我们可能注意到访问到v5时，如果我们仅仅从图的相关节点的连通性来判断是否应该继续深入的话，我们会在v5节点形成一个环，从而造成程序的死循环，这里就需要一个标志数组来标志哪些节点是已经访问了的，这样不仅仅能够保证不形成环也可以为节点回溯提供保证。说到这里我们基本已经解决了连通图的DFS，我们主要到如果是非连通图我们不能通过子图的连通性来访问到所有的节点，所以下面一段代码就是必须的了；



```cpp
for(i=0; i<n; i++)

   if(!color[i])
   {
      printf(" V%d ", i+1);

      DFS_Visit(G,i,n);
 
      printf("\n");
   }
```



 这段代码从第一个节点测试到最后一个节点测试是否已经访问过，如果是非连通图，中间一定会有在第一次访问完成之后还有节点没有访问，所有利用标志数组就可以轻易的达到这个目的。

上图测试用例.txt

1 2

1 3

2 4

2 5

4 8

5 8

3 6

3 7

6 7

测试程序：



```cpp
#include <stdio.h>
#include <string.h>
/*
dfs:深度优先搜索，利用了递归的方法，以根结点为起点，逐条分支深度搜索到底（先是节点的最左边分支被搜）
 */

const int GNumber = 8;
int  G[GNumber][GNumber];
int  color[GNumber];       //标志数组

void  DFS_Visit(int  G[][GNumber], int i, int n)
{
             int j;
             color[i] = 1;
             for(j=0; j< n; j++)         //遍历所有n个节点中，所有与i相同的节点，都将以这些节点为起点继续搜索
             {
                          if(G[i][j] && !color[j])
                          {
                                       printf(" V%d ", j+1);
                                       color[j] = 1;
                                       DFS_Visit(G, j, n);
                          }
             }

}

void  DFS(int G[][GNumber], int n)
{
             int i;
             memset(color, 0, sizeof(color));
             //n个顶点，每个顶点都要作为起始点尝试搜索访问，不可能总是任何一个顶点开始都能遍历整个图
             for(i=0; i<n; i++)
             {
                          if(!color[i])     //顶点i未曾访问过
                          {
                                       printf(" V%d ", i+1);  //首先输出i,然后继续搜索查找/访问与i连通的节点
                                       DFS_Visit(G,i,n);
                                       printf("\n");
                          }
             }

}

int   main()
{
          //   FILE *fr;
             int i,j;
         //    fr = fopen("图的遍历测试用例.txt","r");
         /*    if(!fr)
             {
                          printf("fopen failed\n");
                          return -1;
             }
             */
      //       while(fscanf(fr,"%d%d", &i, &j) != EOF)
             while(scanf("%d%d",&i,&j)!=EOF)
             {
                          G[i-1][j-1] = 1;          //注意存储图的数组是从0开始编号的
                          G[j-1][i-1] = 1;
             }
             DFS(G,GNumber);
//             getchar();
             return  0;
}
```



 程序结果：
     V1  V2  V4  V8  V5  V3  V6  V7

##  2.广度优先搜索(BFS)

 示例如下：红色代表的是正搜索，最下面为标志数组。

![](https://img-blog.csdn.net/20150504221600931)


 程序设计：一个图如果要层次遍历的话，那么他应该是连通图，不然层次没法分，对一个连通图进行层次遍历，我们模拟一下就知道，如上图，当访问了v1节点后，我们就应该访问第二层都为它的子节点，我们这里以顺序从左到右访问，那么应该访问的是v2，v3，为了能够表示访问的顺序，我们这里设置一个先进先出的结构，很明显就是一个队列了，要访问前v1将它放入队列，然后访问v1，并将他的子节点放到队列中：v1，v2，v3；访问了v1后出队输出，我们继续访问队列中的元素，以队列中的元素为根节点找他的子节点并加入到队列中，队列为空。这里的标志数组标志着节点是否进入过队列，这里由于元素很少而且队列中的元素肯定不会超过顶点个数，所以我直接使用的数组来模拟队列。
测试程序：

```cpp
#include <stdio.h>
#include <string.h>
/*
 BFS :广度优先搜索，类似与队列的先进先出顺序进行遍历，因此可以采用队列存储遍历过的节点，然后再按照队列的思想“先进先出”，一次出队
 */

const int GNumber = 8;
int G[GNumber][GNumber];
int color[GNumber];         // 访问标志数组, 防止回环 ( 重复访问 )

struct Queue                    //用数组模拟队列
{
              int queue[GNumber];
              int start;
              int end;
}MyQueue;

void BFS(int G[][GNumber], int n)
{
                int j;
                 MyQueue.queue[MyQueue.end++] = 0; //节点0，入队
                 color[0] = 1;              //访问第一个节点，标识为真

                 /*
                for(j=0; j<n; j++)
                {
                      if(G[i][j] && !color[j])
                      {
                           printf(" V%d ", j+1);
                           color[j] = 1;
                           MyQueue.queue[MyQueue.end++] = j;
                      }
                }
                */

               while(MyQueue.end != MyQueue.start) //直到队列中元素全部出队结束
              {
                        //color[MyQueue.start] = 1;
                        for(j=0; j<n; j++)
                        {
                                if(G[MyQueue.start][j] && !color[j])  //j与队列中当前节点start连通，且未访问过
                                {
                                     color[j] = 1;               //标识j节点访问标志
                                     MyQueue.queue[MyQueue.end++] = j;  //j入队
                                }
                        }
                        //关键在于这一句，利用队列先进先出的思想，在while()循环中，逐个，逐层输出节点
                        printf(" V%d ", MyQueue.queue[MyQueue.start++]+1);
                        //每次输出一个节点后，起始标志Start++，向前挪动一位
                }
}

int main(int argc, char **argv)
{
//        FILE *fr;
        int i,j;
        /*
        fr = fopen("图的遍历测试用例.txt","r");
        if(!fr)
        {
            printf("fopen failed\n");
            return -1;
        }
        while(fscanf(fr,"%d%d", &i, &j) != EOF)
        */
        while(scanf("%d%d",&i,&j)!=EOF)
        {
            G[i-1][j-1] = 1;              //注意存储图的数组是从0开始编号的
            G[j-1][i-1] = 1;
        }
        memset(&MyQueue, 0, sizeof(MyQueue));
        memset(color, 0, sizeof(color));

        BFS(G,GNumber);

//        getchar();
         return 0;
}
```

   程序结果：
   V1  V2  V3  V4  V5  V6  V7  V8

## [图的dfs与bfs（深搜广搜）c++ STL模板实现](http://www.cnblogs.com/PegasusWang/archive/2013/04/06/3002511.html)
bfs通过检测边发现点，被发现点（但未探索）入队。（被探索是指是否检测过与该点相关联的临近顶点）一个顶点被完全探索当且仅当他的所有边被检测。一个顶点探索完选另一个顶点，被选点应位于被发现但未被探索点队列的队首。待探索点集为空时算法结束。（bfs探索顺序与发现顺序一致，dfs发现后马上探索）

```cpp
#include <iostream>
#include <cstdio>
#include <list>
#include <vector>
#include <queue>
using namespace std;
int n;
vector< list<int> > graph;
bool visited[100] = {0};
void dfs(int v)
{
    list<int>::iterator it;
    visited[v] = true;
    printf("%5d", v);
    for (it = graph[v].begin(); it != graph[v].end(); ++it)
        if (!visited[*it])
            dfs(*it);
}
void bfs(int v)
{
    list<int>::iterator it;
    printf("%5d", v);
    visited[v] = true;
    queue<int> t;
    t.push(v);
    while (!t.empty())
    {
        v = t.front();
        t.pop();
        for (it = graph[v].begin(); it != graph[v].end(); ++it)
            if (!visited[*it])
            {
                printf("%5d", *it);
                t.push(*it);
                visited[*it] = true;
            }
    }
    cout << endl;
}
int  main()
{
    //freopen("in.txt", "r", stdin);
    cout << "input the vertex num:"<< endl;
    cin >> n;
    vector< list<int> >::iterator it;
    for (int i = 0; i < n; ++i)
    {
        list<int> il;
        int t;
        while (cin >> t && t != n)
                   il.push_back(t);
                   graph.push_back(il);
    }
    cout << "result for bfs:" << endl;
    bfs(0);
    memset(visited, 0, sizeof(visited));                   //重新初始化标志数组
    cout << "result for dfs:" << endl;
    dfs(0);
    return 0;
}
```

![](http://images.cnitblog.com/blog/434467/201304/06204631-6ba55fd3e8e84364b437e12571d32cd0.jpg)

按照链表表示输入以下数据：

8

0 1 2 8

1 0 3 4 8

2 0 5 6 8

3 1 7 8

4 1 7 8

5 2 7 8

6 2 7 8

7 3 4 5 6 8      //表示7与3，4，5，6是连通的


最后一个8用来标识这个节点输入结束。可以得到深搜和广搜的结果。




