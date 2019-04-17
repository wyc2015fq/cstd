# In Action（HDU-3339） - Alex_McAvoy的博客 - CSDN博客





2018年05月21日 23:59:13[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：48








> 
# Problem Description

Since 1945, when the first nuclear bomb was exploded by the Manhattan Project team in the US, the number of nuclear weapons have soared across the globe.

Nowadays,the crazy boy in FZU named AekdyCoin possesses some nuclear weapons and wanna destroy our world. Fortunately, our mysterious spy-net has gotten his plan. Now, we need to stop it.

But the arduous task is obviously not easy. First of all, we know that the operating system of the nuclear weapon consists of some connected electric stations, which forms a huge and complex electric network. Every electric station has its power value. To start the nuclear weapon, it must cost half of the electric network's power. So first of all, we need to make more than half of the power diasbled. Our tanks are ready for our action in the base(ID is 0), and we must drive them on the road. As for a electric station, we control them if and only if our tanks stop there. 1 unit distance costs 1 unit oil. And we have enough tanks to use.

Now our commander wants to know the minimal oil cost in this action.

# Input

The first line of the input contains a single integer T, specifying the number of testcase in the file.<br> For each case, first line is the integer n(1<= n<= 100), m(1<= m<= 10000), specifying the number of the stations(the IDs are 1,2,3...n), and the number of the roads between the station(bi-direction).<br> Then m lines follow, each line is interger st(0<= st<= n), ed(0<= ed<= n), dis(0<= dis<= 100), specifying the start point, end point, and the distance between.

Then n lines follow, each line is a interger pow(1<= pow<= 100), specifying the electric station's power by ID order.

# Output

The minimal oil cost in this action.<br> If not exist print "impossible"(without quotes).

# Sample Input

**2**

**2 30 2 92 1 31 0 2132 12 1 313 **

# Sample Output

**5**

**impossible**


题意：有一个基地和n个发电站，每个发电站有能量P[i]，现要使所有的发电站不能正常工作，需要用坦克占领所有发电站的能量的总和的一半以上，占领K个发电站需K辆坦克，每辆坦克从基地出发，如果能实现就输出每辆坦克所花费的总和

思路：问题即求n个发电站中找出几个站的能量和超出总能量和的一半且这几个站到基地的距离和最小，采用01背包了，将n个站到基地的总距离和看成背包的最大容量V，能量看成是价值，问题也就是求花最少的费用能得到的最大价值

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#define INF 999999999
#define N 101
#define MOD 1000000007
#define E 1e-12
using namespace std;
struct Node{
    long long dis;
    long long w;
}a[N];
int g[N][N];
int n,m;
long long dp[100001];
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&m);
        for(int i=0;i<=n;i++)//初始化
        {
            a[i].dis=INF;
            for(int j=0;j<=n;j++)
                g[i][j]=INF;
        }


        long long x,y,dis;
        while(m--)
        {
            scanf("%lld%lld%lld",&x,&y,&dis);
            if(g[x][y]>dis)
            {	
                g[x][y]=dis;
            	g[y][x]=dis;
            }
        }

        long long sum=0;
        for(int i=1;i<=n;i++)//计算总能量
        {
            scanf("%lld",&a[i].w);
            sum+=a[i].w;
        }

        /*SPFA计算最短路*/
        queue<int> q;
        int vis[N];
        int head=0;

        memset(vis,0,sizeof(vis));
        a[head].dis=0;
        q.push(head);
        while(!q.empty())
        {
            head=q.front();
            q.pop();
            vis[head]=0;
            for(int i=1;i<=n;i++)
                if(a[i].dis>a[head].dis+g[head][i])
                {
                    a[i].dis=a[head].dis+g[head][i];
                    if(!vis[i])
                    {
                        q.push(i);
                        vis[i]=1;
                    }
                }
        }

        /*01背包计算*/
        long long V=0;
        memset(dp,0,sizeof(dp));//背包初始化
        for(int i=1;i<=n;i++)//计算背包最大容量
            if(a[i].dis!=INF)
                V+=a[i].dis;

        for(int i=1;i<=n;i++)
            if(a[i].dis!=INF)//有路可走时
                for(long long j=V;j>=a[i].dis;j--)
                    if(dp[j-a[i].dis]||j==a[i].dis)//恰好装入背包的条件
                        if(dp[j]<dp[j-a[i].dis]+a[i].w)
                            dp[j]=dp[j-a[i].dis]+a[i].w;

        long long ans=0;
        for(int i=1;i<=V;i++)
            if(dp[i]>=(sum/2)+1)//当价值满足大于一半时,即为最小费用
            {
                ans=i;
                break;
            }
        if(ans!=0)
            printf("%lld\n",ans);
        else
            printf("impossible\n");
    }
    return 0;
}
```





