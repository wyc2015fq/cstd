# URAL   1057  Amount of Degrees  （数位DP，入门） - xcw0754 - 博客园
# [URAL   1057  Amount of Degrees  （数位DP，入门）](https://www.cnblogs.com/xcw0754/p/4851713.html)
题意：
　　求给定区间[X,Y]中满足下列条件的整数个数：这个数恰好等于K个互不相等的，B的整数次幂之和。例如，设X=15，Y=20，K=2，B=2，则有且仅有下列三个数满足了要求：  17 = 24+20， 18 = 24+21， 20 = 24+22。(以B为底数，幂次数不允许相同)
　　参考论文[--》》论文中的题。](http://wenku.baidu.com/view/d2414ffe04a1b0717fd5dda8.html?from=search)
思路：
　　论文倒是容易看明白，但是这个转成B进制的思想一直转不过来。其实转成B进制后变成 a1*Bn+a2*Bn-1...an*B0。其中ai是系数。范围是[0,B-1]。但是看了论文知道，里面画的那棵01树（树上的01就是代表系数a），只有从根走到叶子，经过的1的个数为K才是满足要求的。那么如果a大于0怎么办？那么从树上该点开始的整棵子树就可以全部进行考虑了。而如果刚好考虑的位为1的呢？那么取该位为0的那棵子树就行了。
　　两种实现
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <map>
 7 #include <algorithm>
 8 #include <vector>
 9 #include <iostream>
10 #define pii pair<int,int>
11 #define INF 0x7f3f3f3f
12 #define LL long long
13 using namespace std;
14 const double PI  = acos(-1.0);
15 const int N=35; //注意大小
16 
17 int f[N][N];
18 
19 void pre_cal()  //预处理组合数
20 {
21     f[0][0]=1;
22     for(int i=1; i<N; i++) //位数
23     {
24         f[i][0]=f[i][i]=1;
25         for(int j=1; j<i; j++) //多少个1
26         {
27             f[i][j]=f[i-1][j]+f[i-1][j-1];
28         }
29     }
30 }
31 
32 int bit[N];
33 int cal(int n,int k,int b)
34 {
35     memset(bit, 0, sizeof(bit));
36     int len=0, cnt=0, ans=0;
37     while(n)    //转成b进制
38     {
39         bit[++len]=n%b;
40         n/=b;
41     }
42     for(int i=len; i>0; i--)
43     {
44         if(bit[i]>1)
45         {
46             ans+=f[i][k-cnt];   //取整棵子树
47             break;
48         }
49         else if( bit[i]==1 )
50         {
51             ans+=f[i-1][k-cnt]; //统计左边的
52             if(++cnt>k)   break;  //已超
53         }
54     }
55     if(cnt==k)  ans++;
56     return ans;
57 }
58 
59 int main()
60 {
61     //freopen("input.txt","r",stdin);
62     pre_cal();
63     int x, y, k, b;
64     while(~scanf("%d%d%d%d",&x,&y,&k,&b))
65         printf("%d\n", cal(y,k,b)-cal(x-1,k,b));
66     return 0;
67 }
AC代码
```
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <map>
 7 #include <algorithm>
 8 #include <vector>
 9 #include <iostream>
10 #define pii pair<int,int>
11 #define INF 0x7f3f3f3f
12 #define LL long long
13 using namespace std;
14 const double PI  = acos(-1.0);
15 const int N=35;
16 
17 int f[N][N];
18 void pre_cal()  //预处理组合数
19 {
20     f[0][0]=1;
21     for(int i=1; i<N; i++) //位数
22     {
23         f[i][0]=f[i][i]=1;
24         for(int j=1; j<i; j++) //多少个1
25         {
26             f[i][j]=f[i-1][j]+f[i-1][j-1];
27         }
28     }
29 }
30 int bit[N];
31 int cal(int n,int k,int b)
32 {
33     memset(bit, 0, sizeof(bit));
34     int len=0, cnt=0, ans=0, flag=1;
35     while(n)    //转成b进制
36     {
37         bit[++len]=n%b;
38         n/=b;
39         if(bit[len]>1)  flag=0;
40     }
41 
42     if(flag==0)
43     {
44         //找到第一位大于1的，改为1，然后后面可以全部改成1了
45         for(int i=len; i>0; i--)
46             if(bit[i]>1)
47             {
48                 for(int j=i; j>0; j--)    bit[j]=1;
49                 break;
50             }
51     }
52 
53     for(int i=len; i>0; i--)
54     {
55         if( bit[i] )
56         {
57             ans+=f[i-1][k-cnt]; //统计左边的
58             if(++cnt>k)   break;  //已超
59         }
60     }
61     if(cnt==k) ans++;
62     return ans;
63 }
64 
65 int main()
66 {
67     //freopen("input.txt","r",stdin);
68     pre_cal();
69     int x, y, k, b;
70     while(~scanf("%d%d%d%d",&x,&y,&k,&b))
71         printf("%d\n", cal(y,k,b)-cal(x-1,k,b));
72     return 0;
73 }
AC代码
```

