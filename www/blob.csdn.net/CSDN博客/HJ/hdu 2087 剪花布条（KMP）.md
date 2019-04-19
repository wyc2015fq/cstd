# hdu  2087  剪花布条（KMP） - HJ - CSDN博客
2017年01月18日 13:42:55[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：202
Problem Description 
一块花布条，里面有些图案，另有一块直接可用的小饰条，里面也有一些图案。对于给定的花布条和小饰条，计算一下能从花布条中尽可能剪出几块小饰条来呢？
Input 
输入中含有一些数据，分别是成对出现的花布条和小饰条，其布条都是用可见ASCII字符表示的，可见的ASCII字符有多少个，布条的花纹也有多少种花样。花纹条和小饰条不会超过1000个字符长。如果遇见#字符，则不再进行工作。
Output 
输出能从花纹布中剪出的最多小饰条个数，如果一块都没有，那就老老实实输出0，每个结果之间应换行。
Sample Input
abcde a3 
aaaaaa  aa 
#
Sample Output
0 
3
Author 
qianneng
Source 
冬练三九之二
Recommend 
lcy
统计出现次数，注意重复出现的时候前后两次不能重叠部分。
```cpp
#include<iostream>
#include<cstdio>
#include<cstring> 
using namespace std;
int cnt,n,m,last,nxt[1005];
char S[1005],T[1005];
void getNext()
{
    memset(nxt,0,sizeof(nxt));
    for(int i=1;i<m;i++)
    {
        int j=nxt[i];
        while(j&&T[i]!=T[j])
          j=nxt[j];
        nxt[i+1]=(T[i]==T[j])?j+1:0;
    }
}
void KMP()
{
    n=strlen(S);
    m=strlen(T);
    getNext();
    int j=0;
    for(int i=0;i<n;i++)
    {
        while(j&&S[i]!=T[j])
          j=nxt[j];
        if(S[i]==T[j])
          j++;
        if(j==m)
        {
            if(cnt==0)
            {
                cnt++;
                last=i;
            }
            else if(i-last>=m)    //保证不和上次一匹配的串没有重叠部分 
            {
                cnt++;
                last=i;
            }
        }
    }
}
int main()
{
    while(~scanf("%s",S))
    {
        if(S[0]=='#')
          break;
        scanf("%s",T);
        cnt=0;
        KMP();
        printf("%d\n",cnt);
    }
    return 0;
}
```
