# Building roads（POJ-2749） - Alex_McAvoy的博客 - CSDN博客





2018年12月17日 17:15:48[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：62
个人分类：[POJ																[图论——2-SAT](https://blog.csdn.net/u011815404/article/category/8348292)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Farmer John's farm has N barns, and there are some cows that live in each barn. The cows like to drop around, so John wants to build some roads to connect these barns. If he builds roads for every pair of different barns, then he must build N * (N - 1) / 2 roads, which is so costly that cheapskate John will never do that, though that's the best choice for the cows. 

Clever John just had another good idea. He first builds two transferring point S1 and S2, and then builds a road connecting S1 and S2 and N roads connecting each barn with S1 or S2, namely every barn will connect with S1 or S2, but not both. So that every pair of barns will be connected by the roads. To make the cows don't spend too much time while dropping around, John wants to minimize the maximum of distances between every pair of barns. 

That's not the whole story because there is another troublesome problem. The cows of some barns hate each other, and John can't connect their barns to the same transferring point. The cows of some barns are friends with each other, and John must connect their barns to the same transferring point. What a headache! Now John turns to you for help. Your task is to find a feasible optimal road-building scheme to make the maximum of distances between every pair of barns as short as possible, which means that you must decide which transferring point each barn should connect to. 

We have known the coordinates of S1, S2 and the N barns, the pairs of barns in which the cows hate each other, and the pairs of barns in which the cows are friends with each other. 

Note that John always builds roads vertically and horizontally, so the length of road between two places is their Manhattan distance. For example, saying two points with coordinates (x1, y1) and (x2, y2), the Manhattan distance between them is |x1 - x2| + |y1 - y2|. 

# **Input**

The first line of input consists of 3 integers N, A and B (2 <= N <= 500, 0 <= A <= 1000, 0 <= B <= 1000), which are the number of barns, the number of pairs of barns in which the cows hate each other and the number of pairs of barns in which the cows are friends with each other. 

Next line contains 4 integer sx1, sy1, sx2, sy2, which are the coordinates of two different transferring point S1 and S2 respectively. 

Each of the following N line contains two integer x and y. They are coordinates of the barns from the first barn to the last one. 

Each of the following A lines contains two different integers i and j(1 <= i < j <= N), which represent the i-th and j-th barns in which the cows hate each other. 

The same pair of barns never appears more than once. 

Each of the following B lines contains two different integers i and j(1 <= i < j <= N), which represent the i-th and j-th barns in which the cows are friends with each other. The same pair of barns never appears more than once. 

You should note that all the coordinates are in the range [-1000000, 1000000]. 

# Output

You just need output a line containing a single integer, which represents the maximum of the distances between every pair of barns, if John selects the optimal road-building scheme. Note if there is no feasible solution, just output -1.

# Sample Input

**4 1 112750 28546 15361 320556706 388710754 816612668 1938015788 160593 42 3**

# Sample Output

**53246**


题意：有 n 个农场与 2 个集合点 s1、s2 现在要在 s1、s2 间连接一条路，然后把每个农场与 s1或 s2 间连接一条路，有一些农场因为相互憎恨因此不能连接到一个集合点，有些农场相互喜欢必须要连接到集合点，输出所有可行方案中使任意两个农场的最大距离最小的值，若没有可行方案输出 -1

思路：

根据题意，要求最大距离最小的值，可以判断需要使用二分来做，然后对于两个集合点，每个农场只有选择连或不连，而且必须选择一个集合点连接，连接一个就不能连接另一个，满足 2-SAT 的条件，因此此题可以在二分时不断使用 2-SAT 建图进行判断是否满足条件，以此来求最大距离最小值

首先，根据题意，先添加初始条件，设 a、b 为任意两农场，0 为连接到集合点 S1，1 为连接到集合点 S2，则：

对于 a、b 不能连接一个集合点的有：
- a 连接 S1 导致 b 连接 S2：(a,0,b,1)
- a 连接 S2 导致 b 连接 S1：(a,1,b,0)
- b 连接 S1 导致 a 连接 S2：(b,0,a,1)
- b 连接 S2 导致 a 连接 S1：(b,1,a,0)

对于 a、b 必须连接一个集合点的有：
- a 连接 S1 导致 b 连接 S1：(a,0,b,0)
- a 连接 S2 导致 b 连接 S2：(a,1,b,1)
- b 连接 S1 导致 a 连接 S1：(b,0,a,0)
- b 连接 S2 导致 a 连接 S2：(b,1,a,1)

添加好限制条件后，对最大距离的值 mid 进行二分，看任意两点距离小于等于 mid 时，是否有可行的解，若大于 mid 说明不具有可行的解，据此来添加新的限制条件：
- 若 a 与 b 同时连接到 S1，他们的距离 > mid，说明 a 与 b 不能同时连接，即：(a,0,b,1) 且 (b,0,a,1)
- 若 a 与 b 同时连接到 S2，他们的距离 > mid，说明 a 与 b 不能同时连接，即：(a,1,b,0) 且 (b,1,a,0)
- 若 a 连接 S1，b 连接 S2，他们的距离 > mid，说明 a 与 b 需要同时连接，即：(a,0,b,0) 且 (a,1,b,1)
- 若 a 连接 S2，b 连接 S1，他们的距离 > mid，说明 a 与 b 需要同时连接，即：(a,1,b,1) 且 (a,0,b,0)

再根据新的限制条件直接进行判断有无解即可

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
bool vis[N*2];
int Stack[N*2],top;
vector<int> G[N*2];
void init(int n){
    memset(vis,false,sizeof(vis));
    for(int i=0;i<2*n;i++)
        G[i].clear();
}
void addOrClause(int x,int xVal,int y,int yVal){
    x=x*2+xVal;
    y=y*2+yVal;
    G[x^1].push_back(y);
    G[y^1].push_back(x);
}
void addAndClause(int x,int xval,int y,int yval) {
    x=x*2+xval;
    y=y*2+yval;
    G[x].push_back(y);
}
bool dfs(int x){
    if(vis[x^1])
        return false;

    if(vis[x])
        return true;
    vis[x]=true;

    Stack[top++]=x;

    for(int i=0;i<G[x].size();i++)
        if(!dfs(G[x][i]))
            return false;

    return true;
}
bool twoSAT(int n){
    for(int i=0;i<2*n;i+=2){
        if(!vis[i] && !vis[i+1]){
            top=0;

            if(!dfs(i)){
                while(top>0)
                    vis[Stack[--top]]=false;

                if(!dfs(i+1))
                    return false;
            }
        }
    }
    return true;
}
struct Node{
    int x,y;
    Node(){}
    Node(int x,int y):x(x),y(y){}
};
Node s[2];//两个集合点
Node node[N];//n个农场
int sLen;//集合点的距离
int a[N][2];//相互讨厌
int b[N][2];//相互喜欢
int len(int i,int j){//第i个点到第j个集合点的哈夫曼距离
    return fabs(node[i].x-s[j].x)+fabs(node[i].y-s[j].y);
}
int n,p,q;
bool judge(int mid){
    init(n);
    //初始条件
    for(int i=0;i<p;i++){//相互喜欢
       addAndClause(a[i][0],0,a[i][1],1);
       addAndClause(a[i][0],1,a[i][1],0);
       addAndClause(a[i][1],0,a[i][0],1);
       addAndClause(a[i][1],1,a[i][0],0);
    }
    for(int i=0;i<q;i++){//相互讨厌
       addAndClause(b[i][0],0,b[i][1],0);
       addAndClause(b[i][0],1,b[i][1],1);
       addAndClause(b[i][1],0,b[i][0],0);
       addAndClause(b[i][1],1,b[i][0],1);
    }

    //限制条件
    for(int i=0;i<n;i++){
       for(int j=i+1;j<n;j++){
           if(len(i,0)+len(j,0)>mid){//i、j都连接到S1
               addAndClause(i,0,j,1);
               addAndClause(j,0,i,1);
           }
           if(len(i,1)+len(j,1)>mid){//i、j都连接到S2
               addAndClause(i,1,j,0);
               addAndClause(j,1,i,0);
           }
           if(len(i,0)+len(j,1)+sLen>mid){//i连接到S1，j连接到S2
               addAndClause(i,0,j,0);
               addAndClause(j,1,i,1);
           }
           if(len(i,1)+len(j,0)+sLen>mid){//i连接到S2，j连接到S1
               addAndClause(i,1,j,1);
               addAndClause(j,0,i,0);
           }
       }
    }

    twoSAT(n);
}
int main(){

    while(scanf("%d%d%d",&n,&p,&q)!=EOF&&(n+p+q)){

        scanf("%d%d%d%d",&s[0].x,&s[0].y,&s[1].x,&s[1].y);
        sLen=fabs(s[0].x-s[1].x)+abs(s[0].y-s[1].y);//两集合点距离

        int minn=INF,maxx=-INF;
        for(int i=0;i<n;i++){
            scanf("%d%d",&node[i].x,&node[i].y);
            minn=min(minn,min(len(i,0)*2,len(i,1)*2));//最小距离
            maxx=max(maxx,max(len(i,0)*2+sLen,len(i,1)*2+sLen));//最大距离
        }

        for(int i=0;i<p;i++){//相互喜欢
            int x,y;
            scanf("%d%d",&x,&y);
            a[i][0]=x-1;
            a[i][1]=y-1;
        }
        for(int i=0;i<q;i++){//相互讨厌
            int x,y;
            scanf("%d%d",&x,&y);
            b[i][0]=x-1;
            b[i][1]=y-1;
        }

        int left=minn,right=maxx;
        if(!judge(right))
            printf("-1\n");
        else{
            while(left<right){
                int mid=left+(right-left)/2;
                if(judge(mid))
                    right=mid;
                else
                    left=mid+1;
            }
            printf("%d\n",right);
        }

    }

    return 0;
}
```






