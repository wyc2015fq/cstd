# hdu1102——Constructing Roads - westbrook1998的博客 - CSDN博客





2018年08月14日 15:45:30[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：23标签：[最小生成树](https://so.csdn.net/so/search/s.do?q=最小生成树&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
There are N villages, which are numbered from 1 to N, and you should build some roads such that every two villages can connect to each other. We say two village A and B are connected, if and only if there is a road between A and B, or there exists a village C such that there is a road between A and C, and C and B are connected.  

  We know that there are already some roads between some villages and your job is the build some roads such that all the villages are connect and the length of all the roads built is minimum.  

  Input 

  The first line is an integer N (3 <= N <= 100), which is the number of villages. Then come N lines, the i-th of which contains N integers, and the j-th of these N integers is the distance (the distance should be an integer within [1, 1000]) between village i and village j.  

  Then there is an integer Q (0 <= Q <= N * (N + 1) / 2). Then come Q lines, each line contains two integers a and b (1 <= a < b <= N), which means the road between village a and village b has been built.  

  Output 

  You should output a line contains an integer, which is the length of all the roads to be built such that all the villages are connected, and this value is minimum.  

  Sample Input 

  3 

  0 990 692 

  990 0 179 

  692 179 0 

  1 

  1 2 

  Sample Output 

  179
居然是多组测试样例….

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=105;
const int INF=0x3f3f3f3f;
int g[N][N];
bool vis[N];
int low[N];
int n,q,a,b;
int prim(){
    int ans=0;
    int Min=INF;
    int pos=1;
    vis[pos]=true;
    for(int i=1;i<=n;i++){
        if(i!=pos){
            low[i]=g[pos][i];
        }
    }
    for(int i=1;i<n;i++){
        Min=INF;
        for(int j=1;j<=n;j++){
            if(!vis[j] && low[j]<Min){
                Min=low[j];
                pos=j;
            }
        }
        if(Min==INF){
            break;
        }
        vis[pos]=true;
        ans+=Min;
        for(int j=1;j<=n;j++){
            if(!vis[j] && low[j]>g[pos][j]){
                low[j]=g[pos][j];
            }
        }
    }
    return ans;
}
int main(void){
    while(~scanf("%d",&n)){
        memset(g,INF,sizeof(g));
        memset(vis,false,sizeof(vis));
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                scanf("%d",&g[i][j]);
            }
        }
        scanf("%d",&q);
        while(q--){
            scanf("%d%d",&a,&b);
            g[a][b]=g[b][a]=0;
        }
        int ans=prim();
        printf("%d\n",ans);
    }
    return 0;
}
```





