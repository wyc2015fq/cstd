# POJ 2923 Relocation（01背包+状态压缩） - xcw0754 - 博客园
# [POJ 2923 Relocation（01背包+状态压缩）](https://www.cnblogs.com/xcw0754/p/4485280.html)
题意:有人要搬家，有两辆车可以运送，有若干家具，车有容量限制，而家具也有体积，那么如何运送会使得运送车次最少？规定两车必须一起走，两车一次来回只算1躺。
思路：家具怎么挑的问题，每趟车有两种可能：1带走此家具；2此趟暂时不带走。那就是01背包了。但是限制是两只车的容量。求的是趟数。
1）数据量较小，将这10件以内的所有物品的可能组合记录一下，有2^10种，其中是包含了一些运不走的组合，滤掉，只留下可能的组合。对每种可能的新组合进行01背包，即考虑对于新组合i这种运送方案该不该取，如果取了能使车次更少，那就取。那就得穷举除了i所组合的物品外的所有可能的组合，所要决策的是 “该组合与i组合”后有没有可能减少车次，没有的话不更新了，按老方案。
```
1 #include <iostream>
 2 #include <cstring>
 3 #include <cstdio>
 4 #define INF 0x1fffffff
 5 using namespace std;
 6 const int N=14;
 7 int n,car1,car2;
 8 int w[N];
 9 int tmp_dp[1<<N];
10 int pre[1<<N];
11 int dp[1<<N];
12 
13 int can_take(const int j)
14 {
15     int sum=0;
16     memset(tmp_dp,0,sizeof(tmp_dp));
17     for(int i=0; i<n; i++)
18     {
19         if( (1<<i)&j )
20         {
21             sum+=w[i];
22             if(car1+car2<sum)
23                 return 0;
24             for(int k=car1; k>=w[i]; k-- )//对其中一只车进行01背包
25                 tmp_dp[k]=max(tmp_dp[k], tmp_dp[k-w[i]]+w[i]);
26         }
27     }
28     if(sum-tmp_dp[car1]>car2)    return 0;
29     return 1;
30 }
31 
32 int cal()
33 {
34     memset(pre,0,sizeof(pre));
35     int ful=(1<<n)-1, len=0;
36     for(int i=ful; i>0; i-- )   //预处理
37         if( can_take(i) )
38             pre[len++]=i;
39     int sta=0;
40     dp[0]=0;
41     for(int i = 1; i<=ful; i++) dp[i] = INF;    //初始化
42 
43     for(int i=0; i<len; i++)    //每个组合品
44     {
45         for(int j=ful-pre[i]; j>=0; j--)    //扣除i这几种物品，穷举其他所有的组合品（包括空组合），看是否与组合品i组合会使用更少的次数。如果原来已经有方案运走包括组合品i与其他一些组合的车次更少，那么不考虑运送组合品i（因为i组合得不合理）。
46         {
47             if( !(j&pre[i]))    //j和组合品i无交集，在原来的方案j上考虑第i个放不放，若放就将车次+1。如果放，则要更新的应该是j|pre[i]这个放了i组合品的状态。
48             {
49                 dp[j|pre[i]]=min(dp[j]+1, dp[j|pre[i]]);    //（放， 不放）
50             }
51         }
52     }
53     return dp[ful];
54 }
55 
56 
57 int main() {
58     freopen("input.txt", "r", stdin);
59     int t, e=0;
60     cin>>t;
61     while(t--)
62     {
63         scanf("%d %d %d", &n, &car1, &car2);
64         for(int i=0; i<n; i++)    scanf("%d", &w[i]);
65         printf("Scenario #%d:\n%d\n\n", ++e, cal());
66     }
67     return 0;
68 }
AC代码
```
2) WA思路：对所有可能运走的组合计算其最大运送量并记录其状态，每步从中找出不与之前相交的最大运送量，看几趟之后能全送走。贪心的思想，每步择运送量最大的，只要维持没有交集就行了，最后肯定全都送走。过了样例，过不了poj的数据。难道还有一种运法：每一趟不是最佳，但是和下一趟组合起来就是最佳？比如有两趟的方案100+60，但也有方案80+70，这样就变成每趟非最佳，但是总方案却是最佳。MYBE！
```
1 #include <iostream>
 2 #include <cstring>
 3 #include <cstdio>
 4 using namespace std;
 5 const int N=14;
 6 int n,car1,car2;
 7 int w[N];
 8 int tmp_dp[1<<N];
 9 int pre[1<<N];
10 
11 int can_take(const int j)
12 {
13     int sum=0;
14     memset(tmp_dp,0,sizeof(tmp_dp));
15     for(int i=0; i<n; i++)
16     {
17         if( (1<<i)&j )
18         {
19             sum+=w[i];
20             if(car1+car2<sum)
21                 return 0;
22             for(int k=car1; k>=w[i]; k-- )
23                 tmp_dp[k]=max(tmp_dp[k], tmp_dp[k-w[i]]+w[i]);
24         }
25     }
26     if(sum-tmp_dp[car1]>car2)    return 0;
27     return sum;
28 }
29 
30 int cal()
31 {
32     memset(pre,0,sizeof(pre));
33     int ful=(1<<n)-1, len=0;
34 
35     for(int i=ful; i>0; i-- )   //预处理:pre[i]表示i这种组合的家具价值
36         pre[i]=can_take(i);
37 
38     int sta=0;
39     for(int i=0; i<n; i++)    //最多n趟
40     {
41         int v=0,s=0;
42         for(int j=ful; j>=0; j--)   //扫描除了sta的所有组合，挑出运送量最大的。
43         {
44             if(!(j&sta))    //无交集
45             {
46                 if(pre[j]>v)
47                 {
48                     v=pre[j];
49                     s=j;
50                 }
51             }
52         }
53         sta|=s;
54         if((sta&ful)==ful)  //如果已经运完
55             return i+1;
56     }
57     return n;
58 }
59 
60 
61 int main() {
62     //freopen("input.txt", "r", stdin);
63     int t, e=0;
64     cin>>t;
65     while(t--)
66     {
67         scanf("%d %d %d", &n, &car1, &car2);
68         for(int i=0; i<n; i++)    scanf("%d", &w[i]);
69 
70         printf("Scenario #%d:\n%d\n\n", ++e, cal());
71     }
72     return 0;
73 }
WA代码
```

