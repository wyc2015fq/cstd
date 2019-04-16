# Nyoj  Arbitrage(Floyd or spfa or Bellman-Ford) - Vmetrio - 博客园







# [Nyoj  Arbitrage(Floyd or spfa or Bellman-Ford)](https://www.cnblogs.com/wangmengmeng/p/5366063.html)





描述
Arbitrage is the use of discrepancies in currency exchange rates to transform one unit of a currency into more than one unit of the same currency. For example, suppose that 1 US Dollar buys 0.5 British pound, 1 British pound buys 10.0 French francs, and 1 French franc buys 0.21 US dollar. Then, by converting currencies, a clever trader can start with 1 US dollar and buy 0.5 * 10.0 * 0.21 = 1.05 US dollars, making a profit of 5 percent. 

Your job is to write a program that takes a list of currency exchange rates as input and then determines whether arbitrage is possible or not.


- 输入
The input file will contain one or more test cases. Om the first line of each test case there is an integer n (1<=n<=30), representing the number of different currencies. The next n lines each contain the name of one currency. Within a name no spaces will appear. The next line contains one integer m, representing the length of the table to follow. The last m lines each contain the name ci of a source currency, a real number rij which represents the exchange rate from ci to cj and a name cj of the destination currency. Exchanges which do not appear in the table are impossible.
Test cases are separated from each other by a blank line. Input is terminated by a value of zero (0) for n.- 输出
For each test case, print one line telling whether arbitrage is possible or not in the format "Case case: Yes" respectively "Case case: No".- 样例输入
3
USDollar
BritishPound
FrenchFranc
3
USDollar 0.5 BritishPound
BritishPound 10.0 FrenchFranc
FrenchFranc 0.21 USDollar

3
USDollar
BritishPound
FrenchFranc
6
USDollar 0.5 BritishPound
USDollar 4.9 FrenchFranc
BritishPound 10.0 FrenchFranc
BritishPound 1.99 USDollar
FrenchFranc 0.09 BritishPound
FrenchFranc 0.19 USDollar

0- 样例输出
Case 1: Yes
Case 2: No- 来源
[NKOJ or 1996/97 Ulm Internal Contest](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=NKOJ+or+1996%2F97+Ulm+Internal+Contest)- 上传者
[苗栋栋](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=%E8%8B%97%E6%A0%8B%E6%A0%8B)


题意：给出一些货币和货币之间的兑换比率，问是否可以使某种货币经过一些列兑换之后，货币值增加。举例说就是1美元经过一些兑换之后，超过1美元。可以输出Yes，否则输出No。



