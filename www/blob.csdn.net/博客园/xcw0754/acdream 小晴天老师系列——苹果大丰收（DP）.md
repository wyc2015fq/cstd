# acdream  小晴天老师系列——苹果大丰收（DP） - xcw0754 - 博客园
# [acdream  小晴天老师系列——苹果大丰收（DP）](https://www.cnblogs.com/xcw0754/p/4650983.html)
### 　　小晴天老师系列——苹果大丰收
#### Problem Description
小晴天的后花园有好多好多的苹果树，某天，苹果大丰收~小晴天总共摘了M个苹果，我们假设苹果之间是不可分辨的。
为了保存苹果，小晴天买了N个一模一样的箱子，想要把苹果放进去，允许有的箱子是空的，请问小晴天有多少种不同的放法呢？
例如对于4个苹果，3个箱子，2+1+1和1+2+1和1+1+2 是同一种分法。
#### Input
多组数据，首先是一个正整数t（t<=100）表示数据的组数。
每组数据均包含二个整数M和N(1<=M，N<=10)。
#### Output
对于每组数据，输出一个整数，表示不同的放法数。
#### Sample Input
1
7 3
#### Sample Output
8
#### Hint
对于7个苹果，3个箱子
有7+0+0=6+1+0=5+2+0=4+3+0=5+1+1=4+2+1=3+2+2=3+3+1
这8种放法。

思路：DP解。枚举前面每个盒子可以放多少个，只要保证后面的每一个盒子中的苹果数不大于前面任意一个盒子中的苹果数即可，即让他们有序的排放在前面几个盒子中。
```
1 /*
 2 * this code is made by xcw0754
 3 * Problem: 1707
 4 * Verdict: Accepted
 5 * Submission Date: 2015-07-15 14:06:15
 6 * Time: 0MS
 7 * Memory: 1676KB
 8 */
 9 //#pragma comment(linker,"/STACK:102400000,102400000")
10 #include <iostream>
11 #include <stdio.h>
12 #include <string.h>
13 #include <vector>
14 #include <stack>
15 #include <algorithm>
16 #include <map>
17 #include <bits/stdc++.h>
18 #define LL long long
19 #define pii pair<int,int>
20 #define INF 0x7f7f7f7f
21 using namespace std;
22 const int N=12000+50;
23  
24 vector<int> vect;
25 int t, nn, mm;
26  
27 int DFS(int m, int n, int far)
28 {
29     if( n==1 && far<m)   return 0;
30     if( n==1 && far>=m)  return 1;
31     if( m<=1)  return 1;
32  
33     int ans=0;
34     for(int i=1; i<=m; i++)
35     {
36         if(far>=i)
37             ans+=DFS(m-i, n-1, i);
38     }
39     return ans;
40 }
41  
42 int main()
43 {
44     //freopen("input.txt", "r", stdin);
45     cin>>t;
46     while(t--)
47     {
48         scanf("%d%d",&mm, &nn);   //m个苹果， n个箱子
49         printf("%d\n",DFS(mm,nn,mm));
50     }
51     return 0;
52 }
AC代码
```

