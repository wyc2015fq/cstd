# 【VC6.0你赢了】校赛B题_Frog追及问题 - CD's Coding - CSDN博客





2014年04月27日 20:03:52[糖果天王](https://me.csdn.net/okcd00)阅读数：487标签：[vc](https://so.csdn.net/so/search/s.do?q=vc&t=blog)
个人分类：[Coding Competition](https://blog.csdn.net/okcd00/article/category/2133459)











```cpp
#include <cstdio>
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

__int64 x,y,m,n,L,cnt=0;

__int64 judge()
{
        __int64 dist=(y-x<0)?(L+y-x):(y-x);//B 领先dist距离
        if(m>n)
                if(dist%(m-n)==0)return dist/(m-n);
                else {
                                __int64 mod= dist %(m-n);
                                cnt+=dist/(m-n);
                                __int64 per= L % (m-n);
                                if (per==0)return -1;
                                int* mval = new int[m-n+1];
                                while(1)
                                {
                                        mval[mod]=1;
                                        if(mod-per==0)return cnt + L/(m-n);
                                        else if(mod-per<0)
                                                {
                                                        mod=mod-per+m-n;
                                                        cnt+=L/(m-n)+1;
                                                }
                                        else 
                                                {
                                                        mod=mod-per;
                                                        cnt+=L/(m-n);
                                                }
                                        if(mval[mod]==1)return -1;
                                }
                         }
        else if(m==n)return -1;
        else {
                                dist=(x-y<0)?L+x-y:x-y;
                        if(dist%(n-m)==0)return dist/(n-m);
                        else
                        {
                                __int64 mod= dist % (n-m);
                                cnt+=dist/(n-m);
                                __int64 per= L % (n-m);
                                if (per==0)return -1;
                                int* mval = new int[n-m+1];
                                while(1)
                                {
                                        mval[mod]=1;
                                        if(mod-per==0)return cnt + L/(n-m);
                                        else if(mod-per<0)
                                                {
                                                        mod=mod-per+n-m;
                                                        cnt+=L/(n-m)+1;
                                                }
                                        else 
                                                {
                                                        mod=mod-per;
                                                        cnt+=L/(n-m);
                                                }
                                        if(mval[mod]==1)return -1;

                                }
                        }
                }
}


int main()
{
        FILE *fp1;
        FILE *fp2;
        fp1=fopen("frog.txt","r");
        fp2=fopen("estdout.pc2","w");
        fscanf(fp1,"%I64d%I64d%I64d%I64d%I64d",&x,&y,&m,&n,&L);
        //long long dist=y-x;//B 领先dist
        //long long spd=m-n;//A相对于B spd
        __int64 ans=judge();
        if(ans==-1)fprintf(fp2,"Impossible");
        else fprintf(fp2,"%I64d",ans);        
        return 0;
}
```


今天呢学校出了个校赛，有个第二题甚是有趣，最大的乐趣是VC6.0你们懂么……VC6.0!!!

题意呢是说俩青蛙在一个环形跑道上跑，A青蛙在x坐标，B青蛙在y坐标，A的速度是跳一下m米，B的是跳一下n米，问它俩能跳到一起么，跳得到的话跳多少步才行。

以上这就是对于VC6.0才能运行的代码……




1、文件输入输出：



```cpp
FILE *fp1;
        FILE *fp2;
        fp1=fopen("frog.txt","r");
        fp2=fopen("estdout.pc2","w");
        fscanf & fprintf
```


2、long long=__int64

3、不能int val[T]，必须

```cpp
int* mval = new int[m-n+1];
```



