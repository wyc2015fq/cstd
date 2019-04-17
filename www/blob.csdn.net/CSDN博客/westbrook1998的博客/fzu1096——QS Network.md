# fzu1096——QS Network - westbrook1998的博客 - CSDN博客





2018年08月13日 13:22:49[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：34标签：[最小生成树](https://so.csdn.net/so/search/s.do?q=最小生成树&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
In the planet w-503 of galaxy cgb, there is a kind of intelligent creature named QS. QScommunicate with each other via networks. If two QS want to get connected, they need to buy two network adapters (one for each QS) and a segment of network cable. Please be advised that ONE NETWORK ADAPTER CAN ONLY BE USED IN A SINGLE CONNECTION.(ie. if a QS want to setup four connections, it needs to buy four adapters). In the procedure of communication, a QS broadcasts its message to all the QS it is connected with, the group of QS who receive the message broadcast the message to all the QS they connected with, the procedure repeats until all the QS’s have received the message. 

  A sample is shown below: 
![这里写图片描述](https://odzkskevi.qnssl.com/1e0ebd4772deef86fa1caa580fe339bb?v=1534124773)

  A sample QS network, and QS A want to send a message. 

  Step 1. QS A sends message to QS B and QS C; 

  Step 2. QS B sends message to QS A ; QS C sends message to QS A and QS D; 

  Step 3. the procedure terminates because all the QS received the message. 

  Each QS has its favorate brand of network adapters and always buys the brand in all of its connections. Also the distance between QS vary. Given the price of each QS’s favorate brand of network adapters and the price of cable between each pair of QS, your task is to write a program to determine the minimum cost to setup a QS network. 

  Input 

  The 1st line of the input contains an integer t which indicates the number of data sets. 

  From the second line there are t data sets. 

  In a single data set,the 1st line contains an interger n which indicates the number of QS.  

  The 2nd line contains n integers, indicating the price of each QS’s favorate network adapter.  

  In the 3rd line to the n+2th line contain a matrix indicating the price of cable between ecah pair of QS. 

  Constrains: 

  All the integers in the input are non-negative and not more than 1000. 

  Output 

  for each data set,output the minimum cost in a line. NO extra empty lines needed. 

  Sample Input 

  1 

  3 

  10 20 30 

  0 100 200 

  100 0 300 

  200 300 0 

  Sample Output 

  370
最小生成树 注意边权要加上两个点的网络适配器的值

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=1010;
int a[N];
int t,n;
int g[N][N];
int lowcost[N];
bool vis[N];
const int INF=0x3f3f3f3f;
void init(){
    memset(a,0,sizeof(a));
    memset(g,0,sizeof(g));
    memset(lowcost,0,sizeof(lowcost));
    memset(vis,false,sizeof(vis));
}
int prim(){
    int Min=INF;
    int pos=1;
    int ans=0;
    vis[pos]=true;
    for(int i=1;i<=n;i++){
        if(i!=pos){
            lowcost[i]=g[pos][i];
        }
    }
    for(int i=1;i<n;i++){
        Min=INF;
        for(int j=1;j<=n;j++){
            if(!vis[j] && lowcost[j]<Min){
                Min=lowcost[j];
                pos=j;
            }
        }
        ans+=Min;
        vis[pos]=true;
        for(int j=1;j<=n;j++){
            if(!vis[j] && lowcost[j]>g[pos][j]){
                lowcost[j]=g[pos][j];
            }
        }
    }
    return ans;
}
int main(void){
    //freopen("data.txt","r",stdin);
    scanf("%d",&t);
    while(t--){
        init();
        scanf("%d",&n);
        for(int i=1;i<=n;i++){
            scanf("%d",&a[i]);
        }
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                scanf("%d",&g[i][j]);
                g[i][j]+=(a[i]+a[j]);
            }
        }
        int ans=prim();
        printf("%d\n",ans);
    }
    return 0;
}
```





