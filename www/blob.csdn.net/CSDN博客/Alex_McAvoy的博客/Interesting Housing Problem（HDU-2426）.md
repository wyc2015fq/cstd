# Interesting Housing Problem（HDU-2426） - Alex_McAvoy的博客 - CSDN博客





2018年12月06日 22:11:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：48








> 
# Problem Description

For any school, it is hard to find a feasible accommodation plan with every student assigned to a suitable apartment while keeping everyone happy, let alone an optimal one. Recently the president of University ABC, Peterson, is facing a similar problem. While Peterson does not like the idea of delegating the task directly to the class advisors as so many other schools are doing, he still wants to design a creative plan such that no student is assigned to a room he/she dislikes, and the overall quality of the plan should be maximized. Nevertheless, Peterson does not know how this task could be accomplished, so he asks you to solve this so-called "interesting" problem for him.

Suppose that there are N students and M rooms. Each student is asked to rate some rooms (not necessarily all M rooms) by stating how he/she likes the room. The rating can be represented as an integer, positive value meaning that the student consider the room to be of good quality, zero indicating neutral, or negative implying that the student does not like living in the room. Note that you can never assign a student to a room which he/she has not rated, as the absence of rating indicates that the student cannot live in the room for other reasons.

With limited information available, you've decided to simply find an assignment such that every student is assigned to a room he/she has rated, no two students are assigned to the same room, and the sum of rating is maximized while satisfying Peterson's requirement. The question is … what exactly is the answer?

# **Input**

There are multiple test cases in the input file. Each test case begins with three integers, N, M, and E (1 <= N <= 500, 0 <= M <= 500, 0 <= E <= min(N * M, 50000)), followed by E lines, each line containing three numbers, Si, Ri, Vi, (0 <= Si < N, 0 <= Ri < M, |Vi| <= 10000), describing the rating Vi given by student Si for room Ri. It is guaranteed that each student will rate each room at most once.

Each case is followed by one blank line. Input ends with End-of-File.

# Output

For each test case, please output one integer, the requested value, on a single line, or -1 if no solution could be found. Use the format as indicated in the sample output.

# Sample Input

**3 5 50 1 50 2 71 1 61 2 32 4 5**

**1 1 10 0 0**

**1 1 0**

# Sample Output

**Case 1: 18Case 2: 0Case 3: -1**


题意：有 n 个学生 m 个房间 e 组打分，每组打分代表学生 x 对房间 y 打分 w，如果分数大于等于 0，表示他可能会选择这个房间，如果一个房间分数小于 0 或者没打过分，则不能选择，现在问是否存在一个匹配，使得每个学生分得一个房间，每个房间最多一个学生，并且这些房间都是学生打过分且分数大于 0 的房间，若存在，则输出分数和的最大值，若不存在，输出 -1

思路：首先，当学生数大于房间数时，直接输出 -1，然后选取学生为左点集房间为右点集，对学生 i 打过分的房间 j 建一条边，边的权值是学生打的分数，若对一个房间，某学生打的分数小于 0 或者其为对这个房间打过分，则设这条边的权值为负无穷，那么当求最优匹配的时候，总权值即为最优匹配的权值解，若存在某条边的权值是负无穷，则说明问题无解

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
int n,m;
int G[N][N];
int Lx[N],Ly[N];
bool visX[N],visY[N];
int linkX[N],linkY[N];
bool dfs(int x){
    visX[x]=true;
    for(int y=1;y<=m;y++){
        if(!visY[y]){
            int temp=Lx[x]+Ly[y]-G[x][y];
            if(temp==0){
                visY[y]=true;
                if(linkY[y]==-1 || dfs(linkY[y])){
                    linkX[x]=y;
                    linkY[y]=x;
                    return true;
                }
            }
        }
    }
    return false;
}
void update(){
    int minn=INF;
    for(int i=1;i<=n;i++)
        if(visX[i])
            for(int j=1;j<=m;j++)
                if(!visY[j])
                    minn=min(minn,Lx[i]+Ly[j]-G[i][j]);

    for(int i=1;i<=n;i++)
        if(visX[i])
            Lx[i]-=minn;

    for(int i=1;i<=m;i++)
        if(visY[i])
            Ly[i]+=minn;
}
int KM(){
    memset(linkX,-1,sizeof(linkX));
    memset(linkY,-1,sizeof(linkY));

    memset(Lx,0,sizeof(Lx));
    memset(Ly,0,sizeof(Ly));
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            Lx[i]=max(Lx[i],G[i][j]);

    for(int i=1;i<=n;i++){
        while(true){
            memset(visX,false,sizeof(visX));
            memset(visY,false,sizeof(visY));

            if(dfs(i))
                break;
            else
                update();
        }
    }

    int ans=0;
    for(int i=1;i<=m;i++){
        if(linkY[i]!=-1){
            if(G[linkY[i]][i]!=-INF)
                ans+=G[linkY[i]][i];
            else
                ans=-1;
        }
    }
    return ans;
}
int main(){
    int e;
    int Case=1;
    while(scanf("%d%d%d",&n,&m,&e)!=EOF&&(n+m+e)){
        if(n>m)
            printf("Case %d: %d\n",Case++,-1);
        else{
            for(int i=1;i<=n;i++)
                for(int j=1;j<=m;j++)
                    G[i][j]=-INF;

            while(e--){
                int x,y,w;
                scanf("%d%d%d",&x,&y,&w);
                x++,y++;
                if(w>=0)
                    G[x][y]=w;
            }

            printf("Case %d: %d\n",Case++,KM());
        }
    }
    return 0;
}
```





