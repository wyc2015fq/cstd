# UVALive 7270  Osu! Master（简单水题） - HJ - CSDN博客
2017年10月27日 13:12:37[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：213
[https://icpcarchive.ecs.baylor.edu/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=5282](https://icpcarchive.ecs.baylor.edu/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=5282)
题目大意： 
如果前面一个字符为“B”或者“C”，后面就会接一个正整数x，x值从1连续增长到k的序列视为一个pattern，单独的一个S视为一个pattern。 
统计序列中pattern的总数。
解题思路： 
刚开始还在想怎么判断连续增长，最后突然发现真是智障，只要判断多少个1就行了。。然后加上“S”的个数。
```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
using namespace std;
int main()
{
    int n;
    while(cin>>n)
    {
        int ans=0,a;
        char c;
        for(int i=1;i<=n;i++)
        {
            cin>>c;
            if(c=='S')
              ans++;
            else
            {
                cin>>a;
                if(a==1)
                  ans++;
            }
        }
        printf("%d\n",ans);
    }
    return 0;
}
```
