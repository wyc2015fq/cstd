# 【模逆元】大组合数模运算 _CDTemplate - CD's Coding - CSDN博客





2015年04月22日 15:43:29[糖果天王](https://me.csdn.net/okcd00)阅读数：713








### ![](https://img-blog.csdn.net/20150422154118206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 制作起因：

便签纸上一直有这一行

【>整理模逆元 exgcd / 大数同余模运算】

今天嵌入式专业课一狠心一咬牙，把它完成！给它打上勾！于是—— 还真被我弄出来了……




### 模板来源：

依照以前Astar第二场的AC代码敲了半天，捣鼓出来一个大数组合数模运算代码，提供外接接口，可用部分包括：

求模逆元，扩展GCD，大数同余模运算，大数组合数计算。（转载请注明[okcd00](blog.csdn.net/okcd00)哟~）

### 参数说明：

T：数据组数

lim:：取模的模值

Jval[x]：x的阶乘模lim后的值


n：组合数的下标

m：组合数的上标

_Ni（x，m）：x对于m的模逆元

C（n，m）：下标为n，上标为m的组合数




### Code：



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
const ll lim=1000000007;  //lim := vmod
  
ll Jval[2000001];
void Jc_init() //初始化阶乘值的数组  
{  
    Jval[0]=1;  
    for(int i=1;i<=2000000;i++)  
    Jval[i]=(i*Jval[i-1])%lim;  
}  
  
//三元组gcd(a,b) == ax +by == d;     
struct gcdstruct //定义结构体存储(_Ni的计算)
{     
        ll d;    
        ll x;    
        ll y;    
};    
gcdstruct EXTENDED_EUCLID(ll a,ll b)  //EUCLID算法  
{    
    gcdstruct aa,bb;    
    if(b==0)
	{
    	aa.d = a;    
    	aa.x = 1;    
    	aa.y = 0;    
        return aa;    
    }    
    else
	{    
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
  
  
ll C(ll n,ll m) //获得C(n,m) 的组合数结果 := n!/((n-m)!*m!)
{   
    ll	Cans=	(	Jval[n]*_Ni(Jval[m],lim) +lim	) %lim;  // Get n!/m! 
    	Cans=	(	Cans*_Ni(Jval[n-m],lim) +lim   ) %lim;  	// Get Cans/(n-m)! 
    return (Cans+lim)%lim ;   
}  

int main()  
{  
    int T=0;	scanf("%d",&T); 
    ll n=0,m=0;		Jc_init();   
    for(int cnt=1;cnt<=T;cnt++)   
    {  
        scanf("%I64d%I64d",&n,&m);  
        printf("Case #%d: \n",cnt);
        cout<<"===<Jval>===\n n!="<<Jval[n]<<"; m!="<<Jval[m]<<endl;
        cout<<"===<_Ni>===\n m!^-1="<<_Ni(Jval[m],lim)
        			<<"; (n-m)!^-1="<<_Ni(Jval[n-m],lim)<<endl;
        printf("C(%d,%d) = %I64d (Mod lim)\n",n,m,C(n,m));  
    }  
      
    return 0;  
}
```





