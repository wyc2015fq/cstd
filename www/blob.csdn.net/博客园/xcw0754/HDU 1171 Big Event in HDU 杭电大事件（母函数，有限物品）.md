# HDU 1171 Big Event in HDU 杭电大事件（母函数，有限物品） - xcw0754 - 博客园
# [HDU 1171 Big Event in HDU 杭电大事件（母函数，有限物品）](https://www.cnblogs.com/xcw0754/p/4519789.html)
题意：
　　分家问题，对每种家具都估个值，给出同样价值的家具有多少个，要求尽可能平分，打印的第一个数要大于等于第二个数。
思路：
　　可以用背包做，也可以用母函数。母函数的实现只需要注意一个点，就是每次以一种价格递增，而不是自加。每类家具有上限，就是该类家具的价值*件数。注意判断输入的结束标志是n<0。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=55;
 4 int tmp[50*50*101/2], ans[50*50*101/2], big; //上限是50*50*100,但一半就够了
 5 int v[55], m[55];
 6 
 7 int cal(int n)  //返回第二个学院所得资产
 8 {
 9     memset(ans,0,sizeof(ans));
10     int half=(big>>1);//求一半以下就行，多了也没用
11     for(int i=0,t=0; i<=half&&t<=m[0]; t++,i+=v[0] )    ans[i]=1;
12     for(int i=1; i<n; i++)
13     {
14         memset(tmp,0,sizeof(int)*(half+1));
15         for(int k=0; k<=half; k++)
16             for(int j=0,t=0; t<=m[i]&&j+k<=half; t++,j+=v[i] )
17                 tmp[j+k]+=ans[k];
18         memcpy(ans,tmp,sizeof(int)*(half+1));
19     }
20     for(int i=half; i>0; i--)
21         if(ans[i])    return i;
22     return 0;
23 }
24 
25 int main()
26 {
27     //freopen("input.txt", "r", stdin);
28     int n, a, b;
29     while(scanf("%d",&n), n>0)//这个判断结束要注意
30     {
31         big=0;
32         for(int i=0; i<n; i++)
33         {
34             scanf("%d%d",&v[i],&m[i]);
35             big+=v[i]*m[i];
36         }
37         int tmp=cal(n);
38         printf("%d %d\n",big-tmp,tmp);
39     }
40     return 0;
41 }
AC代码
```

