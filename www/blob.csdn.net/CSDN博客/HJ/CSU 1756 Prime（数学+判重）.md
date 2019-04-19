# CSU  1756  Prime（数学+判重） - HJ - CSDN博客
2017年02月02日 01:50:31[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：254
如果a,b的最大公约数是1，说明a,b是一对互素的数，给定你n个数字，希望你找出互素的数的对数 
Input
第一行输入一个正整数T，表示数据组数
每组数据第一行输入一个正整数n，表示数字的个数(n<=10000)
接下来一行输入n个正整数，每个数字大小不超过1000。 
Output
输出互素的数的对数 
Sample Input
1 
4 
10 9 6 35
Sample Output
3
解题思路： 
本题主要的关键点是如何去重，因为数据量有10000之多但是数据大小只有1000以内，因此排序去掉重复的数字用数组标记一下，然后就可以在1000以内开两个循环暴力解决互素问题了。
```cpp
#include<bits/stdc++.h>
using namespace std;
int s[1000005],num[1005];     //num[] 累计s[]中重复出现某些数据的个数 
int gcd(int a,int b)
{
    return a%b==0?b:gcd(b,a%b);
}
int main()
{
    int t,n;
    cin>>t;
    while(t--)
    {
        cin>>n;
        memset(num,0,sizeof(num));
        for(int i=0;i<n;i++)
          cin>>s[i];
        sort(s,s+n);
        int k=1,sum=0;
        num[s[0]]++;
        for(int i=1;i<n;i++)
        {
            if(s[i]!=s[i-1])
              s[k++]=s[i];
            num[s[i]]++;
        }
        for(int i=0;i<k;i++)
          for(int j=0;j<=i;j++)
            if(gcd(s[i],s[j])==1&&s[i]!=s[j])
              sum+=(long long)num[s[i]]*num[s[j]];     //如果s[i]和s[j]互素，把两个数重复出现的数量相乘累加到sum 
            else if(s[i]==1&&num[s[i]]>1)
              sum+=num[s[i]]*(num[s[i]]-1)/2;     //额外考虑1的情况 
        cout<<sum<<endl;  
    }
    return 0;
}
```
