# poj3169——Layout - westbrook1998的博客 - CSDN博客





2018年08月19日 19:51:20[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：28








> 
Like everyone else, cows like to stand close to their friends when queuing for feed. FJ has N (2 <= N <= 1,000) cows numbered 1..N standing along a straight line waiting for feed. The cows are standing in the same order as they are numbered, and since they can be rather pushy, it is possible that two or more cows can line up at exactly the same location (that is, if we think of each cow as being located at some coordinate on a number line, then it is possible for two or more cows to share the same coordinate).  

  Some cows like each other and want to be within a certain distance of each other in line. Some really dislike each other and want to be separated by at least a certain distance. A list of ML (1 <= ML <= 10,000) constraints describes which cows like each other and the maximum distance by which they may be separated; a subsequent list of MD constraints (1 <= MD <= 10,000) tells which cows dislike each other and the minimum distance by which they must be separated.  

  Your job is to compute, if possible, the maximum possible distance between cow 1 and cow N that satisfies the distance constraints. 

  Input 

  Line 1: Three space-separated integers: N, ML, and MD.  

  Lines 2..ML+1: Each line contains three space-separated positive integers: A, B, and D, with 1 <= A < B <= N. Cows A and B must be at most D (1 <= D <= 1,000,000) apart.  

  Lines ML+2..ML+MD+1: Each line contains three space-separated positive integers: A, B, and D, with 1 <= A < B <= N. Cows A and B must be at least D (1 <= D <= 1,000,000) apart. 

  Output 

  Line 1: A single integer. If no line-up is possible, output -1. If cows 1 and N can be arbitrarily far apart, output -2. Otherwise output the greatest possible distance between cows 1 and N. 

  Sample Input 

  4 2 1 

  1 3 10 

  2 4 20 

  2 3 3 

  Sample Output 

  27 

  Hint 

  Explanation of the sample:  

  There are 4 cows. Cows #1 and #3 must be no more than 10 units apart, cows #2 and #4 must be no more than 20 units apart, and cows #2 and #3 dislike each other and must be no fewer than 3 units apart.  

  The best layout, in terms of coordinates on a number line, is to put cow #1 at 0, cow #2 at 7, cow #3 at 10, and cow #4 at 27.
这题一开始也没思路 查了一下发现是差分约束转化为最短路的问题 

也就是两个节点之间的边权不是一个确定的数 而是一个不等式，但是我们如果把这些不等关系都转成<= 然后求最短路 就相当于求出在约束条件下的最大可能距离了（因为最短路是从无穷大向下约束的，所以最后肯定得到符合<=条件的最大值） 

然后如果有负环就无解 解为INF就说明任意位置
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <queue>
using namespace std;
const int N=1005;
const int INF=0x3f3f3f3f;
int g[N][N];
int n,ml,md;
int dis[N];
int vis[N];
int u,v,w;
int inque[N];
bool flag;
int spfa(int s){
    for(int i=1;i<=n;i++){
        dis[i]=INF;
    }
    vis[s]=true;
    dis[s]=0;
    queue<int> q;
    q.push(s);
    inque[s]=1;
    while(!q.empty()){
        int cur=q.front();
        q.pop();
        vis[cur]=false;
        for(int i=1;i<=n;i++){
            if(g[cur][i]==INF){
                continue;
            }
            if(dis[cur]+g[cur][i]<dis[i]){
                dis[i]=dis[cur]+g[cur][i];
                if(!vis[i]){
                    q.push(i);
                    vis[i]=true;
                    inque[i]++;
                    if(inque[i]>n){
                        flag=true;
                        break;
                    }
                }
            }
        }
        if(flag){
            break;
        }
    }

    //负环 无解
    if(flag){
        return -1;
    }
    //最短路为INF 任意摆放位置
    else if(dis[n]==INF){
        return -2;
    }
    else{
        return dis[n];
    }
}
int main(void){
    //freopen("data.txt","r",stdin);
    scanf("%d%d%d",&n,&ml,&md);
    memset(g,INF,sizeof(g));
    //差分约束 求最大可能距离 转换为<=的不等式组
    //同理 若求最小可能距离 >=求最长路
    //按顺序摆放
    //S[i]-S[i-1]>=0 S[i-1]-S[i]<=0
    for(int i=1;i<=n;i++){
        g[i][i-1]=0;
    }
    //B-A<=X
    while(ml--){
        scanf("%d%d%d",&u,&v,&w);
        g[u][v]=w;
    }
    //D-C>=Y 即C-D<=-Y
    while(md--){
        scanf("%d%d%d",&u,&v,&w);
        g[v][u]=-w;
    }
    printf("%d\n",spfa(1));
    return 0;
}
```






