# hdu  5135 Little Zu Chongzhi's Triangles（贪心+求三角形面积） - HJ - CSDN博客
2017年10月27日 19:37:32[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：237
[http://acm.hdu.edu.cn/showproblem.php?pid=5135](http://acm.hdu.edu.cn/showproblem.php?pid=5135)
题目大意： 
给出n根木棍，要求不能拼接不能折断，选择任意根组成三角形，最后求能组成的所有三角形面积的最大值。
解题思路： 
将所有木棍从大到小排序，之后每次选择邻近的三根木棍，如果不能组成三角形，把最长的那根舍弃，依次贪心即可。
```cpp
#include<iostream>
#include<cstdio>
#include <algorithm>
#include<cmath>
using namespace std;
double ans;
int n,s[15],vis[15];
bool cmp(int x, int y) {
    return x>y;
}
double area(int a,int b,int c)
{
    double p=(a+b+c)/2.0;
    return sqrt(p*(p-a)*(p-b)*(p-c));
}
bool check(int a,int b,int c)
{
    return (a+b>c)&&(abs(a-b)<c);
}
int main()
{
    while(cin>>n&&n)
    {
        for(int i=0;i<n;i++)
          cin>>s[i];
        sort(s,s+n,cmp);
        ans=0;
        int cnt=0,blag=0;
        double sum = 0;
        for(int i=0;i<n;i++)
        {
            blag++;
            if(blag==3) 
            {
                if(check(s[cnt],s[cnt-1],s[cnt-2]))
                {
                    sum+=area(s[cnt],s[cnt-1],s[cnt-2]);
                    blag=0;
                }
                else
                  blag--;
            }
            cnt++;
        }
        printf("%.2lf\n",sum);
    }
    return 0;
}
```
