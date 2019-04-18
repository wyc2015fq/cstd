# 大整数除法O(nlogn) - 知乎
# 

高精度加法、减法均可以在 ![O(N)](https://www.zhihu.com/equation?tex=O%28N%29) 内完成（N是位数），在FFT的帮助下高精度乘法可以做到 ![O(N\log N)](https://www.zhihu.com/equation?tex=O%28N%5Clog+N%29) （或许是 ![O(N\log N \log \log N)](https://www.zhihu.com/equation?tex=O%28N%5Clog+N+%5Clog+%5Clog+N%29) ?），那么加减乘除就剩下了除法，我们可以试着把除法的复杂度降下来。

### Naive的想法1：

既然直接列竖式做除法是 ![O(N^2)](https://www.zhihu.com/equation?tex=O%28N%5E2%29) 的，那么我们是不是可以二分答案呢？

仔细想想，二分一个 ![\log ](https://www.zhihu.com/equation?tex=%5Clog+) ，乘法一个 ![\log ](https://www.zhihu.com/equation?tex=%5Clog+) ，所以复杂度是 ![O(N\log^2N)](https://www.zhihu.com/equation?tex=O%28N%5Clog%5E2N%29) ，稳稳的。

哎呀，二分的 ![\log ](https://www.zhihu.com/equation?tex=%5Clog+) 是与位数同级的（即 ![O(N)](https://www.zhihu.com/equation?tex=O%28N%29) ），到头来还是 ![O(N^2)](https://www.zhihu.com/equation?tex=O%28N%5E2%29) 的。

### Naive的想法2：

既然多项式除法可以做到 ![O(N\log N)](https://www.zhihu.com/equation?tex=O%28N%5Clog+N%29) ，那么是不是整数除法也可以看做是多项式除法呢？

不行，两者定义有差异（多项式除法要求余数度数小于被除数而整数要求余数数值上小于被除数）。

## 正确做法：

二分的问题出在哪里？要求答案达到 ![N](https://www.zhihu.com/equation?tex=N) 位精度需要二分 ![O(N)](https://www.zhihu.com/equation?tex=O%28N%29) 次，在大数据下即使是 ![\log](https://www.zhihu.com/equation?tex=%5Clog) 也会很大。比二分更快的是牛顿迭代法，同样使答案达到 ![N](https://www.zhihu.com/equation?tex=N) 位精度只需要迭代 ![O(\log N)](https://www.zhihu.com/equation?tex=O%28%5Clog+N%29) 次。

我们要求 ![x=\frac AB](https://www.zhihu.com/equation?tex=x%3D%5Cfrac+AB) ，可以先求 ![\frac 1B](https://www.zhihu.com/equation?tex=%5Cfrac+1B) 再乘上 ![A](https://www.zhihu.com/equation?tex=A) ，这样化出来的式子不包含除法:

![x_{n+1}=x_n-\frac{\frac 1{x_n}-B}{-\frac1{x_n}^2}=x_n(2-x_n*B)\\](https://www.zhihu.com/equation?tex=x_%7Bn%2B1%7D%3Dx_n-%5Cfrac%7B%5Cfrac+1%7Bx_n%7D-B%7D%7B-%5Cfrac1%7Bx_n%7D%5E2%7D%3Dx_n%282-x_n%2AB%29%5C%5C)

时间复杂度：

初始时有效位数为1，之后每迭代一次位数翻倍，乘法使用FFT优化后，时间复杂度满足

![T(n)=T(n/2)+O(n\log n)\\](https://www.zhihu.com/equation?tex=T%28n%29%3DT%28n%2F2%29%2BO%28n%5Clog+n%29%5C%5C)

故 ![T(n)=O(n\log n)](https://www.zhihu.com/equation?tex=T%28n%29%3DO%28n%5Clog+n%29) （n为有效位数）。

实现细节：

1.为了方便可以将整数转换成多项式 ![a_0+a_1x^{-1}+a_2x^{-2}+a_3x^{-3}+...+a_nx^{-n}](https://www.zhihu.com/equation?tex=a_0%2Ba_1x%5E%7B-1%7D%2Ba_2x%5E%7B-2%7D%2Ba_3x%5E%7B-3%7D%2B...%2Ba_nx%5E%7B-n%7D) 。

2.类似多项式求逆，每一次计算时只需要取 ![B](https://www.zhihu.com/equation?tex=B) 的前 ![n](https://www.zhihu.com/equation?tex=n) 位参加计算，同样答案需要舍弃后面的位数。

3.迭代初值在 ![(0,2/B)](https://www.zhihu.com/equation?tex=%280%2C2%2FB%29) 收敛，那么初值可以设置为答案的第一位（例如 ![B=7](https://www.zhihu.com/equation?tex=B%3D7) 时初值设为 ![0.1](https://www.zhihu.com/equation?tex=0.1) , ![B=1.23](https://www.zhihu.com/equation?tex=B%3D1.23) 时，初值设为 ![0.8](https://www.zhihu.com/equation?tex=0.8) ）。

> 例：求 ![\frac{6666666}{23333}](https://www.zhihu.com/equation?tex=%5Cfrac%7B6666666%7D%7B23333%7D) 。
转换问题变成求 ![\frac{6.666666}{2.3333}\times10^2](https://www.zhihu.com/equation?tex=%5Cfrac%7B6.666666%7D%7B2.3333%7D%5Ctimes10%5E2) 。先求 ![\frac1{2.3333}](https://www.zhihu.com/equation?tex=%5Cfrac1%7B2.3333%7D) ，初值设为 ![\lfloor\frac{10}{23}\rfloor=0.4](https://www.zhihu.com/equation?tex=%5Clfloor%5Cfrac%7B10%7D%7B23%7D%5Crfloor%3D0.4) ,
第一次迭代： ![x=x(2-x*2.3)=0.4](https://www.zhihu.com/equation?tex=x%3Dx%282-x%2A2.3%29%3D0.4)
第二次迭代： ![x=x(2-x*2.333)=0.426](https://www.zhihu.com/equation?tex=x%3Dx%282-x%2A2.333%29%3D0.426)
第三次迭代： ![x=x(2-x*2.3330000)=0.4285620](https://www.zhihu.com/equation?tex=x%3Dx%282-x%2A2.3330000%29%3D0.4285620)
第四次迭代：![x=x(2-x*2.333000000000000)=0.428577550543594](https://www.zhihu.com/equation?tex=x%3Dx%282-x%2A2.333000000000000%29%3D0.428577550543594)
第五次迭代 ![x=x(2-x*2.3330..00)=0.4285775511078729688709038105305](https://www.zhihu.com/equation?tex=x%3Dx%282-x%2A2.3330..00%29%3D0.4285775511078729688709038105305)
需要注意的是最后的答案只有一半的位数是正确的，计算答案得到 ![\frac{6666666}{23333}=285.718338833411905](https://www.zhihu.com/equation?tex=%5Cfrac%7B6666666%7D%7B23333%7D%3D285.718338833411905) 。

下面是NTT实现的高精度除法(答案向下取整)。

```cpp
#include<algorithm> 
#include<iostream>
#include<cstring>
#include<cstdio>
#include<vector>
#define loop(n,i) for(register int i=1;i<=(n);++i)
#define MAX 500009
#define zxc(x) cerr<<(#x)<<'='<<(x)<<'\n'
#define zxcv(x) cerr<<(#x)<<'='<<(x)<<','
#define int long long
#define P 998244353
using namespace std;
inline int icin(){
	char c=getchar();int s=;bool sign=;
	while(!isdigit(c)&&c^'-')c=getchar();
	if(c=='-')c=getchar(),sign=;
	while(isdigit(c))s=(s<<)+(s<<)+c-'0',c=getchar();
	return sign?-s:s;
}
typedef vector<int> Poly;
inline int Quick(int a,int m){int ans=;for(;m;m>>=,a=a*a%P) if(m&) ans=ans*a%P;return ans;}
int R[MAX];
inline void NTT(Poly& a,int x,int type){
	int n=<<x;
	for(register int i=;i<n;++i) if((R[i]=R[i>>]>>|(i&)<<x-)>i) swap(a[i],a[R[i]]);
	for(register int s=;s<=x;++s){
		int len=<<s,mid=len>>;
		int w=Quick(,type*(P-)/len+P-);
		for(register int k=;k<n;k+=len){
			int d=;
			for(register int j=;j<mid;++j){
				int u=a[j+k],v=a[j+k+mid]*d%P;
				a[j+k]=(u+v)%P,a[j+k+mid]=(u-v+P)%P;
				d=d*w%P;
			}
		}
	}
	if(type==-) for(register int i=,inv=Quick(n,P-);i<n;++i) a[i]=a[i]*inv%P;
}
ostream& operator <<(ostream& out,Poly x){for(register int i=;i<x.size();++i) out<<x[i]<<' ';return out;}
inline int ceilog(int x){int ans=;while(<<ans<x)ans++;return ans;}
Poly operator *(Poly A,Poly B){
	int deg=A.size()+B.size()-;
	int x=ceilog(deg);
	A.resize(<<x),B.resize(<<x);
	NTT(A,x,),NTT(B,x,);
	for(register int i=;i<<<x;++i) A[i]=A[i]*B[i]%P;
	NTT(A,x,-);
	A.resize(deg);
	for(register int i=deg-;i;--i){
		A[i-]+=A[i]/;
		A[i]%=;
	}
	return A;
}

inline Poly Inverse(Poly A){
	Poly B,C;B.resize();B[]=/(A[]*+(A.size()>?A[]:));
	int x=ceilog(A.size())+;
	for(register int s=;s<=;++s){//迭代次数 
		C.resize(<<s),B.resize(<<s);
		for(register int i=;i<min(<<s,(int)A.size());++i) C[i]=A[i];
		for(register int i=min(<<s,(int)A.size());i<<<s;++i) C[i]=;
		C=B*C;
		for(register int i=;i<C.size();++i) C[i]=-C[i];
		C[]=-C[];
		for(register int i=C.size()-;i;--i){
			C[i-]+=(C[i]-)/;
			C[i]=(C[i]+)%;
		}
		B=B*C;
		B.resize(<<s);
	}
	return B;
}
Poly A,B;
int rest;
main(){
	char c=getchar();while(!isdigit(c)) c=getchar();
	while(isdigit(c)) A.push_back(c-'0'),c=getchar();
	rest+=A.size();
	c=getchar();while(!isdigit(c)) c=getchar();
	while(isdigit(c)) B.push_back(c-'0'),c=getchar();
	rest-=B.size();
	A=A*Inverse(B);
	for(register int i=;i<=rest;++i) if(!(i==&&A[i]==)) cout<<A[i];
	cout<<'\n';
}
```

