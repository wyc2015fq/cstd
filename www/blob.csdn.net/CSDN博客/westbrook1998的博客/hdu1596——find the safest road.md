# hdu1596——find the safest road - westbrook1998的博客 - CSDN博客





2018年08月07日 10:15:12[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：34








> 
XX星球有很多城市，每个城市之间有一条或多条飞行通道，但是并不是所有的路都是很安全的，每一条路有一个安全系数s,s是在 0 和 1 间的实数(包括0，1)，一条从u 到 v 的通道P 的安全度为Safe(P) = s(e1)*s(e2)…*s(ek) e1,e2,ek是P 上的边 ，现在8600 想出去旅游，面对这这么多的路，他想找一条最安全的路。但是8600 的数学不好，想请你帮忙 ^_^ 

  Input 

  输入包括多个测试实例，每个实例包括：  

  第一行：n。n表示城市的个数n<=1000;  

  接着是一个n*n的矩阵表示两个城市之间的安全系数，(0可以理解为那两个城市之间没有直接的通道)  

  接着是Q个8600要旅游的路线,每行有两个数字，表示8600所在的城市和要去的城市 

  Output 

  如果86无法达到他的目的地，输出”What a pity!”,  

  其他的输出这两个城市之间的最安全道路的安全系数,保留三位小数。 

  Sample Input 

  3 

  1 0.5 0.5 

  0.5 1 0.4 

  0.5 0.4 1 

  3 

  1 2 

  2 3 

  1 3 

  Sample Output 

  0.500 

  0.400 

  0.500
最短路的简单变形 把INF换成0 把min换成max 

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=1020;
const int INF=0x3f3f3f3f;
double m[MAXN][MAXN];
int n,q,s,t;
void floyd(){
    for(int k=0;k<n;k++){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                m[i][j]=max(m[i][j],m[i][k]*m[k][j]);
                //printf("%d %d %lf\n",i,j,m[i][j]);
            }
        }
    }
}
int main(void){
    while(~scanf("%d",&n)){
        memset(m,0.0,sizeof(m));
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                scanf("%lf",&m[i][j]);
            }
        }
        floyd();
        scanf("%d",&q);
        while(q--){
            scanf("%d%d",&s,&t);
            if(m[s-1][t-1]==0){
                printf("What a pity!\n");
            }
            else{
                printf("%.3lf\n",m[s-1][t-1]);
            }
        }
    }
    return 0;
}
```





