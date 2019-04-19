# hdu 6375 度度熊学队列（list或者map+deque） - HJ - CSDN博客
2018年08月14日 08:23:20[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：87
[http://acm.hdu.edu.cn/showproblem.php?pid=6375](http://acm.hdu.edu.cn/showproblem.php?pid=6375)
解题思路：
主要是一个双向链表的模拟，对于反转，只要将链表两端互换一下即可。下面给出两种STL做法。
list实现：
```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<list>
using namespace std;
const int maxn=150010;
list<int> loli[maxn];
int n,q,op,u,v,w,val;
void read(int &x)
{
    char ch=getchar();x=0;
    for(;ch<'0'||ch>'9';ch=getchar());
    for(;ch>='0'&&ch<='9';ch=getchar()) x=x*10+ch-'0';
}
int main()
{
    while(scanf("%d%d",&n,&q)!=EOF)
    {
        for(int i=1;i<=n;i++)
          loli[i].clear();
        while(q--)
        {
            read(op);
            if(op==1)
            {
                read(u),read(w),read(val);
                w==0?loli[u].push_front(val):loli[u].push_back(val);
            }
            else if(op==2)
            {
                read(u),read(w);
                if(loli[u].empty())
                  printf("-1\n");
                else
                {
                    if(w==0)
                    {
                        printf("%d\n",loli[u].front());
                        loli[u].pop_front();
                    }
                    else
                    {
                        printf("%d\n",loli[u].back());
                        loli[u].pop_back();
                    }
                }
            }
            else
            {
                read(u),read(v),read(w);
                if(w==1)
                  loli[v].reverse();
                loli[u].splice(loli[u].end(),loli[v]);
            }
        }
    }
    return 0;
}
```
map + deque实现 ，map主要是减少内存开销
