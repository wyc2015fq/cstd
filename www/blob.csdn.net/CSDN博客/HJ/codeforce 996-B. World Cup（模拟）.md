# codeforce 996-B. World Cup（模拟） - HJ - CSDN博客
2018年08月08日 20:35:14[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：36
个人分类：[====ACM====																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/996/B](http://codeforces.com/problemset/problem/996/B)
解题思路：
定义一个查找指针p初始化为1，循环节为n，然后找出最小数字，判断指针p，在这个数变成0的时候，是否不超过这个数的位置，如果不超过，则答案即为这个数所在的位置，否则对下一轮模拟一遍才能得出答案。
```cpp
#include<iostream>
#include<algorithm>
#include<queue>
#include<set>
#include<vector>
#include<cstdio>
#include<cstring>
#include<cmath>
using namespace std;
#define mod 1000000007
typedef long long ll;
const int INF=0x3f3f3f3f;
int s[100005];
int main()
{
    int n;
    while(cin>>n)
    {
        int min_s=INF,pos;
        for(int i=1;i<=n;i++)
        {
            cin>>s[i];
            if(min_s>s[i])
            {
                min_s=s[i];
                pos=i;
            }
        }
        int blag=0,res,p=min_s/n;
        for(int i=1;i<=n;i++)
        {
            s[i]-=p*n;
            if(s[i]<=i-1)
            {
                blag=1;
                res=i;
                break;
            }
        }
        if(blag==0)
        {
            for(int i=1;i<=n;i++)
            {
                s[i]-=n;
                if(s[i]<=i-1)
                {
                    res=i;
                    break;
                }
            }
        }
        cout<<res<<endl;
    }
    return 0;
}
```
