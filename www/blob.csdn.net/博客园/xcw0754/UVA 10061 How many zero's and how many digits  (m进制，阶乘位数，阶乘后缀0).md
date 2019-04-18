# UVA 10061 How many zero's and how many digits ? (m进制，阶乘位数，阶乘后缀0) - xcw0754 - 博客园
# [UVA 10061 How many zero's and how many digits ? (m进制，阶乘位数，阶乘后缀0)](https://www.cnblogs.com/xcw0754/p/4606346.html)
题意：
　　给出两个数字a和b，求a的阶乘转换成b进制后，输出
　　（1）后缀中有多少个连续的0？　　
　　（2）数a的b进制表示法中有多少位？
思路：逐个问题解决。
　　设a！=k。  k暂时不用直接转成b进制。
（1）阶乘后缀0问题。先看这个十进制后缀0的例子：http://www.cnblogs.com/xcw0754/p/4604473.html
　　解法差不多，稍变化。 首先将b分解成若干质数（比如8=｛2*2*2｝）保存在一个集合A中（注意自然数的质数分解是唯一的），只要有一个序列A就能构成一个0，因为满b就进位，b可以表示成10，表示一个1*b+0。那么我们需要知道k中究竟有多少个集合A。将k也质数分解成集合B，在集合B中每次减去一个A，每成功减掉1个A就有1个后缀0。
　　直接求a！不是会爆吗？是的。将k进行质数分解和将｛1~a｝进行质数分解是一样的。那么就遍历[1,a]逐个进行分解，注意大于b的质数就不用去求了，其不可能用来组成b的。求完的质数的集合B应该是2有几个，3有几个，5有几个，7有几个...。如果集合A中需要4个2，那就看B中有多少个2，每两个2就可以组成一个后缀0，如果B中有5个2，那么就有2个0啦。
　　总之，这步需要将b质数分解成A，将[1,a]质数分解成集合B，再看B中有多少个A。
（2）阶乘的位数问题。
　　先举个例子： 一个三位数n满足102 <= n < 103
　　那么他的位数w 满足 w = log103 = 3。 因此只要求lgn 向下取整 +1就是位数后因为阶乘比如5阶乘的话是5 * 4 * 3 * 2 * 1。他的位数就满足lg5 * 4 * 3 * 2 * 1 = lg5 + lg4 + lg3 + lg2 + lg1 这里用相加的就不会超过数字上限。
　　当然这是十进制下得。如果是m进制下，就把log10n 换成logmn 就可以了。用高中知识：logm(n)的表示方法是 lgn / lgm。 这里有个double向下取整精度的问题要注意：转换成int时候，要floor(算出来的位数 + 1e-9) + 1.    注意：1e-9不要用变量存起来。
　　最后得出位数的计算方式为：floor( logmn + logm(n - 1) + ...+ logm1 + 1e-9) + 1.红色部分就是所要算的主要数字，用一个循环累加即可。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=2147483647;
 5 int has[999];  //哈希
 6 int need[999]; //进制的分解
 7 
 8 int second(int a,int b)    //总位数
 9 {
10     double tmp=0.0;
11     for(int i=2; i<=a; i++)    tmp+=log10(i);
12     if(b!=10)    tmp/=log10(b);     //转进制,10没必要转
13     return floor(tmp + 1e-9)+1;
14 }
15 
16 int first(int a,int b)     //后缀0个数
17 {
18     memset(has,0,sizeof(has));
19     memset(need,0,sizeof(need));
20     for(int i=2; i<=a; i++)             //先分解阶乘中每个数
21     {
22         int t=i;
23         for(int j=2; j<=t&&j<=b; j++)   //质数为j
24         {
25             while(t%j==0)               //能被j整除的，全部除掉
26             {
27                 has[j]++;
28                 t/=j;
29             }
30         }
31     }
32     int t=b;
33     for(int i=2; i<=b; i++)      //进制b进行分解
34     {
35         while(t%i==0)
36         {
37             need[i]++;          //需要质数的个数记录起来
38             t/=i;
39         }
40     }
41 
42     int cnt=N;
43     for(int i=2; i<=b; i++)
44         if(need[i]>0)    cnt=min(cnt,has[i]/need[i]);    //木桶原理,最低的一块木板起作用
45     return (cnt>=N?0:cnt);
46 }
47 
48 
49 
50 int main()
51 {
52     //freopen("e://input.txt", "r", stdin);
53     int a, b;
54     while(~scanf("%d%d",&a,&b))
55         printf("%d %d\n", first(a,b), second(a,b));
56     return 0;
57 }
58 
59 AC代码
AC代码
```

