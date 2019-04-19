# hdu 1536 S-Nim（SG函数） - HJ - CSDN博客
2017年09月05日 18:33:13[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：171
个人分类：[====ACM====																[----------数学----------																[简单博弈																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7150812)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=1536](http://acm.hdu.edu.cn/showproblem.php?pid=1536)
SG函数练习题。应该注意的是，h[]数组判定的时候应该选择bool型，这样既可以省空间，在memset的时候也可以省时间（因为bool一个字节，而int需要四个字节）快了三倍！！！
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
#define maxn 10005
int n,sg[maxn],a[105];
bool h[maxn];
void get_sg()
{
    memset(sg,0,sizeof(sg));
    for(int i=1;i<maxn;i++)
    {
        memset(h,0,sizeof(h));
        for(int j=0;j<n;j++)
          if(i>=a[j])
            h[sg[i-a[j]]]=1;
        for(int j=0;j<maxn;j++)
          if(!h[j])
          {
              sg[i]=j;
              break;
          }
    }
}
int main()
{
    int m,num;
    while(scanf("%d",&n)!=EOF&&n)
    {
        for(int i=0;i<n;i++)
          scanf("%d",&a[i]);
        sort(a,a+n);
        get_sg();
        scanf("%d",&m);
        while(m--)
        {
            int x,ans=0;
            scanf("%d",&num);
            while(num--)
            {
                scanf("%d",&x);
                ans^=sg[x];
            }
            if(ans)
              printf("W");
            else
              printf("L");
        }
        printf("\n");
    }
    return 0;
}
```
