# hdu 3517 SPY（Map） - HJ - CSDN博客
2018年04月20日 18:51:27[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：65
个人分类：[====ACM====																[--------数据结构--------																[STL																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308005)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=3527](http://acm.hdu.edu.cn/showproblem.php?pid=3527)
题目大意：有三个城市，第二个城市向第一个城市派出了几个间谍，每个人用一个字符串表示。实际上就是求有哪些字符串只在B和A中同时出现过。
解题思路：用map判断一下即可。
```cpp
#include<iostream>
#include<cstring>
#include<map>
using namespace std;
const int N=1e4+10;
map<string,int> mp;
char a[N][40],b[N][40],c[N][40];
int vis[N];
int main()
{
    int n,m,k;
    while(cin>>n>>m>>k)
    {
        memset(vis,0,sizeof(vis));
        mp.clear();
        for(int i=1;i<=n;i++)
        {
            cin>>a[i];
            mp[a[i]]=i;
        }
        for(int i=1;i<=m;i++)
        {
            cin>>b[i];
            vis[mp[b[i]]]=1;
        }
        for(int i=1;i<=k;i++)
        {
            cin>>c[i];
            vis[mp[c[i]]]=0;
        }
        int blag=0;
        for(int i=1;i<=m;i++)
        {
            if(vis[mp[b[i]]])
            {
                if(blag)
                  cout<<" ";
                cout<<b[i];
                blag=1;
            }
        }
        if(blag==0)
          cout<<"No enemy spy";
        cout<<endl;
    }
    return 0;
}
```
