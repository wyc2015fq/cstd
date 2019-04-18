# HDU 1018 Big Number （阶乘位数） - xcw0754 - 博客园
# [HDU 1018 Big Number （阶乘位数）](https://www.cnblogs.com/xcw0754/p/4604430.html)
题意：
　　给一个数n，返回该数的阶乘结果是一个多少位（十进制位）的整数。
思路：
　　用对数log来实现。
　　举个例子 一个三位数n 满足102 <= n < 103：
　　那么它的位数w 满足 w = lg103 = 3。 因此只要求lgn 向下取整 +1就是位数。然后因为阶乘比如5阶乘的话是5 * 4 * 3 * 2 * 1。位数就满足lg 5 * 4 * 3 * 2 * 1 = lg5 + lg4 + lg3 + lg2 + lg1。用加法就不会超过数字上限。
　　当然这是十进制下得。如果是m进制下 ，就把lgn 换成logm(n)就可以了。 logm(n)的表示方法是 lgn / lgm。高中数学的知识。。 这里有个double向下取整精度的问题要注意。转换成int时候，要floor(算出来的位数 + 1e-7) + 1.
　　最后得出位数的计算方式为：floor( logm(n) + logm(n - 1) + ...+ logm(1) + 1e-9 ) + 1.
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=1e-7;
 5 int a;
 6 
 7 int second()    //总位数
 8 {
 9     double tmp=0.0;
10     for(int i=2; i<=a; i++)    tmp+=log10(i);
11     return floor(tmp+N)+1;
12 }
13 int main()
14 {
15     //freopen("input.txt", "r", stdin);
16     int t;
17     cin>>t;
18     while(t--)
19     {
20         scanf("%d",&a);
21         printf("%d\n",second());
22     }
23     return 0;
24 }
AC代码
```

