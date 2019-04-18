# HDU 2577 How to Type (DP，经典) - xcw0754 - 博客园
# [HDU 2577 How to Type (DP，经典)](https://www.cnblogs.com/xcw0754/p/4550454.html)
题意：
　　打字游戏，求所按的最少次数。给出一个串，其中有大小写，大写需要按下cap键切换到大写，或者在小写状态下按shift+键，这样算两次，打小写时则相反。注意：在打完所有字后，如果cap键是开着的，要关它，也就是要算多一次。
思路：
　　DP，根据每个字符打完后cap键盘是开着的还是关着的，最后dp[最后一个字符][关着的]为答案。规模降低到1个字符，每次考虑增加一个字符，打这个字符有两种选择，从上一个字符打完后的cap键关/开的两种状态来按下此字符，按完此字符后考虑使cap键开着或者关掉。
dp[当前字符][关着的]= 可从两种途径而来（取最小即可）：（1）dp[上一个字符][关着的]  （2）dp[上一个字符][开着的]
dp[当前字符][开着的]= 同上。
```
1 #include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 using namespace std;
 5 const int N=105;
 6 char str[N];
 7 int dp1[N], dp2[N];
 8 
 9 int cal(int len)
10 {
11     
12     dp2[0]=1;    //亮
13     for(int i=1; i<=len; i++)
14     {
15         if( str[i]<='Z' )     //大写
16         {
17             dp1[i]=min(dp1[i-1]+2, dp2[i-1]+2);
18             dp2[i]=min(dp1[i-1]+2, dp2[i-1]+1);
19         }
20         else    //小写
21         {
22 
23             dp1[i]=min(dp1[i-1]+1, dp2[i-1]+2);
24             dp2[i]=min(dp1[i-1]+2, dp2[i-1]+2);            
25         }
26     }    
27     return min(dp1[len],dp2[len]+1);
28 }
29 
30 
31 
32 int main()
33 {
34     //freopen("input.txt", "r", stdin);
35     int t;
36     cin>>t;
37     while(t--)
38     {
39         memset(dp1,0,sizeof(dp1));
40         memset(dp2,0,sizeof(dp2));
41         scanf("%s",str+1);
42         cout<<cal(strlen(str+1))<<endl;
43     }
44     return 0;
45 }
AC代码
```

