# ZOJ3785 What day is that day?  快速幂+找规律 - 紫芝的博客 - CSDN博客





2018年05月13日 12:26:02[紫芝](https://me.csdn.net/qq_40507857)阅读数：47










[点击打开链接](https://cn.vjudge.net/contest/228540#problem/O)


It's Saturday today, what day is it after 11 + 22 + 33 + ... + NN days? 




## Input


There are multiple test cases. The first line of input contains an integer T indicating the number of test cases. For each test case:

 There is only one line containing one integer N (1 <= N <= 1000000000). 


## Output


 For each test case, output one string indicating the day of week. 
Sample Input2
1
2
Sample OutputSunday
Thursday
Hint
 A week consists of Sunday, Monday, Tuesday, Wednesday, Thursday, Friday and Saturday. 



## 题意：


今天星期六，求1^1+2^2……N^N天后是星期几



## 思路：

同余与模算术，利用快速幂取模的算法，时间复杂度为O(logn)。


1.先用快速幂求出11 ， 22 +，33 ， ... ，NN       


对7取模之后的结果，发现循环节长度为42，即 (1^1)%7=(43^43)%7, (2^2)%7=(44^44)%7, (3^3)%7=(45^45)%7,

(n^n)%7=( (42+n)^(42+n) )%7




```cpp
#include<bits/stdc++.h>
using namespace std;
int fast_pow(int a,int n,int mod)
{
    int ret=1;
    while(n)
    {
        if(n&1)
            ret=ret*a%mod;
        a=a*a%mod;
        n>>=1;
    }
    return ret;
}
int main()
{
    for(int i=1;i<=1000;i++)
    {
        printf("%d,",fast_pow(i,i,7));
        if(i%7==0)
            printf("\n");
    }
    return 0;
}
//以42为一组
```
1,4,6,4,3,1,0,
1,1,4,2,1,6,0,
1,2,5,1,5,1,0,
1,4,1,4,4,6,0,
1,1,3,2,6,1,0,
1,2,2,1,2,6,0,

1,4,6,4,3,1,0,
1,1,4,2,1,6,0,
1,2,5,1,5,1,0,
1,4,1,4,4,6,0,
1,1,3,2,6,1,0,
1,2,2,1,2,6,0,

1,4,6,4,3,1,0,
1,1,4,2,1,6,0,
1,2,5,1,5,1,0,
1,4,1,4,4,6,0,
1,1,3,2,6,1,0,
1,2,2,1,2,6,0,

1,4,6,4,3,1,0,
1,1,4,2,1,6,0,
1,2,5,1,5,1,0,
1,4,1,4,4,6,0,
1,1,3,2,6,1,0,
1,2,2,1,2,6,0,

1,4,6,4,3,1,0,
1,1,4,2,1,6,0,
1,2,5,1,5,1,0,
1,4,1,4,4,6,0,
1,1,3,2,6,1,0,
1,2,2,1,2,6,0,

1,4,6,4,3,1,0,
1,1,4,2,1,6,0,
1,2,5,1,5,1,0,
1,4,1,4,4,6,0,
1,1,3,2,6,1,0,
1,2,2,1,2,6,0,

1,4,6,4,3,1,0,
1,1,4,2,1,6,0,
1,2,5,1,5,1,0,
1,4,1,4,4,6,0,
1,1,3,2,6,1,0,
1,2,2,1,2,6,0,

1,4,6,4,3,1,0,
1,1,4,2,1,6,0,
1,2,5,1,5,1,0,
1,4,1,4,4,6,0,
1,1,3,2,6,1,0,
1,2,2,1,2,6,0,


2.然后打表求出[1,42]区间每个数n的(n^n)%7,再求a数组的前缀和b数组，

sum表示一个循环节所贡献的天数，即sum=（1^1+2^2+3^3+......+41^41+41^42）%7=6;

对于每一个样例n，直接计算即可





## AC代码：

```cpp
#include<bits/stdc++.h>
using namespace std;
char s[10][10]={"Saturday","Sunday","Monday","Tuesday","Wednesday","Thursday","Friday"};
int fast_pow(int a,int n,int mod)//快速幂取模
{
    int ret=1;
    while(n){
        if(n&1)
            ret=ret*a%mod;
        a=a*a%mod;
        n>>=1;
    }
    return ret;
}
int a[50];//a[n]表示(n^n)%7
int b[50];//b[n]表示a[1]+a[2]+....+a[n]
int main()
{
	ios::sync_with_stdio(0);
	b[0]=0;

    for(int i=1;i<=42;i++)//循环节为42
    {
        a[i]=fast_pow(i,i,7);
        b[i]=b[i-1]+a[i];
    }
    int t;
    cin>>t;
    while(t--){
        int n;
        cin>>n;
        int sum=b[42]%7;//一个循环节贡献的天数
        int num=n/42;//循环节的个数
        int ans=(sum*num%7+b[n%42]%7)%7;
         printf("%s\n",s[ans]);
    }
	return 0;
}
```
另一种计算sum的方法：

```cpp
#include<bits/stdc++.h>
using namespace std;
char s[10][10]={"Saturday","Sunday","Monday","Tuesday","Wednesday","Thursday","Friday"};
int fast_pow(int a,int n,int mod)//快速幂取模
{
    int ret=1;
    while(n){
        if(n&1)
            ret=ret*a%mod;
        a=a*a%mod;
        n>>=1;
    }
    return ret;
}
int a[50];//a[n]表示(n^n)%7
int b[50];//b[n]表示a[1]+a[2]+....+a[n]
int main()
{
	ios::sync_with_stdio(0);
	b[0]=0;
	int sum=0;
    for(int i=1;i<=42;i++)//循环节为42
    {
        a[i]=fast_pow(i,i,7);
        b[i]=b[i-1]+a[i];
        sum=(sum%7+a[i]%7)%7;//重点理解
    }
    int t;
    cin>>t;
    while(t--){
        int n;
        cin>>n;
        int ans=(sum*(n/42)%7+b[n%42]%7)%7;
         printf("%s\n",s[ans]);
    }
	return 0;
}
```




