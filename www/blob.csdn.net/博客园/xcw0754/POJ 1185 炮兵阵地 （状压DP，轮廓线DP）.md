# POJ 1185 炮兵阵地 （状压DP，轮廓线DP） - xcw0754 - 博客园
# [POJ 1185 炮兵阵地 （状压DP，轮廓线DP）](https://www.cnblogs.com/xcw0754/p/4858618.html)
题意：
　　给一个n*m的矩阵，每个格子中有'P'或者'H'，分别表示平地和高原，平地可以摆放大炮，而大炮的攻击范围在4个方向都是2格（除了自身位置），攻击范围内不能有其他炮，问最多能放多少个炮？（n<=100,m<=10）
思路：
　　明显需要记录到最顶上的2格，所以一共需要记录2*m个格子有没有放炮，2*m<=20，这个数字还是很大的。但是由于炮的攻击范围比较大，所以能放得下的炮比较少，也就意味着状态比较少，那么只要不用枚举[0,1<<2*m)这么大的范围都是可以解决的。即使n=100且m=10，状态数仍然很小。弄个哈希模板就解决了。
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
 16 const int N=105;
 17 
 18 struct Hash_Map{
 19     static const int mod=1237;  //根据状态数设计的哈希函数
 20     static const int N=10000;   //状态数
 21     int head[mod];      //桶指针
 22     int next[N];        //记录链的信息
 23     int status[N];      //状态
 24     int value[N];       //状态对应的DP值。
 25     int size;           //状态数
 26     void clear(){    //每次调用都要清除哈希表中的状态
 27         memset(head, -1, sizeof(head));
 28         size = 0;
 29     }
 30     void insert(int st, int val){  //插入状态st的值为val
 31         int h = st%mod;
 32         for(int i=head[h]; i!=-1; i=next[i]){
 33             if(status[i] == st){ //这个状态已经存在，累加进去。
 34                 value[i] = max(value[i], val);
 35                 return ;
 36             }
 37         }
 38         status[size]=st;    value[size]=val;//找不到状态st，则插入st。
 39         next[size]=head[h]; head[h]=size++; //新插入的元素在队头
 40     }
 41 }hashmap[2];
 42 
 43 char g[N][14];
 44 
 45 
 46 
 47 int cal(int n,int m)
 48 {
 49     int cur=0, mod=1<<2*m-1;
 50     hashmap[0].clear();
 51     hashmap[0].insert(0,0);
 52     for(int i=1; i<=n; i++)
 53     {
 54         for(int j=1; j<=m; j++)
 55         {
 56             cur^=1;
 57             hashmap[cur].clear();
 58             for(int k=0; k<hashmap[cur^1].size; k++)
 59             {
 60                 int s=hashmap[cur^1].status[k];
 61                 int v=hashmap[cur^1].value[k];
 62                 int t=(s&(mod-1))<<1;
 63                 //cout<<"123"<<endl;
 64                 if(g[i][j]=='P')    //可以放炮
 65                 {
 66                     if(s&(1<<2*m-1))    //上2
 67                         hashmap[cur].insert(t,v);
 68                     else if(s&(1<<m-1)) //上1
 69                         hashmap[cur].insert(t,v);
 70                     else if(j>2&&s&2)   //左2
 71                         hashmap[cur].insert(t,v);
 72                     else if(j>1&&s&1)    //左1
 73                         hashmap[cur].insert(t,v);
 74                     else    //可防可不放
 75                     {
 76                         hashmap[cur].insert(t,v);
 77                         hashmap[cur].insert(t+1,v+1);
 78                     }
 79                 }
 80                 else    hashmap[cur].insert(t,v);
 81             }
 82         }
 83     }
 84     int ans=-1;
 85     for(int i=0; i<hashmap[cur].size; i++)
 86         ans=max(ans,hashmap[cur].value[i]);
 87     return ans;
 88 }
 89 
 90 int main()
 91 {
 92     //freopen("input.txt","r",stdin);
 93     int n, m;
 94     while(~scanf("%d%d",&n,&m))
 95     {
 96         for(int i=1; i<=n; i++)    scanf("%s",g[i]+1);
 97         printf("%d\n",cal(n,m));
 98     }
 99     return 0;
100 }
AC代码
```

