# POJ 3254 Corn Fields （状压DP，轮廓线DP） - xcw0754 - 博客园
# [POJ 3254 Corn Fields （状压DP，轮廓线DP）](https://www.cnblogs.com/xcw0754/p/4857690.html)
题意：
　　有一个n*m的矩阵（0<n,m<=12），有部分的格子可种草，有部分不可种，问有多少种不同的种草方案（完全不种也可以算1种，对答案取模后输出）？
思路：
　　明显的状压DP啦，只是怎样压缩状态？跟轮廓线DP一样，按格子为单位来设计状态，一个状态只需要表示到其上方和左方的格子，所以最多只需要保存min(n,m)个01状态就行了（可以尝试旋转一下矩阵），最多需要12位。用哈希表来做会比较快吧，不用去考虑无效的状态，比如出现相邻两个1。
```
1 //#include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <cstring>
  5 #include <cmath>
  6 #include <map>
  7 #include <algorithm>
  8 #include <vector>
  9 #include <iostream>
 10 #define pii pair<int,int>
 11 #define INF 0x7f3f3f3f
 12 #define LL long long
 13 #define ULL unsigned long long
 14 using namespace std;
 15 const double PI  = acos(-1.0);
 16 const int N=13;
 17 const int M=100000000;
 18 int g[N][N];
 19 struct Hash_Map
 20 {
 21     static const int mod=1237;
 22     static const int N=10000;
 23     int head[mod];      //桶指针
 24     int next[N];        //记录链的信息
 25     int status[N];      //状态
 26     int  value[N];       //状态对应的DP值。
 27     int size;
 28     void clear()    //清除哈希表中的状态
 29     {
 30         memset(head, -1, sizeof(head));
 31         size = 0;
 32     }
 33 
 34     void insert(int st, int val)  //插入状态st的值为val
 35     {
 36         int h = st%mod;
 37         for(int i=head[h]; i!=-1; i=next[i])
 38         {
 39             if(status[i] == st) //这个状态已经存在，累加进去。
 40             {
 41                 value[i] += val;
 42                 value[i]%=M;
 43                 return ;
 44             }
 45         }
 46         status[size]= st;           //找不到状态st，则插入st。
 47         value[size] = val;
 48         next[size] = head[h] ;      //新插入的元素在队头
 49         head[h] = size++;
 50     }
 51 }hashmap[2];
 52 
 53 int cal(int n,int m)
 54 {
 55     int cur=0, mod=(1<<m-1)-1;
 56     hashmap[cur].clear();
 57     hashmap[0].insert(0,1);
 58     for(int i=1; i<=n; i++)
 59     {
 60         for(int j=1; j<=m; j++)
 61         {
 62             cur^=1;
 63             hashmap[cur].clear();
 64             for(int k=0; k<hashmap[cur^1].size; k++)
 65             {
 66                 int s=hashmap[cur^1].status[k];
 67                 int v=hashmap[cur^1].value[k];
 68                 int t=(s&mod)<<1;   //去掉最高位
 69                 hashmap[cur].insert(t,v);       //不放
 70                 if(g[i][j])
 71                 {
 72                     if( s&(1<<m-1) )    continue;
 73                     if( j!=1 && (s&1) ) continue;   //不能放
 74                     hashmap[cur].insert(t^1,v);
 75                 }
 76             }
 77         }
 78     }
 79     int ans=0;
 80     for(int k=0; k<hashmap[cur].size; k++)
 81     {
 82         ans+=hashmap[cur].value[k];
 83         ans%=M;
 84     }
 85     return ans;
 86 }
 87 
 88 
 89 int main()
 90 {
 91     //freopen("input.txt","r",stdin);
 92     int n, m;
 93     while(~scanf("%d%d",&n,&m))
 94     {
 95         for(int i=1; i<=n; i++)
 96             for(int j=1; j<=m; j++)
 97                 scanf("%d",&g[i][j]);
 98         printf("%d\n",cal(n,m));
 99     }
100     return 0;
101 }
AC代码
```

