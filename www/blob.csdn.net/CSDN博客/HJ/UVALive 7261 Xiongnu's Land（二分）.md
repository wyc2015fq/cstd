# UVALive 7261  Xiongnu's Land（二分） - HJ - CSDN博客
2017年10月27日 13:19:33[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：187
[https://icpcarchive.ecs.baylor.edu/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=5273](https://icpcarchive.ecs.baylor.edu/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=5273)
题目大意 
王上有一块土地，上面有一些封地，现在需要平行y轴纵向切一刀，左边的给霍去病，右边的给卫青。在切的时候需要满足两个条件。第一个是左边的封地与右边的封地之差要最小。第二个是在第一个条件的前提下，左边的土地不能比右边的土地小并且左边的土地还要尽量大。问那个刀口在x轴的哪个地方？
解题思路： 
二分x轴，每次判断封地面积的差值是否最小即可。得出最小值之后，再尽量在不改变最小值的情况下，把x值往右边移动
```cpp
#include<iostream>
#include<cstring>
using namespace std;
typedef long long LL;
const int maxn=10005;
struct node{
    int x,tt,y,h;
}s[maxn];
int n,m;
LL get(int x)
{
    LL ans=0;
    for(int i=0;i<m;i++)
      if(s[i].x<x)
        ans+=(LL)(min(s[i].y,x)-s[i].x)*s[i].h;
    return ans;
}
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        LL ans=0;
        cin>>n>>m;
        for(int i=0;i<m;i++)
        {
            cin>>s[i].x>>s[i].tt>>s[i].y>>s[i].h;
            ans+=(LL)s[i].y*s[i].h;
            s[i].y+=s[i].x;
        }
        int l=0,r=n;
        while(l<r)
        {
            int mid=(l+r)/2;
            LL area=get(mid);
            if(2*area<ans)
              l=mid+1;
            else
              r=mid;
        }
        LL res=get(r);
        while(get(r)==res&&r<=n)
          r++;
        cout<<r-1<<endl;
    }
    return 0;
}
```
