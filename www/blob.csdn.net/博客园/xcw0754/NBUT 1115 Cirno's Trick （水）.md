# NBUT 1115  Cirno's Trick （水） - xcw0754 - 博客园
# [NBUT 1115  Cirno's Trick （水）](https://www.cnblogs.com/xcw0754/p/4538730.html)
题意：
　　给出多个double数，去掉其最小的和最大的，再对余下的求均值。
思路：
　　再输入时将最大和最小去掉，顺便统计非最值的和，输出时除一下个数即可。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 
 4 int main()
 5 {
 6     //freopen("input.txt", "r", stdin);
 7     int n;
 8     while(~scanf("%d",&n))
 9     {
10         if(n==2)
11         {
12             cout<<"0.00"<<endl;
13             continue;
14         }
15         double small, big;
16         cin>>small>>big;
17         if(small>big)   swap(small,big);
18         double ans=0.0, tmp=0.01;
19 
20         for(int i=0; i<n-2; i++)
21         {
22             cin>>tmp;
23             if(tmp<small)
24             {
25                 ans+=small;
26                 small=tmp;
27             }
28             else if(tmp>big)
29             {
30                 ans+=big;
31                 big=tmp;
32             }
33             else    ans+=tmp;
34         }
35         printf("%.2f\n",ans/(n-2));
36     }
37 
38     return 0;
39 }
AC代码
```

