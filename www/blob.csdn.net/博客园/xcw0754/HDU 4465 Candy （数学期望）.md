# HDU 4465 Candy （数学期望） - xcw0754 - 博客园
# [HDU 4465 Candy （数学期望）](https://www.cnblogs.com/xcw0754/p/4753221.html)
题意：有两个盒子各有n个糖（n<=2*105），每天随机选1个（概率分别为p，1-p），然后吃掉一颗糖。直到有一天打开盒子一看，这个盒子没有糖了。输入n，p，求此时另一个盒子里糖的个数的数学期望。
思路：假设没糖的是A盒子，而B盒子还有0~n个糖。由于B盒子还有0个糖的情况的期望必为0，所以省略，只需要计算1~n的。
　　（1）当A盒没有糖时，B盒就可能有1~n个糖，概率为C(n+i，i)*(pn+1)*(1-p)n-i。为啥还带个大C？这是情况的种数（想象取糖时还有个顺序，有C种可能的顺序），不然的话，单靠这两个小于1的数是超级小的。
　　（2）根据（1）种的概率公式，穷举B盒可能还有 i 个糖，那么对于每种情况，期望值为i*C(n+i，i)*(pn+1)*(1-p)n-i，累加这些期望值就行了。同理，B盒没有糖也是这样算，只是概率换成了（1-p）。两种情况的累加期望就是答案。
　　（3）这样还是不行，求C时会爆LL，对p求幂时结果又太小，精度损失严重。C(n+i，i)*(pn+1)*(1-p)n-i这个式子的结果本身是不大的。考虑取这个式子对数，化成相加的形式x=logC(n+i，i)+ log(pn+1)+log(1-p)n-i ，（注意指数可以提到前面作为乘的形式），求出x作为指数来求ex这样就OK了（这个函数是exp(x) ）。
　　（4）这个C还是很难求，比如当n=200000时，i 还没有到10时，C(200000+10, 10)就爆了。对此，由于在穷举i时，C(n+i,i)是可以递推的，那么我们可以先将C给逐步取对数，再相加就行了。递推是这样的，c+=log((n+i)/i)。
　　（5）总复杂度是O(n)。时间在500ms以下。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x3f3f3f3f
 4 #define LL long long
 5 using namespace std;
 6 const int N=40086;
 7 double cal(double p,int n)
 8 {
 9     double  c=0, ans=0;
10     ans=n*exp((n+1)*log(p));    //第2个盒子取0个的情况：即还剩下n个。
11     for(int i=1; i<n; i++)      //在第2个盒子中取了i个。
12     {
13         c+=log((double)(n+i)/i);   //log(c)这样求才能防溢出。
14         ans+=(n-i)*exp( c+ (n+1)*log(p)+ i*log(1-p) );
15     }
16     return ans;
17 }
18 
19 int main()
20 {
21     //freopen("input.txt", "r", stdin);
22     int Case=0, n;
23     double p;
24     while(~scanf("%d %lf", &n, &p))
25         printf("Case %d: %.6f\n", ++Case, cal(p, n)+cal(1-p,n) );
26     return 0;
27 }
AC代码
```

