# HDU Today (图论) - Vmetrio - 博客园







# [HDU Today (图论)](https://www.cnblogs.com/wangmengmeng/p/5308046.html)





经过锦囊相助，海东集团终于度过了危机，从此，HDU的发展就一直顺风顺水，到了2050年，集团已经相当规模了，据说进入了钱江肉丝经济开发区500强。这时候，XHD夫妇也退居了二线，并在风景秀美的诸暨市浬浦镇陶姚村买了个房子，开始安度晚年了。
这样住了一段时间，徐总对当地的交通还是不太了解。有时很郁闷，想去一个地方又不知道应该乘什么公交车，在什么地方转车，在什么地方下车（其实徐总自己有车，却一定要与民同乐，这就是徐总的性格）。
徐总经常会问蹩脚的英文问路：“Can you help me?”。看着他那迷茫而又无助的眼神，热心的你能帮帮他吗？
请帮助他用最短的时间到达目的地（假设每一路公交车都只在起点站和终点站停，而且随时都会开）。




### Input

输入数据有多组，每组的第一行是公交车的总数N(0<=N<=10000)；
第二行有徐总的所在地start，他的目的地end；
接着有n行，每行有站名s，站名e，以及从s到e的时间整数t(0<t<100)(每个地名是一个长度不超过30的字符串)。
note：一组数据中地名数不会超过150个。
如果N==-1，表示输入结束。




### Output

如果徐总能到达目的地，输出最短的时间；否则，输出“-1”。




### Sample Input
6
xiasha westlake
xiasha station 60
xiasha ShoppingCenterofHangZhou 30
station westlake 20
ShoppingCenterofHangZhou supermarket 10
xiasha supermarket 50
supermarket westlake 10
-1
### Sample Output
50


Hint:
The best route is:
xiasha->ShoppingCenterofHangZhou->supermarket->westlake


虽然偶尔会迷路，但是因为有了你的帮助
**和**从此还是过上了幸福的生活。

――全剧终――


某地到某地  需要用到map来存储字符串(把地点用数字来标记)......之后就是Dijkstra....

如果超时就用 long long定义mp.....数组不要开太大....不然会Memory Limit Exceed.....



代码:



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
14 #define MAX 101
15 
16 int dis[MAX],vis[MAX];
17 int mp[MAX][MAX];
18 
19 int dijkstra(int s,int e)
20 {
21     memset(vis,0,sizeof(vis));
22     for(int i=1; i<=e; i++)
23         dis[i]=mp[s][i];
24     dis[s]=0;
25     vis[s]=1;
26     while(true){
27         int min=INF;
28         int p;
29         for(int i=1; i<=e; i++){
30             if(!vis[i] && dis[i]<min){
31                 min=dis[i];
32                 p=i;
33             }
34         }
35         if(min==INF)
36             break;
37         vis[p]=1;
38         for(int i=1; i<=e; i++){
39             if(!vis[i] && dis[i]>min+mp[p][i])
40                 dis[i]=min+mp[p][i];
41         }
42     }
43 }
44 
45 int main()
46 {
47     int n,count;
48     char a[33],b[33],a1[33],b1[33];
49     int cost;
50     map<string,int> che;
51     while(~scanf("%d",&n)){
52         if(n==-1)
53             break;
54         int flag=0;
55         che.clear();
56         memset(mp,INF,sizeof(mp));
57         count=3;
58         scanf("%s%s",a,b);
59         if(strcmp(a,b)==0)
60             flag=1;
61         che[a]=1,che[b]=2;
62         for(int i=0; i<n; i++){
63             scanf("%s%s%d",a1,b1,&cost);
64             if(!che[a1])
65                 che[a1]=count++;
66             if(!che[b1])
67                 che[b1]=count++;
68             if(mp[che[a1]][che[b1]]>cost)
69                 mp[che[a1]][che[b1]]=mp[che[b1]][che[a1]]=cost;
70         }
71         if(flag){
72             printf("0\n");
73             continue;
74         }
75         dijkstra(1,count-1);
76         if(dis[2]==INF)
77             printf("-1\n");
78         else
79             printf("%d\n",dis[2]);
80     }
81 }
```














