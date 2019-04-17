# Prim算法和Kruskal算法 - 在思索中前行！ - CSDN博客





2014年09月02日 18:15:42[_Tham](https://me.csdn.net/txl16211)阅读数：687标签：[prim																[kruskal](https://so.csdn.net/so/search/s.do?q=kruskal&t=blog)](https://so.csdn.net/so/search/s.do?q=prim&t=blog)
个人分类：[NOIP 图论																[数据结构](https://blog.csdn.net/txl16211/article/category/2409063)](https://blog.csdn.net/txl16211/article/category/2451417)









Prim算法和Kruskal算法都能从连通图找出最小生成树。区别在于Prim算法是以某个顶点出发挨个找，而Kruskal是先排序边，每次选出最短距离的边再找。




      一、Prim（普里姆算法）算法：

        Prim算法实现的是找出一个有权重连通图中的最小生成树，即：具有最小权重且连接到所有结点的树。(强调的是树，树是没有回路的)。

    Prim算法是这样来做的： 

    首先以一个结点作为最小生成树的初始结点，然后以迭代的方式找出与最小生成树中各结点权重最小边，并加入到最小生成树中。加入之后如果产生回路则跳过这条边，选择下一个结点。当所有结点都加入到最小生成树中之后，就找出了连通图中的最小生成树了。



    Prim算法最小生成树查找过程：




![](http://dl.iteye.com/upload/attachment/482507/81eaa7e5-492e-3cc4-bdc4-062ae70f8a1e.jpg)



C语言实现：






```cpp
#include <stdio.h>
#include <stdlib.h>
#define maxint 1073741824
int main()
{
　　FILE *input=fopen("input.txt","r"),*out=fopen("output.txt","w");
　　int n,m,i,j,x,y,w;
　　fscanf(input,"%d %d",&n,&m);
　　int map[n][n],E[m][3],tree[m],Mst[n][n];
　　/*Mst表示最小生成树的邻接矩阵，map是原图，E是边集，其中E[0]和E[1]是边的两个顶点，E[2]是边的权值，tree是用于判断原图的点是否在最小生成树中*/
　　memset(tree,0,sizeof(tree));
　　for(i=0; i<n; i++)
　　{
　　 for(j=0; j<n; j++)
　　  { 
　    　map[i][j]=maxint;
　　    Mst[i][j]=maxint;
　 　 }
　　  E[i][0]=E[i][1]=maxint;
　  }
　　for(i=0; i<m; i++)
　　{
　　  fscanf(input,"%d %d %d",&x,&y,&w);
　　  if(w<map[x][y])
　　  {
　　    map[x][y]=w;
　　    map[y][x]=w;
　　  }
　　}
　　int min=maxint,next=0,now=0,k=0;
　　tree[0]=1;
　　for(i=0; i<n; i++)
　　{
　　  for(j=0; j<n; j++)
　　  {
　　    if(map[now][j]!=maxint && tree[j]==0)
　　    {
　　      E[k][0]=now;
　　      E[k][2]=map[now][j];
　　      E[k++][1]=j;
　　    }
　　  }
　　 for(j=0; j<k; j++)
　　 {
　　   if(E[j][2]<min && tree[E[j][1]]==0)
　　   {
　　    min=E[j][2];
　　    x=E[j][0];
　　    y=E[j][1];
　　    next=y;
　　   }
　　 }
　　 tree[next]=1;
　　 now=next;
　　 Mst[x][y]=map[x][y];
　　 Mst[y][x]=map[y][x];
　　 min=maxint;
　  }
　　for(i=0; i<n; i++)
　　{
　　  for(j=0; j<n; j++)
　　  {
　　    if(Mst[i][j]==maxint) //判断两点是否连通
　　       fprintf(out,"00 "); //美化输出，不必多加探究
　　    else
　　    {
　　       fprintf(out,"%d ",Mst[i][j]); //输出生成树的邻接矩阵，要输出树的自己可以根据邻接矩阵的数据进行加工
　　    }
　　  }
　　  fprintf(out,"\n");
　　}
　　fclose(input);
　　fclose(out);
　　return 0;
} // 程序未考虑不是连通图的情况，修改很简单，判断生成树的节点数量是否等于原图的节点数量
　　//如果小于（不会有大于）则本图不是连通图
　　//其实prim和迪杰斯特拉算法核心有相似之处
```






**    二、Kruskal（克鲁斯卡尔）算法：**

    Kruskal算法与Prim算法的不同之处在于，Kruskal在找最小生成树结点之前，需要对所有权重边做从小到大排序。将排序好的权重边依次加入到最小生成树中，如果加入时产生回路就跳过这条边，加入下一条边。当所有结点都加入到最小生成树中之后，就找出了最小生成树。



C语言实现：






```cpp
/*  Kruskal.c
　　Copyright (c) 2002, 2006 by ctu_85
　　All Rights Reserved.
    I am sorry to say that the situation of unconnected graph is not concerned 
	*/
#include "stdio.h"
#define maxver 10
#define maxright 100
int G[maxver][maxver],record=0,touched[maxver][maxver];
int circle=0;
int FindCircle(int,int,int,int);
int main()
{
    int path[maxver][2],used[maxver][maxver]; 

　　int i,j,k,t,min=maxright,exsit=0;
　　int v1,v2,num,temp,status=0;
　　restart:
　　printf("Please enter the number of vertex(s) in the graph:\n");
　　scanf("%d",&num);
　　if(num>maxver||num<0)
　　{
　　    printf("Error!Please reinput!\n");
　　    goto restart;
　　}
　　for(j=0;j<num;j++)
　　 for(k=0;k<num;k++)
　　 {
　　   if(j==k)
　　   {
　　    G[j][k]=maxright;
　　    used[j][k]=1;
　　    touched[j][k]=0;
　　   }
　　   else
　　   if(j<k)
　　   {
　　    re:
　　    printf("Please input the right between vertex %d and vertex %d,if no edge exists please input -1:\n",j+1,k+1);
　　    scanf("%d",&temp);
　　    if(temp>=maxright||temp<-1)
　　    {
　　      printf("Invalid input!\n");
　　      goto re;
　　    }
　　    if(temp==-1)
　　    temp=maxright;
　　    G[j][k]=G[k][j]=temp;
　　    used[j][k]=used[k][j]=0;
　　    touched[j][k]=touched[k][j]=0;
　　   }
　　 }
　　 for(j=0;j<num;j++)
　　 {
　　   path[j][0]=0;
　　   path[j][1]=0;
　　 }
　　 for(j=0;j<num;j++)
　　 {
　　   status=0;
　　   for(k=0;k<num;k++)
　　   if(G[j][k]<maxright)
　　   {
　　     status=1;
　　     break;
　　   }
　　   if(status==0)
　　   break;
　　 }
　　 for(i=0;i<num-1&&status;i++)
　　 {
　　   for(j=0;j<num;j++)
　　   for(k=0;k<num;k++)
　　   if(G[j][k]<min&&!used[j][k])
　　   {
　　     v1=j;
　　     v2=k;
　　     min=G[j][k];
　　   }
　　 if(!used[v1][v2])
　　 {
　　   used[v1][v2]=1;
　　   used[v2][v1]=1;
　　   touched[v1][v2]=1;
　　   touched[v2][v1]=1;
　　   path[0]=v1;
　　   path[1]=v2;
　　   for(t=0;t<record;t++)
　　   FindCircle(path[t][0],path[t][0],num,path[t][0]);
　　   if(circle)
　　   {/*if a circle exsits,roll back*/
　　     circle=0;
　　      i--;
　　     exsit=0;
　　     touched[v1][v2]=0;
　　     touched[v2][v1]=0;
　　     min=maxright;
　　   }
　　   else
　　   {
　　     record++;
　　     min=maxright;
　　   }
　　  }
　　}
　　if(!status)
　　printf("We cannot deal with it because the graph is not connected!\n");
　　else
　　{
　　  for(i=0;i<num-1;i++)
　　  printf("Path %d:vertex %d to vertex %d\n",i+1,path[0]+1,path[1]+1);
　　}
　　return 1;
}
int FindCircle(int start,int begin,int times,int pre)
{ /* to judge whether a circle is produced*/
　　int i;
　　for(i=0;i<times;i++)
　　if(touched[begin]==1)
　　{
　　  if(i==start&&pre!=start)
　　  {
　　    circle=1;
　　    return 1;
　　    break;
　　  }
　　else
　　  if(pre!=i)
　　    FindCircle(start,i,times,begin);
　　  else
　　    continue;
　　}
　　return 1;
}
```










 无疑，Kruskal算法在效率上要比Prim算法快，因为Kruskal只需要对权重边做一次排序，而Prim算法则需要做多次排序。尽管Prim算法每次做的算法涉及的权重边不一定会涵盖连通图中的所有边，但是随着所使用的排序算法的效率的提高，Kruskal算法和Prim算法之间的差异将会清晰的显性出来。













