# HDU 4055 The King’s Ups and Downs（DP计数） - xcw0754 - 博客园
# [HDU 4055 The King’s Ups and Downs（DP计数）](https://www.cnblogs.com/xcw0754/p/4471982.html)
题意：
　　国王的士兵有n个，每个人的身高都不同，国王要将他们排列，必须一高一矮间隔进行，即其中的一个人必须同时高于（或低于）左边和右边。问可能的排列数。例子有1千个，但是最多只算到20个士兵，并且20个的情况的答案已给出。
思路：是此题[HDU 4055 Number String(DP计数)](http://www.cnblogs.com/xcw0754/p/4471344.html) 的简单版，所以看此题解就行了。数量较小，可以预先算出来。要同时考虑 <><>和><><这样的两种情况。
```
1 #include <iostream>
 2 #include <stdio.h>
 3 #include <string.h>
 4 #include <stdlib.h>
 5 using namespace std;
 6 const int N=23;
 7 long long dp1[N][N];
 8 long long dp2[N][N];
 9 long long ans[N];
10 int j, n;
11 int cal()
12 {
13     ans[1]=1;
14     dp1[0][1]=dp2[0][1]=1;
15     for(int i=1; i<N; i++)
16     {
17         if(i&1)     //大于
18         {
19             for(int j=1; j<=i+1; j++)
20             {
21                 dp1[i][j]=dp1[i][j-1];
22                 dp1[i][j]+=dp1[i-1][j-1];
23                 ans[i+1]+=dp1[i][j];
24             }
25         }
26         else
27         {
28             for(int j=i+1; j>0; j--)
29             {
30                 dp1[i][j]=dp1[i][j+1];
31                 dp1[i][j]+=dp1[i-1][j];
32                 ans[i+1]+=dp1[i][j];
33             }
34         }
35     }
36 
37     for(int i=1; i<N; i++)
38     {
39         if(i&1)     //大于
40         {
41             for(int j=i+1; j>0; j--)
42             {
43                 dp2[i][j]=dp2[i][j+1];
44                 dp2[i][j]+=dp2[i-1][j];
45                 ans[i+1]+=dp2[i][j];
46             }
47         }
48         else
49         {
50             for(int j=1; j<=i+1; j++)
51             {
52                 dp2[i][j]=dp2[i][j-1];
53                 dp2[i][j]+=dp2[i-1][j-1];
54                 ans[i+1]+=dp2[i][j];
55             }
56         }
57     }
58     return 0;
59 }
60 int main()
61 {
62     //freopen("input.txt","r",stdin);
63     cal();
64     int p;
65     cin>>p;
66     while(p--)
67     {
68         scanf("%d%d",&j,&n);
69         cout<<j<<" "<<ans[n]<<endl;
70     }
71     return 0;
72 }
易理解版本
```
```
1 #include <iostream>
 2 #include <stdio.h>
 3 #include <string.h>
 4 #include <stdlib.h>
 5 using namespace std;
 6 const int N=21;
 7 long long dp[N][N],ans[N];
 8 int j, n;
 9 void cal()
10 {
11     ans[1]=1;
12     for(int k=0; k<2; k++)
13     {
14         dp[0][1]=1;
15         for(int i=1; i<N; i++)
16         {
17             if((i+k)&1)     //大于
18             {
19                 for(int j=1; j<=i+1; j++)
20                 {
21                     dp[i][j]=dp[i][j-1];
22                     dp[i][j]+=dp[i-1][j-1];
23                     ans[i+1]+=dp[i][j];
24                 }
25             }
26             else
27             {
28                 for(int j=i+1; j>0; j--)
29                 {
30                     dp[i][j]=dp[i][j+1];
31                     dp[i][j]+=dp[i-1][j];
32                     ans[i+1]+=dp[i][j];
33                 }
34             }
35         }
36         memset(dp,0,sizeof(dp) );
37     }
38 }
39 int main()
40 {
41     //freopen("input.txt","r",stdin);
42     cal();
43     int p;cin>>p;
44     while(p--)
45     {
46         scanf("%d%d",&j,&n);
47         cout<<j<<" "<<ans[n]<<endl;
48     }
49     return 0;
50 }
节省一半空间和代码量的版本
```
```
1 #include <iostream>
 2 #include <stdio.h>
 3 #include <string.h>
 4 #include <stdlib.h>
 5 using namespace std;
 6 const int N=21;
 7 long long dp[2][N],ans[N];
 8 int j, n;
 9 void cal()
10 {
11     ans[1]=1;
12     for(int k=0; k<2; k++)
13     {
14         dp[0][1]=1;
15         for(int i=1; i<N; i++)
16         {
17             if((i+k)&1)     //大于
18             {
19                 for(int j=1; j<=i+1; j++)
20                 {
21                     dp[i&1][j]=dp[i&1][j-1];
22                     dp[i&1][j]+=dp[~i&1][j-1];
23                     ans[i+1]+=dp[i&1][j];
24                 }
25             }
26             else
27             {
28                 for(int j=i+1; j>0; j--)
29                 {
30                     dp[i&1][j]=dp[i&1][j+1];
31                     dp[i&1][j]+=dp[~i&1][j];
32                     ans[i+1]+=dp[i&1][j];
33                 }
34             }
35         }
36         memset(dp,0,sizeof(dp) );
37     }
38 }
39 int main()
40 {
41     //freopen("input.txt","r",stdin);
42     cal();
43     int p;cin>>p;
44     while(p--)
45     {
46         scanf("%d%d",&j,&n);
47         cout<<j<<" "<<ans[n]<<endl;
48     }
49     return 0;
50 }
滚动数组版本（更少空间）
```

