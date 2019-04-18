# Trade-----HDU3401----单调队列优化的DP - weixin_33985507的博客 - CSDN博客
2013年07月15日 22:18:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
题目地址：[http://acm.hdu.edu.cn/showproblem.php?pid=3401](http://acm.hdu.edu.cn/showproblem.php?pid=3401)
题目意思：
有T天，你每天可以以API买进，BPI卖出，最多买ASI个，最多卖BSI个
最多只能持有MAXP个商品，相邻两次交易要大于W天
问你最多在第T天结束的时候能赚多少钱
解题思路：
有三个状态转移方程，其中dp[i][j]表示第i天手上有j个商品时我赚的钱
dp[i][j] = max(dp[i][j],dp[i-1][j])   //即针对前一天我啥也不做
对于买而言，有一个递推式
dp[i][j] = max(dp[i][j],max(dp[i-w-1][k] - (j-k)*buyp[i]  )
进一步拆开就是 dp[i][j] = max(dp[i][j], max(dp[i-w-1][k]+k*buyp[i]) - j*buyp[i] );
因为是买，所以0<=(j-k)<=canbuyn[i]
我们可以利用优先队列将对于i来说能看到的最大放在队首
对于卖而言同理，只不过方程变成
dp[i][j] = max(dp[i][j], max(dp[i-w-1][k]+k*sellp[i])-j*sellp[i]
0<=(k-j)<=canselln[i]
下面上代码：
```cpp
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<cstdlib>
using namespace std;
const int maxn = 3000;
struct node
{
    int val,pos;
}q[maxn];
int sellp[maxn],buyp[maxn];
int canselln[maxn],canbuyn[maxn];
int dp[maxn][maxn];
int T,n,maxp,w;
const int inf = 0x3f3f3f3f;
void dpf()
{
    for(int i=1;i<=n;i++)
    {
        for(int j=0;j<=maxp;j++)
            dp[i][j] = -inf;
    }
    //在前w+1天不能卖，只能买
    for(int i=1;i<=n;i++)
    {
        for(int j=0;j<=min(maxp,canbuyn[i]);j++)
        {
            dp[i][j] = -buyp[i]*j;  //买的都是负账
        }
    }
    for(int i=2;i<=n;i++) //针对前一天我啥也不做
    {
        for(int j=0;j<=maxp;j++)
            dp[i][j] = max(dp[i][j],dp[i-1][j]);
    }
    for(int i=w+2;i<=n;i++)   //前w+1天除了买啥也不能做，既能买又能卖得从w+2开始
    {
        int pre = i-w-1;
        //对于买而言，有一个递推式
        //dp[i][j] = max(dp[i][j],max(dp[i-w-1][k] - (j-k)*buyp[i]  )
        //进一步拆开就是 dp[i][j] = max(dp[i][j], max(dp[i-w-1][k]+k*buyp[i]) - j*buyp[i] );
        //因为是买，所以0<=(j-k)<=canbuyn[i]
        //我们可以利用优先队列将对于i来说能看到的最大放在队首
        int head,tail;
        head = 0;
        tail = -1;
        for(int j=0;j<=maxp;j++)
        {
            dp[i][j] = max(dp[i][j],dp[i-1][j]);
            node tmp;
            tmp.val = dp[pre][j]+j*buyp[i];
            tmp.pos = j;
            //插入到优先队列里面去
            while(head<=tail && q[tail].val < tmp.val) tail--;
            q[++tail] = tmp;
            //队首还要满足0<=(j-k)<=canbuyn[i]
            while(head<=tail && (j-q[head].pos)>canbuyn[i] )head++;
            //取队首
            if(head<=tail) dp[i][j] = max(dp[i][j],q[head].val-j*buyp[i]);
        }
        //对于卖而言同理，只不过方程变成
        //dp[i][j] = max(dp[i][j], max(dp[i-w-1][k]+k*sellp[i])-j*sellp[i]
        //0<=(k-j)<=canselln[i]
        head=0;
        tail=-1;
        for(int j=maxp;j>=0;j--)
        {
            dp[i][j] = max(dp[i][j],dp[i-1][j]);
            node tmp;
            tmp.val = dp[pre][j]+j*sellp[i];
            tmp.pos = j;
            while(head<=tail && q[tail].val < tmp.val) tail--;
            q[++tail] = tmp;
            while(head<=tail && (q[head].pos-j) > canselln[i]) head++;
            if(head<=tail) dp[i][j] = max(dp[i][j],q[head].val-j*sellp[i]);
        }
    }
}
int main()
{
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d%d%d",&n,&maxp,&w);
        for(int i=1;i<=n;i++)
            scanf("%d%d%d%d",&buyp[i],&sellp[i],&canbuyn[i],&canselln[i]);
        dpf();
        int ans = 0;
        for(int j=0;j<=maxp;j++)
            ans = max(ans,dp[n][j]);
        printf("%d\n",ans);
    }
    return 0;
}
```
