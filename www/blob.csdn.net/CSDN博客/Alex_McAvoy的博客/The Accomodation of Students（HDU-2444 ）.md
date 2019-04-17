# The Accomodation of Students（HDU-2444 ） - Alex_McAvoy的博客 - CSDN博客





2018年11月27日 00:29:13[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：26
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

There are a group of students. Some of them may know each other, while others don't. For example, A and B know each other, B and C know each other. But this may not imply that A and C know each other.

Now you are given all pairs of students who know each other. Your task is to divide the students into two groups so that any two students in the same group don't know each other.If this goal can be achieved, then arrange them into double rooms. Remember, only paris appearing in the previous given set can live in the same room, which means only known students can live in the same room.

Calculate the maximum number of pairs that can be arranged into these double rooms.

# **Input**

Proceed to the end of file.

For each data set:

The first line gives two integers, n and m(1<n<=200), indicating there are n students and m pairs of students who know each other. The next m lines give such pairs.

# Output

If these students cannot be divided into two groups, print "No". Otherwise, print the maximum number of pairs that can be arranged in those rooms.

# Sample Input

**4 41 21 31 42 36 51 21 31 42 53 6**

# Sample Output

**No3**


题意：给出 n 个学生 m 对关系，每对关系形如（i,j）表示第 i 个学生与第 j 个学生相互认识，现要将 n 个学生分为两组，每组中的任意两个学生都不相互认识，如果能分组的话，就将两个人放到一个房间中，求最多需要的房间数

思路：第一问实质是要判断图能否二分，第二问则是求如果能分组的情况下的最大匹配数，由于重新编号再计算最大匹配较为繁琐，因此可以将原图翻倍处理，计算出翻倍后的最大匹配数再除以二即可

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
#define N 10001
#define LL long long
using namespace std;
bool vis[N];
int link[N];
int color[N];
vector<int> G[N];
bool dfs(int x){
    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];
        if(!vis[y]){
            vis[y]=true;
            if(link[y]==-1 || dfs(link[y])){
                link[y]=x;
                return true;
            }
        }
    }
    return false;
}
bool bipartite(int x){
    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];
        if(color[y]==color[x])
            return false;
        else if(color[y]==0){
            color[y]=3-color[x];
            if(!bipartite(y))
                return false;
        }
    }
    return true;
}
int hungarian(int n)
{
    memset(color,0,sizeof(color));
    for(int i=1;i<=n;i++){
        if(!color[i]){
            color[i]=1;
            if(!bipartite(i))
                return -1;
        }
    }


    int ans=0;
    for(int i=1;i<=n;i++){
        memset(vis,false,sizeof(vis));
        if(dfs(i))
            ans++;
    }
    return ans;
}
int main(){
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        memset(link,-1,sizeof(link));
        for(int i=0;i<N;i++)
            G[i].clear();

        while(m--){
            int x,y;
            scanf("%d%d",&x,&y);
            G[x].push_back(y);
            G[y].push_back(x);
        }

        int res=hungarian(n);
        if(res==-1)
            printf("No\n");
        else
            printf("%d\n",res/2);
    }
    return 0;
}
```






