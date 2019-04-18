# HDU 5326 work （回溯，树） - xcw0754 - 博客园
# [HDU 5326 work （回溯，树）](https://www.cnblogs.com/xcw0754/p/4684155.html)
题意：给一棵树，每个结点的子树下的结点都是它的统治对象，问有多少个统治对象数目为k的结点？
思路：每个结点都设一个cnt来记数，只要将每个结点往上回溯，直到树根，经过的每个结点都将计数器加1。最后再扫一遍，将cnt等于k的结点统计一下。
```
#include <bits/stdc++.h>
#define LL long long
#define pii pair<int,int>
#define INF 0x7f7f7f7f
using namespace std;
const int N=200;
int cnt[N];
unordered_map<int,int>  mapp;
int k;
int cal(int n)
{
    memset(cnt,0,sizeof(cnt));
    for(int i=1; i<=n; i++)
    {
        int t=i;
        while(mapp[t])
        {
            t=mapp[t];
            cnt[t]++;
        }
    }
    int ans=0;
    for(int i=1; i<=n; i++)    if(cnt[i]==k)   ans++;
    return ans;
}
int main()
{
    //freopen("input.txt", "r", stdin);
    int n, m,  t, a, b, c, j=0;
    while(~scanf("%d%d",&n,&k))
    {
        mapp.clear();
        for(int i=1; i<n; i++)
        {
            scanf("%d%d",&a,&b);
            mapp[b]=a;
        }
        printf("%d\n",cal(n));
    }
    return 0;
}
AC代码
```

