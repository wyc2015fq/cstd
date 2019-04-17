# Air Raid（POJ-1422） - Alex_McAvoy的博客 - CSDN博客





2018年12月03日 22:13:42[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：68








> 
# Problem Description

Consider a town where all the streets are one-way and each street leads from one intersection to another. It is also known that starting from an intersection and walking through town's streets you can never reach the same intersection i.e. the town's streets form no cycles. 

With these assumptions your task is to write a program that finds the minimum number of paratroopers that can descend on the town and visit all the intersections of this town in such a way that more than one paratrooper visits no intersection. Each paratrooper lands at an intersection and can visit other intersections following the town streets. There are no restrictions about the starting intersection for each paratrooper. 

# **Input**

Your program should read sets of data. The first line of the input file contains the number of the data sets. Each data set specifies the structure of a town and has the format: 

no_of_intersections 

no_of_streets 

S1 E1 

S2 E2 

...... 

Sno_of_streets Eno_of_streets 

The first line of each data set contains a positive integer no_of_intersections (greater than 0 and less or equal to 120), which is the number of intersections in the town. The second line contains a positive integer no_of_streets, which is the number of streets in the town. The next no_of_streets lines, one for each street in the town, are randomly ordered and represent the town's streets. The line corresponding to street k (k <= no_of_streets) consists of two positive integers, separated by one blank: Sk (1 <= Sk <= no_of_intersections) - the number of the intersection that is the start of the street, and Ek (1 <= Ek <= no_of_intersections) - the number of the intersection that is the end of the street. Intersections are represented by integers from 1 to no_of_intersections. 

There are no blank lines between consecutive sets of data. Input data are correct. 

# Output

The result of the program is on standard output. For each input data set the program prints on a single line, starting from the beginning of the line, one integer: the minimum number of paratroopers required to visit all the intersections in the town.

# Sample Input

**2433 41 32 3331 31 22 3**

# Sample Output

**21**


题意： t 组数据，n 个点 m 条边，通过点-点的形式表示一条街道，问如何放置最少的人到交叉路口中，使得所有人能在不重复走同样交点的条件下，使所有的人遍历整个城市的所有点

思路：根据题意可以建立一个 DAG 图，对于 DAG 中的 i-j 边，在二分图中建立左 i 右 j 边，每个人走的是一条有向简单路径，要求的就是最少可以用多少路径覆盖所有点且任意两条路径间没有重复的点，实质就是最小路径覆盖问题

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
int hungarian(int x){

    int ans=0;
    for(int i=1;i<=x;i++){
        memset(vis,false,sizeof(vis));
        if(dfs(i))
            ans++;
    }
    return ans;
}
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        int n,m;
        scanf("%d%d",&n,&m);

        memset(link,-1,sizeof(link));
        for(int i=0;i<=n;i++)
            G[i].clear();

        while(m--){
            int x,y;
            scanf("%d%d",&x,&y);
            G[x].push_back(y);
        }

        printf("%d\n",n-hungarian(n));
    }
    return 0;
}
```





