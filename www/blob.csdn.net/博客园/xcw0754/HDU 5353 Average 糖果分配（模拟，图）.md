# HDU 5353 Average 糖果分配（模拟，图） - xcw0754 - 博客园
# [HDU 5353 Average 糖果分配（模拟，图）](https://www.cnblogs.com/xcw0754/p/4709310.html)
题意：有n个人坐在圆桌上，每个人带着糖果若干，每次只能给旁边的人1科糖果，而且坐相邻的两个人最多只能给一次（要么你给我，要么我给你），问是否能将糖果平均分了。
思路：
　　明显每个人最多只能多于平均值2个糖果，因为他只能分别往左和右边的人给1颗。而多于平均值1的人可以任意选1个方向，只要到最后所有人满足了即可。多余糖果超过3的、平均数是浮点型的都是无解。
　　求解步骤：
　　在第i和第i+1个人之间建两条边（即无向边拆成2条有向边），分别从一方指向另一方。1和n也建两条。分两步：
　　（1）将持有2个多余糖果的人先处理，用DFS分别向左边和右边各深搜1次，直到遇到缺糖的人，只能成功，且要标记走过的路径（直接封掉这些无向边，即两条有向边）。
　　（2）将持有1个多余糖果的人先左边尝试给别人糖果，遇到标记的边则返回（路已经被走过了），从另一个方向尝试给别人糖果。
　　如果有任何一步失败，则无解。若以上都成功了，那么糖果就能成功平均分配。
　　但是可能会将两条边都同时标记了，那么他们互相抵消了，比如3 1 1 3 四个人。那么如果，1沿着1-2-3给了3一个糖果，而4沿着4->3>2给了2一个糖果，那么就成功了，而他们走过的边也互相抵消了，即2-3和3-2抵消了，相当于1只走到2，而4只走到3。这一步只需要在输出答案时处理一下即可。
 　　终于AC了 ！~~~ 　　上面提到的，属于同一条无向边的拆成而来的两条有向边，若同时被标记了（即都被送糖果的人走过了），那么要及时将这两条边抹去标记，恢复为可行状态。其他的人才可以继续通行这些路。只是实在想不出恢复与不恢复的区别，只是恢复了就AC了。坑~~~~耗费我几小时青春。
