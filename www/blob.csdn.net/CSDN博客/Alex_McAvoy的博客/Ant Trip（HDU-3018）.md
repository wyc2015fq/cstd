# Ant Trip（HDU-3018） - Alex_McAvoy的博客 - CSDN博客





2019年02月24日 17:01:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：170








> 
# Problem Description

Ant Country consist of N towns.There are M roads connecting the towns. 

Ant Tony,together with his friends,wants to go through every part of the country. 

They intend to visit every road , and every road must be visited for exact one time.However,it may be a mission impossible for only one group of people.So they are trying to divide all the people into several groups,and each may start at different town.Now tony wants to know what is the least groups of ants that needs to form to achieve their goal. 

# **Input**

Input contains multiple cases.Test cases are separated by several blank lines. Each test case starts with two integer N(1<=N<=100000),M(0<=M<=200000),indicating that there are N towns and M roads in Ant Country.Followed by M lines,each line contains two integers a,b,(1<=a,b<=N) indicating that there is a road connecting town a and town b.No two roads will be the same,and there is no road connecting the same town.

# Output

For each test case ,output the least groups that needs to form to achieve their goal.

# Sample Input

**3 31 22 31 3**

**4 21 23 4**

# Sample Output

**12**


题意：每组数据有 n 个城市 m 条无向路，现在将所有蚂蚁分成若干个小组，小组开始可以在不同的城镇，要求访问每一条道路，求最小的小组数

思路：本质上是一个一笔画问题，要求问最少画几笔能走完所有路，因此直接求欧拉图的个数与连通图中奇数点的个数/2的和即可

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
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

int n,m;
int degree[N];
int father[N];
int vis[N];
int Find(int x){
    if(father[x]==x)
        return x;
    return father[x]=Find(father[x]);
}
void Union(int x,int y){
    x=Find(x);
    y=Find(y);
    if(x!=y)
        father[x]=y;
}
int main(){

    while(scanf("%d%d",&n,&m)!=EOF){
        memset(vis,0,sizeof(vis));
        memset(degree,0,sizeof(degree));
        for(int i=1;i<=n;i++)
            father[i]=i;


        for(int i=1;i<=m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            degree[x]++;
            degree[y]++;
            Union(x,y);
        }

        int num=0;
        for(int i=1;i<=n;i++){
            if(degree[i]%2){//点的度数为奇数
                num++;
                vis[Find(i)]=1;//标记父节点
            }
        }
        num/=2;//奇度数点除以2

        for(int i=1;i<=n;i++){
            if(degree[i]!=0){//不考虑单纯的点
                if(!vis[Find(i)]&&Find(i)==i)//能形成欧拉回路且奇度数不相连
                    num++;
            }
        }
        printf("%d\n",num);
    }
    return 0;
}
```





