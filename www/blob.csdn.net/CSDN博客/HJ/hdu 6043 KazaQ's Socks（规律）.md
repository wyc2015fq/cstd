# hdu  6043 KazaQ's Socks（规律） - HJ - CSDN博客
2017年08月01日 18:43:06[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：233
Problem Description 
KazaQ wears socks everyday.
At the beginning, he has n pairs of socks numbered from 1 to n in his closets.
Every morning, he puts on a pair of socks which has the smallest number in the closets.
Every evening, he puts this pair of socks in the basket. If there are n−1 pairs of socks in the basket now, lazy KazaQ has to wash them. These socks will be put in the closets again in tomorrow evening.
KazaQ would like to know which pair of socks he should wear on the k-th day.
Input 
The input consists of multiple test cases. (about 2000)
For each case, there is a line contains two numbers n,k (2≤n≤109,1≤k≤1018).
Output 
For each test case, output “Case #x: y” in one line (without quotes), where x indicates the case number starting from 1 and y denotes the answer of corresponding case.
Sample Input
3 7 
3 6 
4 9
Sample Output
Case #1: 3 
Case #2: 1 
Case #3: 2
题目大意： 
有一个人他有n双袜子，但是他很懒，总是要等到第二天只有一双袜子穿的时候，这一天晚上才会把n-1双袜子全部洗掉。已知他每天都从箱子里拿标号最小的袜子，问k天之后，他应该穿标号为多少的袜子？
解题思路： 
知道规律是1,2,3,…,n,1,2,3,…,n-1,1,2,3,….,n循环的那一刻我的内心是崩溃的。
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
int main()
{
    long long n,k,ans,tt=0;
    while(cin>>n>>k)
    {
        if(n>=k)
          ans=k;
        else
        {
            k-=n;
            int p=k/(n-1),q=k%(n-1);
            if(p%2)
              ans=q==0?n-1:q;
            else
              ans=q==0?n:q;
        }
        printf("Case #%lld: %lld\n",++tt,ans);
    }
    return 0;
}
```
