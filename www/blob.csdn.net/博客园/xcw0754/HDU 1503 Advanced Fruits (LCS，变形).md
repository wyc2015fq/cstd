# HDU 1503 Advanced Fruits (LCS，变形) - xcw0754 - 博客园
# [HDU 1503 Advanced Fruits (LCS，变形)](https://www.cnblogs.com/xcw0754/p/4642157.html)
题意：
　　给两个水果名，要求他们的LCS部分只输出1次，其他照常输出，但是必须保持原来的顺序！
思路：
　　求LCS是常规的，但是输出麻烦了，要先求LCS，再标记两串中的所有LCS字符，在遇到LCS字符时，先输串1的，再输串2的，然后输该字符，以保证每个LCS字符输出前，两串中该字符前面的字符全部已输出。
```
1 //#pragma comment(linker,"/STACK:102400000,102400000")
 2 #include <iostream>
 3 #include <stdio.h>
 4 #include <string.h>
 5 #include <vector>
 6 #include <stack>
 7 #include <algorithm>
 8 #include <map>
 9 #include <bits/stdc++.h>
10 #define LL long long
11 #define pii pair<int,int>
12 #define INF 0x7f7f7f7f
13 using namespace std;
14 const int N=100+5;
15 char s1[N], s2[N];
16 
17 int mark[N][N]; 
18 int cnt[N][N];
19 
20 int pos1[N];
21 int pos2[N];
22 
23 void LCS()
24 {
25     memset(mark, 0, sizeof(mark));
26     memset(cnt, 0, sizeof(cnt));
27     int len1=strlen(s1+1);
28     int len2=strlen(s2+1);
29 
30     for(int i=1; i<=len1; i++)
31     {
32         for(int j=1; j<=len2; j++)
33         {
34             if(s1[i]==s2[j])
35             {
36                 cnt[i][j]=cnt[i-1][j-1]+1;
37                 mark[i][j]=3;   //斜的
38             }
39             else if(cnt[i-1][j]>=cnt[i][j-1])
40             {
41                 cnt[i][j]=cnt[i-1][j];
42                 mark[i][j]=2;   //上边
43             }
44             else
45             {
46                 cnt[i][j]=cnt[i][j-1];
47                 mark[i][j]=1;   //左边
48             }
49         }
50     }
51     int t1=len1, t2=len2;
52     memset(pos1,0,sizeof(pos1));
53     memset(pos2,0,sizeof(pos2));
54     while(t1>0 && t2>0)
55     {
56         int pre=mark[t1][t2];
57 
58         if(pre==3)
59         {
60             pos1[t1]=1;
61             pos2[t2]=1;
62             t1--,t2--;
63         }
64         else if(pre==2)    t1--;
65         else    t2--;
66     }
67     int i=1, j=1;
68 
69     while(1)
70     {
71         while(i<=len1 && !pos1[i])
72             printf("%c",s1[i++]);
73         i++;
74         while(j<=len2 && !pos2[j])
75             printf("%c",s2[j++]);
76         if(j<=len2)    printf("%c",s2[j]);
77         j++;
78         if(i>len1&&j>len2)  return ;
79     }
80 }
81 
82 int main()
83 {
84     freopen("input.txt", "r", stdin);
85     while(~scanf("%s%s", s1+1, s2+1))
86     {
87         LCS();
88         printf("\n");
89     }
90     return 0;
91 }
AC代码
```

