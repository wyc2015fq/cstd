# HDU 1496 Equations  等式（二分+暴力，技巧） - xcw0754 - 博客园
# [HDU 1496 Equations  等式（二分+暴力，技巧）](https://www.cnblogs.com/xcw0754/p/4550489.html)
题意：给出4个数字a,b,c,d，求出满足算式a*x1^2+b*x2^2+c*x3^2+d*x4^2=0的 （x1,x2,x3,x4） 的组合数。x的范围[-100,100]，四个数字的范围 [-50,50] ，不能为0。
思路：对于每套给出的a,b,c,d四个数字，如果纯暴力的话要穷举100*100*100*100，每个例子要算1亿次，必须超时。可以算式左边两个部分右移，变成a*x1^2+b*x2^2  = -（c*x3^2+d*x4^2），那么只需要计算出左边的所有可能的结果，判断右边是否能否组成这个结果的取负，若存在，则组合数加1。解法，x只按照正数来穷举即可，最后的结果乘以2*2*2*2=16即可。穷举左边的所有可能，最多100*100种可能，全部结果存起来，再穷举右边所有的可能结果取负，再在左边所得结果中判断是否存在，若存在则符合。最重要的一点是：test例子中含有大量的组合数为0的例子，也就是四个数字全部大于0 或者全部小于0，是不可能的出现的，要判断，否则，你会一直处于TLE状态。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 int a, b, c, d;
 4 int has1[1000006];
 5 int has2[1000006];
 6 int qq[105][105][105][105];
 7 
 8 
 9 int cal()
10 {
11     memset(has1,0,sizeof(has1));
12     memset(has2,0,sizeof(has2));
13     int ans=0;
14     for(int x1=1; x1<=100; x1++)
15         for(int x2=1; x2<=100; x2++)
16         {
17             int tmp=a*x1*x1+ b*x2*x2;
18             if( tmp>=0 )
19                 has1[tmp]++;
20             else
21                 has2[-tmp]++;
22         }
23 
24 
25     for(int x3=1; x3<=100; x3++)
26     {
27         for(int x4=1; x4<=100; x4++)
28         {
29             int tmp=c*x3*x3+d*x4*x4;
30             if(tmp>0)
31                 ans+=has2[tmp];    
32             else
33                 ans+=has1[-tmp];    
34         }
35     }
36     return (ans<<4);
37 }
38 
39 
40 int main()
41 {
42     //freopen("input.txt", "r", stdin);
43     while(~scanf("%d%d%d%d",&a,&b,&c,&d))
44     {
45     
46         if(a>0 && b>0 && c>0 && d>0 || a<0 && b<0 && c<0 && d<0)  
47         {  
48                 printf("0\n");  
49                 continue;  
50         }  
51     
52 
53         if(!qq[a+50][b+50][c+50][d+50])
54         {
55             qq[a+50][b+50][c+50][d+50]=cal()+1;
56         }
57         printf("%d\n",qq[a+50][b+50][c+50][d+50]-1);
58     }
59 
60     return 0;
61 }
AC代码
```

