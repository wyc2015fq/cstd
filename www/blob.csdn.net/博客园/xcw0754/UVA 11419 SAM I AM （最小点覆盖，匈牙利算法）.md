# UVA 11419 SAM I AM （最小点覆盖，匈牙利算法） - xcw0754 - 博客园
# [UVA 11419 SAM I AM （最小点覆盖，匈牙利算法）](https://www.cnblogs.com/xcw0754/p/4738688.html)
题意：给一个r*c的矩阵，某些格子中可能有一些怪物，可以在一行或一列防止一枚大炮，大炮会扫光整行/列的怪，问最少需要多少炮？输出炮的位置。
思路:
　　先每行和列都放一个炮，把炮当成点，把怪当成边，一边连接行炮，另一边是列炮，只要其中任何一个扫了，另一个就不必了。所以明显是求最小点覆盖，用最少的点（即炮）覆盖最多的边（即怪）。进而转成最大匹配。因 最大匹配数=最小覆盖数。
　　这题还得求炮的位置，也就是覆盖点集。可以根据匈牙利数来构造，而且可以用同匈牙利算法那个DFS函数，稍微加点东西而已。神奇之处在于，从S中所有未盖点出发扩展匈牙利树，标记树上的S和T集中的点，则S中没有被标记的加上T中被标记的，就是一个最小覆盖集了。
　　一个比较直观的解释：假设S集中已经匹配的点就是我们要选择的覆盖集，但是S集中难免会有一些点是未匹配的，如果它没有边时这还不要紧，要是恰好有边时不就覆盖不到了吗？考虑将已经选择的覆盖集转移一下，因为之前纯选择S集中的匹配点，不如把某些点从覆盖集中删去，而换成T集中相应数量的匹配点。但是换哪些比较合适？当然是得覆盖到S集中那些有边的未匹配点，还有那些刚删的匹配点啦。这可以从那个带有边的未匹配点出发扩展匈牙利树，就会将T集中部分点给勾选出来了，这些点正是我们想要的，而左边集应该删的是哪些点？就是也是匈牙利树扩展出来的点，哈哈~
```
1 #include <bits/stdc++.h>
 2 #define max(X,Y) ((X) > (Y) ? (X) : (Y))
 3 #define min(X,Y) ((X) < (Y) ? (X) : (Y))
 4 #define pii pair<int,int>
 5 #define INF 0x7f7f7f7f
 6 #define LL long long
 7 using namespace std;
 8 const int N=1010;
 9 int g[N][N];
10 int girl[N], boy[N], S[N], vis[N];
11 int r,c;
12 
13 int hungary(int x)
14 {
15     S[x]=1;
16     for(int i=1; i<=c; i++)
17     {
18         if(!vis[i] && g[x][i])
19         {
20             vis[i]=true;
21             if(!girl[i] || hungary(girl[i]))
22             {
23                 girl[i]=x;
24                 boy[x]=i;
25                 return true;
26             }
27         }
28     }
29     return false;
30 }
31 
32 
33 
34 int cal(int n)
35 {
36     memset(girl, 0 ,sizeof(girl));
37     memset(boy, 0 ,sizeof(boy));
38     int cnt=0;
39     for(int i=1; i<=r; i++)
40     {
41         memset(vis, 0 ,sizeof(vis));
42         if(hungary(i))  cnt++;
43     }
44     printf("%d",cnt);
45     memset(S, 0 ,sizeof(S));
46     memset(vis, 0 ,sizeof(vis));
47     for(int i=1; i<=r; i++)    if(!boy[i])    hungary(i);         //未匹配的男
48 
49     for(int i=1; i<=r; i++)
50         if(!S[i])    printf(" r%d", i);
51     for(int i=1; i<=c; i++)
52         if(vis[i])   printf(" c%d", i);
53     printf("\n");
54 }
55 
56 int main()
57 {
58     freopen("input.txt", "r", stdin);
59     int a,b,n;
60     while(scanf("%d%d%d", &r,&c,&n), r+c+n)
61     {
62         memset(g, 0, sizeof(g));
63         for(int i=1; i<=n; i++)
64         {
65             scanf("%d%d",&a,&b);
66             g[a][b]=1;  //单向
67         }
68         cal(n);
69     }
70     return 0;
71 }
AC代码
```

