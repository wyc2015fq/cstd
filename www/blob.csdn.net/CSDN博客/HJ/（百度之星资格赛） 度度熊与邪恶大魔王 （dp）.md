# （百度之星资格赛） 度度熊与邪恶大魔王 （dp） - HJ - CSDN博客
2017年08月05日 17:24:16[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：811
度度熊为了拯救可爱的公主，于是与邪恶大魔王战斗起来。
邪恶大魔王的麾下有n个怪兽，每个怪兽有a[i]的生命值，以及b[i]的防御力。
度度熊一共拥有m种攻击方式，第i种攻击方式，需要消耗k[i]的晶石，造成p[i]点伤害。
当然，如果度度熊使用第i个技能打在第j个怪兽上面的话，会使得第j个怪兽的生命值减少p[i]-b[j]，当然如果伤害小于防御，那么攻击就不会奏效。
如果怪兽的生命值降为0或以下，那么怪兽就会被消灭。
当然每个技能都可以使用无限次。
请问度度熊最少携带多少晶石，就可以消灭所有的怪兽。 
Input
本题包含若干组测试数据。
第一行两个整数n，m,表示有n个怪兽，m种技能。
接下来n行，每行两个整数，a[i],b[i]，分别表示怪兽的生命值和防御力。
再接下来m行，每行两个整数k[i]和p[i]，分别表示技能的消耗晶石数目和技能的伤害值。
数据范围:
1<=n<=100000
1<=m<=1000
1<=a[i]<=1000
0<=b[i]<=10
0<=k[i]<=100000
0<=p[i]<=1000 
Output
对于每组测试数据，输出最小的晶石消耗数量，如果不能击败所有的怪兽，输出-1 
Sample Input
1 2 
3 5 
7 10 
6 8 
1 2 
3 5 
10 7 
8 6
Sample Output
6 
18
因为血量和防御数据量很小，那么就可以用dp[i][j]表示一种防御为i 血量为j 的怪物需要使用的最少晶石数 
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdio> 
using namespace std;
#define INF 1e18
int k[1005],p[1005],a[100010],b[100010];
long long dp[15][1005];              //dp[i][j] 表示一种防御为i 血量为j 的怪物需要使用的最少晶石数 
int main()
{
    long long n,m;
    while(cin>>n>>m)
    {
        for(int i=1;i<=n;i++)
          cin>>a[i]>>b[i];
        for(int i=1;i<=m;i++)
          cin>>k[i]>>p[i];
        for(int i=0;i<=10;i++)          
        {
            for(int j=1;j<=1000;j++)
              dp[i][j]=INF;           //开始的时候赋值为无穷大 
            dp[i][0]=0;                      //血量为0的怪物不需要晶石 
            for(int j=1;j<=m;j++) 
            {
                if(p[j]<=i)              //如果造成的伤害小于防御 那么就无效 
                  continue;
                for(int q=1;q<=1000;q++)            //否则枚举血量 求出需要的最少晶石数 
                {
                     int kk=q-p[j]+i>0?q-p[j]+i:0;
                     dp[i][q]=min((long long)dp[i][kk]+k[j],dp[i][q]);
                }
            }
        }
        long long ans=0;
        for(int i=1;i<=n;i++)
          ans+=dp[b[i]][a[i]];
        if(ans>=INF)
          cout<<-1<<endl;
        else
          cout<<ans<<endl;
    }
    return 0;
}
```
