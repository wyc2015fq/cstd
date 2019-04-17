# poj3660——Cow Contest - westbrook1998的博客 - CSDN博客





2018年08月19日 19:42:28[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：24标签：[floyd																[最短路																[传递闭包](https://so.csdn.net/so/search/s.do?q=传递闭包&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)





> 
N (1 ≤ N ≤ 100) cows, conveniently numbered 1..N, are participating in a programming contest. As we all know, some cows code better than others. Each cow has a certain constant skill rating that is unique among the competitors. 

  The contest is conducted in several head-to-head rounds, each between two cows. If cow A has a greater skill level than cow B (1 ≤ A ≤ N; 1 ≤ B ≤ N; A ≠ B), then cow A will always beat cow B. 

  Farmer John is trying to rank the cows by skill level. Given a list the results of M (1 ≤ M ≤ 4,500) two-cow rounds, determine the number of cows whose ranks can be precisely determined from the results. It is guaranteed that the results of the rounds will not be contradictory. 

  Input 

  * Line 1: Two space-separated integers: N and M 

  * Lines 2..M+1: Each line contains two space-separated integers that describe the competitors and results (the first integer, A, is the winner) of a single round of competition: A and B 

  Output 

  * Line 1: A single integer representing the number of cows whose ranks can be determined 

  Sample Input 

  5 5 

  4 3 

  4 2 

  3 2 

  1 2 

  2 5 

  Sample Output 

  2
一开始以为是拓扑排序 百度了发现是用floyd求传递闭包，也就是先求出所有点之间的通路，然后如果一个点和其他的点都有通路的话，他的位置就能确定了

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=105;
int g[N][N];
int n,m;
int u,v;
int main(void){
    scanf("%d%d",&n,&m);
    memset(g,0,sizeof(g));
    while(m--){
        scanf("%d%d",&u,&v);
        g[u][v]=1;
    }
    for(int k=1;k<=n;k++){
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                if(g[i][k] && g[k][j]){
                    g[i][j]=1;
                }
            }
        }
    }
    int ans=0;
    int j;
    for(int i=1;i<=n;i++){
        for(j=1;j<=n;j++){
            if(i==j){
                continue;
            }
            if(!g[i][j] && !g[j][i]){
                break;
            }
        }
        if(j>n){
            ans++;
        }
    }
    printf("%d\n",ans);
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=最短路&t=blog)](https://so.csdn.net/so/search/s.do?q=floyd&t=blog)




