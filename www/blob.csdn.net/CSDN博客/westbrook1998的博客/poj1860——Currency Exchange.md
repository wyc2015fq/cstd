# poj1860——Currency Exchange - westbrook1998的博客 - CSDN博客





2018年08月18日 21:59:28[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：413








> 
Several currency exchange points are working in our city. Let us suppose that each point specializes in two particular currencies and performs exchange operations only with these currencies. There can be several points specializing in the same pair of currencies. Each point has its own exchange rates, exchange rate of A to B is the quantity of B you get for 1A. Also each exchange point has some commission, the sum you have to pay for your exchange operation. Commission is always collected in source currency.  

  For example, if you want to exchange 100 US Dollars into Russian Rubles at the exchange point, where the exchange rate is 29.75, and the commission is 0.39 you will get (100 - 0.39) * 29.75 = 2963.3975RUR.  

  You surely know that there are N different currencies you can deal with in our city. Let us assign unique integer number from 1 to N to each currency. Then each exchange point can be described with 6 numbers: integer A and B - numbers of currencies it exchanges, and real R AB, C AB, R BA and C BA - exchange rates and commissions when exchanging A to B and B to A respectively.  

  Nick has some money in currency S and wonders if he can somehow, after some exchange operations, increase his capital. Of course, he wants to have his money in currency S in the end. Help him to answer this difficult question. Nick must always have non-negative sum of money while making his operations.  

  Input 

  The first line of the input contains four numbers: N - the number of currencies, M - the number of exchange points, S - the number of currency Nick has and V - the quantity of currency units he has. The following M lines contain 6 numbers each - the description of the corresponding exchange point - in specified above order. Numbers are separated by one or more spaces. 1<=S<=N<=100, 1<=M<=100, V is real number, 0<=V<=10 3.  

  For each point exchange rates and commissions are real, given with at most two digits after the decimal point, 10 -2<=rate<=10 2, 0<=commission<=10 2.  

  Let us call some sequence of the exchange operations simple if no exchange point is used more than once in this sequence. You may assume that ratio of the numeric values of the sums at the end and at the beginning of any simple sequence of the exchange operations will be less than 10 4.  

  Output 

  If Nick can increase his wealth, output YES, in other case output NO to the output file. 

  Sample Input 

  3 2 1 20.0 

  1 2 1.00 1.00 1.00 1.00 

  2 3 1.10 1.00 1.10 1.00 

  Sample Output 

  YES
要求通过数次货币的兑换之后最后还赚了，就是求一个正权环，但这里的权值不是简单的一个数，而是一个操作 

用spfa

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <queue>
#define _clr(x,a) memset(x,a,sizeof(x));
using namespace std;
const int N=215;
const int INF=0x3f3f3f3f;
int e=0;
int n,m,s;
double v;
double g1[N][N];
double g2[N][N];
double dis[N];
bool vis[N];
//记录到这个点走过多少个点了
int cnt[N];
bool spfa(){
    for(int i=1;i<=n;i++){
        dis[i]=0;
    }
    vis[s]=true;
    dis[s]=v;
    queue<int> q;
    q.push(s);
    while(!q.empty()){
        int cur=q.front();
        q.pop();
        vis[cur]=false;
        for(int i=1;i<=n;i++){
            if(i==cur || g1[cur][i]==INF){
                continue;
            }
            if((dis[cur]-g2[cur][i])*g1[cur][i]>dis[i]){
                dis[i]=(dis[cur]-g2[cur][i])*g1[cur][i];
                if(!vis[i]){
                    q.push(i);
                    vis[i]=true;
                }
                cnt[i]++;
                if(dis[s]>v || cnt[i]>=1000){
                    return true;
                }
            }
        }
    }
    return false;
}
int main(void){
    //freopen("data.txt","r",stdin);
    scanf("%d%d%d%lf",&n,&m,&s,&v);
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            if(i==j){
                g1[i][j]=0;
                g2[i][j]=0;
            }
            else{
                g1[i][j]=INF;
                g2[i][j]=INF;
            }
        }
    }
    while(m--){
        int u,v;
        double c1,d1,c2,d2;
        scanf("%d%d%lf%lf%lf%lf",&u,&v,&c1,&d1,&c2,&d2);
        g1[u][v]=c1;
        g2[u][v]=d1;
        g1[v][u]=c2;
        g2[v][u]=d2;
    }
    if(spfa()){
        printf("YES\n");
    }
    else{
        printf("NO\n");
    }
    return 0;
}
```





