# HDU 1729 Stone Game 石头游戏 （Nim, sg函数） - xcw0754 - 博客园
# [HDU 1729 Stone Game 石头游戏 （Nim, sg函数）](https://www.cnblogs.com/xcw0754/p/4517955.html)
题意:
　　有n个盒子，每个盒子可以放一定量的石头，盒子中可能已经有了部分石头。假设石头无限，每次可以往任意一个盒子中放石头，可以加的数量不得超过该盒中已有石头数量的平方k^2，即至少放1个，至多放k^2个。
思路：
　　跟常规nim的区别就是加了个限制“每次加的量不超平方”。盒子容量上限是100万，那么就不能直接计算SG了，会超时。sg打表后找规律。根据剩下多少个空位来决定sg值。都是0123456这样子递增的，碰到不能一次加满就变为0，然后继续递增，一直这样。
　　我的方案是，对于每个盒子大小，找到除了自己之外的第一个P状态，记录下来。根据给定的s和c来查表，找到大于c且离c最近的一个p点，然后用该点减去c就是sg值了。
```
1 #include <iostream>
 2 #include <cstdio>
 3 #include <cmath>
 4 using namespace std;
 5 const int N=1001000;
 6 int p[N];//p[i]表示的值经过一次不能到达i，但是经过2次必定可以到达i，也就是说i是一个P点，p[i]是上一个P点
 7 void cal()
 8 {
 9     //p[1]之前无必败点，只有（1，0）这一个必胜点，要特殊处理
10     int k=1;
11     while(k+k*k<N)
12     {
13         int q=k+k*k+1;
14         int up=k+1+(k+1)*(k+1);
15         for(int i=q; i<=min(up,N-1); i++)    p[i]=k;
16         k++;
17     }
18 }
19 int get_sg(int s,int c)
20 {
21     int tmp=s;
22     while(s&&s>=c)
23     {
24         tmp=s;
25         s=p[s];
26     }
27     if(tmp==1)    return 1;   //对付（1，0）的情况
28     return (tmp-c);
29 }
30 
31 int main()
32 {
33     cal();//预处理
34     //freopen("input.txt","r",stdin);
35     int n, j=0,ans,s,c;
36     while(scanf("%d",&n), n)
37     {
38         ans=0;
39         while(n--)
40         {
41             scanf("%d%d",&s,&c);
42             if(s&&c)    ans^=get_sg(s,c);
43         }
44         if(ans)    printf("Case %d:\nYes\n",++j);
45         else    printf("Case %d:\nNo\n",++j);
46 
47     }
48 
49     return 0;
50 }
AC代码
```

