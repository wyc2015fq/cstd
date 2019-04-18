# UVA 1175 Ladies' Choice 女士的选择（稳定婚姻问题，GS算法） - xcw0754 - 博客园
# [UVA 1175 Ladies' Choice 女士的选择（稳定婚姻问题，GS算法）](https://www.cnblogs.com/xcw0754/p/4737685.html)
题意：
　　给出每个男的心目中的女神排序，给出每个女的心目中的男神排序，即两个n*n的矩阵，一旦任意两个非舞伴的男女同学觉得对方都比现任舞伴要好，他们就会抛弃舞伴而在一起。为了杜绝这种现象，求每个男的最后所搭配的女伴是谁。
思路：　　
　　怎么感觉题意有点问题，输出的是第i行的男人所搭配的女人，即输出的是女人！
　　每个男人依次选择最喜欢的女人求婚，若同一个女的没人抢，则临时抢亲成功，若需要抢，明显女的都是喜欢挑长腿oba啦，矮冬瓜自动加入单身狗队列继续求婚！当单身狗所求的女神已订婚，只要女神更爱单身狗，没有什么可以阻挡他们，女神果断抛弃未婚夫，未婚夫加入单身狗队列。直至地球上没有单身狗为止！
　　复杂度O(n2)，每个男的最多只会向n个女的都求婚一次，而女的没有选择权，只有接受和拒绝。著名的Gale-Shapley算法。
```
1 #include <bits/stdc++.h>
 2 #define max(X,Y) ((X) > (Y) ? (X) : (Y))
 3 #define min(X,Y) ((X) < (Y) ? (X) : (Y))
 4 #define pii pair<int,int>
 5 #define INF 0x7f7f7f7f
 6 #define LL long long
 7 using namespace std;
 8 const int N=1010;
 9 
10 deque<int> que[N];      //心目中的女神排名队列
11 int girl[N], boy[N];
12 int imp[N][N];
13 
14 int cal(int n)
15 {
16     memset(girl, 0, sizeof(girl) );
17     deque<int> single;
18     for(int i=1; i<=n; i++) single.push_back(i);
19 
20     while(!single.empty())
21     {
22         int x=single.front();  single.pop_front();   //取出单身狗
23         int op=que[x].front(); que[x].pop_front();   //取出x的暗恋对象
24 
25         if(!girl[op])    girl[op]=x,boy[x]=op;       //女神初恋
26         else if(  imp[op][x] < imp[op][girl[op]] )   //女神抛弃现任
27         {
28             single.push_back(girl[op]);
29             girl[op]=x;
30             boy[x]=op;
31         }
32         else    single.push_back(x);        //单身狗被拒绝
33     }
34     for(int i=1; i<=n; i++)    printf("%d\n",boy[i]);
35 }
36 
37 
38 int main()
39 {
40     //freopen("input.txt", "r", stdin);
41     int t, n, b, j=0;
42     cin>>t;
43     while(t--)
44     {
45         if(j)   printf("\n");
46         j++;
47         scanf("%d", &n);
48         for(int i=1; i<=n; i++)
49         {
50             que[i].clear();
51             for(int j=1; j<=n; j++)
52             {
53                 scanf("%d",&b);
54                 que[i].push_back(b);
55             }
56         }
57         for(int i=1; i<=n; i++)
58         {
59             for(int j=1; j<=n; j++)
60             {
61                 scanf("%d",&b);
62                 imp[i][b]=j;    //b对于i来说，排在第几。
63             }
64         }
65 
66         cal(n);
67     }
68     return 0;
69 }
AC代码
```

