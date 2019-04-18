# NBUT 1116  Flandre's Passageway （LIS变形） - xcw0754 - 博客园
# [NBUT 1116  Flandre's Passageway （LIS变形）](https://www.cnblogs.com/xcw0754/p/4538315.html)
题意：
　　给一个有n*m格子的矩形，设每格边长100，要从（1，1）走到（n，m）需要耗(n+m)*100，但是其中有一些格子是可以直接穿过的，也就是走对角线，是100*根号2长，给出k个可以穿过的格子，要求最短路径是多少？
思路：
　　研究一下知道当选择了某个可穿过的格子（x，y），那么对于任意格子（>x,y）和(x,>y)都是不能再选的，因为这样会更长，也就是说同一行最多只能选一个格子穿过。一开始想到的是在一个拓扑序列中求最小路径的权之和，这个模型倒是没错，但是怎么建立一个这样的图就麻烦了。再想到用DP来穷举每个格子，复杂度O(N*M)，上限有100亿，会超时，而且当k=1，n=m=100000时，复杂度还要n*m。看到别人提出LIS最长递增子序列。先按x坐标排个序，对于每个可穿的格子，判断若要穿过此格子，其前面还能穿过几个。按照O(N^2)的方法做的，代码较短。
```
1 #include<bits/stdc++.h>
 2 using namespace std;
 3 const int N=2010;
 4 int n, m, k, dp[N];
 5 struct node
 6 {
 7     int x,y;
 8 }a[N];
 9 int cmp(node a, node b)
10 {
11     return a.x < b.x ? 1 :0;
12 }
13 bool cpr(node *a, node *b)//这里与LIS不同在：这是二维的
14 {
15     if(a->x < b->x && a->y < b->y )
16         return true;
17     else
18         return false;
19 }
20 int cal()
21 {
22     memset(dp,0,sizeof(dp));
23     int big=0;
24     for(int i=1; i<=k; i++)
25     {
26         int j=i, tmp=0;
27         while(--j)
28             if( dp[j]>tmp && cpr(&a[j],&a[i]))    tmp=dp[j];
29         dp[i]=tmp+1;
30         if(dp[i]>big)   big=dp[i];
31     }
32     return big;
33 }
34 int main()
35 {
36     //freopen("e://input.txt","r",stdin);
37     while(cin>>n>>m)
38     {
39         cin>>k;
40         a[0].x=a[0].y=-1;
41         for(int i=1; i<=k; i++)
42             scanf("%d%d",&a[i].x,&a[i].y);  //x是n那边的
43         sort(a+1,a+k+1,cmp);
44         int cnt=cal();
45         double ans=(n+m-2*cnt)*100+sqrt(2.0)*100*cnt;
46         printf("%d\n",(int)(ans+0.5));
47     }
48     return 0;
49 }
AC代码
```

