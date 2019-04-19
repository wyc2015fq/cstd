# hdu   5536  Chip Factory（01字典树） - HJ - CSDN博客
2017年01月16日 00:49:52[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：244
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------字符串---------																[字典树](https://blog.csdn.net/feizaoSYUACM/article/category/7238010)](https://blog.csdn.net/feizaoSYUACM/article/category/7238090)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
John is a manager of a CPU chip factory, the factory produces lots of chips everyday. To manage large amounts of products, every processor has a serial number. More specifically, the factory produces n chips today, the i-th chip produced this day has a serial number si.
At the end of the day, he packages all the chips produced this day, and send it to wholesalers. More specially, he writes a checksum number on the package, this checksum is defined as below:
```
maxi,j,k(si+sj)⊕sk
```
which i,j,k are three different integers between 1 and n. And ⊕ is symbol of bitwise XOR.
Can you help John calculate the checksum number of today?
Input 
The first line of input contains an integer T indicating the total number of test cases.
The first line of each test case is an integer n, indicating the number of chips produced today. The next line has n integers s1,s2,..,sn, separated with single space, indicating serial number of each chip.
1≤T≤1000 
3≤n≤1000 
0≤si≤109 
There are at most 10 testcases with n>100
Output 
For each test case, please output an integer indicating the checksum number in a line.
Sample Input
2 
3 
1 2 3 
3 
100 200 300
Sample Output
6 
400
Source 
2015ACM/ICPC亚洲区长春站-重现赛（感谢东北师大） 
解法：01字典树，把十进制用二进制表示，构造一棵01字典树，暴力枚举a[i]+a[j]，寻找异或最大即可
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
const int N=100010;
struct node{
    int ch[2],size;
}T[N];
int root=1,tot=1,a[N],t,n;
void Insert(int x)         //插入 
{
    int o=root;
    T[o].size++;
    for(int k=30;k>=0;k--)
    {
        int c=(x&(1<<k))?1:0;
        if(!T[o].ch[c])
          T[o].ch[c]=++tot;
        o=T[o].ch[c];
        T[o].size++;
    }   
}
void Delete(int x)         //删除 
{
    int o=root;
    T[o].size--;
    for(int k=30;k>=0;k--)
    {
        int c=(x&(1<<k))?1:0;
        o=T[o].ch[c];
        T[o].size--;
    }
}
int Query(int x)            //查询 
{
    int o=root;
    for(int k=30;k>=0;k--)
    {
        int c=(x&(1<<k))?1:0;     //0或者是1两种情况 
        if(c)
        {
            if(T[o].ch[0]&&T[T[o].ch[0]].size)    //因为是xor,因此需要反着处理，下同 
              o=T[o].ch[0];
            else
            {
                o=T[o].ch[1];
                x^=(1<<k);
            }
        } 
        else
        {
            if(T[o].ch[1]&&T[T[o].ch[1]].size)
            {
                o=T[o].ch[1];
                x^=(1<<k);
            }
            else
              o=T[o].ch[0];
        }
    }
    return x;
}
int main()
{
    scanf("%d",&t);
    while(t--)
    {
        int ans=0;
        scanf("%d",&n);
        for(int i=1;i<=n;i++)
          scanf("%d",&a[i]);
        for(int i=1;i<=n;i++)
          Insert(a[i]);
        for(int i=1;i<=n;i++)
        {
            Delete(a[i]);
            for(int j=i+1;j<=n;j++)
            {
                Delete(a[j]);
                ans=max(ans,Query(a[i]+a[j]));     //暴力枚举a[i]+a[j]，不断插入删除，知道ans最大 
                Insert(a[j]);
            }
            Insert(a[i]);
        }
        printf("%d\n",ans);
        for(int i=1;i<=tot;i++)
          T[i].ch[0]=T[i].ch[1]=T[i].size=0;
        tot=1;
    }
    return 0;
}
```
