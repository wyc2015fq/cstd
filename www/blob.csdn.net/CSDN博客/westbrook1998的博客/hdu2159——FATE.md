# hdu2159——FATE - westbrook1998的博客 - CSDN博客





2018年08月25日 18:30:50[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：24








> 
最近xhd正在玩一款叫做FATE的游戏，为了得到极品装备，xhd在不停的杀怪做任务。久而久之xhd开始对杀怪产生的厌恶感，但又不得不通过杀怪来升完这最后一级。现在的问题是，xhd升掉最后一级还需n的经验值，xhd还留有m的忍耐度，每杀一个怪xhd会得到相应的经验，并减掉相应的忍耐度。当忍耐度降到0或者0以下时，xhd就不会玩这游戏。xhd还说了他最多只杀s只怪。请问他能升掉这最后一级吗？ 

  Input 

  输入数据有多组，对于每组数据第一行输入n，m，k，s(0 < n,m,k,s < 100)四个正整数。分别表示还需的经验值，保留的忍耐度，怪的种数和最多的杀怪数。接下来输入k行数据。每行数据输入两个正整数a，b(0 < a,b < 20)；分别表示杀掉一只这种怪xhd会得到的经验值和会减掉的忍耐度。(每种怪都有无数个) 

  Output 

  输出升完这级还能保留的最大忍耐度，如果无法升完这级输出-1。 

  Sample Input 

  10 10 1 10 

  1 1 

  10 10 1 9 

  1 1 

  9 10 2 10 

  1 1 

  2 2 

  Sample Output 

  0 

  -1 

  1
完全背包 容忍度做容量 经验做价值 但是还要加一维杀怪的个数

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#define _clr(x,a) memset(x,a,sizeof(x));
using namespace std;
const int N=105;
const int INF=0x3f3f3f3f;
int n,m,k,s;
//dp[i][j]表示在杀i个怪物的前提下，容忍度为j获得的最大经验值
int dp[N][N];
//经验
int v[N];
//忍耐度
int w[N];
int main(void){
    //freopen("data.txt","r",stdin);
    while(~scanf("%d%d%d%d",&n,&m,&k,&s)){
        _clr(v,0);
        _clr(w,0);
        _clr(dp,0);
        for(int i=0;i<k;i++){
            scanf("%d%d",&v[i],&w[i]);
        }
        int ans=INF;
        //怪物种类
        for(int i=0;i<k;i++){
            //忍耐度
            for(int j=w[i];j<=m;j++){
                //杀怪个数
                for(int l=1;l<=s;l++){
                    dp[l][j]=max(dp[l][j],dp[l-1][j-w[i]]+v[i]);
                    //足够升级
                    if(dp[l][j]>=n){
                        ans=min(ans,j);
                    }
                }
            }
        }
        if(ans==INF){
            printf("-1\n");
        }
        else{
            printf("%d\n",m-ans);
        }
    }
    return 0;
}
```





