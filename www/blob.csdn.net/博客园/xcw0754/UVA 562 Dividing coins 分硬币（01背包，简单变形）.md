# UVA 562 Dividing coins 分硬币（01背包，简单变形） - xcw0754 - 博客园
# [UVA 562 Dividing coins 分硬币（01背包，简单变形）](https://www.cnblogs.com/xcw0754/p/4474355.html)
题意：一袋硬币两人分，要么公平分，要么不公平，如果能公平分，输出0，否则输出分成两半的最小差距。
思路：将提供的整袋钱的总价取一半来进行01背包，如果能分出出来，就是最佳分法。否则背包容量为一半总价的包能装下的硬币总值就是其中一个人能分得的最多的钱了，总余下的钱减去这包硬币总值。（只需要稍微考虑一下总值是奇数/偶数的问题）
```
1 #include <iostream>
 2 #include <stdio.h>
 3 #include <string.h>
 4 #include <cmath>
 5 using namespace std;
 6 const int N=100000;
 7 int w[N], dp[N];
 8 
 9 void cal(int n, int cnt)
10 {
11     int m;
12     if(cnt&1==1)    m = (cnt>>1) + 1;//奇数
13     else            m=cnt>>1;
14 
15     for(int i=0; i<n; i++)
16         for(int j=m; j>=w[i]; j--)
17             dp[j]=max( dp[j-w[i]]+w[i], dp[j] );
18 
19     if(cnt&1==1)//奇数
20     {
21         if(dp[m]==m)    cout<<"1"<<endl; //最公平
22         else    cout<<cnt-dp[m]-dp[m]<<endl;
23     }
24     else
25     {
26         if(dp[m]==m)    cout<<"0"<<endl;  //最公平
27         else            cout<<cnt-dp[m]-dp[m]<<endl;
28     }
29 
30 }
31 
32 int main()
33 {
34     //freopen("input.txt","r",stdin);
35     int t, n, cnt;
36     cin>>t;
37     while(t--)
38     {
39         memset(dp, 0, sizeof(dp));
40         memset(w, 0, sizeof(w));
41         cin>>n;
42         cnt=0;
43         for(int i=0; i<n; i++)
44         {
45             scanf("%d",&w[i]);
46             cnt+=w[i];
47         }
48         if(n==0)
49             cout<<"0"<<endl;
50         else if(n==1)
51             cout<<w[0]<<endl;
52         else if(n==2)
53             cout<<abs(w[0]-w[1])<<endl;
54         else
55             cal(n, cnt);
56     }
57     return 0;
58 }
AC代码
```

