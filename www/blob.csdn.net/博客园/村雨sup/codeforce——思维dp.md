# codeforce——思维dp - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Codeforces 822C Hacker, pack your bags!(思维)](http://www.cnblogs.com/fu3638/p/7111450.html)





题目大意：给你n个旅券，上面有开始时间l,结束时间r，和花费cost，要求选择两张时间不相交的旅券时间长度相加为x,且要求花费最少。

```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int moder = 10000;
const int maxn = 2000000;
const int M = 2e5+10;
struct node
{
    int l;int r;int cs;
};

bool cmp1(const node &a,const node &b)
{
    return a.l < b.l;
}
bool cmp2(const node &a, const node &b)
{
    return a.r < b.r;
}
ll mincost[M];

ll gmin(ll a,ll b)
{
    return a>b?b:a;
}

int main()
{
    int n,x;
    cin >> n >> x;
    node save1[n];
    node save2[n];
    for(int i=0;i<M;i++)
        mincost[i]=INT_MAX;
    for(int i=0;i < n;i++)
    {
        cin >> save1[i].l >> save1[i].r >> save1[i].cs;
        save2[i] = save1[i];
    }
    sort(save1,save1+n,cmp1);
    sort(save2,save2+n,cmp2);

    int minn = INT_MAX;
    int j=0;
    for(int i=0;i < n;i++)
    {
        while(j < n&&save2[j].r < save1[i].l)
        {
            mincost[save2[j].r-save2[j].l+1] = gmin(mincost[save2[j].r-save2[j].l+1],save2[j].cs);
            j++;
        }
        int k = x-(save1[i].r-save1[i].l+1);
        if(k > 0&&minn > mincost[k]+save1[i].cs)
            minn = mincost[k]+save1[i].cs;
    }
    if(minn ==  INT_MAX)
        printf("-1\n");
    else
        printf("%d\n",minn);

    return 0;
}
```

——













