# hdu 3068 最长回文（Manancher 算法） - HJ - CSDN博客
2017年01月18日 01:43:45[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：828
Problem Description 
给出一个只由小写英文字符a,b,c…y,z组成的字符串S,求S中最长回文串的长度. 
回文就是正反读都是一样的字符串,如aba, abba等
Input 
输入有多组case,不超过120组,每组输入为一行小写英文字符a,b,c…y,z组成的字符串S 
两组case之间由空行隔开(该空行不用处理) 
字符串长度len <= 110000
Output 
每一行一个整数x,对应一组case,表示该组case的字符串中所包含的最长回文长度.
Sample Input
aaaa
abab
Sample Output
4 
3
比较好理解，随便找一个博客看看就会弄懂了，如下代码Manacher可做模板：
```
#include<iostream>
#include<cstring>
#include<cstdio>
using namespace std;
char s[10000005];
int len,p[10000005];
int Manacher()
{
    int ans=0,mx=0,id=0;
    p[0]=1;
    for(int i=1;i<len;i++)
    {
        p[i]=p[id]+id>i?min(p[2*id-i],p[id]+id-i):1;
        while(s[i+p[i]]==s[i-p[i]])
          p[i]++;
        id=i+p[i]>id+p[id]?i:id;
        ans=max(ans,p[i]);
    }
    return ans-1;
}
int main()
{
    while(scanf("%s",s)!=EOF)
    {
        len=strlen(s);
        for(int i=len;i>=0;i--)   //插入了len+1个'#',最终的s长度是1~(len+len+1)即2*len+1,首尾s[0]和s[2*len+2]要插入不同的字符 
        {
            s[i*2+2]=s[i];
            s[i*2+1]='#';
        }
        s[0]='*';    //s[0]='*',s[len*2+2]='\0',防止在while时p[i]越界 
        len=len*2+2;
        printf("%d\n",Manacher()); 
    }
    return 0;
}
```
