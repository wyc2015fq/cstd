# UVA 408 Uniform Generator 伪随机数（水） - xcw0754 - 博客园
# [UVA 408 Uniform Generator 伪随机数（水）](https://www.cnblogs.com/xcw0754/p/4606385.html)
题意：根据这个式子来递推求得每个随机数x，step和mod给定，seed(0)=0。如果推出来的序列是mod个不重复的数字（0~mod-1）则打印good，否则bad（因为不能产生所有的数）。
思路：
　　用一个数组记录所产生过的数，当出现数字已记录过时，判断是否个数为mod个。若是就返回good。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=10000001;
 5 bool ans[N];
 6 
 7 int main()
 8 {
 9     //freopen("input.txt", "r", stdin);
10     int a, b;
11     while(~scanf("%d%d",&a,&b))
12     {
13         memset(ans,0,sizeof(ans));
14         ans[0]=1;
15         int cur=0, flag=1;
16         for(int i=2; i<=b; i++)
17         {
18             cur=(cur+a)%b;
19             if(ans[cur])    //鸽笼原理
20             {
21                 printf("%10d%10d    Bad Choice\n\n",a,b);
22                 flag=0;
23                 break;
24             }
25             else    ans[cur]=1;
26         }
27         if(flag)    printf("%10d%10d    Good Choice\n\n",a,b);
28     }
29     return 0;
30 }
AC代码
```

