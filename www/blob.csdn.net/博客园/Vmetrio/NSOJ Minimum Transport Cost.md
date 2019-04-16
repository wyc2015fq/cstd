# NSOJ  Minimum Transport Cost - Vmetrio - 博客园







# [NSOJ  Minimum Transport Cost](https://www.cnblogs.com/wangmengmeng/p/5311914.html)







These are N cities in Spring country. Between each pair of cities there may be one transportation track or none. Now there is some cargo that should be delivered from one city to another. The transportation fee consists of two parts: 
The cost of the transportation on the path between these cities, and

a certain tax which will be charged whenever any cargo passing through one city, except for the source and the destination cities.

You must write a program to find the route which has the minimum cost.




### Input

First is N, number of cities. N = 0 indicates the end of input.

The data of path cost, city tax, source and destination cities are given in the input, which is of the form:

a11 a12 ... a1N
a21 a22 ... a2N
...............
aN1 aN2 ... aNN
b1 b2 ... bN

c d
e f
...
g h

where aij is the transport cost from city i to city j, aij = -1 indicates there is no direct path between city i and city j. bi represents the tax of passing through city i. And the cargo is to be delivered from city c to city d, city e to city f, ..., and g = h = -1. You must output the sequence of cities passed by and the total cost which is of the form:




### Output

From c to d :
Path: c-->c1-->......-->ck-->d
Total cost : ......
......

From e to f :
Path: e-->e1-->..........-->ek-->f
Total cost : ......

Note: if there are more minimal paths, output the lexically smallest one. Print a blank line after each test case.





### Sample Input
5
0 3 22 -1 4
3 0 5 -1 -1
22 5 0 9 20
-1 -1 9 0 4
4 -1 20 4 0
5 17 8 3 1
1 3
3 5
2 4
-1 -1
0
### Sample Output
From 1 to 3 :
Path: 1-->5-->4-->3
Total cost : 21

From 3 to 5 :
Path: 3-->4-->5
Total cost : 16

From 2 to 4 :
Path: 2-->1-->5-->4
Total cost : 17






   看来自己floyd还是没有深刻的理解....用Floyd存路径很是不错...get....



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
16 int cost[MAX];
17 int mp[MAX][MAX],path[MAX][MAX];
18 int n;
19 
20 void floyd()
21 {
22     for(int k=1; k<=n; k++)
23         for(int i=1; i<=n; i++)
24             for(int j=1; j<=n; j++){
25                 int ans=mp[i][k]+mp[k][j]+cost[k];
26                 if(mp[i][j]>ans){
27                     mp[i][j]=ans;
28                     path[i][j]=path[i][k];
29                 }
30                 else if(mp[i][j]==ans && path[i][j]>path[i][k]){
31                     path[i][j]=path[i][k];
32                 }
33             }
34 }
35 
36 int main()
37 {
38     while(~scanf("%d",&n)&&n!=0){
39         for(int i=1; i<=n; i++){
40             for(int j=1; j<=n; j++){
41                 if(i!=j)
42                     mp[i][j]=INF;
43                 else
44                     mp[i][j]=0;
45                 path[i][j]=j;
46             }
47         }
48         for(int i=1; i<=n; i++){
49             for(int j=1; j<=n; j++){
50                 scanf("%d",&mp[i][j]);
51                 if(mp[i][j]==-1)
52                     mp[i][j]=INF;
53             }
54         }
55         for(int i=1; i<=n; i++){
56             scanf("%d",&cost[i]);
57         }
58         floyd();
59         int a,b;
60         while(~scanf("%d%d",&a,&b)){
61             if(a==-1&&b==-1)
62                 break;
63             printf("From %d to %d :\n",a,b);
64             printf("Path: ");
65             int s=a;   
66             while(s!=b){
67                 printf("%d-->",s);
68                 s=path[s][b];
69             }
70             printf("%d\n",b);
71             printf("Total cost : %d\n\n", mp[a][b]);
72         }
73 
74     }
75 }
```














