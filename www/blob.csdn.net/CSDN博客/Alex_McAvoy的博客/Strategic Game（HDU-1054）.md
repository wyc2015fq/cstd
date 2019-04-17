# Strategic Game（HDU-1054） - Alex_McAvoy的博客 - CSDN博客





2018年12月02日 18:30:28[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：47
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Bob enjoys playing computer games, especially strategic games, but sometimes he cannot find the solution fast enough and then he is very sad. Now he has the following problem. He must defend a medieval city, the roads of which form a tree. He has to put the minimum number of soldiers on the nodes so that they can observe all the edges. Can you help him?

Your program should find the minimum number of soldiers that Bob has to put for a given tree.

# **Input**

The input file contains several data sets in text format. Each data set represents a tree with the following description:

the number of nodes

the description of each node in the following format

node_identifier:(number_of_roads) node_identifier1 node_identifier2 ... node_identifier

or

node_identifier:(0)

The node identifiers are integer numbers between 0 and n-1, for n nodes (0 < n <= 1500). Every edge appears only once in the input data.

For example for the tree: 

![](https://img-blog.csdnimg.cn/20181202181723466.gif)

the solution is one soldier ( at the node 1).

# Output

The output should be printed on the standard output. For each given input data set, print one integer number in a single line that gives the result (the minimum number of soldiers).

# Sample Input

**40:(1) 11:(2) 2 32:(0)3:(0)53:(3) 1 4 21:(1) 02:(0)0:(0)4:(0)**

# Sample Output

**12**


题意：给一个具有 n 个点的树的所有边的信息，现在问最少需要多少个点放到树上，使得树的任意一条边都至少有一个端点被覆盖

思路：实质是要求最小点覆盖数，由于树是天然二分都，因此可以使用翻倍法将树上奇数层点与偶数层点分别作为左点集与右点集，求树的最小覆盖数最后除以 2 即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
bool vis[N];
int link[N];
vector<int> G[N];
bool dfs(int x){
    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];
        if(!vis[y]){
            vis[y]=true;
            if(link[y]==-1 || dfs(link[y]))	{
                link[y]=x;
                return true;
            }
        }
    }
    return false;
}
int hungarian(int n){
    int ans=0;
    for(int i=0;i<n;i++){
        memset(vis,false,sizeof(vis));
        if(dfs(i))
            ans++;
    }
    return ans;
}
int main(){
    int n;
    while(scanf("%d",&n)!=EOF&&(n)){
        memset(link,-1,sizeof(link));
        for(int i=0;i<n;i++)
            G[i].clear();

        for(int i=0;i<n;i++){
            int x,y,num;
            scanf("%d:(%d)",&x,&num);
            while(num--){
                scanf("%d",&y);
                G[x].push_back(y);
                G[y].push_back(x);
            }
        }

        printf("%d\n",hungarian(n)/2);
    }
    return 0;
}
```






