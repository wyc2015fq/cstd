# HDU 5289 Assignment （数字序列，ST算法） - xcw0754 - 博客园
# [HDU 5289 Assignment （数字序列，ST算法）](https://www.cnblogs.com/xcw0754/p/4665490.html)
题意：
　　给一个整数序列，多达10万个，问：有多少个区间满足“区间最大元素与最小元素之差不超过k”。k是给定的.
思路：
　　如果穷举，有O（n*n）复杂度。可以用ST算法先预处理每个区间最大和最小，O（nlogn）。再扫一遍整个序列，两个指针L,R用于这样判断：如果A[L,R]这个区间满足要求，则R++，否则统计ans+=R-L，且l++。因为在[L,R]这个区间是满足要求的，那么以L开头的，[L,L]、[L,L+1]...[L,R-1]就都是满足要求的，刚好R-L个。
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

