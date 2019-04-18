# acdream 1681  跳远女王（BFS，暴力） - xcw0754 - 博客园
# [acdream 1681  跳远女王（BFS，暴力）](https://www.cnblogs.com/xcw0754/p/4572510.html)
#### Problem Description
娜娜觉得钢琴很无趣了，就抛弃了钢琴，继续往前走，前面是一片湖，娜娜想到湖的对岸，可惜娜娜找了好久都没找到小桥和小船，娜娜也发现自己不是神仙，不能像八仙过海一样。正当娜娜发愁的时候，娜娜发现湖上面有一些石头！娜娜灵机一动，发现可以沿着石头跳吖跳吖，这样一直跳下去，或许能跳到对岸！
娜娜把所有石头的位置都告诉你，然后娜娜能跳的最远距离也是知道的~请聪明的你告诉娜娜，她能够顺利到达对岸吗？
为了能够顺利的表达每个石头的位置，假设娜娜正在x轴上，表示湖的一岸，湖的另一岸是直线y=y0，湖中的石头都以有序二元组<x,y>表示，我们可以假设湖是无穷宽，两个石头的距离为几何距离，石头与岸的距离为点到直线的距离。
#### Input
多组数据，首先是一个正整数t(t<=20)表示数据组数
对于每组数据首先是三个整数y0(1<=y0<=1000),n(0<=n<=1000),d(0<=d<=1000)，分别表示湖的另一岸的位置、石头的个数、娜娜一次最远能跳的距离。
接下来是n行，每行是两个整数x,y（0<=|x|<=1000,0<y<y0）
#### Output
对于每组数据，如果娜娜能够到达湖的另一岸，先输出“YES”，再输出一个整数，表示娜娜最少要跳多少次才能到达另一岸，
如果娜娜不能到达湖的另一岸，先输出“NO”，再输出一个整数，表示娜娜距离湖的另一岸最近的距离。(注意大小写)
#### Sample Input
2
4 3 1
0 1
0 2
0 3
6 3 2
0 1
1 2
2 3
#### Sample Output
YES
4
NO
3
#### Hint
样例一，从x轴->(0,1)->(0,2)->(0,3)->对岸，总共跳4步，输出4
样例二，从x轴->(0,1)->(1,2)->(2,3)，此时距离对岸的距离为3，最大跳跃距离为2，无法到达对岸，故输出3
题意：以x轴为岸，跳到对面岸yy上，给出多块石头在y=0和y=yy之间，有石头就可以站，每次跳有距离的限制，问是否能到达对岸，若不行，距离对岸多远？
思路：广搜，石头才1000个，以每个石头为索引，每块维护一个队列，就是此石头可以跳到的石头都入队。在BFS时只有起跳岸和到达岸两个地方需要特殊处理。起跳点直接忽略，以起跳岸一步可达的石头为起点来搜。在搜到每个点时判断是否可以一步到达对面岸。
```
1 * Problem: 1681
  2 * Verdict: Accepted
  3 * Submission Date: 2015-06-09 20:14:58
  4 * Time: 180MS
  5 * Memory: 1984KB
  6 */
  7 #include <bits/stdc++.h>
  8 #define LL long long
  9 using namespace std;
 10 const int N=1005;
 11 int yy, n, d;
 12 bool vis[N];
 13  
 14 struct node
 15 {
 16     int num;
 17     int x,y;
 18 }a[N];
 19  
 20 vector< vector< node > > vect;
 21 deque< node >  que;
 22 int cal()
 23 {
 24     que.clear();
 25     int cnt=0, up=0;
 26     for(int i=0; i<n; i++)//直接以第一个队列为起点，即起跳岸一步可达之石
 27     {
 28         if(a[i].y<=d)
 29         {
 30             up=max(up,a[i].y);
 31             que.push_back( a[i] );
 32             vis[a[i].num]=1;    //已经浏览过
 33             cnt=1;
 34         }
 35     }
 36  
 37     while(!que.empty())//BFS过程
 38     {
 39         cnt++;
 40         int siz=que.size();
 41  
 42         for(int i=0; i<siz; i++)    //每个点
 43         {
 44             node tmp=que.front();
 45             for(int j=0; j<vect[tmp.num].size(); j++)   //可达的点
 46             {
 47                 node q=vect[tmp.num][j];
 48                 if(q.y+d>=yy)   //已到
 49                 {
 50                     printf("YES\n%d\n",cnt+1);
 51                     return -1;
 52                 }
 53                 else if(vis[q.num]==false)
 54                 {
 55                     que.push_back(q);
 56                     vis[q.num]=1;
 57                     up=max(up,q.y);     //可达的点，的最大值
 58                 }
 59             }
 60             que.pop_front();
 61         }
 62     }
 63     return up;
 64 }
 65  
 66  
 67 double dis(node a, node b)//求距离
 68 {
 69     return sqrt( (b.y-a.y)*(b.y-a.y)+(b.x-a.x)*(b.x-a.x)) ;
 70 }
 71  
 72 int pre()//此石头可达的石头都入队，注：只往上跳，或横跳，不往下跳
 73 {
 74     vect.clear();
 75     vector< node > tmp;
 76     for(int i=0; i<n; i++)
 77     {
 78         vect.push_back(tmp);
 79         for(int j=0; j<n; j++)
 80         {
 81             if(a[i].x==a[j].x && a[i].y==a[j].y)
 82                 continue;
 83             if( a[j].y>=a[i].y && dis(a[j],a[i])<=d )
 84             {
 85                 vect[i].push_back(a[j]);
 86             }
 87         }
 88     }
 89 }
 90  
 91  
 92  
 93 int main()
 94 {
 95    // freopen("e://input.txt", "r", stdin);
 96     int t;
 97     vector<int> tmp;
 98     cin>>t;
 99     while(t--)
100     {
101         memset(vis,0,sizeof(vis));
102         scanf("%d %d %d", &yy, &n, &d);
103         for(int i=0; i<n; i++)
104         {
105             a[i].num=i;
106             scanf("%d%d",&a[i].x,&a[i].y);
107         }
108  
109         if(d>=yy)//一步即达
110         {
111             printf("YES\n1\n");
112             continue;
113         }
114  
115         pre();  //求每个石头队列
116  
117         int ans=cal();
118         if(ans>-1)
119             printf("NO\n%d\n",yy-ans);
120     }
121  
122     return 0;
123 }
AC代码
```
