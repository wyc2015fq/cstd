# HDU 1114 Piggy-Bank 猪仔储钱罐（完全背包） - xcw0754 - 博客园
# [HDU 1114 Piggy-Bank 猪仔储钱罐（完全背包）](https://www.cnblogs.com/xcw0754/p/4231338.html)
题意：
　　给定一个存钱罐中要存硬币，知道空罐的重量和欲装满的重量，是否能装入？若能，打印最小价值。（注：能装的硬币重量一定刚刚好，里面的总价值要达到最小）
输入：
　　包含了T个测试例子，在第一行给出。接下来有T个例子，每个例子第一行包括两整数E和F，分别代表空罐的重量和装满钱的重量，单位都为克。 1 <= E <= F <= 10000. 第二行包含了一个整数N，代表了硬币的种类。(1 <= N <= 500)接下来N行是N种硬币的信息，每行有两个整数P和W，分别代表价值和重量，单位是克。
输出:每例输出一行，若能装入，打印The minimum amount of money in the piggy-bank is X. 其中X是钱的总额。若不能装入，打印 This is impossible.
思路：
　　和完全背包一样，不同的是硬币的重量要恰好，不能多或少，总价值要最小，这与完全背包问题相反。需要特别地处理这两个问题，重量要恰好，那么在更新dp的时候要保证这一点，总价要最小，那么在比较的时候要用min而不是max了。
```
1 #include <iostream>
 2 #define MAX 0xfffffff
 3 using namespace std;
 4 //要求：1、刚好装满    2、总价值最小
 5 int value[501];
 6 int weight[501];
 7 int dp[10010];
 8 int min(int a,int b)
 9 {
10     return a<b?a:b;
11 }
12 int cal(int v,int n)    //空间、种类
13 {
14     int i,j;
15     dp[0]=0;
16     for(i=1;i<=v;i++)
17         dp[i]=MAX;
18     for(i=1;i<=n;i++)
19         for(j=weight[i];j<=v;j++)
20         {
21                 dp[j]=min( dp[j] , dp[j-weight[i]] + value[i] );
22         }
23     return dp[v];
24 }
25 void main()
26 {
27     int T;
28     scanf("%d",&T);
29     int E,F,N,ans;
30     while(T--)
31     {
32         int i;
33         scanf("%d%d",&E,&F);    //空罐、满罐
34         scanf("%d",&N);            //多少种硬币
35         for(i=1;i<=N;i++)    
36         {
37             scanf("%d%d",&value[i],&weight[i]);    //价值、重量
38         }
39         ans=cal(F-E,N);
40         if(ans==MAX)
41             printf("This is impossible.\n");
42         else
43             printf("The minimum amount of money in the piggy-bank is %d.\n",ans);
44     }
45 }
1114
```

