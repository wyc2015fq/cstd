# HDU 5303 Delicious Apples 美味苹果  (DP) - xcw0754 - 博客园
# [HDU 5303 Delicious Apples 美味苹果  (DP)](https://www.cnblogs.com/xcw0754/p/4675868.html)
题意：
　　给一个长为L的环，起点在12点钟位置，其他位置上有一些苹果，每次带着一个能装k个苹果的篮子从起点出发去摘苹果，要将全部苹果运到起点需要走多少米？
思路：
　　无论哪处地方，只要苹果数超过k个，那么必须一次专程来运走！所以一开始可以先将他们mod k，去掉的部分先计算出来。
　　那么剩下的局面再来用贪心来做。苹果树上的苹果拆成单个来看待，最多也才10w个，那么只要保存每个苹果到起点的距离即可。接下来考虑两种情况：
（1）不饶整个圈，只走半圈：用贪心来做，类似于背包那样，dist[i-k]+pos[i]表示运完第i个苹果所需的路长。要考虑 i<k 的情况。
（2）饶圈仅1次：计算（1）时，左右都有1次是篮子不满的，那么走整个环可能更加短。走一圈，枚举在左边带i个，则右边就带k-i个，会不会小于（1）所计算的结果，会则取最小。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=105000;
 7 
 8 int app[N], l, n, k;
 9 LL dist1[N], dist2[N] ;
10 
11 int main()
12 {
13     freopen("e://input.txt", "r", stdin);
14     int t, a, b;
15     cin>>t;
16     while(t--)
17     {
18         memset(dist1,0,sizeof(dist1));
19         memset(dist2,0,sizeof(dist2));
20         memset(app,0,sizeof(app));
21 
22         scanf("%d%d%d", &l, &n, &k);
23 
24         int cnt=0;
25         for(int i=0; i<n; i++)
26         {
27             scanf("%d%d", &a, &b );
28             while(b--)    app[cnt++]=a;//拆成单个，记录距离
29         }
30 
31         vector<int> seq1, seq2;
32         for(int i=0; i<cnt; i++)
33         {
34             if( 2*app[i]<=l )    seq1.push_back(app[i]);
35             else                 seq2.push_back(l-app[i]);
36         }
37 
38         sort(seq1.begin(),seq1.end());
39         sort(seq2.begin(),seq2.end());
40 
41         for(int i=0; i<seq1.size(); i++)
42             dist1[i+1]=(i+1<=k? seq1[i] :dist1[i+1-k]+seq1[i] );
43 
44         for(int i=0; i<seq2.size(); i++)    //同理
45             dist2[i+1]=(i+1<=k? seq2[i] :dist2[i+1-k]+seq2[i] );
46 
47         LL len=(dist1[seq1.size()]+dist2[seq2.size()])*2;   //最坏的情况就是这样了。不绕圈。
48 
49         for(int i=0; i<=k; i++) //饶一圈，看带多少合适。
50         {
51             int left=max(0, (int)seq1.size()-i); //饶圈时，从左边带走i个。
52             int right=max(0, (int)seq2.size()-(k-i)); //另外只能从右边带k-i个了。
53             len=min(len, 2*(dist1[left]+dist2[right]) + l );
54         }
55         printf("%lld\n", len);
56     }
57     return 0;
58 }
AC代码
```

