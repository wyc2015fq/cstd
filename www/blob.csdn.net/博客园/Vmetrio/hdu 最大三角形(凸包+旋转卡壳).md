# hdu  最大三角形(凸包+旋转卡壳) - Vmetrio - 博客园







# [hdu  最大三角形(凸包+旋转卡壳)](https://www.cnblogs.com/wangmengmeng/p/5316737.html)





老师在计算几何这门课上给Eddy布置了一道题目，题目是这样的：给定二维的平面上n个不同的点，要求在这些点里寻找三个点，使他们构成的三角形拥有的面积最大。
Eddy对这道题目百思不得其解，想不通用什么方法来解决，因此他找到了聪明的你，请你帮他解决这个题目。




### Input

输入数据包含多组测试用例，每个测试用例的第一行包含一个整数n，表示一共有n个互不相同的点，接下来的n行每行包含2个整数xi,yi，表示平面上第i个点的x与y坐标。你可以认为：3 <= n <= 50000 而且 -10000 <= xi, yi <= 10000.




### Output

对于每一组测试数据，请输出构成的最大的三角形的面积，结果保留两位小数。
每组输出占一行。




### Sample Input
3
3 4
2 6
3 7
6
2 6
3 9
2 0
8 0
6 6
7 7
### Sample Output
1.50
27.00


旋转卡壳:http://www.cnblogs.com/Booble/archive/2011/04/03/2004865.html



代码:

```
1 #include <cstdio>
 2 #include <iostream>
 3 #include <cstring>
 4 #include <algorithm>
 5 #include <cmath>
 6 using namespace std;
 7 #define eps 1e-8
 8 struct node
 9 {
10     int x,y;
11 }
12 ;
13 node p[50005];
14 node res[50005];
15 int cross(node p0,node p1,node p2)
16 {
17     return (p0.x-p2.x)*(p1.y-p2.y)-(p1.x-p2.x)*(p0.y-p2.y);
18 }
19 bool cmp(node a,node b)
20 {
21     if(a.x==b.x)
22         return a.y<b.y;
23     else
24         return a.x<b.x;
25 }
26 int Graham(int n)
27 {
28     int len;
29     int top=0;
30     sort(p,p+n,cmp);
31     for(int i=0; i<n; i++)
32     {
33         while(top>1&&cross(res[top-1],p[i],res[top-2])<=0)
34             top--;
35         res[top++]=p[i];
36     }
37     len=top;
38     for(int i=n-2; i>=0; i--)
39     {
40         while(top>len&&cross(res[top-1],p[i],res[top-2])<=0)
41             top--;
42         res[top++]=p[i];
43     }
44     if(n>1)
45         top--;
46     return top;
47 }
48 int main()
49 {
50     int n;
51     while(cin>>n)
52     {
53         for(int i=0; i<n; i++)
54             cin>>p[i].x>>p[i].y;
55         int dian=Graham(n);
56         int ans=-1;
57         for(int i=0; i<dian; i++)
58             for(int j=i+1; j<dian; j++)
59                 for(int k=j+1; k<dian; k++)
60                     ans=max(ans,cross(res[j],res[k],res[i]));
61         printf("%.2lf\n",0.5*ans);
62     }
63     return 0;
64 }
```