```
1 #include <bits/stdc++.h>
  2 #define INF 0x7f7f7f7f
  3 #define pii pair<int,int>
  4 #define LL long long
  5 using namespace std;
  6 const int N=101000;
  7 struct node
  8 {
  9     int from,to;
 10     bool vis;
 11     node(){};
 12     node(int from,int to,bool vis):from(from), to(to), vis(vis){};
 13 }edge[N*5];
 14 int edge_cnt, candy[N], even;
 15 vector<int> vect[N];
 16 vector<pii> ans;
 17 
 18 void add_node(int from,int to)
 19 {
 20     edge[edge_cnt]=node(from, to, 0);
 21     vect[from].push_back(edge_cnt++);
 22 }
 23 
 24 bool cansolve(int n, LL sum)    //是否有解
 25 {
 26     if(sum%n)    return false;
 27     for(int i=1; i<=n; i++)    if( abs(candy[i]-even )>2 ) return false;    //超过了2，只有两条边，不可能！
 28     return true;
 29 }
 30 
 31 bool DFS(int s,int far,int flag)    //注：far是为了让深搜时不能走回头。处理1时可忽略（任一方向皆可）。
 32 {
 33     if(candy[s]<even)
 34     {
 35         candy[s]++;         //给它
 36         return true;
 37     }
 38 
 39     for(int i=0; i<vect[s].size(); i++)
 40     {
 41         int t=vect[s][i];
 42         if(!edge[t].vis && edge[t].to!=far)
 43         {
 44             edge[t].vis=1;
 45             if(DFS(edge[t].to, s, flag))
 46             {
 47                 if(flag)
 48                 {
 49                     edge[t^1].vis=1;    //对于2的，一旦走过，直接封掉"后悔边"。
 50                     ans.push_back( make_pair(edge[t].from, edge[t].to) );//因为输出答案的关系，这里要先提交答案
 51                 }
 52                 if(!flag&&edge[t^1].vis)
 53                     edge[t].vis=edge[t^1].vis=0;    //重点在这里，如果抵消了，要及时将被抵消边恢复为可行。
 54                 return true;
 55             }
 56             edge[t].vis=0;  //此方向无解，还原路径。
 57         }
 58     }
 59     return false;
 60 }
 61 
 62 int cal(int n)
 63 {
 64     LL sum=0;
 65     for(int i=1; i<=n; i++)  sum+=candy[i];
 66     even=sum/n;
 67     if( !cansolve(n, sum) ) //判断是否有解
 68     {
 69         printf("NO\n");
 70         return 0;
 71     }
 72 
 73     for(int i=1; i<=n; i++) //建边
 74     {
 75         add_node(i, i%n+1 );
 76         add_node(i%n+1, i);
 77     }
 78 
 79 
 80     for(int i=1; i<=n; i++) //处理多余2的
 81     {
 82         if(candy[i]-even==2)
 83         {
 84             candy[i]=even;
 85             if(!DFS(i, (i+1)%n+1, 1) || !DFS(i, (n+i-1)%n+1, 1) ) //加个far是为了让它不能走回头
 86             {
 87                 printf("NO\n");     //只要有一个不ok，都是不行。
 88                 return 0;
 89             }
 90         }
 91     }
 92 
 93     for(int i=1; i<=n; i++) //处理多余1的
 94     {
 95         if(candy[i]-even==1 )
 96         {
 97             candy[i]=even;
 98             if( !DFS(i, -1, 0)   )    //far为-1就是可以让DFS往两个方向都尝试。
 99             {
100                 printf("NO\n"); //任一方向都给不出去，无解。
101                 return false;
102             }
103         }
104     }
105     return true;
106 }
107 
108 int main()
109 {
110     freopen("input.txt", "r", stdin);
111     int t, n;
112     cin>>t;
113     while(t--)
114     {
115         scanf("%d",&n);
116         for(int i=1; i<=n; i++)    vect[i].clear();
117         for(int i=1; i<=n; i++)    scanf("%d", &candy[i]);
118         edge_cnt=0;
119         ans.clear();
120         if(n==2)
121         {
122             if(candy[1]==candy[2])  printf("YES\n0\n");
123             else if( abs(candy[1]-candy[2])==2 )
124             {
125                 printf("YES\n1\n");
126                 if( candy[1]>candy[2])    printf("1 2\n");
127                 else    printf("2 1\n");
128             }
129             else    puts("NO");
130             continue;
131         }
132 
133 
134         if(cal(n))
135         {
136             for(int i=0; i<edge_cnt; i+=2)
137             {
138                 if( edge[i].vis && edge[i+1].vis )    continue; //抵消了，或者是处理2的时候被封的。
139                 if( edge[i].vis )    ans.push_back( make_pair(edge[i].from,   edge[i].to)  );
140                 if( edge[i+1].vis )  ans.push_back( make_pair(edge[i+1].from, edge[i+1].to));
141             }
142             printf("YES\n%d\n", ans.size());
143             for(int i=0; i<ans.size(); i++)    printf("%d %d\n", ans[i].first, ans[i].second );
144         }
145 
146     }
147     return 0;
148 }
AC代码
```
```
1 #include <bits/stdc++.h>
  2 #define INF 0x7f7f7f7f
  3 #define pii pair<int,int>
  4 #define LL long long
  5 using namespace std;
  6 const int N=101000;
  7 struct node
  8 {
  9     int from,to;
 10     bool vis;
 11     node(){};
 12     node(int from,int to,bool vis):from(from), to(to), vis(vis){};
 13 }edge[N*20];
 14 int edge_cnt, candy[N], even;
 15 vector<int> vect[N];
 16 
 17 
 18 void add_node(int from,int to)
 19 {
 20     edge[edge_cnt]=node(from, to, 0);
 21     vect[from].push_back(edge_cnt++);
 22 }
 23 
 24 bool cansolve(int n, LL sum)    //是否有解
 25 {
 26     if(sum%n>0)    return false;
 27     for(int i=1; i<=n; i++)    if( abs(candy[i]-even )>2 ) return false;
 28     return true;
 29 }
 30 
 31 bool DFS(int s,int far)
 32 {
 33     if(candy[s]<even)
 34     {
 35         candy[s]++;
 36         return true;
 37     }
 38 
 39     for(int i=0; i<vect[s].size(); i++)
 40     {
 41         int t=vect[s][i];
 42         if(!edge[t].vis && edge[t].to!=far)
 43         {
 44             edge[t].vis=1;
 45             if(DFS(edge[t].to, s)) return true;
 46             edge[t].vis=0;  //不成功，抹去标记
 47         }
 48     }
 49     return false;
 50 }
 51 
 52 int cal(int n)
 53 {
 54     LL sum=0;
 55     for(int i=1; i<=n; i++)  sum+=candy[i];
 56     even=sum/n;
 57     if( !cansolve(n, sum) )
 58     {
 59         printf("NO\n");
 60         return 0;
 61     }
 62     add_node(n, 1);//建图
 63     add_node(1, n);
 64     for(int i=1; i<n; i++)
 65     {
 66         add_node(i, i+1);
 67         add_node(i+1, i);
 68     }
 69 
 70 
 71     for(int i=1; i<=n; i++) //处理多2的
 72     {
 73         if(candy[i]-even==2)
 74         {
 75             candy[i]=even;
 76             if(!DFS(i, i+1) || !DFS(i, i-1) )
 77             {
 78                 printf("NO\n");
 79                 return 0;
 80             }
 81         }
 82     }
 83 
 84     for(int i=1; i<=n; i++) //处理多1的
 85     {
 86         if(candy[i]-even==1 )
 87         {
 88             candy[i]=even;
 89             if( !DFS(i, -1)   )
 90             {
 91                 printf("NO\n");
 92                 return false;
 93             }
 94         }
 95     }
 96     for(int i=1; i<=n; i++) 
 97     {
 98         if(candy[i]!=even )//不能平均分配
 99         {
100             printf("NO\n");
101             return false;
102         }
103     }
104     return true;
105 }
106 
107 vector<pii> ans;
108 
109 int main()
110 {
111     freopen("input.txt", "r", stdin);
112     int t, n;
113     cin>>t;
114     while(t--)
115     {
116         scanf("%d",&n);
117         for(int i=1; i<=n; i++)    vect[i].clear();
118 
119         for(int i=1; i<=n; i++)    scanf("%d", &candy[i]);
120         edge_cnt=0;
121 
122         if(n==1)//特判
123         {
124             printf("YES\n0\n");
125             continue;
126         }
127         else if(n==2)
128         {
129             if(candy[1]==candy[2])  printf("YES\n0\n");
130             else if( abs(candy[1]-candy[2])==2 )
131             {
132                 printf("YES\n1\n");
133                 if( candy[1]>candy[2])    printf("1 2\n");
134                 else    printf("2 1\n");
135             }
136             else    puts("NO");
137             continue;
138         }
139 
140 
141         if(cal(n))
142         {
143             ans.clear();
144             for(int i=0; i<edge_cnt; i+=2)
145             {
146                 if( edge[i].vis && edge[i+1].vis )    continue; //抵消了
147                 if( edge[i].vis )    ans.push_back( make_pair(edge[i].from,   edge[i].to)  );
148                 if( edge[i+1].vis )  ans.push_back( make_pair(edge[i+1].from, edge[i+1].to));
149             }
150             printf("YES\n%d\n", ans.size());
151             for(int i=0; i<ans.size(); i++)    printf("%d %d\n", ans[i].first, ans[i].second );
152         }
153     }
154     return 0;
155 }
WA代码
```

