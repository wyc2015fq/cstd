# 数独  (dfs) - Vmetrio - 博客园







# [数独  (dfs)](https://www.cnblogs.com/wangmengmeng/p/5254969.html)









自从2006年3月10日至11日的首届数独世界锦标赛以后，数独这项游戏越来越受到人们的喜爱和重视。
据说，在2008北京奥运会上，会将数独列为一个单独的项目进行比赛，冠军将有可能获得的一份巨大的奖品———HDU免费七日游外加lcy亲笔签名以及同hdu acm team合影留念的机会。
所以全球人民前仆后继，为了奖品日夜训练茶饭不思。当然也包括初学者linle，不过他太笨了又没有多少耐性，只能做做最最基本的数独题，不过他还是想得到那些奖品，你能帮帮他吗？你只要把答案告诉他就可以，不用教他是怎么做的。

数独游戏的规则是这样的：在一个9x9的方格中，你需要把数字1-9填写到空格当中，并且使方格的每一行和每一列中都包含1-9这九个数字。同时还要保证，空格中用粗线划分成9个3x3的方格也同时包含1-9这九个数字。比如有这样一个题，大家可以仔细观察一下，在这里面每行、每列，以及每个3x3的方格都包含1-9这九个数字。

例题：
![](http://acm.hdu.edu.cn/data/images/C31-1001-1.jpg)

答案：
![](http://acm.hdu.edu.cn/data/images/C31-1001-2.jpg)




### Input

本题包含多组测试，每组之间由一个空行隔开。每组测试会给你一个 9*9 的矩阵，同一行相邻的两个元素用一个空格分开。其中1-9代表该位置的已经填好的数，问号（?）表示需要你填的数。




### Output

对于每组测试，请输出它的解，同一行相邻的两个数用一个空格分开。两组解之间要一个空行。
对于每组测试数据保证它有且只有一个解。




### Sample Input
7 1 2 ? 6 ? 3 5 8
? 6 5 2 ? 7 1 ? 4
? ? 8 5 1 3 6 7 2
9 2 4 ? 5 6 ? 3 7
5 ? 6 ? ? ? 2 4 1
1 ? 3 7 2 ? 9 ? 5
? ? 1 9 7 5 4 8 6
6 ? 7 8 3 ? 5 1 9
8 5 9 ? 4 ? ? 2 3
### Sample Output
7 1 2 4 6 9 3 5 8
3 6 5 2 8 7 1 9 4
4 9 8 5 1 3 6 7 2
9 2 4 1 5 6 8 3 7
5 7 6 3 9 8 2 4 1
1 8 3 7 2 4 9 6 5
2 3 1 9 7 5 4 8 6
6 4 7 8 3 2 5 1 9
8 5 9 6 4 1 7 2 3



[](http://115.159.40.116/contest_show.php?cid=10)



代码:


```
1 #include <stdio.h>
 2 #include <string.h>
 3 #include <math.h>
 4 #include <algorithm>
 5 #include <iostream>
 6 #include <ctype.h>
 7 #include <iomanip>
 8 #include <queue>
 9 #include <stdlib.h>
10 using namespace std;
11 
12 char mp[10][10];
13 int hang[10][10],lie[10][10],sum[10][10][10],x[100],y[100];
14 int con,n,ans,c=0;
15 
16 void dfs(int m)
17 {
18     int i,j,q1,q2;
19     if(m==n){
20         if(c==0)
21             c=1;
22         else
23             printf("\n");
24         ans=1;
25         for(i=0; i<9; i++){
26             for(j=0; j<9; j++){
27                 if(j!=0)
28                     printf(" ");
29                 printf("%c",mp[i][j]);        
30             }
31             printf("\n");
32         }
33         return;
34     }
35     for(i=1; i<=9; i++){
36         q1=x[m]/3;
37         q2=y[m]/3;
38         if(hang[x[m]][i]==1||lie[y[m]][i]==1||sum[q1][q2][i]==1)
39             continue;
40         mp[x[m]][y[m]]=i+'0';
41         hang[x[m]][i]=1;
42         lie[y[m]][i]=1;
43         sum[q1][q2][i]=1;
44         dfs(m+1);
45         if(ans)   return;
46         hang[x[m]][i]=0;
47         lie[y[m]][i]=0;
48         sum[q1][q2][i]=0;
49     }
50 }
51 
52 int main()
53 {
54     int t;
55     while(~scanf("%s",mp[0])){
56         getchar();
57         for(int i=1; i<9; i++){
58             scanf("%c",&mp[0][i]);
59             getchar();
60         }
61         for(int i=1; i<9; i++){
62             for(int j=0; j<9; j++){
63                 scanf("%c",&mp[i][j]);
64                 getchar();
65             }
66         }
67         memset(hang,0,sizeof(hang));
68         memset(lie,0,sizeof(lie));
69         memset(sum,0,sizeof(sum));
70         con=0;
71         for(int i=0; i<9; i++){
72             for(int j=0; j<9; j++){
73                 if(mp[i][j]=='?'){
74                     x[con]=i;
75                     y[con++]=j;
76                     
77                 }
78                 else{
79                     t=mp[i][j]-'0';
80                     hang[i][t]=1;
81                     lie[j][t]=1;
82                     sum[i/3][j/3][t]=1;
83                 }
84             }
85         }
86         n=con;
87         ans=0;
88         dfs(0);
89     }
90 }
```




















