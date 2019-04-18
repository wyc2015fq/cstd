# HDU  4407  Sum 第37届ACM/ICPC 金华赛区 第1008题 （容斥原理） - weixin_33985507的博客 - CSDN博客
2012年09月22日 22:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
# Sum
**Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 358    Accepted Submission(s): 109**
Problem Description
XXX is puzzled with the question below: 
1, 2, 3, ..., n (1<=n<=400000) are placed in a line. There are m (1<=m<=1000) operations of two kinds.
Operation 1: among the x-th number to the y-th number (inclusive), get the sum of the numbers which are co-prime with p( 1 <=p <= 400000).
Operation 2: change the x-th number to c( 1 <=c <= 400000).
For each operation, XXX will spend a lot of time to treat it. So he wants to ask you to help him.
Input
There are several test cases.
The first line in the input is an integer indicating the number of test cases.
For each case, the first line begins with two integers --- the above mentioned n and m.
Each the following m lines contains an operation.
Operation 1 is in this format: "1 x y p". 
Operation 2 is in this format: "2 x c".
Output
For each operation 1, output a single integer in one line representing the result.
Sample Input
1 3 3 2 2 3 1 1 3 4 1 2 3 6
Sample Output
7 0
Source
[2012 ACM/ICPC Asia Regional Jinhua Online](http://acm.hdu.edu.cn/search.php?field=problem&key=2012%20ACM/ICPC%20Asia%20Regional%20Jinhua%20Online&source=1&searchmode=source)
Recommend
zhoujiaqi2010
**把p进行质因子分解。然后按照容斥原理来求和p不互素的和。**
**用map记录修改的值。**
**具体看代码。**
**坑爹啊。。。比赛没有状态。。。。结束后一写就AC了。。。**
**看来以后我应该写这类题目**
**求明天RP爆发**
```
//1008
#include<stdio.h>
#include<iostream>
#include<map>
#include<set>
#include<algorithm>
#include<string.h>
#include<stdlib.h>
using namespace std;
int gcd(int a,int b)
{
    if(b==0)return a;
    return gcd(b,a%b);
}
long long sum1(int x,int y,int p)//求区间[x,y]中p的倍数的和
{
    if(p>y)return 0;
    int t1=x/p;
    int t2=y/p;
    if(t1*p<x)t1++;
    if(t2<t1)return 0;
    long long sum=0;
    sum=(long long)p*(t1+t2)*(t2-t1+1)/2;
    return sum;
}
const int MAXN=400000;
int prime[MAXN+1];
int getPrime()//得到小于等于MAXN的素数，prime[0]存放的是个数
{
    memset(prime,0,sizeof(prime));
    for(int i=2;i<=MAXN;i++)
    {
        if(!prime[i]) prime[++prime[0]]=i;
        for(int j=1;j<=prime[0]&&prime[j]<=MAXN/i;j++)
        {
            prime[prime[j]*i]=1;
            if(i%prime[j]==0) break;
        }
    }
    return prime[0];
}
long long factor[100][2];
int facCnt;
int getFactors(long long x)//把x进行素数分解
{
    facCnt=0;
    long long tmp=x;
    for(int i=1;prime[i]<=tmp/prime[i];i++)
    {
        factor[facCnt][1]=0;
        if(tmp%prime[i]==0)
        {
            factor[facCnt][0]=prime[i];
            while(tmp%prime[i]==0)
            {
                   factor[facCnt][1]++;
                   tmp/=prime[i];
            }
            facCnt++;
        }
    }
    if(tmp!=1)
    {
        factor[facCnt][0]=tmp;
        factor[facCnt++][1]=1;
    }
    return facCnt;
}
long long SS(int s,int x,int y)
//求[x,y]之间是素因子倍数的和。素因子的状态是s,0表示没有这个素因子，
//1表示有。容斥原理。先不管加减，先奇数加，偶数加，最后按照正负去调整就好了。
{
    long long ans=0;
    int cnt=0;
    int p=1;
    for(int i=0;i<facCnt;i++)
    {
        if(s&(1<<i))
        {
            cnt++;
            p*=factor[i][0];
        }
    }
    ans=sum1(x,y,p);
    if(cnt%2)ans=-ans;
    return ans;
}
long long solve1(int x,int y,int p)//求[x,y]之间和p不互素的数的和
{
    getFactors(p);
    long long ans=0;
    for(int i=1;i<(1<<facCnt);i++)
      ans+=SS(i,x,y);
    if(ans<0)ans=-ans;
    return ans;
}
map<int,int>mp;
map<int,int>::iterator it;
long long query(int x,int y,int p)//查询。改变被修改了的就可以了
{
    long long ans=(long long)(x+y)*(y-x+1)/2;
    long long temp=solve1(x,y,p);
    ans-=temp;
    for(it=mp.begin();it!=mp.end();it++)
    {
        int t1=it->first;
        if(t1<x||t1>y)continue;
        int t2=it->second;
        if(gcd(t1,p)==1)ans-=t1;//本来互素的要减掉
        if(gcd(t2,p)==1)ans+=t2;//修改后互素的要加上
    }
    return ans;
}
int main()
{
    getPrime();
   // freopen("H.in","r",stdin);
   // freopen("H.out","w",stdout);
    int T;
    int n,m;
    int x,y,p;
    int t;
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d%d",&n,&m);
        mp.clear();
        while(m--)
        {
            scanf("%d",&t);
            if(t==1)
            {
                scanf("%d%d%d",&x,&y,&p);
                if(x>y)swap(x,y);
                printf("%I64d\n",query(x,y,p));
            }
            else
            {
                scanf("%d%d",&x,&p);
                mp[x]=p;
            }
        }
    }
    return 0;
}
```
