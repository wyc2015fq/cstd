# hihoCoder #1038 : 01背包 - xcw0754 - 博客园
# [hihoCoder #1038 : 01背包](https://www.cnblogs.com/xcw0754/p/4436348.html)
思路：模板。
```
1 #include <iostream>
 2 #include <stdio.h>
 3 #include <cstring>
 4 #include <string>
 5 using namespace std;
 6 
 7 int dp[100005];
 8 int coms[505];
 9 int value[505];
10 int max(int a, int b){return a>b?a:b;}
11 
12 int main()
13 {
14     //freopen("e:input.txt","r",stdin);
15     int n, m;
16     while(cin>>n >>m )
17     {
18         for(int i=0; i<n; i++)
19             cin>>coms[i]>>value[i];
20 
21         memset(dp, 0, sizeof(dp));
22 
23         for(int i=0; i<n; i++)
24             for(int j=m-1; j>=coms[i]; j--)
25                 dp[j] = max( dp[j], dp[j-coms[i]]+value[i] );    //对于每种奖品，有两种可能，1换  2不换
26         cout<<dp[m-1]<<endl;
27     }
28     return 0;
29 }
1038
```

