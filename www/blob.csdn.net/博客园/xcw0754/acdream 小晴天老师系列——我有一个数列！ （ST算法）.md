# acdream  小晴天老师系列——我有一个数列！ （ST算法） - xcw0754 - 博客园
# [acdream  小晴天老师系列——我有一个数列！ （ST算法）](https://www.cnblogs.com/xcw0754/p/4877170.html)
　　小晴天老师系列——我有一个数列！
Time Limit: 20000/10000MS (Java/Others) Memory Limit: 128000/64000KB (Java/Others)
Submit Status
Problem Description
小晴天：“我有一个数列！”
小晴天：“我还要有很多很多的数列！”
于是小晴天就把这个数列的所有连续子数列写出来。
然后小晴天把每个连续子数列中的最大的数写出来。
那么，有多少个比K大呢？
Input
多组数据，首先是一个正整数t(t<=100)，表示数据的组数
对于每组数据，首先是两个整数n(1<=n<=200000)，K(0<=K<=10^9).，但所有数据中的n之和不超过1000000.
接下来是n个整数a[i](1<=a[i]<=10^9)
Output
对于每组数据，输出一个整数，表示最大元素大于K的连续子序列的个数。
Sample Input
2
3 2
1 2 3 
3 1 
1 2 3
Sample Output
3
5
Hint
对于样例一，共有6个连续子序列{1}{2}{3}{1,2}{2,3}{1,2,3}（注意{1,3}不满足题意，因为不连续），其中最大元素大于2的共有3个{3}{2,3}{1,2,3}，对于样例二，大于1的连续子序列共有5个，{2}{3}{1,2}{2,3}{1,2,3}
思路：ST算法可以解决。用 O(n*n)枚举每个区间，用ST算法在O(1)找到每个区间的最大再与k比较。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=100500;
 5 int a[N], cur;
 6 int big[N][20], small[N][20];
 7 
 8 void pre_cal(int n)
 9 {
10     memset(big,0,sizeof(big));
11     memset(small,0,sizeof(small));
12 
13     for(int i=1; i<=n; i++)
14     {
15         big[i][0]=a[i];
16         small[i][0]=a[i];
17     }
18 
19     for(int i=2,q=1; i<=n; i<<=1,q++)   //以i为距离
20     {
21 
22         for(int j=1; j<=n; j++ ) 
23         {
24             if(j+i-1<=n)
25             {
26                 big[j][q]=max(big[j][q-1],big[ j+i/2 ][q-1]);
27                 small[j][q]=min(small[j][q-1],small[ j+i/2 ][q-1]);
28             }
29             else    break;
30         }
31     }
32 }
33 
34 unordered_map<int,int> mapp,mapp2;
35 void init() //获得二进制最高位，这个也可以先处理的。
36 {
37     for(int j=1; j<=N; j++)
38     {
39         int tmp=0, cnt=0;
40         for(int i=1; i<30; i++)//找出二进制最高位的1
41         {
42             if(!(j>>i))
43             {
44                 tmp=((j>>(i-1))<<(i-1));
45                 break;
46             }
47             cnt++;
48         }
49         mapp2[j]=tmp;//记录j只剩最高位的值
50         mapp[j]=cnt;//记录tmp是2的几次幂
51     }
52 }
53 
54 bool iscor(int l, int r) //判断这个区间是否满足要求
55 {
56     int len=r-l+1;
57     int da=  max(   big[l][ mapp[len] ],   big[ r-mapp2[len]+1 ][ mapp[len] ]);
58     int xiao=min( small[l][ mapp[len] ], small[ r-mapp2[len]+1 ][ mapp[len] ]);
59     return da-xiao<cur? true :false;
60 
61 }
62 
63 int main(void)
64 {
65     freopen("e://input.txt", "r", stdin);
66     int t, n;
67     init();
68     cin>>t;
69     while(t--)
70     {
71         scanf("%d%d",&n,&cur);
72         for(int i=1; i<=n; i++)    scanf("%d",&a[i]);
73         pre_cal(n);
74         LL cnt=0;
75         int l=1, r=1;
76         while(r<=n)
77         {
78             if( iscor(l,r) )    r++;
79             else
80             {
81                 cnt+=r-l;
82                 l++;
83             }
84         }
85         while(l<r) cnt+=r-l,l++;
86         printf("%lld\n",cnt);
87     }
88     return 0;
89 }
AC代码
```

