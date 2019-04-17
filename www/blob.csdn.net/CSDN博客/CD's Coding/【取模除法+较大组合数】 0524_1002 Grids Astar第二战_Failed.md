# 【取模除法+较大组合数】 0524_1002.Grids  Astar第二战_Failed - CD's Coding - CSDN博客





2014年05月24日 17:43:04[糖果天王](https://me.csdn.net/okcd00)阅读数：1036










# Grids
**Time Limit: 10000/5000 MS (Java/Others)    Memory Limit: 65535/65535 K (Java/Others)Total Submission(s): 0    Accepted Submission(s): 0**



Problem Description


　　度度熊最近很喜欢玩游戏。这一天他在纸上画了一个2行N列的长方形格子。他想把1到2N这些数依次放进去，但是为了使格子看起来优美，他想找到使每行每列都递增的方案。不过画了很久，他发现方案数实在是太多了。度度熊想知道，有多少种放数字的方法能满足上面的条件？






Input


　　第一行为数据组数T(1<=T<=100000)。
　　然后T行，每行为一个数N(1<=N<=1000000)表示长方形的大小。






Output


　　对于每组数据，输出符合题意的方案数。由于数字可能非常大，你只需要把最后的结果对1000000007取模即可。






Sample Input


2
1
3






Sample Output


Case #1:
1
Case #2:
5


*Hint*

对于第二组样例，共5种方案，具体方案为：
![](http://acm.hdu.edu.cn/data/images/C497-2003-1.jpg)






Wrong Answer Code:(大数取模 除法方面神烦)



```cpp
#include<cmath>
#include<cstdio>
#include<string>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

long long lim=1000000007;
//   (a / b) % m = ( a % (m*b)) / b 
//    ( (a%m) * (b^(-1)%m ) % m   both_prime
long long answer(long long N)
{
	long long b_n1=41666667;
	//int mod24=0;
	long long ans1=N%lim;
	long long ans2=(ans1*N)%lim;
	long long ans3=(ans2*N)%lim;
	long long ans4=(ans3*N)%lim;
	long long ret=( ( (9*ans4)%lim -(74*ans3)%lim 
					+(243*ans2)%lim-(322*ans1)%lim+168 )*(b_n1) ) %lim   ;
	return ret;
}

int main()
{
	int T=0;
	long long N=0;
	scanf("%d",&T);
	for(int i=1;i<=T;i++) 
	{
		scanf("%lld",&N);
		printf("Case #%d:\n%lld\n",i,answer(N));
	}
	
	return 0;
}
```



【取模除法】：From _小秋 xuhanqiu







通用公式:                 (a / b) % m = ( a % (m*b)) / b




笔者注：鉴于ACM题目特别喜欢M=1000000007,为质数：

当gcd(b,m) = 1, 有性质: (a/b)%m = (a*b^-1)%m, 其中b^-1是b模m的逆元.

求出b相对于m的逆元b^(-1)，即b*(b^(-1)) = 1 (mod m)。有b*b^(-1) - km = 1，其中k是一整数. 用Extended Euclid算法可以求出`b^(-1)。然后计算a*b^(-1) mod m = ( (a%m) * (b^(-1)%m ) % m; 其值与(a/b) mod m相同

推导：a/b = x (mod m) --两边同乘一个数--> a = bx (mod m) ---x=b^-1a-> a = (b^-1) ba (mod m)

再利用b^-1*b = 1(mod m) . 所以可以得出 x = b^-1*a是成立的。

所以 (a/b) mod m 的解与 (a*b^-1)%m的解是一样的。 而后着可以利用模对乘法的线性性





 Idea From _YCZ：C(2n,n)/(n+1)



Now I am AC:



```cpp
#include<cmath>
#include<cstdio>
#include<string>
#include<vector>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;
typedef long long ll;
const long long M=1000000007;

ll Jval[2000001];
void Jc_init() //初始化阶乘值的数组
{
	Jval[0]=1;
	for(int i=1;i<=2000000;i++)
	Jval[i]=(i*Jval[i-1])%M;
}

/*************TOOSLOW!!!*********************
ll _Ni(ll x)
{
	ll _ni;
	int cnt=0;
	while(1)
	{
		_ni=cnt*M+1;
		cnt++;
		if(_ni%x == 0)
		{
	//		cout<<"_ni/x \t" <<_ni/x <<endl;
			return _ni/x;
		}
	}
}
********************************************/

//三元组gcd(a,b) == ax +by == d;   
 struct gcdstruct{     //定义结构体存储（For _Ni的计算）
        ll d;  
        ll x;  
        ll y;  
};  
gcdstruct EXTENDED_EUCLID(ll a,ll b)  //EUCLID算法
{  
    gcdstruct aa,bb;  
    if(b==0){  
             aa.d = a;  
             aa.x = 1;  
             aa.y = 0;  
             return aa;  
            }  
    else{  
         bb = EXTENDED_EUCLID(b,a%b);   
         aa.d = bb.d;  
         aa.x = bb.y;  
         aa.y = bb.x - bb.y * (a/b);  
         }  
    return aa;  
}   
    
ll _Ni(ll a,ll m)  //获得a对m的模逆元
 {  
      ll x;  
      gcdstruct aa;  
      aa = EXTENDED_EUCLID(a,m);  
      return aa.x;  
}     


ll C(ll N)//Jval[2N]/Jval[N]/Jval[N] //获得C（2n,n）的组合数结果
{ 
	ll Cans=	(	Jval[2*N]*_Ni(Jval[N],M)   ) %M;
		Cans=	 (	Cans *_Ni(Jval[N],M)   ) %M;
//		cout<<"Jval[2N]:"<<Jval[2*N]<<endl;
//		cout<<"Jval[N]:"<<Jval[N]<<endl;
//	cout<<"Cans: \t"<<Cans<<endl; 
	return Cans; 
}


ll solve(ll N)
{
	ll answer=C(N)%M *_Ni(N+1,M) ;
	return (answer%M+M) % M;
}

int main()
{
	int T=0;
	ll N=0;
	Jc_init();
	scanf("%d",&T);
	for(int cnt=1;cnt<=T;cnt++) 
	{
		scanf("%lld",&N);
		printf("Case #%d:\n%lld\n",cnt,solve(N));
	}
	
	return 0;
}
```







