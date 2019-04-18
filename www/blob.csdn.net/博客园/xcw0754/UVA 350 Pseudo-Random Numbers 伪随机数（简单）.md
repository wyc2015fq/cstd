# UVA  350  Pseudo-Random Numbers 伪随机数（简单） - xcw0754 - 博客园
# [UVA  350  Pseudo-Random Numbers 伪随机数（简单）](https://www.cnblogs.com/xcw0754/p/4606372.html)
题意：给定Z, I, M,  L，根据随机数产生式k=(Z*L+I)%M。但是L表示的是上一个产生的数，比如根据产生式产生了序列｛2，5，4，3｝那么5是由L=2算来的，4由L=5算来的..第1个所产生的数所需的L由系统给定。那么肯定会产生一个环，到某个位置就会开始产生重复的序列，比如12345345345...。求循环中有多少个数（循环节）？（如例是3，分别是345）
思路：因为z i m 都是不变的，当L一定，那么产生的数肯定是一定的，所以只要L有重复出现，那就会从那里开始循环。标记一下所产生的数字再第几个开始产生，一旦产生重复就可以计算其循环节多长。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=10005;
 5 bool ans[N];
 6 int num[N];
 7 int main()
 8 {
 9     //freopen("input.txt", "r", stdin);
10     int z, i, m, l, j=0;
11     while(~scanf("%d%d%d%d",&z,&i,&m,&l))
12     {
13         if(!z && !i && !m && !l )    break;//结束
14         memset(ans,0,sizeof(ans));
15         memset(num,0,sizeof(num));
16 
17         int cur=l, cnt=1;
18         ans[cur]=true;
19         num[cur]=1;
20         while(1)
21         {
22             cur=(z*cur+i)%m;
23             if(ans[cur])//已经出现过了
24             {
25                 printf("Case %d: %d\n",++j,cnt-num[cur]+1);
26                 break;
27             }
28             ans[cur]=1;//标记已经出现过
29             num[cur]=++cnt;//在第几步出现过
30         }
31     }
32     return 0;
33 }
AC代码
```

