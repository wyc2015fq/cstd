# acdream  LCM Challenge (最小公倍数) - xcw0754 - 博客园
# [acdream  LCM Challenge (最小公倍数)](https://www.cnblogs.com/xcw0754/p/4650776.html)
### 　　　　　　　　LCM Challenge
Time Limit: 2000/1000MS (Java/Others)    Memory Limit: 128000/64000KB (Java/Others)
Submit [Status](http://acdream.info/contest?cid=1372#status--A)
#### Problem Description
Some days ago, I learned the concept of LCM (least common multiple). I've played with it for several times and I want to make a big number with it.
But I also don't want to use many numbers, so I'll choose three positive integers (they don't have to be distinct) which are not greater than n. Can you help me to find the maximum possible least common multiple of these three integers?
#### Input
The first line contains an integer n (1 ≤ n ≤ 10^6) — the n mentioned in the statement.
#### Output
Print a single integer — the maximum possible LCM of three not necessarily distinct positive integers that are not greater than n.
#### Sample Input
9
#### Sample Output
504
题意：给一个数n，要求找3个小于等于n的数，3个数的最小公倍数LCM（a1,a2,a3）最大。
分析：其实就是要找3个两两互质的数，但是只是如何找的问题。
　　根据连续的两个奇数互质，连续的两个自然数互质。那么答案很明显了，3个最大的互质的数的最小公倍数肯定最大。
　　如果n是奇数，那么答案是n*(n-1)*(n-2)。
　　如果n是偶数就麻烦了点，要么是如果n和n-2互质，那么答案就是n*(n-1)*(n-2)，因为他们是最大的3个互质的数。但是如果是n和n-2不互质，那么答案是(n-3)*(n-1)*(n-2)，因为n-1之后肯定是奇数了，用奇数的方法。
```
1 /*
 2 * this code is made by xcw0754
 3 * Problem: 1077
 4 * Verdict: Accepted
 5 * Submission Date: 2015-07-16 10:24:02
 6 * Time: 0MS
 7 * Memory: 1672KB
 8 */
 9 #include <bits/stdc++.h>
10 #define LL long long
11 #define pii pair<int,int>
12 #define INF 0x7f7f7f7f
13 using namespace std;
14 const int N=200000+50;
15  
16 LL cal(LL n)    //偶数
17 {
18     LL ans=0;
19     //n n-1 n-3
20     if(__gcd(n,n-3)==1) ans=max(ans, n*(n-1)*(n-3));
21  
22     //n-1 n-2 n-3
23     ans=max(ans, (n-1)*(n-2)*(n-3));
24  
25     return ans;
26 }
27  
28  
29 int main()
30 {
31     //freopen("input.txt", "r", stdin);
32     LL n;
33     scanf("%lld",&n);
34     if(n==1)    printf("1\n");
35     else if(n==2)    printf("2\n");
36     else if(n==3)    printf("6\n");
37     else if(n==4)    printf("12\n");
38     else if(n==5)    printf("60\n");
39     else
40     {
41         if((n&1)==0)    cout<<cal(n)<<endl;
42         else    cout<<n*(n-1)*(n-2)<<endl;
43     }
44     return 0;
45 }
AC代码
```

