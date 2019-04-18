# POJ 3260 The Fewest Coins 最少硬币个数（完全背包+多重背包，混合型） - xcw0754 - 博客园
# [POJ 3260 The Fewest Coins 最少硬币个数（完全背包+多重背包，混合型）](https://www.cnblogs.com/xcw0754/p/4493874.html)
题意:FJ身上有各种硬币，但是要买m元的东西，想用最少的硬币个数去买，且找回的硬币数量也是最少（老板会按照最少的量自动找钱），即掏出的硬币和收到的硬币个数最少。
思路：老板会自动找钱，且按最少的找，硬币数量也不限，那么可以用完全背包得出组成每个数目的硬币最少数量。而FJ带的钱是有限的，那么必须用多重背包，因为掏出的钱必须大于m，那么我们所要的是大于等于m钱的硬币个数，但是FJ带的钱可能很多，超过m的很多倍都可能，那么肯定要有个背包容量上限，网上说的根据抽屉原理是m+max*max，这里的max指的是最大面值。而给多了的钱上限是max*max，那么找回的钱也必须是max*max，所以完全背包部分的背包容量是max*max。穷举这max*max个可能就行了。
我的思路：与上面不同的是多重背包的容量应该是m+max，因为如果需要找回的钱大于max，那么老板也只是拿多几张最大面额的给你而已。比如买条烟1329块钱，13+1+1+4=19张RMB， 那么我们可以给他14张，15张，16张，17张，18张100的，老板会相应找回71块，171块，271块，371块，471块，你再往上加钱的话，老板也只是拿更多的100还你，这是多余的。那么最多不会超过一张一百（最大面额）的，也就是1329+100=1429为背包容量。错了很多次！
```
1 #include <iostream>
 2 #include <stdio.h>
 3 #include <cstring>
 4 #include <algorithm>
 5 #define INF 0x0ffffffa
 6 using namespace std;
 7 const int N=120;
 8 const int limit=30000;
 9 int n, t;
10 int com[limit];    //完全背包
11 int mul[limit];    //多重背包
12 int big;
13 struct node
14 {
15     int v,c;
16 }coin[N];
17 
18 int cal()
19 {
20     for(int i=1; i<=big*big; i++)        com[i]=INF;
21     com[0]=0;
22     for(int i=0; i<n; i++)        //完全背包
23     {
24         for(int j=0; j+coin[i].v<=big*big; j++) //上限big*big
25         {
26             if(com[j+coin[i].v]>com[j]+1)
27                 com[j+coin[i].v]= com[j]+1 ;
28         }
29 
30     }
31 
32     int upto= t+ big*big;   //多重背包上限
33     for(int i=1; i<=upto; i++)        mul[i]=INF;
34     mul[0]=0;
35     for(int i=0; i<n; i++ )        //多重背包：01背包+二进制
36     {
37         int k=1,tmp=coin[i].c;
38         while(1)
39         {
40             if(k>tmp&&tmp)    k=tmp;
41             else if(k>tmp)        break;
42             for(int j=upto; j>=k*coin[i].v; j-- )
43                 if(mul[j-k*coin[i].v ]!=INF&&mul[j-k*coin[i].v]+k<mul[j])
44                     mul[j]=mul[j-k*coin[i].v]+k;
45             tmp-=k;
46             k<<=1;
47         }
48     }
49     int ans=mul[t]; //刚好给t元
50     for(int i=t+1,j=1; i<upto; i++,j++ )
51     {
52         if(com[j]==INF || mul[i]==INF)    continue;   //INF的表示不能刚好凑成这个价，滤掉。
53         else if(ans>mul[i]+com[j] )    ans= mul[i]+com[j];
54     }
55 
56     if(ans==INF)    return -1;
57     return ans;
58 }
59 int main()
60 {
61     //freopen("input.txt","r",stdin);
62     while(cin>>n>>t)
63     {
64         big=-1;
65         for(int i=0; i<n; i++)
66         {
67             scanf("%d",&coin[i].v);
68             if(big<coin[i].v)    big=coin[i].v;
69         }
70         for(int i=0; i<n; i++)    scanf("%d",&coin[i].c);
71         printf("%d\n",cal());
72     }
73 
74     return 0;
75 }
AC代码
```

