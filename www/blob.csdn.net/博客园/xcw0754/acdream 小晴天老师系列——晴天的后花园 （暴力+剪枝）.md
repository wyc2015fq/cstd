# acdream 小晴天老师系列——晴天的后花园 （暴力+剪枝） - xcw0754 - 博客园
# [acdream 小晴天老师系列——晴天的后花园 （暴力+剪枝）](https://www.cnblogs.com/xcw0754/p/4650972.html)
### 　　　　　　小晴天老师系列——晴天的后花园
Time Limit: 10000/5000MS (Java/Others)    Memory Limit: 128000/64000KB (Java/Others)
#### Problem Description
小晴天非常漂亮的后花园，打算以后退休之后在里面种种花，养养草，所以现在小晴天打算为他的后花园围上栅栏。
小晴天的后花园可以看成是一个m*n的矩形，但是其中有一些地方种了树，这些地方都不能安装栅栏，现在小晴天把后花园的平面图告诉你了，请你帮忙设计一个最大的矩形栅栏。
#### Input
单组数据，第一行包括两个整数m,n(1<=m,n<=500)，接下来m行，每行包括n个字符，仅由'x'和'.'组成（ASCII码分别是120与46）.
其中‘x’表示这个方格有树木，‘.’表示这个点可以建造栅栏
#### Output
若可以围成一个闭合栅栏，输出一个整数，表示栅栏能打到的最大周长
否则输出impossible
#### Sample Input
4 5
.....
.x.x.
.....
.....
2 2
.x
x.
2 5
.....
xxxx.
#### Sample Output
14
impossible
impossible
#### Hint
样例一可以绕着整个图一圈，周长为2*(4+3)=14
样例二与三因为有树木的存在，不能建造一个闭合的栅栏。
本题为单文件读写，样例包括三个文件
思路：暴力的话就是选两个点，穷举任意的矩形。但是这样可能会超时，加些必要的剪枝可以过。
　　穷举第一个点(x1,y1)，第二个点(x2,y2)。第一个点任意摆放，第2个点必须从x1+1，y1+1开始穷举，这样可以减少一些不必要的计算。首先要得到从第一个点开始，y1列可以到达哪列，即中间不能有任何树木，同理，行也是这样。然后第二个点的y2先穷举，遇到一个树就break，这样又减少了计算。这样子就保证了3个边都是无障碍的，另外一个边就得靠计算了，可以先预处理。
```
1 /*
 2 * this code is made by xcw0754
 3 * Problem: 1705
 4 * Verdict: Accepted
 5 * Submission Date: 2015-07-15 23:53:13
 6 * Time: 28MS
 7 * Memory: 4664KB
 8 */
 9 #include <bits/stdc++.h>
10 #define LL long long
11 using namespace std;
12 const int N=505;
13 int m, n;
14 int g[N][N];
15 int row[N][N];
16 int col[N][N];
17 LL ans;
18 
19 void secon(int r,int c)
20 {
21     int up_m=r;
22     int up_n=c;
23     while( up_m<=m && !g[up_m][c])  up_m++;
24     while( up_n<=n && !g[r][up_n])  up_n++;
25  
26     for(int i=r+1; i<up_m; i++)
27     {
28         for(int j=c+1; j<up_n; j++)
29         {
30             if(g[i][j]) break;
31  
32             if( !(col[j][i]-col[j][r]) )
33                 ans=max(ans, (LL)2*(i-r)+2*(j-c) );
34         }
35     }
36  
37 }
38  
39 int cal()   //穷举:从小开始，遇到点就结束
40 {
41  
42     for(int i=1; i<m; i++)
43     {
44         for(int j=1; j<n; j++)
45         {
46             if(g[i][j]) continue;
47             secon(i,j);
48         }
49     }
50     return ans;
51 }
52  
53 int main()
54 {
55     //freopen("e://input.txt","r",stdin);
56     char c;
57     scanf("%d%d",&m,&n);
58     for(int i=1; i<=m; i++)
59     {
60         getchar();
61         for(int j=1; j<=n; j++)
62         {
63             if(getchar()=='x')  g[i][j]=1;
64             row[i][j]=row[i][j-1]+g[i][j];  //行先计算了
65         }
66     }
67  
68     for(int i=1; i<=n; i++) //计算列的
69         for(int j=1; j<=m; j++)
70             col[i][j]=col[i][j-1]+g[j][i];
71  
72  
73     for(int i=0; i<n+1; i++)    g[m+1][i]=1;
74     for(int i=0; i<m+1; i++)    g[i][n+1]=1;
75 
76     if(!cal())  puts("impossible");
77     else    printf("%lld\n",ans);
78 }
AC代码
```

