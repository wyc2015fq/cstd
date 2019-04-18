# ZOJ 3466 The Hive II （插头DP，变形） - xcw0754 - 博客园
# [ZOJ 3466 The Hive II （插头DP，变形）](https://www.cnblogs.com/xcw0754/p/4798940.html)
题意：有一个n*8的蜂房（6边形的格子），其中部分是障碍格子，其他是有蜂蜜的格子，每次必须走1个圈取走其中的蜂蜜，在每个格子只走1次，且所有蜂蜜必须取走，有多少种取法？
![](https://images2015.cnblogs.com/blog/641737/201509/641737-20150910193248372-1486443982.jpg)
思路：
　　以前涉及的只是n*m的矩阵，也就是四边形的，现在变成了6边形，那么每个格子也就有6个方向可以出/进。为了方便考虑，将蜂房变成按列来扫，那么轮廓线需要2*n+1个插头信息。这里不需要用到括号表示法或者最小表示法，只需要表示该位置是否有线即可，所以每个插头仅需1个位就可以表示了。
　　转置一下后的蜂房应该是这样的：
![](https://images2015.cnblogs.com/blog/641737/201509/641737-20150910193729325-1547854998.png)
　　轮廓线是一个左，一个左上，一个右上来组成的。当前格子为奇数行时，状态需要左移2位，因为有两个方向（左，左上）是空出来的。其他的只需要根据奇偶数行来判断即可。
2700ms+
```
1 #include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <cstring>
  5 #define pii pair<int,int>
  6 #define INF 0x3f3f3f3f
  7 #define LL long long
  8 using namespace std;
  9 const int N=26;
 10 int g[N][N], cur, n, m;
 11 struct Hash_Map
 12 {
 13     static const int mod=10007;
 14     static const int NN=500010; //至少50万
 15     int head[mod];       //桶指针
 16     int next[NN];        //记录链的信息
 17     LL  status[NN];      //状态
 18     LL  value[NN];       //状态对应的DP值。
 19     int size;
 20 
 21     void clear()    //清除哈希表中的状态
 22     {
 23         memset(head, -1, sizeof(head));
 24         size = 0;
 25     }
 26 
 27     void insert(LL st, LL val)  //插入状态st的值为val
 28     {
 29         int h = st%mod;
 30         for(int i=head[h]; i!=-1; i=next[i])
 31         {
 32             if(status[i] == st) //这个状态已经存在，累加进去。
 33             {
 34                 value[i] += val;
 35                 return ;
 36             }
 37         }
 38 
 39         status[size]= st;           //找不到状态st，则插入st。
 40         value[size] = val;
 41         next[size] = head[h] ;      //新插入的元素在队头
 42         head[h] = size++;
 43     }
 44 }hashmap[2];
 45 inline int getbit(LL s,int pos)
 46 {
 47     return ((s>>pos)&1);
 48 }
 49 
 50 inline LL setbit(LL s,int pos,int bit)
 51 {
 52     s&=~((LL)1<<pos);
 53     return (s|(LL)bit<<pos);
 54 }
 55 
 56 void DP(int i,int j)
 57 {
 58     for(int k=0; k<hashmap[cur^1].size; k++)
 59     {
 60         LL s=hashmap[cur^1].status[k];
 61         LL v=hashmap[cur^1].value[k];
 62         int R=getbit(s,2*j), DL=getbit(s,2*j+1), DR=getbit(s,2*j+2);
 63         LL t=(setbit(s,2*j+0,0)&setbit(s,2*j+1,0)&setbit(s,2*j+2,0));
 64         if(g[i][j]==0)  //必走格子
 65         {
 66             if( R && DL && DR )   continue;   //3个插头，冲突了。
 67             if( R && DL || R && DR || DL && DR )    hashmap[cur].insert(t,v);   //合并
 68             else if( R || DL || DR )    //延续
 69             {
 70                 if( j+1<m )    hashmap[cur].insert(setbit(t,2*j+2,1), v);       //右
 71                 if( i+1<n )
 72                 {
 73                     if( !(i&1) || j+1<m  ) hashmap[cur].insert(setbit(t,2*j+1,1), v);   //右下
 74                     if(  (i&1) || j>0 )    hashmap[cur].insert(setbit(t,2*j+0,1), v);   //左下
 75                 }
 76             }
 77             else    //新插头
 78             {
 79                 if(i+1<n)
 80                 {
 81                     if( (j>0&&!(i&1)) || (j+1<m&&(i&1)) )   //（左下，右下）
 82                         hashmap[cur].insert( setbit(s,2*j+0,1)|setbit(s,2*j+1,1), v);
 83                     if( j+1<m )
 84                     {
 85                         if( (i&1) || j>0 )     //（左下，右）
 86                             hashmap[cur].insert( setbit(s,2*j+0,1)|setbit(s,2*j+2,1), v);
 87                         if( !(i&1)  || j+1<m )   //（右下，右）
 88                             hashmap[cur].insert( setbit(s,2*j+1,1)|setbit(s,2*j+2,1), v);
 89                     }
 90                 }
 91             }
 92         }
 93         else if(R+DL+DR==0)    hashmap[cur].insert(s,v); //障碍格子
 94     }
 95 }
 96 
 97 void cal()
 98 {
 99     cur=0;
100     hashmap[cur].clear();
101     hashmap[cur].insert(0,1);
102     for(int i=0; i<n; i++)
103     {
104         if( !(i&1) )    //只有奇数行才需要左移
105         {
106             for(int k=0; k<hashmap[cur].size; k++)    hashmap[cur].status[k]<<=2;
107         }
108         for(int j=0; j<m; j++)
109         {
110             cur^=1;
111             hashmap[cur].clear();
112             DP(i,j);
113         }
114     }
115 }
116 LL print()
117 {
118     for(int i=0; i<hashmap[cur].size; i++)
119         if(hashmap[cur].status[i]==0)
120             return hashmap[cur].value[i];
121     return 0;
122 }
123 
124 int main()
125 {
126     freopen("input.txt", "r", stdin);
127     while(~scanf("%d%d",&n,&m))
128     {
129         memset(g, 0, sizeof(g));
130         char c1,c2;
131         for(int i=0; i<m; i++)
132         {
133             c1=getchar();
134             while(!isalpha(c1)) c1=getchar();
135             c2=getchar();
136             while(!isalpha(c2)) c2=getchar();
137             g[c2-'A'][c1-'A']=1;    //1为障碍格子
138         }
139         m=8;
140         swap(n,m);
141         for(int i=0; i*2<n; i++)
142         {
143             for(int j=0; j<m; j++)
144                 swap(g[i][j],g[n-1-i][j]);
145         }
146 
147         cal();
148         printf("%lld\n", print() );
149     }
150     return 0;
151 }
AC代码
```

