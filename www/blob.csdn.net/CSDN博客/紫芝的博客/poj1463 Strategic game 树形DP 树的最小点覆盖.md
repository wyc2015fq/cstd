# poj1463 Strategic game 树形DP 树的最小点覆盖 - 紫芝的博客 - CSDN博客





2018年07月29日 10:27:12[紫芝](https://me.csdn.net/qq_40507857)阅读数：38








# Strategic game
|**Time Limit:** 2000MS||**Memory Limit:** 10000K|
|----|----|----|
|**Total Submissions:** 9293||**Accepted:** 4358|

Description

Bob enjoys playing computer games, especially strategic games, but sometimes he cannot find the solution fast enough and then he is very sad. Now he has the following problem. He must defend a medieval city, the roads of which form a tree. He has to put the minimum number of soldiers on the nodes so that they can observe all the edges. Can you help him? 


Your program should find the minimum number of soldiers that Bob has to put for a given tree. 


For example for the tree: 

![](http://poj.org/images/1463_1.jpg)



the solution is one soldier ( at the node 1).

Input

The input contains several data sets in text format. Each data set represents a tree with the following description: 
- 

- the number of nodes 
- the description of each node in the following format 

	node_identifier:(number_of_roads) node_identifier1 node_identifier2 ... node_identifiernumber_of_roads 

	or 

	node_identifier:(0) 



The node identifiers are integer numbers between 0 and n-1, for n nodes (0 < n <= 1500);the number_of_roads in each line of input will no more than 10. Every edge appears only once in the input data.

Output

The output should be printed on the standard output. For each given input data set, print one integer number in a single line that gives the result (the minimum number of soldiers). An example is given in the following:

Sample Input

4
0:(1) 1
1:(2) 2 3
2:(0)
3:(0)
5
3:(3) 1 4 2
1:(1) 0
2:(0)
0:(0)
4:(0)
Sample Output

1
2
Source

[Southeastern Europe 2000](http://poj.org/searchproblem?field=source&key=Southeastern+Europe+2000)

### 题意：

一城堡的所有的道路形成一个n个节点的树，如果在一个节点上放上一个士兵，那么和这个节点相连的边就会被看守住，问把所有边看守住最少需要放多少士兵。

### 分析：

典型树形动态规划：树的最小点覆盖

•**第一步：确定状态**

•f[x][1]以x为根的子树在x上放置的士兵的最少所需的士兵数目

•f[x][0]以x为根的子树x上不放置的士兵的最少所需的士兵数目

•**第二步：确定状态转移方程**

•f[x][1] =1 + Σ min(f[son][0],f[son][1]) 

•若x上放置的士兵，它的儿子们可放可不放，取最小值

•f[x][0] = Σ f[son][1] 

•若x上不放置的士兵，它的儿子们都必须放！

•结果为min(f[root][0], f[root][1])，取根节点放与不放的最小值

```
#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f
const int maxn=1600;
int n;
int father[maxn];
vector<int>G[maxn];//存储有向图
int dp[2][maxn];
void dfs(int x)//无需指定x的根节点
{
    for(int i=0;i<G[x].size();i++)
    {
        int son=G[x][i];
        dfs(son);
        dp[0][x]+=dp[1][son];//这一点不放，则儿子必须放
        dp[1][x]+=min(dp[0][son],dp[1][son]);//这一点放
    }
}
int main()
{
    while(scanf("%d",&n)!=EOF){
        memset(father,-1,sizeof(father));
        memset(dp,0,sizeof(dp));
 
        for(int i=0;i<n;i++)
        {
            dp[1][i]=1;
            G[i].clear();
        }
        for(int i=0;i<n;i++)
        {
            int id,num,kid;
            scanf("%d:(%d)",&id,&num);
            for(int j=0;j<num;j++)
            {
                scanf("%d",&kid);
                G[id].push_back(kid);
                father[kid]=0;
            }
        }
        int root=1;
        for(int i=0;i<n;i++)
        {
            if(father[i]==-1)
            {
                root=i;break;
            }
        }
        //printf("R:%d\n",root);
        dfs(root);
        printf("%d\n",min(dp[1][root],dp[0][root]) );
    }
    return 0;
}
```







