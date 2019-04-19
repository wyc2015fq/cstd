# FZU  2275 Game（KMP） - HJ - CSDN博客
2017年08月23日 16:19:48[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：221
[http://acm.fzu.edu.cn/problem.php?pid=2275](http://acm.fzu.edu.cn/problem.php?pid=2275)
题目大意： 
A、B两个人玩一个游戏，现在他们各有一个数，并且分别都有两种选择：一种是把数反转，另一种是把数整除10。如果A、B两个人在某一次操作中数相同了，那么A就算获胜。假设两个人足够聪明，问A有没有必胜的把握？
解题思路： 
把两个数当做字符串来处理，判断B串是否为A串的一个子串，并且再判断一次B串的反串是否为A串的一个子串。综合考虑一下。
```cpp
#include<iostream>
#include<algorithm>
#include<string>
#include<cstdio>
#include<cstring>
using namespace std;
#define maxn 1000005
char a[maxn],b[maxn],c[maxn];
int nxt[maxn],n,m;
void getNext()
{
    memset(nxt,0,sizeof(nxt));
    for(int i=1;i<m;i++)
    {
        int j=nxt[i];
        while(j&&b[i]!=b[j])
          j=nxt[j];
        nxt[i+1]=(b[i]==b[j])?j+1:0;
    }
}
int KMP()
{
    n=strlen(a);
    m=strlen(b);
    getNext();
    for(int i=0,j=0;i<n;i++)
    {
        while(j&&a[i]!=b[j])
          j=nxt[j];
        if(a[i]==b[j])
          j++;
        if(b[j-1]=='0')
        {
            if(i!=n-1&&j==m)
              return 1;
        }
        else
        {
            if(j==m)
              return 1;
        }
    }
    return 0;
} 
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        cin>>a>>b;
        int ans=KMP();         //查找A串中是否存在B串一样的子串 
        if(ans==0)
        {
            for(int i=0,j=m-1;i<m;i++,j--)
              c[i]=b[j];
            int k=0,p=0;
            for(;k<m;k++)
              if(c[k]!='0')
                break;
            for(;k<m;k++,p++)
              b[p]=c[k];
            b[p]='\0';
            ans=KMP();                    //查找A串中是否存在B串反转之后一样的子串 
            if(ans==0)
              cout<<"Bob"<<endl;
            else
              cout<<"Alice"<<endl;
        }
        else
          cout<<"Alice"<<endl;
    } 
    return 0;
}
```
