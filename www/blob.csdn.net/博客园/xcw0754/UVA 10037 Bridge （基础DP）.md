# UVA 10037 Bridge （基础DP） - xcw0754 - 博客园
# [UVA 10037 Bridge （基础DP）](https://www.cnblogs.com/xcw0754/p/4847033.html)
 题意：
　　过河模型：有n个人要渡河，每个人渡河所耗时可能不同，只有1只船且只能2人/船，船速取决于速度慢的人。问最少耗时多少才能都渡完河？
思路：
　　n<2的情况比较简单。
　　考虑n>2的情况，第一次肯定是两个耗时少的先过去。接下来有两种渡河方式，有可能是{a回，另外2人去，b回，a和b去}，也可能是{a回，a和另一人去}。也就是说a和b的协作可以送走其他2个人，或者是a自己当船夫，送走另外一个人。这样子就有两种决策啦。
　　先将他们排个序（升序），然后a和b先过去。如果还有人没有过河，若left>=2时，可以选择第一种渡河方式，也可以选择第二种。当left==1就只能选择第二种了。然后转移行了。dp[i]表示送走[1->i]的所有人的花费。
```
1 #include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <cstring>
  5 #include <cmath>
  6 #include <map>
  7 #include <algorithm>
  8 #include <vector>
  9 #include <iostream>
 10 #define pii pair<int,int>
 11 #define INF 0x7f7f7f7f
 12 #define LL unsigned long long
 13 using namespace std;
 14 const double PI  = acos(-1.0);
 15 const int N=50100;
 16 int dp[N], path[N];
 17 deque<int> que;
 18 vector<int> vect,ans[N];
 19 int cal(int n)
 20 {
 21     sort(vect.begin(), vect.end());
 22 
 23     int a=vect[0];
 24     if(n==1)
 25     {
 26         printf("%d\n", a );
 27         printf("%d\n", a );
 28         return 0;
 29     }
 30     int b=vect[1];
 31     if( n==2 )
 32     {
 33         printf("%d\n", b );
 34         printf("%d %d\n", a, b );
 35         return 0;
 36     }
 37     //DP******************
 38     dp[1]=b;
 39     path[1]=2;
 40     for(int i=2; i<vect.size(); i++)
 41     {
 42         int t1=a+vect[i];
 43         if(dp[i]>dp[i-1]+t1)        //送1个
 44         {
 45             dp[i]= dp[i-1]+t1 ;
 46             path[i]=1;
 47         }
 48 
 49         if( i+1==vect.size() )  continue;   //最后1个了
 50         int t2=a+b*2+vect[i+1];
 51         if(dp[i+1]>dp[i-1]+t2 )        //送2个
 52         {
 53             dp[i+1]=dp[i-1]+t2;
 54             path[i+1]=2;
 55         }
 56     }
 57     que.clear();    //寻找路径
 58     int t=n-1;
 59     while(t>0)
 60     {
 61         if(path[t]==1)    que.push_front(1);
 62         else              que.push_front(2);
 63         t-=path[t];
 64     }
 65     //输出*********************************
 66     printf("%d\n", dp[n-1]);
 67     printf("%d %d\n", a, b);    //第一步必定是a和b先过去
 68     que.pop_front();
 69     int cnt=1;
 70     while(!que.empty())
 71     {
 72         t=que.front();        que.pop_front();
 73         cnt+=t;
 74         if(t==1)
 75         {
 76             printf("%d\n", a);
 77             printf("%d %d\n", a, vect[cnt]);
 78         }
 79         if(t==2)
 80         {
 81             printf("%d\n", a);
 82             printf("%d %d\n", vect[cnt-1], vect[cnt]);
 83             printf("%d\n", b);
 84             printf("%d %d\n", a, b);
 85         }
 86     }
 87 }
 88 
 89 
 90 int main()
 91 {
 92     freopen("input.txt", "r", stdin);
 93     int t, n, a;
 94     cin>>t;
 95     while(t--)
 96     {
 97         vect.clear();
 98         for(int i=0; i<N; i++)    ans[i].clear();
 99         memset(dp, 0x7f, sizeof(dp));
100         memset(path, 0, sizeof(path));
101 
102 
103         scanf("%d",&n);
104         for(int i=1; i<=n; i++)
105         {
106             scanf("%d", &a);
107             vect.push_back(a);
108         }
109         cal(n);
110         if(t)   puts("");
111     }
112 }
AC代码
```

