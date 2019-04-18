# POJ Washing Clothes 洗衣服 （01背包，微变型） - xcw0754 - 博客园
# [POJ Washing Clothes 洗衣服 （01背包，微变型）](https://www.cnblogs.com/xcw0754/p/4522097.html)
题意：有多种颜色的衣服，由两个人合作来洗，必须洗完一种颜色才能洗下一种，求需要多少时间能洗完。
思路：将衣服按颜色分类，对每种颜色进行01背包，容量上限是该种颜色衣服全部洗完的耗时长一半，其实就是在最公平地平分工作量。因为一个先洗完就得等到另一人洗完。最后把洗完每种颜色的时长加起来返回。注:poj不允许用map，不然更省事，根据string和int做个哈希映射。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <vector>
 6 using namespace std;
 7 int m, n, big[12], dp[50050];
 8 vector< vector<int> >   vect;
 9 char s[11][12], str[15];
10 int cal()
11 {
12     int ans=0;
13     for(int i=1,sum=0; i<=m; i++) //每种颜色
14     {
15         if(vect[i].empty()) continue;
16         memset(dp,0,sizeof(dp));
17         for(int k=0; k<vect[i].size(); k++) //每件物品
18         {
19             sum+=vect[i][k];
20             for(int j=(big[i]>>1); j>=vect[i][k]; j--)  //每种容量
21                 dp[j]=max(dp[j],dp[j-vect[i][k]]+vect[i][k]);   //01背包背一半出来
22         }
23         ans+=big[i]-dp[big[i]>>1];//取其大者
24     }
25     return ans;
26 }
27 
28 void init() //初始化
29 {
30     memset(big,0,sizeof(big));
31     vect.clear();
32     vector<int> tmp;
33     for(int i=0; i<=m; i++)
34         vect.push_back(tmp);
35 }
36 
37 int main()
38 {
39     //freopen("input.txt", "r", stdin);
40     while(scanf("%d%d",&m,&n))
41     {
42         if(!m&&!n)  break;
43         init();
44         for(int i=1; i<=m; i++)    scanf("%s",s[i]);    //输入颜色
45 
46         for(int i=0,t=0; i<n; i++)  //输入时间+颜色
47         {
48             scanf("%d%s",&t,str);
49             for(int j=1; j<=m; j++) //颜色分类
50                 if(strlen(s[j])==strlen(str)&&strcmp(s[j],str)==0)
51                 {
52                     big[j]+=t;
53                     vect[j].push_back(t);
54                     break;
55                 }
56         }
57         printf("%d\n",cal());
58     }
59     return 0;
60 }
AC代码
```

