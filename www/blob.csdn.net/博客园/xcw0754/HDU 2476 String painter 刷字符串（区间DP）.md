# HDU 2476 String painter 刷字符串（区间DP） - xcw0754 - 博客园
# [HDU 2476 String painter 刷字符串（区间DP）](https://www.cnblogs.com/xcw0754/p/4257673.html)
题意：
　　给出两个串s1和s2，每次可以将s1中的一个整个区间刷成同个字母，问最少刷几次才能让s1变成s2？ 
思路：
　　假设最坏情况，两串没任何一个位置是相同的，那么全都得刷，相当于将一个空白串刷成s2。
　　这一步得求出所有子区间的最低次数。由于两串可能存在某些相同的字母，可以不刷，这样有可能会出现更优的解了。
```
1 #include <iostream>
 2 #include <cstring>
 3 #include <algorithm>
 4 #define N 110
 5 using namespace std;
 6 char a[N], b[N];
 7 int len, dp[N][N], ans[N];    //ans[i]表示将a[0~i]刷成b[0~i]的最小次数
 8 
 9 void paint_b()  //空白串刷成串b
10 {
11     for(int i=0; i<len; i++)
12     {
13         for(int j=i; j>=0; j--)
14         {
15             dp[j][i]=dp[j+1][i]+1;
16             for(int k=j+1; k<=i; k++)
17             {
18                 if(b[j]!=b[k])  continue;
19                 dp[j][i]=min(dp[j][i], dp[j+1][k] + dp[k+1][i]);
20             }
21         }
22     }
23 }
24 void paint_a()
25 {
26     memcpy(ans, dp, sizeof(int)*len); //拷贝dp[0][0~len-1].
27     if(a[0]==b[0])    ans[0]=0; //处理第一个
28     else              ans[0]=1;
29     for(int i=1; i<len; i++)    //考虑前[0~i]
30     {
31         if(a[i]==b[i])
32         {
33             ans[i]=ans[i-1];    //a[i]不需要刷
34             continue;
35         }
36         for(int j=0; j<i; j++ ) //枚举相同的位置
37         {
38             if(a[j]!=b[j])  continue;
39             ans[i]= min(ans[i], ans[j-1]+dp[j+1][i] );  //两种决策：a[j]刷or不刷
40         }
41     }
42 }
43 int main()
44 {
45     while(cin>>a>>b)
46     {
47         memset(dp, 0, sizeof(dp));
48         len=strlen(a);
49         paint_b();    //求空白串刷出串b的最小次数
50         paint_a();    //将串a刷成串b的最小刷法次数
51         printf("%d\n", ans[len-1]);
52     }
53     return 0;
54 }
AC代码
```

