# hihoCoder #1165 : 益智游戏  (挑战赛11 B题) - xcw0754 - 博客园
# [hihoCoder #1165 : 益智游戏  (挑战赛11 B题)](https://www.cnblogs.com/xcw0754/p/4476504.html)
题意:在一个序列中找到两个数a和b，使得a*b的因子个数最多，输出最多的因子个数。
思路：数据较多，处理会很慢。对序列中每个数字进行质数分解求因子个数，然后按照因子个数降序排列，对前50个因子最多的数进行暴力求两两之积的因子个数就行了。1s左右就能出结果。低于50的就会WA了。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 int a[100005];
 4 pair<int,int> pa[100005];
 5 int getAllFactors(int x, vector<int> &vect) //辅助！
 6 {
 7     for (int j = 2 ; j * j <= x ; ++ j)
 8     {
 9         while (x % j == 0)
10             vect.push_back(j) , x /= j;
11     }
12     if(x>1)
13         vect.push_back(x);
14     return vect.size();
15 }
16 
17 int getFactors(vector<int> &num) 
18 {
19     vector< vector<int> >   all;
20     all.resize(num.size());
21     for(int i=0; i<num.size(); i++)
22         getAllFactors( num[i], all[i]);
23 
24     vector<int> tmp;
25     for(int i=0; i<all.size(); i++)
26         tmp.insert(tmp.end(), all[i].begin(), all[i].end());
27     sort(tmp.begin(), tmp.end());
28 
29     int z=1;
30     for (int i = 0 ; i < tmp.size() ; i++)   
31     {
32         int l = i;
33         while (l < tmp.size() && tmp[l] ==tmp[i])   
34             ++ l;
35         z *= l - i + 1; 
36         i = l - 1;
37     }
38     return z;
39 }
40 
41 int factors(int N)  
42 {
43     if(1 == N) return 1;
44     int cnt = 2;  
45     for (int j = 2 ; j * j <= N ; j++)
46     {
47         if (N % j == 0)
48         {
49             cnt++;
50             if (j * j != N)  
51                 cnt++;
52         }
53     }
54     return cnt;
55 }
56 
57 int main() {
58     //freopen("input.txt", "r", stdin);
59     int t;
60     while(cin>>t)
61     {
62         for(int i=0; i<t; i++)
63         {
64             scanf("%d",&a[i]);
65             pa[i]=make_pair(factors(a[i]),a[i]);
66         }
67 
68         sort(pa,pa+t);
69         reverse(pa,pa+t);
70         vector<int> tmp;
71         int n = t>500? 500: t;
72         int ans=0;
73         for(int i=0; i<n; i++)
74         {
75             for(int j=i; j<n; j++)
76             {
77                 tmp.clear();
78                 tmp.push_back(pa[i].second);
79                 tmp.push_back(pa[j].second);
80                 ans=max(getFactors(tmp), ans);
81             }
82         }
83         cout<<ans<<endl;
84     }
85     return 0;
86 }
AC代码
```

