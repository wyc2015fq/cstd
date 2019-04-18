# HDU 1693 Eat the Trees （插头DP） - xcw0754 - 博客园
# [HDU 1693 Eat the Trees （插头DP）](https://www.cnblogs.com/xcw0754/p/4782016.html)
题意：给一个n*m的矩阵，为1时代表空格子，为0时代表障碍格子，问如果不经过障碍格子，可以画一至多个圆的话，有多少种方案？（n<12，m<12）
思路：
　　这题不需要用到最小表示法以及括号表示法。
　　以一个非障碍格子为单位进行DP转移，所以可以用滚动数组。只需要保存m+1个插头的状态，其中有一个是右插头，其他都是下插头，若有插头的存在，该位为1，否则为0，初始时都是0。
　　需要考虑的是，（1）如果两个边缘都是插头，那么必须接上它们；（2）如果仅有一边是插头，则延续插头，可以有两个延续的方向（下和右）；（3）如果都没有插头，那么必须另开两个新插头（新连通分量）。
　　如下图，记录的状态是：101111。由于是按行来保存状态的，第一个格子需要特殊考虑，将所有状态左移一位，最后的一位就是右方向的边缘。假设上行都有下插头，那么此行初始时是011111，可以看到最左边的是0，表示无右插头，注意：我是按照111110保存的，即最低位是最左边。
![](https://images2015.cnblogs.com/blog/641737/201509/641737-20150904170647825-422174169.jpg)
　　初始格子dp[0][0]=1，而答案就是dp[cur][0]了，肯定是无插头存在的状态了，所有的圆圈都是完整的。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x3f3f3f3f
 4 #define LL long long
 5 using namespace std;
 6 const int N=13;
 7 int g[N][N], cur;
 8 LL dp[N][1<<N];
 9 
10 void clear()
11 {
12     cur^=1;
13     memset(dp[cur], 0, sizeof(dp[cur]));
14 }
15 
16 LL cal(int n,int m)
17 {
18     dp[0][0]=1;   //开始时没有任何插头
19     for(int i=0; i<n; i++)  //枚举格子
20     {
21         clear();
22         for(int k=0; k<(1<<m); k++)    dp[cur][k<<1]+=dp[cur^1][k];    //最高位自动会被忽略
23         for(int j=0; j<m; j++)
24         {
25             int r=(1<<j), d=(1<<(j+1));         //r和d 相当于 右和下
26             clear();
27             for(int k=0; k<(1<<(m+1)); k++)            //枚举状态
28             {
29                 if(g[i][j])     //空格
30                 {
31                     if( (k&r) && (k&d) )        //两边都有插头:连起来，变无插头
32                         dp[cur][k^r^d]+=dp[cur^1][k];
33                     else if( k&r || k&d )       //其中一边有插头：可转两个方向
34                     {
35                         dp[cur][k]+=dp[cur^1][k];
36                         dp[cur][k^r^d]+=dp[cur^1][k];
37                     }
38                     else                        //无插头：另开两个新插头
39                         dp[cur][k|r|d]=dp[cur^1][k];
40                 }
41                 else       //障碍格子
42                 {
43                     if( !(k&r) && !(k&d) )
44                         dp[cur][k]=dp[cur^1][k];
45                 }
46             }
47         }
48     }
49     return dp[cur][0];
50 }
51 
52 int main()
53 {
54     //freopen("input.txt", "r", stdin);
55     int n, m, t, Case=0;
56     cin>>t;
57     while(t--)
58     {
59         scanf("%d%d",&n,&m);
60         memset(g, 0, sizeof(g));
61         memset(dp, 0, sizeof(dp));
62         for(int i=0; i<n; i++)
63             for(int j=0; j<m; j++)
64                 scanf("%d",&g[i][j]);
65         printf("Case %d: There are %lld ways to eat the trees.\n", ++Case, cal(n,m));
66     }
67     return 0;
68 }
AC代码
```
　　最小表示法实现：
```
1 #include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <cstring>
  5 #define pii pair<int,int>
  6 #define INF 0x3f3f3f3f
  7 #define LL long long
  8 using namespace std;
  9 const int N=15;
 10 int g[N][N], n, m, cur, code[N];
 11 struct Hash_Map
 12 {
 13     static const int mod=10007;
 14     static const int NN=1000010;
 15     int head[mod];       //桶指针
 16     int next[NN];        //记录链的信息
 17     LL  status[NN];      //状态
 18     LL  value[NN];       //状态对应的DP值。
 19     int size;
 20 
 21     void clear()
 22     {
 23         memset(head, -1, sizeof(head));
 24         size = 0;
 25     }
 26 
 27     void insert(LL st, LL val)
 28     {
 29         int h = st%mod;
 30         for(int i=head[h]; i!=-1; i=next[i])
 31         {
 32             if(status[i] == st)
 33             {
 34                 value[i] += val;
 35                 return ;
 36             }
 37         }
 38         status[size]= st;
 39         value[size] = val;
 40         next[size] = head[h] ;
 41         head[h] = size++;
 42     }
 43 }hashmap[2];
 44 
 45 
 46 void decode(LL s)
 47 {
 48     for(int i=0; i<=m; i++)
 49     {
 50         code[i]=s&7;
 51         s>>=3;        
 52     }
 53 }
 54 int cnt[N];
 55 LL encode()
 56 {
 57     LL s=0;
 58     memset(cnt, -1, sizeof(cnt));
 59     cnt[0]=0;
 60     for(int i=m,up=0; i>=0; i--)
 61     {
 62         if(cnt[code[i]]==-1)    cnt[code[i]]=++up;
 63         code[i]=cnt[code[i]];
 64         s<<=3;
 65         s|=code[i];
 66     }
 67     return s;
 68 }
 69 
 70 void DP(int i,int j)
 71 {
 72     for(int k=0; k<hashmap[cur^1].size; k++)
 73     {
 74         decode(hashmap[cur^1].status[k]);    
 75         LL v=hashmap[cur^1].value[k];
 76         
 77         int R=code[j], D=code[j+1];
 78         
 79         if(g[i][j]==0)    
 80         {
 81             if(R==0&&D==0)    hashmap[cur].insert(encode(),v);
 82             continue;
 83         }
 84         
 85         if(R&&D)
 86         {
 87             code[j]=code[j+1]=0;
 88             if(R==D)    hashmap[cur].insert(encode(),v);
 89             else
 90             {
 91                 for(int r=0; r<=m; r++)
 92                     if(code[r]==R)    code[r]=D;
 93                 hashmap[cur].insert(encode(), v);
 94             }
 95         }
 96         else if(R||D)
 97         {
 98             R+=D;
 99             if(i+1<n)    
100             {
101                 code[j]=R;
102                 code[j+1]=0;
103                 hashmap[cur].insert(encode(), v);
104             }
105             if(j+1<m)    
106             {
107                 code[j]=0;
108                 code[j+1]=R;
109                 hashmap[cur].insert(encode(), v);
110             }
111         }
112         else
113         {
114             code[j]=7;
115             code[j+1]=7;
116             if(i+1<n && j+1<m)    hashmap[cur].insert(encode(), v);
117         }
118     }
119 }
120 
121 void cal()
122 {
123     cur=0;
124     hashmap[cur].clear();
125     hashmap[cur].insert(0,1);
126     for(int i=0; i<n; i++)
127     {
128         for(int j=0; j<hashmap[cur].size; j++)    hashmap[cur].status[j]<<=3;
129         for(int j=0; j<m; j++)
130         {
131             hashmap[cur^=1].clear();
132             DP(i,j);
133             //cout<<hashmap[cur].size<<endl;
134         }
135     }
136 }
137 
138 LL print()
139 {
140     for(int i=0; i<hashmap[cur].size; i++)
141         if(hashmap[cur].status[i]==0)
142             return hashmap[cur].value[i];
143     return 0;
144 }
145 
146 int main()
147 {
148     //freopen("input.txt", "r", stdin);
149     int t, Case=0;
150     cin>>t;
151     while(t--)
152     {
153         scanf("%d%d",&n,&m);
154         for(int i=0; i<n; i++)
155         {
156             for(int j=0; j<m; j++)
157             {
158                 scanf("%d",&g[i][j]);
159             }
160         }
161         cal();
162         printf("Case %d: There are %lld ways to eat the trees.\n", ++Case, print());
163     }
164 }
AC代码
```

