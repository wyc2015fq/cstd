# HDU 4291 A Short problem 短问题 （递推，找规律） - xcw0754 - 博客园
# [HDU 4291 A Short problem 短问题 （递推，找规律）](https://www.cnblogs.com/xcw0754/p/4523389.html)
题意：
　　给出递推式 g(n) = 3g(n - 1) + g(n - 2)，且g(1) = 1，g(0) = 0。求g( g( g(n))) mod 109 + 7。
思路：
　　要求的g( g( g(n)))一共里外3层。看到时间限制1s，数据最大10^18，必定不能老实递推，要么有循环，要么用通项公式。这里用通项公式太麻烦了，数字不好算，而g(n)%109 + 7是有规律的， 在n=222222224之后会出现循环，也就是n=0和n=222222224的g(n)是一样的，这是最外层。那么也就是说在g(g(n))=222222224以上时g( g( g(n))) mod 109 + 7会出现循环了，那么g(g(n))应该模222222224再来代进去算。而g(n)%222222224是不是也会有循环的情况?确实，循环点是183120，那么g(n)的范围在0~183119就行了，即g(n)应该模183120。而g(n)%183120是不是还有循环？确实，循环点在240，也就是说n要模240。
　　到这已经分析完毕，将输入的n先模240，代入g(n)，计算结果（注意要模的是183120）。将结果n再次代入g(n)，计算结果（注意要模的是222222224）。将结果n再次代入g(n)，计算结果（这次要模的是109 + 7）。这已经是结果。计算过程所用的递推式是一样的，只不过最后取模不一样而已。n%222222224仍然很大，用矩阵快速幂函就行了。
```
1 #include<bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const LL mod1=1000000007;
 5 const LL mod2=222222224;
 6 const LL mod3=183120;
 7 
 8 struct mar
 9 {
10     LL a[4];
11 }ta,b,t1;
12 
13 mar mul(mar &a,mar &b,LL &mod)
14 {
15     mar t;
16     t.a[0]= (a.a[0]*b.a[0]%mod+ a.a[1]*b.a[2]%mod)%mod;
17     t.a[2]= (a.a[0]*b.a[1]%mod+ a.a[1]*b.a[3]%mod)%mod;
18     t.a[1]= (a.a[2]*b.a[0]%mod+ a.a[3]*b.a[2]%mod)%mod;
19     t.a[3]= (a.a[2]*b.a[1]%mod+ a.a[3]*b.a[3]%mod)%mod;
20     return t;
21 }
22 
23 
24 LL quick_pow(LL &n,LL mod)
25 {
26     n--;                //只需要n-1个即可
27     b=ta;
28     t1.a[0]=t1.a[3]=1;  t1.a[1]=t1.a[2]=0;
29 
30     while(n>0)
31     {
32         if(n&1==1)    t1=mul(t1,b,mod);
33         b=mul(b,b,mod);
34         n>>=1;
35     }
36     return t1.a[3]%mod;
37 }
38 int main()
39 {
40 
41     ta.a[0]=0;ta.a[1]=1;ta.a[2]=1;ta.a[3]=3;
42     LL n;
43     //freopen("input.txt","r",stdin);
44     while(~scanf("%lld",&n))
45     {
46         n%=240;
47         if(!n)
48         {
49             printf("0\n");
50             continue;
51         }
52         n=quick_pow(n, mod3);
53         n=quick_pow(n, mod2);
54         n=quick_pow(n, mod1);
55         printf("%lld\n",n);
56     }
57 
58     return 0;
59 }
AC代码
```