AC代码:

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
14 #define MAX 111
15 
16 double mp[MAX][MAX];
17 int n,m;
18 
19 void floyd()
20 {
21     for(int k=1; k<=n; k++)
22         for(int i=1; i<=n; i++)
23             for(int j=1; j<=n; j++)
24                 if(mp[i][j]< mp[i][k]*mp[k][j])
25                     mp[i][j]=mp[i][k]*mp[k][j];
26 }
27 
28 void init()
29 {
30     for(int i=1; i<=n; i++){
31         for(int j=1; j<=n; j++){
32             if(i==j)
33                 mp[i][j]=1;
34             else
35                 mp[i][j]=0;
36         }
37     }
38 }
39 
40 int main()
41 {
42     int sum=0;
43     double rate;
44     char a[111],b[111],c[111];
45     while(~scanf("%d",&n)&&n){
46         init();
47         map<string,int> mmp;
48         for(int i=1; i<=n; i++){
49             scanf("%s",a);
50             mmp[a]=i;
51         }
52         scanf("%d",&m);
53         for(int i=1; i<=m; i++){
54             scanf("%s%lf%s",b,&rate,&c);
55             int x=mmp[b];
56             int y=mmp[c];
57             mp[x][y]=rate;
58             //printf("%d\n",mp[x][y]);
59         }
60         floyd();
61         int flag=0;
62         for(int i=1; i<=n; i++){
63             //printf("%d\n",mp[i][i]);
64             if(mp[i][i]>1){
65                 flag=1;
66                 break;
67             }
68         }
69         printf("Case %d: ",++sum);
70         printf("%s\n",flag ? "Yes" : "No");  
71     }
72 }
```



SPFA:

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


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
 14 #define MAX 111
 15 
 16 int n, m;
 17 double  dis[MAX], mp[MAX][MAX];
 18 struct node
 19 {
 20     char name[100];
 21 }a[MAX];
 22 
 23 int find(char *s)
 24 {
 25     for(int i = 0; i < n; i++)
 26         if(strcmp(a[i].name, s) == 0)
 27             return i;
 28 }
 29 
 30 int SPFA(int p)
 31 {
 32     queue<int> q;
 33     bool vis[MAX];
 34     memset(dis,0,sizeof(dis));
 35     memset(vis, 0, sizeof(vis));
 36     while(!q.empty())
 37         q.pop();
 38     dis[p] = 1;
 39     vis[p] = 1;
 40     q.push(p);
 41     while(!q.empty())
 42     {
 43         int x = q.front(); 
 44         q.pop();
 45         vis[x] = false;
 46         for(int i = 0; i < n; i++)
 47         {
 48             if(dis[i] < dis[x] * mp[x][i])
 49             {
 50                 dis[i] = dis[x] * mp[x][i];
 51                 if(dis[p] > 1.0)
 52                     return 1;
 53                 if(!vis[i])
 54                 {
 55                     vis[i] = true;
 56                     q.push(i);
 57                 }
 58             }
 59         }
 60     }
 61     return 0;
 62 }
 63 
 64 int main()
 65 {
 66     int i, j, cas = 0;
 67     char s1[35], s2[35];
 68     double s;
 69     while(~scanf("%d",&n) && n)
 70     {
 71         for(i = 0; i < n; i++)
 72         {
 73             for(j = 0; j < n; j++)
 74             {
 75                 if(i == j)
 76                     mp[i][j] = 1;
 77                 else
 78                     mp[i][j] = 0;
 79             }
 80         }
 81         for(i = 0; i < n; i++)
 82             scanf("%s",a[i].name);
 83         scanf("%d",&m);
 84         for(i = 0; i < m; i++)
 85         {
 86             scanf("%s%lf%s",s1, &s, s2);
 87             int u = find(s1), v = find(s2);
 88             mp[u][v] = s;
 89         }
 90         int flag = 0;
 91         for(i = 0; i < n; i++)
 92         {
 93             if(SPFA(i) == 1)
 94             {
 95                 flag = 1;
 96                 break;
 97             }
 98         }
 99         printf("Case %d: ",++cas);
100         printf("%s\n", flag ? "Yes" : "No");
101     }
102     return 0;
103 }


View Code
```



Bellman_Ford代码(hdu  可过):

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


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
14 #define MAX 111
15 
16 struct node
17 {
18     int x,y;
19     double rate;
20 }e[MAX];
21 
22 int n,m,v;
23 bool flag;
24 double dis[MAX];
25 
26 bool Bellman_Ford(int p)  
27 {  
28     memset(dis,0,sizeof(dis));
29     dis[p]=1;  
30     for(int j=0; j<n; j++)  
31         for(int i=0; i<v; i++)  
32         {  
33             if(dis[e[i].y] < dis[e[i].x] * e[i].rate)  
34                 dis[e[i].y] = dis[e[i].x] * e[i].rate;  
35         }
36     //for(int i=0; i<v; i++)
37     //    printf("%d\n",dis[e[i].y]);      
38     for(int i = 0; i<v; i++)  
39         if(dis[e[i].y] < dis[e[i].x] * e[i].rate)  
40             return true;  
41     return false;  
42 }  
43 
44 int main()
45 {
46     int sum=0;
47     char a[111], b[111], c[111];
48     double rate;  
49     while(~scanf("%d",&n)&&n){
50         v=0;
51         map<string,int> mp;
52         for(int i=1; i<=n; i++){
53             scanf("%s",a);
54             mp[a]=i;
55         }
56         scanf("%d",&m);
57         for(int i=1; i<=m; i++){
58             scanf("%s%lf%s",b,&rate,c);
59             int x=mp[b];
60             int y=mp[c];
61             e[v].x=x;
62             e[v].y=y;
63             e[v++].rate=rate;
64         }
65         flag=Bellman_Ford(1);
66         if (flag) 
67             printf("Case %d: Yes\n",++sum);
68         else 
69             printf("Case %d: No\n", ++sum);
70     }
71 }


View Code
```














