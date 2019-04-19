# hdu  1711 Number Sequence（KMP） - HJ - CSDN博客
2017年01月18日 13:40:11[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：250
Problem Description 
Given two sequences of numbers : a[1], a[2], …… , a[N], and b[1], b[2], …… , b[M] (1 <= M <= 10000, 1 <= N <= 1000000). Your task is to find a number K which make a[K] = b[1], a[K + 1] = b[2], …… , a[K + M - 1] = b[M]. If there are more than one K exist, output the smallest one.
Input 
The first line of input is a number T which indicate the number of cases. Each case contains three lines. The first line is two numbers N and M (1 <= M <= 10000, 1 <= N <= 1000000). The second line contains N integers which indicate a[1], a[2], …… , a[N]. The third line contains M integers which indicate b[1], b[2], …… , b[M]. All integers are in the range of [-1000000, 1000000].
Output 
For each test case, you should output one line which only contain K described above. If no such K exists, output -1 instead.
Sample Input
2 
13 5 
1 2 1 2 3 1 2 3 1 3 2 1 2 
1 2 3 1 3 
13 5 
1 2 1 2 3 1 2 3 1 3 2 1 2 
1 2 3 2 1
Sample Output
6 
-1
Source 
HDU 2007-Spring Programming Contest 
题目大意： 
    给母串a和模式串b，如果b在a中出现过，求b第一次出现在a的首位置序号，如果b在a中匹配不到，输出-1即可。
KMP处理串匹配无论数字还是字符都可以。
```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
using namespace std;
#define nn 1000000+10
#define mm 10001
int a[nn],b[mm],nxt[mm],n,m; 
void getNext()
{
    memset(nxt,0,sizeof(nxt));
    for(int i=1,j=0;i<m;)
    {
        if(b[i]==b[j])
        {
            j++;
            nxt[i++]=j;
        }
        else if(j>0)
          j=nxt[j-1];
        else i++;
    }
}
int Kmp()
{
    getNext();
    int i=0,j=0;
    for(;m-j<=n-i&&j<m;)
    {
        if(a[i]==b[j])
        {
            i++;
            j++;
        }
        else if(j>0)
          j=nxt[j-1];
        else i++;
    }
    if(j==m)
      return i-m+1;
    return -1;
}
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d %d",&n,&m);
        for(int i=0;i<n;i++)
          scanf("%d",&a[i]);
        for(int i=0;i<m;i++)
          scanf("%d",&b[i]);
        printf("%d\n",Kmp()); 
    }
    return 0; 
}
```
