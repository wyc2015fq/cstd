# hihoCoder #1195   高斯消元·一 - xcw0754 - 博客园
# [hihoCoder #1195   高斯消元·一](https://www.cnblogs.com/xcw0754/p/4687480.html)
题意：便利店老板为了促销，推出了组合包的形式，将不同数量的各类商品打包成一个组合。比如2袋薯片，1听可乐的组合只要5元，而1袋薯片，2听可乐的组合只要4元。通过询问老板知道：一共有N种不同的商品和M种不同的商品组合；每一个组合的价格等于组合内商品售价之和，一个组合内同一件商品不会超过10件。
思路：高斯消元！精度的坑啊！一直错在90分！！！终于AC了，很多需要注意的地方！！
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=1010;
 5 
 6 double a[N][N];
 7 double t[N];   //临时行
 8 double value[N];   //临时行
 9 
10 int Gaussian_elimination(int row,int col)
11 {
12     //计算上三角
13     for(int i=1; i<col; i++)   //列
14     {
15         int r=i;
16         for(int j=row; j>i; j--)
17             if( fabs(a[j][i])>fabs(a[r][i]) )
18                 r=j;
19         if(r==i && fabs(a[i][i])<1e-7)   return -1; //注意点1：精度
20         if(r!=i)    swap(a[r], a[i]);
21 
22         for( int j=i+1; j<=row; j++)
23         {
24             for(int k=col; k>i; k--)    //注意点2：反向
25                 a[j][k]-= a[j][i]/a[i][i]*a[i][k];
26             a[j][i]=0;  //注意点3：必须置0
27         }
28     }
29 
30     //检查是否有解
31     for(int i=col-1,j; i<=row; i++)
32     {
33         for(j=1; j<col; j++)    if(fabs(a[i][j])>1e-6) break;
34         if(j==col && fabs(a[i][col])>1e-6 )    return 0;   //无解，左边系数全0，右边值不为0。反过来是可以的，因为可能有东西不用钱呢？
35     }
36 
37 
38     for(int i=col-1; i>0; i--)  //从正方形最后一行开始往上。
39     {
40         for(int j=i+1; j<col; j++)    //消掉i+1~col-1
41             a[i][col]-=a[i][j]*t[j];
42         t[i]=a[i][col]/a[i][i]; //答案
43     }
44     return 1;
45 }
46 
47 int main()
48 {
49     freopen("input.txt", "r", stdin);
50     int m, n;
51 
52     scanf("%d%d",&n,&m);
53     for(int i=1; i<=m; i++)
54         for(int j=1; j<=n+1; j++)
55             scanf("%lf", &a[i][j]); //a[][n+1]存价格
56     int ans=Gaussian_elimination(m, n+1);
57     if(ans==-1)     puts("Many solutions");
58     else if(ans==0) puts("No solutions");
59     else
60     {
61         for(int i=1; i<=n; i++)
62             printf("%d\n", (int)(t[i]+0.5));
63     }
64     return 0;
65 }
AC代码
```

