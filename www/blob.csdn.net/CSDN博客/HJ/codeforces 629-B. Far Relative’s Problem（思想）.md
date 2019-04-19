# codeforces 629-B. Far Relative’s Problem（思想） - HJ - CSDN博客
2017年08月30日 17:29:45[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：174
个人分类：[====ACM====																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/629/B](http://codeforces.com/problemset/problem/629/B)
因为只有366天，因此统计多少天男生在同一天，再统计多少女生在同一天，最后比较大小*2即可。
```
#include<iostream>
#include<cstring>
using namespace std;
int p[400],q[400];
int main()
{
    int n;
    while(cin>>n)
    {
        memset(p,0,sizeof(p));
        memset(q,0,sizeof(q));
        char c;
        int a,b,ans=0;
        for(int i=0;i<n;i++)
        {
            cin>>c>>a>>b;
            if(c=='M')
            {
                for(int i=a;i<=b;i++)
                  p[i]++;
            }
            else
            {
                for(int i=a;i<=b;i++)
                  q[i]++;
            }
        }
        for(int i=1;i<=366;i++)
        {
            int res=min(p[i],q[i]);
            ans=max(ans,res*2);
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
