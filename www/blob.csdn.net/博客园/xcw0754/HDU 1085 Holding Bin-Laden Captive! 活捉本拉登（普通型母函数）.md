# HDU 1085 Holding Bin-Laden Captive! 活捉本拉登（普通型母函数） - xcw0754 - 博客园
# [HDU 1085 Holding Bin-Laden Captive! 活捉本拉登（普通型母函数）](https://www.cnblogs.com/xcw0754/p/4253456.html)
题意：
　　有面值分别为1、2、5的硬币，分别有num_1、num_2、num_5个，问不能组成的最小面值是多少？（0<=每种硬币个数<=1000，组成的面值>0)
思路：
　　母函数解决。只有3个括号要作乘法，分别代表面值1、2、5所能组成的情况。需要两个数组，所能组成的最大值为num_1+2*num_2+5*num_5。如果在这个范围内都能组成，那么最小不能组成的面值为num_1+2*num_2+5*num_5+1。若没有1分钱的硬币，那么不能组成的肯定是1了。
　　数组的用法：ans[]保存第一个括号→sup保存前两个括号的结果→ans[]保存最后结果。
```
1 #include <iostream>
 2 #define N 8100
 3 using namespace std;
 4 int num_1,num_2,num_5,ans[N],sup[N],tar;
 5 int cal_and_search()
 6 {//ans[]→sup[]→ans[]
 7     int i,j,k;
 8     num_2*=2;
 9     num_5*=5;
10     memset(ans,0,sizeof(ans));    //清零
11     memset(sup,0,sizeof(sup));
12     for(i=0;i<=num_1;i++)    //初始化num_1+1个喔
13         ans[i]=1;
14     for(j=0;j<=num_2;j+=2)//头两个括号相乘
15         for(k=0;k<=num_1;k++)
16             sup[j+k]+=ans[k];
17     memset(ans,0,sizeof(ans));    //ans置零
18     for(j=0;j<=num_5;j+=5)    //上一步结果*第3个括号
19         for(k=0;k<=num_1+num_2;k++)
20             ans[j+k]+=sup[k];
21     for(i=1;i<=N;i++)    //搜索
22         if(ans[i]==0)    return i;
23 }
24 int main()
25 {
26     while(scanf("%d%d%d",&num_1,&num_2,&num_5))
27     {
28         if(num_1==0&&num_2==0&&num_5==0)    return 0;    //结束
29         if(num_1==0){printf("1\n");continue;}
30         tar=cal_and_search();
31         printf("%d\n",tar);
32     }
33     return 0;
34 }
1085
```

