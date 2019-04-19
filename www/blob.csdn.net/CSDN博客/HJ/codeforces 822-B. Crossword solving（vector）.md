# codeforces 822-B. Crossword solving（vector） - HJ - CSDN博客
2017年08月30日 17:26:44[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：198
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/822/B](http://codeforces.com/problemset/problem/822/B)
每次查找，保存需要改变数量最少的，最后输出对应下标即可。
```cpp
#include<iostream>
#include<cstring>
#include<vector>
using namespace std;
char p[1005],q[1005];
vector<int> aa[1005];
int main()
{
    int a,b;
    while(cin>>a>>b)
    {
        for(int i=0;i<=1000;i++)
          aa[i].clear();
        cin>>p>>q;
        int ans=2000,tt=0;
        for(int i=0;i<b;i++)
        {
            int k=i,blag=0;
            for(int j=0;j<a;j++,k++)
            {
                if(k==b)
                {
                    blag=1;
                    break;
                }
                if(q[k]!=p[j])
                  aa[i].push_back(j+1);
            }
            if(ans>aa[i].size()&&blag==0)
            {
                ans=aa[i].size();
                tt=i;
            }
        }
        cout<<ans<<endl;
        if(ans==0)
           continue;
        for(int i=0;i<aa[tt].size()-1;i++)
          cout<<aa[tt][i]<<" ";
        cout<<aa[tt][ans-1]<<endl;
    }
    return 0;
}
```
