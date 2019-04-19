# codeforces 995-B. Suit and Tie（暴力+思维） - HJ - CSDN博客
2018年07月03日 21:30:34[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：182
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/995/B](http://codeforces.com/problemset/problem/995/B)
题目大意： 
      有1~n的n对数字任意排列，每次操作可以将任意相邻的两个数进行对换，问最少需要多少次操作使得每一对数字都相邻？
解题思路： 
     因为需要每一对数字都相邻，那么不重复的数字一定在奇数位，因此可以从头枚举每一个不重复的数字，记录已经配好对的数字，用vis数组标记，然后对还未配对的数字进行第二次枚举，进行下标作差即可；
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
int s[1005],vis[105];
int main()
{
    int n;
    while(cin>>n)
    {
        for(int i=1;i<=2*n;i++)
        {
            cin>>s[i];
            vis[s[i]]=0;
        }
        int ans=0;
        for(int i=1;i<=2*n;i++)
        {
            if(vis[s[i]])
              continue;
            int cnt=0;
            for(int j=i+1;j<=2*n;j++)
            {
                if(vis[s[j]])
                  cnt++;
                if(s[j]==s[i])
                  ans+=j-i-1-cnt;
            }
            vis[s[i]]=1;
        }
        cout<<ans<<endl;
    } 
    return 0;
}
```
