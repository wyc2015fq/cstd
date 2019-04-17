# poj2236——Wireless Network - westbrook1998的博客 - CSDN博客





2018年08月15日 21:51:56[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：29标签：[并查集](https://so.csdn.net/so/search/s.do?q=并查集&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
An earthquake takes place in Southeast Asia. The ACM (Asia Cooperated Medical team) have set up a wireless network with the lap computers, but an unexpected aftershock attacked, all computers in the network were all broken. The computers are repaired one by one, and the network gradually began to work again. Because of the hardware restricts, each computer can only directly communicate with the computers that are not farther than d meters from it. But every computer can be regarded as the intermediary of the communication between two other computers, that is to say computer A and computer B can communicate if computer A and computer B can communicate directly or there is a computer C that can communicate with both A and B.  

  In the process of repairing the network, workers can take two kinds of operations at every moment, repairing a computer, or testing if two computers can communicate. Your job is to answer all the testing operations.  

  Input 

  The first line contains two integers N and d (1 <= N <= 1001, 0 <= d <= 20000). Here N is the number of computers, which are numbered from 1 to N, and D is the maximum distance two computers can communicate directly. In the next N lines, each contains two integers xi, yi (0 <= xi, yi <= 10000), which is the coordinate of N computers. From the (N+1)-th line to the end of input, there are operations, which are carried out one by one. Each line contains an operation in one of following two formats:  

  1. “O p” (1 <= p <= N), which means repairing computer p.  

  2. “S p q” (1 <= p, q <= N), which means testing whether computer p and q can communicate.  

  The input will not exceed 300000 lines.  

  Output 

  For each Testing operation, print “SUCCESS” if the two computers can communicate, or “FAIL” if not. 

  Sample Input 

  4 1 

  0 1 

  0 2 

  0 3 

  0 4 

  O 1 

  O 2 

  O 4 

  S 1 4 

  O 3 

  S 1 4 

  Sample Output 

  FAIL 

  SUCCESS
这题无话可说，莫名其妙的tle 莫名其妙的ac 明明一个逗号都没改过

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <string>
#include <iostream>
#define _clr(x,a) memset(x,a,sizeof(x));
using namespace std;
const int N=1205;
int n,d;
bool g[N][N];
double x[N];
double y[N];
int fa[N];
bool rep[N];
int find(int x){
    return fa[x]==x ? x : fa[x]=find(fa[x]);
}
void join(int a,int b){
    int ta=find(a);
    int tb=find(b);
    if(ta!=tb){
        fa[tb]=ta;
    }
}
double dis(int i,int j){
    return sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]))<=d;
}
int main(void){
    //freopen("data.txt","r",stdin);
    scanf("%d%d",&n,&d);
    for(int i=1;i<=n;i++){
        fa[i]=i;
        scanf("%lf%lf",&x[i],&y[i]);
    }
    for(int i=1;i<n;i++){
        for(int j=i+1;j<=n;j++){
            g[i][j]=g[j][i]=dis(i,j);
        }
    }
    getchar();
    char q;
    int a,b;
    while(~scanf("%c%d",&q,&a)){
        if(q=='O'){
            rep[a]=true;
            for(int i=1;i<=n;i++){
                if(a==i || !rep[i]){
                    continue;
                }
                if(g[a][i]){
                    join(a,i);
                }
            }
        }
        else{
            scanf("%d",&b);
            if(find(a)==find(b)){
                printf("SUCCESS\n");
            }
            else{
                printf("FAIL\n");
            }
        }
        getchar();
    }
    return 0;
}
```





