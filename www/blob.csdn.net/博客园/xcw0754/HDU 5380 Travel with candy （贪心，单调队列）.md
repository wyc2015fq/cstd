# HDU 5380 Travel with candy  （贪心，单调队列） - xcw0754 - 博客园
# [HDU 5380 Travel with candy  （贪心，单调队列）](https://www.cnblogs.com/xcw0754/p/4737155.html)
题意：
    有n+1个城市按顺序分布在同一直线上，现在需从0号城市按顺序走到n号城市(保证可达)，从0号城市到i号城市需要消耗ai个糖果，每个城市都可以通过买/卖糖果来赚取更多的钱，价格分别是buyi和selli，保证selli<buyi。由于身上最多只能带C个糖果，且在起点0号城市时身上是没有钱的，问到达n号城市的最小花费(可以是负数，即亏损)？
思路：
    (1)每次离开i城市时，将C补满，并记录身上每个糖果的购买价(因为钱是可以负的，所以一定可以补满)。
    (2)每次到达i城市时，先用最便宜的糖果作为路上的固定消耗。
    (3)卖出糖果挣钱，并以卖价继续持有它(比较难想到的点)。
    (4)"退"掉不挣钱的糖果，注意是退不是卖，即按原价退掉，就当作从未买过这些糖果(这是每次都将C补满的原因)。
　　问题在于怎么知道在哪个城市卖掉比较划算？
　　因为selli<buyi，所以在同城市按sell卖出后再按buy买入来补满C是不划算的，会亏了中间的差价。我们得在后面的城市根据价格，来决策前面的城市该不该卖糖果。那么将可卖的糖果卖出后，以卖价s继续买入，会有两种情况：
　　1、后面卖出可以赚更多。
　　　　之前卖s价所带来利润已经到手，所以后面卖出有赚也是可以保证利润的。
　　2、后面卖出会亏。
　　　　按s价(这是当时新买入价)退掉这些糖果，就相当于在当时就卖掉了。
```
#include <bits/stdc++.h>
#define max(X,Y) ((X) > (Y) ? (X) : (Y))
#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#define pii pair<int,int>
#define INF 0x7f7f7f7f
#define LL long long
using namespace std;
const int N=200010;
int n;
LL dis[N], buy[N], sell[N], cap;
struct node
{
    LL price;
    int cnt;
    node(){};
    node(LL p, int c):price(p),cnt(c){};
};
LL cal()
{
    deque<node> que(1,node( buy[0], cap) ); //当前持有。
    LL ans=cap*buy[0];                      //最小费用，补满。
    for(int i=1; i<=n; i++)
    {
        LL tmp=dis[i]-dis[i-1];     //手续费
        LL sum=cap-tmp;             //持有量。
        for(; !que.empty(); que.pop_front())        //路上消耗最便宜的糖果。
        {
            node &t=que.front();
            if(t.cnt>tmp)
            {
                t.cnt-=tmp;
                break;
            }
            tmp-=que.front().cnt;
        }
        //看能否卖掉一些。
        LL cnt=0;
        for(; !que.empty(); que.pop_front())
        {
            node &t=que.front();
            if(t.price>sell[i])    break;
            cnt+=t.cnt;
        }
        if(cnt) que.push_front(node(sell[i], cnt));     //以sell价继续买入。
        //看能否退掉一些：这里买比前面的还便宜，不如不带过来，那就按原价退
        for( ; !que.empty(); que.pop_back())
        {
            node &t=que.back();
            if(t.price<buy[i])  break;
            ans-=t.price*t.cnt;       
            sum-=t.cnt;                 //当前持有量减少。
        }
        que.push_back(  node(buy[i], cap-sum) );        //补满！
        ans+= (cap-sum)*buy[i];
    }
    while(!que.empty())    //按原价退，相当于从未买过这些
    {
        node &t=que.front();que.pop_front();
        ans-=t.price*t.cnt;
    }
    return ans;
}
int main()
{
    freopen("input.txt", "r", stdin);
    int t;
    cin>>t;
    while(t--)
    {
        scanf("%d%d", &n, &cap);
        for(int i=1; i<=n; i++)    scanf("%lld", &dis[i]);
        for(int i=0; i<=n; i++)    scanf("%lld %lld", &buy[i], &sell[i]);
        printf("%lld\n",cal());
    }
    return 0;
}
View Code
```

