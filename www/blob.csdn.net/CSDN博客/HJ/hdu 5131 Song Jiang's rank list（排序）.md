# hdu 5131 Song Jiang's rank list（排序） - HJ - CSDN博客
2017年10月27日 19:27:38[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：177
[http://acm.hdu.edu.cn/showproblem.php?pid=5131](http://acm.hdu.edu.cn/showproblem.php?pid=5131)
题目意思： 
给出水浒传里面一些人物的英文名字和排名，按照一定要求给他们排序即可
```cpp
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<cstring>
using namespace std;
const int maxn=205;
struct node{
    char s[105];
    int x;
}a[205];
int cmp(node a,node b)
{
    if(a.x==b.x)
      return strcmp(a.s,b.s)<0;
    return a.x>b.x;
}
int main()
{
    int n,m;
    while(cin>>n&&n)
    {
        for(int i=0;i<n;i++)
          cin>>a[i].s>>a[i].x;
        sort(a,a+n,cmp);
        for(int i=0;i<n;i++)
          cout<<a[i].s<<" "<<a[i].x<<endl;
        cin>>m;
        int p;
        char c[105];
        while(m--)
        {
            cin>>c;
            for(int i=0;i<n;i++)
              if(strcmp(c,a[i].s)==0)
              {
                  p=i;
                  break;
              }
            int j=p;
            while(j>=0)
            {
                if(a[j].x==a[p].x)
                  j--;
                else
                  break;
            }
            int q=p-j;          
            if(q==1)
              cout<<p+1<<endl;
            else
              cout<<j+2<<" "<<q<<endl;
        }
    }
    return 0;
}
```
