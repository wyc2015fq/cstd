# acdream  瑶瑶带你玩激光坦克 (模拟) - xcw0754 - 博客园
# [acdream  瑶瑶带你玩激光坦克 (模拟)](https://www.cnblogs.com/xcw0754/p/4651488.html)
### 　　瑶瑶带你玩激光坦克
Time Limit: 2000/1000MS (Java/Others)    Memory Limit: 256000/128000KB (Java/Others)
Submit [Status](http://acdream.info/contest?cid=1372#status--B)
#### Problem Description
有一款名为激光坦克的游戏，游戏规则是用一个坦克发出激光来达到一些目的，激光可以通过一些镜子反射。
机智的瑶瑶为了显示自己的智商高于常人，把这个游戏改造了一下，变成了用激光攻击敌人的游戏。
瑶瑶想知道射一次激光最多可以攻击到多少个敌人。
**PS: 由于激光很强大，可以在击中敌人后穿过它，而瑶瑶自己的坦克由于有特殊装置，所以不会被激光击中，激光也会直接穿过它**
#### Input
第1行两个正整数*n, m (1 ≤ n, m ≤ 1000)*表示地图大小，接下来*n*行每行*m*个字符描述地图。
**. **表示此处为空地
*** **表示此处为障碍（激光不可穿过，激光路径打到障碍时就结束）
**T **代表瑶瑶的坦克位置
**E **代表敌人
**/ **代表按 左下-右上 放置的镜子
**\ **代表按 左上-右下 放置的镜子
#### Output
一个整数代表瑶瑶向某个方向发射激光后最多可以攻击到的敌人数。
#### Sample Input
5 5
.*/E\
E*.*.
E*TEE
\.../
.*\EE
#### Sample Output
4
思路：简单模拟即可。但是不能用递归，会爆。特别要注意的是数据读入的问题（有可能不是你想的那样的输入），还有重复统计（两次射过同个点）的问题。结束的条件是射出场外或'*'或同点同方向遍历过。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=1000+100;
 7 int n, m;
 8 char g[N][N], gg[N][N];
 9 bool vis[N][N][5];
10 int tarx, tary;
11 
12 inline bool isingraph(int x,int y)  //是否在场内
13 {
14     if(x>0&&x<=n && y>0&&y<=m)    return 1;
15     else    return 0;
16 }
17 
18 
19 
20 int shot(int x, int y, int flag)      //1上，2右，3下，4左
21 {
22     int sum=0;
23     while(isingraph(x,y)==1 && gg[x][y]!='*' && vis[x][y][flag]==0 )
24     {
25         vis[x][y][flag]=1;  //注意有4个方向，只有同个点，同个方向遍历过才可结束。可能无限循环。
26         if(gg[x][y]=='\\')
27         {
28             if(flag==1)    y--,flag=4;
29             else if(flag==2)    x++,flag=3;
30             else if(flag==3)    y++,flag=2;
31             else if(flag==4)    x--,flag=1;
32         }
33         else if(gg[x][y]=='/')
34         {
35             if(flag==1)    y++,flag=2;
36             else if(flag==2)    x--,flag=1;
37             else if(flag==3)    y--,flag=4;
38             else if(flag==4)    x++,flag=3;
39         }
40         else        //E . 都在这
41         {
42             if(gg[x][y]=='E')      sum++;  //敌人
43             gg[x][y]='.';           //撞到E就改为'.'   防止多次统计
44             if(flag==1)         x--,flag=1;
45             else if(flag==2)    y++,flag=2;
46             else if(flag==3)    x++,flag=3;
47             else if(flag==4)    y--,flag=4;
48         }
49     }
50     return sum;
51 }
52 
53 
54 int cal()
55 {
56     int ans=0;
57     for(int i=1; i<5; i++)
58     {
59         memset(vis, 0, sizeof(vis));
60         memcpy(gg, g, sizeof(g));   //复制一个图出来，为了不破坏原图
61         ans=max(ans, shot(tarx, tary, i));
62     }
63     return ans;
64 }
65 
66 int main()
67 {
68     char c;
69     freopen("input.txt", "r", stdin);
70     while(~scanf("%d%d",&n,&m))
71     {
72         memset(g,0,sizeof(g));
73         for(int i=1; i<=n; i++)
74         {
75             for(int j=1; j<=m; j++)
76             {
77                 c=getchar();
78                 if(c == 'E' || c == '.' || c == '/' || c == '\\' || c == 'T' || c == '*')
79                     g[i][j]=c;
80                 else    j--;
81                 if(g[i][j]=='T')    tarx=i,tary=j;  //找人物坐标
82             }
83         }
84         printf("%d\n",cal());
85     }
86     return 0;
87 }
AC代码
```

