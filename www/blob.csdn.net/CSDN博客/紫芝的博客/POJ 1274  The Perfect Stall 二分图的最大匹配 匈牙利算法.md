# POJ 1274  The Perfect Stall 二分图的最大匹配 匈牙利算法 - 紫芝的博客 - CSDN博客





2018年08月28日 12:12:17[紫芝](https://me.csdn.net/qq_40507857)阅读数：36








# [The Perfect Stall](http://poj.org/problem?id=1274)
|**Time Limit:** 1000MS||**Memory Limit:** 10000K|
|----|----|----|
|**Total Submissions:** 29041||**Accepted:** 12738|

Description

Farmer John completed his new barn just last week, complete with all the latest milking technology. Unfortunately, due to engineering problems, all the stalls in the new barn are different. For the first week, Farmer John randomly assigned cows to stalls, but it quickly became clear that any given cow was only willing to produce milk in certain stalls. For the last week, Farmer John has been collecting data on which cows are willing to produce milk in which stalls. A stall may be only assigned to one cow, and, of course, a cow may be only assigned to one stall. 

Given the preferences of the cows, compute the maximum number of milk-producing assignments of cows to stalls that is possible. 

Input

The input includes several cases. For each case, the first line contains two integers, N (0 <= N <= 200) and M (0 <= M <= 200). N is the number of cows that Farmer John has and M is the number of stalls in the new barn. Each of the following N lines corresponds to a single cow. The first integer (Si) on the line is the number of stalls that the cow is willing to produce milk in (0 <= Si <= M). The subsequent Si integers on that line are the stalls in which that cow is willing to produce milk. The stall numbers will be integers in the range (1..M), and no stall will be listed twice for a given cow.

Output

For each case, output a single line with a single integer, the maximum number of milk-producing stall assignments that can be made.

Sample Input

5 5
2 2 5
3 2 3 4
2 1 5
3 1 2 5
1 2 

Sample Output

4
Source

[USACO 40](http://poj.org/searchproblem?field=source&key=USACO+40)

### 题意：

有n头奶牛，m个牛棚，每头奶牛有k个喜欢的牛棚，问最多可以使多少头奶牛有自己喜欢的牛棚？

### 分析：

以奶牛为n1顶点集，以牛棚为n2顶点集，建立一个二分图，若奶牛x喜欢牛棚y，则建立一条从x->y的边，然后使用匈牙利算法求二分图最大匹配即可

```cpp
#include<cstdio>
#include<cstring>
using namespace std;
const int N=1001;
int n1,n2,k;
//n1,n2为二分图的顶点集，其中x∈n1,y∈n2
int map[N][N],vis[N],link[N];
//link记录n2中的点y在n1中所匹配的x点的编号

int find(int x)
{
    int i;
    for(i=1;i<=n2;i++)
    {
        //x->i有边，且节点i未被搜索
        if(map[x][i]&&!vis[i])
        {
            vis[i]=1;//标记节点已被搜索

            //如果i不属于前一个匹配M或者i匹配到的节点可以寻找到增广路
            if(link[i]==0||find(link[i])){
                link[i]=x;//更新
                return 1;//匹配成功
            }
        }
    }
    return 0;
}
int main()
{
    while( scanf("%d%d",&n1,&n2)!=EOF){
        memset(map,0,sizeof map);
        memset(link,0,sizeof link);
        for(int j=1;j<=n1;j++)
    {
        scanf("%d",&k);
        for(int i=0;i<k;i++)
    {
        int y;
        scanf("%d",&y);
        map[j][y]=1;
    }
}

    int s=0;
    for(int i=1;i<=n1;i++)
    {
        memset(vis,0,sizeof(vis));
        if(find(i)) s++;
    }
    printf("%d\n",s);
}
    return 0;
}
```





