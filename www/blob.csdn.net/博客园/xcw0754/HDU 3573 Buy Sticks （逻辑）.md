# HDU 3573 Buy Sticks （逻辑） - xcw0754 - 博客园
# [HDU 3573 Buy Sticks （逻辑）](https://www.cnblogs.com/xcw0754/p/4587202.html)
题意：a，b，c三种棍子长度分别为20，28，32，现需要这三种棍子数根，欲买长为75的棍子来剪成这三种（不够长的就废弃） ，问需要买多少根。
思路：将所有棍子尽可能和其他搭配起来，使得数量减到最少。
分情况：       结果（按最坏情况考虑）：
2a+c=72　　要么c=0，要么a=1或a=0
2a+b=68　　要么b=0，要么a=1或a=0
2c=64　　    c=1或c=0
b+c=60       要么c=0，要么b=0
3a=60 　　  a=2或 a=1
2b=56  　　 b=0或b=1
最后剩下的情况：
（1）a有很多，那么直接和b跟c搭配，全部使得b=c=0，a再跟自己搭，剩下a=1或2；
（2）a不够多，那么a可能剩下1或0（一旦有2必定跟其他搭配了），而b+c=1或0；
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=105;
 4 int a,b,c;
 5 int cal()
 6 {
 7     int cnt=0, tmp;
 8 
 9     tmp=min(a/2,c);     //要么a剩1个或完，要么c完 72
10     a-=tmp*2;
11     c-=tmp;
12     cnt+=tmp;
13 
14     tmp=min(a/2,b);     //要么a剩1个或完，要么b完 68
15     a-=tmp*2;
16     b-=tmp;
17     cnt+=tmp;
18 
19     tmp=c/2;            //c要么完，要么剩1         64
20     c-=tmp*2;
21     cnt+=tmp;
22 
23     tmp=min(b,c);       //c和b其中一个完          60
24     c-=tmp;
25     b-=tmp;
26     cnt+=tmp;
27 
28     tmp=a/3;            //a要么完，要么小于3        60
29     a-=tmp*3;
30     cnt+=tmp;
31 
32     tmp=b/2;            //b要么完，要么剩1         56
33     b-=tmp*2;
34     cnt+=tmp;           //b可能还有一个
35 
36     if(a+b+c>75)    return cnt+2;
37     else if(a+b+c>0)    return cnt+1;
38     return cnt;
39 }
40 
41 int main()
42 {
43     freopen("input.txt", "r", stdin);
44     int t, j=0;
45     cin>>t;
46     while(t--)
47     {
48         scanf("%d%d%d",&a,&b,&c);
49         printf("Case %d: %d\n",++j,cal());
50     }
51     return 0;
52 }
AC代码
```

