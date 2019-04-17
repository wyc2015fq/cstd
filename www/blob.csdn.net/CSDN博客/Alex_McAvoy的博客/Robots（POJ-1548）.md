# Robots（POJ-1548） - Alex_McAvoy的博客 - CSDN博客





2018年12月03日 22:03:04[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：52
个人分类：[POJ																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Your company provides robots that can be used to pick up litter from fields after sporting events and concerts. Before robots are assigned to a job, an aerial photograph of the field is marked with a grid. Each location in the grid that contains garbage is marked. All robots begin in the Northwest corner and end their movement in the Southeast corner. A robot can only move in two directions, either to the East or South. Upon entering a cell that contains garbage, the robot will pick it up before proceeding. Once a robot reaches its destination at the Southeast corner it cannot be repositioned or reused. Since your expenses are directly proportional to the number of robots used for a particular job, you are interested in finding the minimum number of robots that can clean a given field. For example, consider the field map shown in Figure 1 with rows and columns numbered as shown and garbage locations marked with a 'G'. In this scheme, all robots will begin in location 1,1 and end in location 6, 7. 

![](https://img-blog.csdnimg.cn/20181203215354568.jpg)


Figure 1 - A Field Map

Figure 2 below shows two possible solutions, the second of which is preferable since it uses two robots rather than three. 

![](https://img-blog.csdnimg.cn/2018120321540776.jpg)


Figure 2 - Two Possible Solutions

Your task is to create a program that will determine the minimum number of robots needed to pick up all the garbage from a field. 

# **Input**

The input consists of one or more field maps followed by a line containing -1 -1 to signal the end of the input data. A field map consists of one or more lines, each containing one garbage location, followed by a line containing 0 0 to signal the end of the map. Each garbage location consists of two integers, the row and column, separated by a single space. The rows and columns are numbered as shown in Figure 1. The garbage locations will be given in row-major order. No single field map will have more than 24 rows and 24 columns. The sample input below shows an input file with two field maps. The first is the field map from Figure 1.

# Output

The output will consist of a single line for each field map containing the minimum number of robots needed to clean the corresponding field.

# Sample Input

**1 21 42 42 64 44 76 60 01 12 24 40 0-1 -1**

# Sample Output

**21**


题意： 给出一个矩阵，从最左上到最右下，且只能向下或向右走，在矩阵中的一些格子中含有一个 G，问最少派多少机器人才能把所有的 G 都走完

思路：对矩阵中的任意两个 Gi、Gj，若 i 的行、列均小于 j 的行、列，那么就一定有一条从 i 到 j 的边，最终一定会形成一个 DAG 图，现在需要在 DAG 图中选择最小的简单路径来覆盖所有的 G 点，也就是求最小路径覆盖数

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
struct Node{
    int x,y;
    bool judge(Node &rhs){
        return (x<=rhs.x&&y<=rhs.y);
    }
}a[N];
int main(){
    int x,y;
    while(scanf("%d%d",&x,&y)!=EOF){
        if(x==-1&&y==-1)
            break;
        if(x==0&&y==0)
            continue;

        int cnt=1;
        a[cnt].x=x;
        a[cnt].y=y;
        while(scanf("%d%d",&x,&y)!=EOF&&(x+y)){
            cnt++;
            a[cnt].x=x;
            a[cnt].y=y;
        }

        memset(link,-1,sizeof(link));
        for(int i=0;i<=cnt;i++)
            G[i].clear();

        for(int i=1;i<=cnt;i++){
            for(int j=1;j<=cnt;j++){
                if(i!=j){
                    if(a[i].judge(a[j])){
                        G[i].push_back(j);
                    }
                }
            }
        }

        printf("%d\n",cnt-hungarian(cnt));
    }
    return 0;
}
```






