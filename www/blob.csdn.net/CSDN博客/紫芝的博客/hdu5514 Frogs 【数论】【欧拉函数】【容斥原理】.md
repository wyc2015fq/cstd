# hdu5514 Frogs 【数论】【欧拉函数】【容斥原理】 - 紫芝的博客 - CSDN博客





2018年09月07日 23:49:26[紫芝](https://me.csdn.net/qq_40507857)阅读数：22








# Frogs

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/65536 K (Java/Others)Total Submission(s): 4159    Accepted Submission(s): 1405****

**Problem Description**

There are m stones lying on a circle, and n frogs are jumping over them.

The stones are numbered from 0 to m−1 and the frogs are numbered from 1 to n. The i-th frog can jump over exactly ai stones in a single step, which means from stone j mod m to stone (j+ai) mod m (since all stones lie on a circle).


All frogs start their jump at stone 0, then each of them can jump as many steps as he wants. A frog will occupy a stone when he reach it, and he will keep jumping to occupy as much stones as possible. A stone is still considered ``occupied" after a frog jumped away.

They would like to know which stones can be occupied by at least one of them. Since there may be too many stones, the frogs only want to know the sum of those stones' identifiers.

**Input**

There are multiple test cases (no more than 20), and the first line contains an integer t,

meaning the total number of test cases.


For each test case, the first line contains two positive integer n and m - the number of frogs and stones respectively (1≤n≤104, 1≤m≤109).


The second line contains n integers a1,a2,⋯,an, where ai denotes step length of the i-th frog (1≤ai≤109).

**Output**

For each test case, you should print first the identifier of the test case and then the sum of all occupied stones' identifiers.

**Sample Input**

```
3
2 12
9 10
3 60
22 33 66
9 96
81 40 48 32 64 16 96 42 72
```

**Sample Output**

Case #1: 42 

Case #2: 1170 

Case #3: 1872

**Source**

[2015ACM/ICPC亚洲区沈阳站-重现赛（感谢东北大学）](http://acm.hdu.edu.cn/search.php?field=problem&key=2015ACM%2FICPC%D1%C7%D6%DE%C7%F8%C9%F2%D1%F4%D5%BE-%D6%D8%CF%D6%C8%FC%A3%A8%B8%D0%D0%BB%B6%AB%B1%B1%B4%F3%D1%A7%A3%A9&source=1&searchmode=source)

**Recommend**

wange2014   |   We have carefully selected several similar problems for you:  [6447](http://acm.hdu.edu.cn/showproblem.php?pid=6447)[6446](http://acm.hdu.edu.cn/showproblem.php?pid=6446)[6445](http://acm.hdu.edu.cn/showproblem.php?pid=6445)[6444](http://acm.hdu.edu.cn/showproblem.php?pid=6444)[6443](http://acm.hdu.edu.cn/showproblem.php?pid=6443)

题意：

有n只青蛙，在长度为m的环上从0开始无限次跳，第i只青蛙每次跳的长度为 a[i]，求所有被青蛙跳到的下标之和

数据范围：T⩽20; 1⩽n⩽10^4; 1⩽m⩽10^9; ai⩽10^9

分析：

格子是否被跳过和gcd（m,ai）有关，对于任意一个gcd（m，ai），其【0，m）内的倍数一定会被跳到，对这个命题进行转化就是对于一个格子x，如果存在gcd（ai，m）| x，那么x点会被跳到。由于gcd（x,m）| m其规模最大为m的约数，gcd（ai，m）| gcd（x,m）是x被跳到的充要条件。可以枚举x的约数g，让gcd（x，m）= g的数形成一个集合，通过判断gcd（ai，m）| g的存在性来判断该集合的数是否被加入。

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn=1e4+7;
int n,m;
int a[maxn];
ll getphi(int m)//欧拉函数
{
    int phi=m;
    for(int i=2;i*i<=m;i++)
    {
        if(m%i==0){
            while(m%i==0)
                m/=i;
            phi=phi/i*(i-1);
        }
    }
    return m>1?phi/m*(m-1):phi;
}
bool ck(int x)
{
    for(int i=0;i<n;i++)
    {
        if(x%a[i]==0)   return true;
    }
    return false;
}
int main()
{
    int T,ca=0;
    scanf("%d",&T);
    while(T--){
        scanf("%d%d",&n,&m);
        for(int i=0;i<n;i++)
        {
            scanf("%d",a+i);
            a[i]=__gcd(a[i],m);
        }
        ll ans=0;
     for(int i=1;i*i<=m;i++)
     {
         if(m%i)    continue;
         if(ck(i))  ans+=(ll)getphi(m/i)*m/2;
         if(i*i==m||i==1)   continue;
         if(ck(m/i))    ans+=(ll)getphi(i)*m/2;
     }
     printf("Case #%d: %lld\n",++ca,ans);
    }
}
```





