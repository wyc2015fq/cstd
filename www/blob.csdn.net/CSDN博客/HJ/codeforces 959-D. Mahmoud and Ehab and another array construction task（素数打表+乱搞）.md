# codeforces 959-D. Mahmoud and Ehab and another array construction task（素数打表+乱搞） - HJ - CSDN博客
2018年07月02日 11:00:55[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：103
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/959/D](http://codeforces.com/problemset/problem/959/D)
题目大意： 
      给出一个串数a，要求构造一个相同个数的数组b，保证b中的数互相互素。
解题思路： 
     首先素数打表，然后枚举a数组中的数，判断是否都在数组中，然后分两类处理即可；
```cpp
#include<iostream>
#include<vector>
#include<cstring>
using namespace std;
const int maxn=2000005;
int num[maxn],vis[maxn];
vector<int> d[maxn];
bool Judge(int x)
{
    for(int i=0;i<d[x].size();i++)
      if(vis[d[x][i]])
        return false;
    return true;
}
void init()
{
    for(int i=2;i<maxn;i++)
      if(num[i]==0)
        for(int j=i;j<maxn;j+=i)
        {
            d[j].push_back(i);
            num[j]=1;
        }
} 
int main()
{
    init();
    int n;
    while(cin>>n)
    {
        memset(vis,0,sizeof(vis));
        int blag=0,res=2;
        for(int i=0,x;i<n;i++)
        {
            cin>>x;
            int m=x;
            if(blag==0)
            {
                while(!Judge(m))
                  m++;
                if(m!=x)
                  blag=1;
            }
            else
            {
                while(!Judge(res))  
                  res++;
                m=res;
            }
            cout<<m<<" ";
            for(int j=0;j<d[m].size();j++)
              vis[d[m][j]]=1;
        }
        cout<<endl;
    }
    return 0;
}
```
