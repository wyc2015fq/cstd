# NBUT 1122 Shameimaru's Candid Camera（水） - xcw0754 - 博客园
# [NBUT 1122 Shameimaru's Candid Camera（水）](https://www.cnblogs.com/xcw0754/p/4538869.html)
题意：
　　给n*m个格子，初始时每个格子中有个数值为0，部分格子中含有炸弹，每个炸弹爆炸可以将周围的8个非炸弹格子中的数值加1，求全部炸弹炸完后那些非0且非炸弹格子中的数是多少。
思路：
　　另开一个矩阵，每炸弹一炸就9个格子全加1，全炸完后再输出时判断是否为0，若是则输出'-'，否则，若是炸弹格子则输出'*'，若是数字就输出该数字。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 int a[550][550];//统计数值
 4 int n, m;
 5 bool b[550][550];
 6 void cal(int x,int y)//9个全加，没关系
 7 {
 8     a[x-1][y-1]++;
 9     a[x-1][y]++;
10     a[x-1][y+1]++;
11 
12     a[x+1][y-1]++;
13     a[x+1][y]++;
14     a[x+1][y+1]++;
15 
16     a[x][y+1]++;
17     a[x][y]++;
18     a[x][y-1]++;
19 }
20 
21 int main()
22 {
23     //freopen("input.txt", "r", stdin);
24     int flag=0;
25     char c;
26     while(cin>>n>>m)
27     {
28         if(flag>0)    printf("\n");//输出格式？
29         else    flag++;
30         memset(a,0,sizeof(a));
31         memset(b,0,sizeof(b));
32 
33         for(int i=1; i<=m; i++) //宽度
34             for(int j=1; j<=n; j++) //高度
35             {
36                 cin>>c;
37                 if(c=='*')    b[i][j]=true; //负代表不能累加
38             }
39 
40         for(int i=1; i<=m; i++)
41             for(int j=1; j<=n; j++)
42                 if(b[i][j]==true)    cal(i,j);
43 
44         for(int i=1; i<=m; i++)
45         {
46             for(int j=1; j<=n; j++)
47             {
48                 if(b[i][j]==false)
49                 {
50                     if(a[i][j]>0)
51                         printf("%d",a[i][j]);
52                     else
53                         printf("-");
54                 }
55                 else
56                     printf("*");
57             }
58             printf("\n");
59         }
60     }
61     return 0;
62 }
AC代码
```

