# NSOJ   畅通工程(并查集) - Vmetrio - 博客园







# [NSOJ   畅通工程(并查集)](https://www.cnblogs.com/wangmengmeng/p/5307988.html)





某省调查城镇交通状况，得到现有城镇道路统计表，表中列出了每条道路直接连通的城镇。省政府“畅通工程”的目标是使全省任何两个城镇间都可以实现交通（但不一定有直接的道路相连，只要互相间接通过道路可达即可）。问最少还需要建设多少条道路？ 




### Input

测试输入包含若干测试用例。每个测试用例的第1行给出两个正整数，分别是城镇数目N ( < 1000 )和道路数目M；随后的M行对应M条道路，每行给出一对正整数，分别是该条道路直接连通的两个城镇的编号。为简单起见，城镇从1到N编号。 
注意:两个城市之间可以有多条道路相通,也就是说
3 3
1 2
1 2
2 1
这种输入也是合法的
当N为0时，输入结束，该用例不被处理。 




### Output

对每个测试用例，在1行里输出最少还需要建设的道路数目。 




### Sample Input
4 2
1 3
4 3
3 3
1 2
1 3
2 3
5 2
1 2
3 5
999 0
0
### Sample Output
1
0
2
998
### Hint

Hint



简单并查集



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
14 #define MAX 1000000 
15 
16 int n,m;
17 int f[1010];
18 
19 void init()
20 {
21     for(int i=1; i<=n; i++){
22         f[i]=i;
23     }
24 }
25 
26 int find(int x)
27 {
28     if(f[x]!=x)
29         f[x]=find(f[x]);
30     return f[x];
31 }
32 
33 void unite(int x,int y)
34 {
35     x=find(x);
36     y=find(y);
37     if(x==y)   return ;
38     if(x<y)
39         f[y]=x;
40     else
41         f[x]=y;
42 }
43 
44 int main()
45 {
46     int x,y;
47     while(~scanf("%d%d",&n,&m)){
48         int sum=0;
49         if(n==0)   break;
50          init();
51         for(int i=0; i<m; i++){
52             scanf("%d%d",&x,&y);
53             unite(x,y);
54         }
55         for(int i=1; i<=n; i++){
56             if(f[i]==i)
57                 sum++;
58         }
59         printf("%d\n",sum-1);
60     }
61 }
```














