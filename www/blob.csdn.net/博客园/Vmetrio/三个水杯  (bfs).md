# 三个水杯   (bfs) - Vmetrio - 博客园







# [三个水杯   (bfs)](https://www.cnblogs.com/wangmengmeng/p/5268083.html)





给出三个水杯，大小不一，并且只有最大的水杯的水是装满的，其余两个为空杯子。三个水杯之间相互倒水，并且水杯没有标识，只能根据给出的水杯体积来计算。现在要求你写出一个程序，使其输出使初始状态到达目标状态的最少次数。




### Input

第一行一个整数N(0<N<50)表示N组测试数据
接下来每组测试数据有两行，第一行给出三个整数V1 V2 V3 (V1>V2>V3 V1<100 V3>0)表示三个水杯的体积。
第二行给出三个整数E1 E2 E3 （体积小于等于相应水杯体积）表示我们需要的最终状态




### Output

每行输出相应测试数据最少的倒水次数。如果达不到目标状态输出-1




### Sample Input
2
6 3 1
4 1 1
9 3 2
7 1 1

### Sample Output
3
-1



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
 13 
 14 int s1,s2,s3;
 15 bool vis[103][103][103];
 16 
 17 struct node
 18 {
 19     int a,b,c;
 20     int bu;
 21 }x,y;
 22 
 23 int bfs()
 24 {
 25     queue<node> q;
 26     while(!q.empty())
 27         q.pop();
 28     q.push(x);
 29     while(!q.empty()){
 30         node r=q.front();
 31         q.pop();
 32         vis[r.a][r.b][r.c]=true;
 33         if(r.a==y.a&&r.b==y.b&&r.c==y.c)  return r.bu;
 34 
 35         if(0<r.a && r.b<s2){
 36             node t=r;
 37             int tt=min(t.a,s2-t.b);
 38             t.a-=tt;
 39             t.b+=tt;
 40             if(vis[t.a][t.b][t.c]==false){
 41                 vis[t.a][t.b][t.c]=true;
 42                 t.bu++;
 43                 q.push(t);
 44             }
 45         }
 46 
 47         if(0<r.a && r.c<s3){
 48             node t=r;
 49             int tt=min(t.a,s3-t.c);
 50             t.a-=tt;
 51             t.c+=tt;
 52             if(vis[t.a][t.b][t.c]==false){
 53                 vis[t.a][t.b][t.c]=true;
 54                 t.bu++;
 55                 q.push(t);
 56             }
 57         }
 58         
 59         if(0<r.b && r.a<s1){
 60             node t=r;
 61             int tt=min(t.b,s1-t.a);
 62             t.b-=tt;
 63             t.a+=tt;
 64             if(vis[t.a][t.b][t.c]==false){
 65                 vis[t.a][t.b][t.c]=true;
 66                 t.bu++;
 67                 q.push(t);
 68             }
 69         }
 70         
 71         if(0<r.b && r.c<s3){
 72             node t=r;
 73             int tt=min(t.b,s3-t.c);
 74             t.b-=tt;
 75             t.c+=tt;
 76             if(vis[t.a][t.b][t.c]==false){
 77                 vis[t.a][t.b][t.c]=true;
 78                 t.bu++;
 79                 q.push(t);
 80             }
 81         }
 82 
 83         if(0<r.c && r.a<s1){
 84             node t=r;
 85             int tt=min(t.c,s1-t.a);
 86             t.c-=tt;
 87             t.a+=tt;
 88             if(vis[t.a][t.b][t.c]==false){
 89                 vis[t.a][t.b][t.c]=true;
 90                 t.bu++;
 91                 q.push(t);
 92             }
 93         }
 94 
 95         if(0<r.c && r.b<s2){
 96             node t=r;
 97             int tt=min(t.c,s2-t.b);
 98             t.c-=tt;
 99             t.b+=tt;
100             if(vis[t.a][t.b][t.c]==false){
101                 vis[t.a][t.b][t.c]=true;
102                 t.bu++;
103                 q.push(t);
104             }
105         }
106     }
107     return -1;
108 }
109 
110 int main()
111 {
112     int n;
113     scanf("%d",&n);
114     while(n--){
115         memset(vis,false,sizeof(vis));
116         scanf("%d %d %d",&s1,&s2,&s3);
117         x.a=s1,x.b=0,x.c=0,x.bu=0;
118         scanf("%d %d %d",&y.a,&y.b,&y.c);
119         if(s1<y.a+y.b+y.c){
120             printf("-1\n");
121             continue;
122         }
123         else{
124             printf("%d\n",bfs());
125         }
126     }
127 }
```














