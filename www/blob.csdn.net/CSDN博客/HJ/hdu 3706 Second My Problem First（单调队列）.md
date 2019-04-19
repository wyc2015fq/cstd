# hdu 3706  Second My Problem First（单调队列） - HJ - CSDN博客
2017年08月16日 17:26:21[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：282
[http://acm.hdu.edu.cn/showproblem.php?pid=3706](http://acm.hdu.edu.cn/showproblem.php?pid=3706)
题目大意就是求出T1*T2*…*Tn%B的值
其中Ti表示在大小为k的窗口中，查找最小的数字。维护一个递增的单调队列，每次累乘即可。
```cpp
#include<iostream>
#include<cstring>
#include<queue>
using namespace std;
typedef long long LL;
deque<pair<int,LL> > q;
LL n,a,b;
int main()
{
    while(cin>>n>>a>>b)
    {
        q.clear();
        LL ans=1,tmp=1;
        for(int i=1;i<=n;i++)
        {
            tmp=tmp*a%b;
            while(!q.empty()&&q.back().second>=tmp)   //维护一个递增的单调队列 
              q.pop_back();
            q.push_back(make_pair(i,tmp));
            while(!q.empty()&&q.front().first<i-a)
              q.pop_front();
            ans=ans*q.front().second%b;
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
