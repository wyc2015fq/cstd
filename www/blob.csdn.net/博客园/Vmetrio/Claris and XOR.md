# Claris and XOR - Vmetrio - 博客园







# [Claris and XOR](https://www.cnblogs.com/wangmengmeng/p/5372629.html)






Problem Description


Claris loves bitwise operations very much, especially XOR, because it has many beautiful features. He gets four positive integers a,b,c,d




Input


The first line contains an integer T\left(1\leq T\leq10,000\right)




Output


For each test case, the only line contains a integer that is the maximum value of x~XOR~y




Sample Input

2
1 2 3 4
5 7 13 15




Sample Output

Copy6
11

Hint

In the first test case, when and only when x=2,y=4







这游戏真难,之前写过一道类似的,就直接敲了...o(n)...超时....





TLE代码:


```
1 #include <vector>
 2 #include <map>
 3 #include <set>
 4 #include <algorithm>
 5 #include <iostream>
 6 #include <cstdio>
 7 #include <cmath>
 8 #include <cstdlib>
 9 #include <string>
10 #include <cstring>
11 #include <queue>
12 using namespace std;
13 #define INF 0x3f3f3f3f
14 #define ll long long
15 
16 int const MAX = 100000005;
17 int n;
18 
19 struct Trie
20 {
21     int root, tot, next[MAX][2], end[MAX];
22     inline int node()
23     {
24         memset(next[tot], -1, sizeof(next[tot]));
25         end[tot] = 0;
26         return tot ++;
27     }
28 
29     inline void Init()
30     {
31         tot = 0;
32         root = node();
33     }
34 
35     inline void insert(ll x)
36     {
37         int p = root;
38         for(int i = 31; i >= 0; i--)
39         {
40             int ID = ((1 << i) & x) ? 1 : 0;
41             if(next[p][ID] == -1)
42                 next[p][ID] = node();
43             p = next[p][ID];
44         }
45         end[p] = x;
46     }
47 
48     inline int search(int x)
49     {
50         int p = root;
51         for(int i = 31; i >= 0; i--)
52         {
53             int ID = ((1 << i) & x) ? 1 : 0;
54             if(ID == 0)
55                 p = next[p][1] != -1 ? next[p][1] : next[p][0];
56             else
57                 p = next[p][0] != -1 ? next[p][0] : next[p][1];
58         }
59         return x ^ end[p];
60     }
61 
62 }trie;
63 
64 int a[2],b[2];
65 int main()
66 {
67     int t;
68     scanf("%d",&t);
69     while(t--)
70     {
71         int n=4;
72         int WTF = 0,ans=0, x;
73         trie.Init();
74         for(int i = 0; i < 2; i++)
75         {
76             scanf("%d", &a[i]);
77         }
78         for(int i = 0; i < 2; i++)
79         {
80             scanf("%d", &b[i]);
81         }
82         for(int i=a[0]; i<=a[1]; i++){
83             //WTF=0;
84             //trie.insert(1);
85             //WTF = WTFx(WTF, trie.search(1));
86             for(int j=b[0]; j<=b[1]; j++){
87                 trie.Init();
88                 trie.insert(i);
89                 WTF = max(WTF, trie.search(i));
90                 trie.insert(j);
91                 WTF = max(WTF, trie.search(j));
92                 ans=max(WTF,ans);
93             }
94         }    
95         printf("%d\n", WTF);
96     }
97 }
```



















