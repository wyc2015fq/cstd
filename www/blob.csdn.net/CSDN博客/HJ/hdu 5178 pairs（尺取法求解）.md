# hdu  5178  pairs（尺取法求解） - HJ - CSDN博客
2016年08月08日 22:56:51[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：500
pairs
Problem Description  
John has n points on the X axis, and their coordinates are (x[i],0),(i=0,1,2,…,n−1). He wants to know how many pairs 《a,b》 that |x[b]−x[a]|≤k.(a小于b)
Input  
The first line contains a single integer T (about 5), indicating the number of cases.  
Each test case begins with two integers n,k(1≤n≤100000,1≤k≤109).  
Next n lines contain an integer xi, means the X coordinates.
Output  
For each case, output an integer means how many pairs《a,b》 that |x[b]−x[a]|≤k.
Sample Input  
2  
5 5  
-100  
0  
100  
101  
102  
5 300  
-100  
0  
100  
101  
102
Sample Output  
3  
10
解析：本题还除了用二分还可以用尺取法求解，注意超时，开一个下标变量从1一次性走到n，因为可以利用每次循环过的以前的条件，比如3~6满足条件，当i走到4的时候，j就不必要从5开始而是直接从7开始走了。
```cpp
#include<iostream>
#include<cstdio> 
#include<cmath>
#include<algorithm>
using namespace std;
long long a[100005];
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        long long n,k,cnt=0,r,l,sum,mid=2;
        scanf("%lld %lld",&n,&k);
        for(int i=1;i<=n;i++)
          scanf("%lld",&a[i]);
        sort(a+1,a+n+1);
        for(int i=1,j=1;i<=n;i++)        //变量j从1走到n，时间复杂度为O（n）
        {
            while(a[j+1]-a[i]<=k)   //循环条件
            {
                if(j+1>n)     //边界判断
                  break;
                j++;
            }
            cnt+=j-i;       //计数变量逐次递加
        }
        printf("%lld\n",cnt);
    }
    return 0;
}
```
