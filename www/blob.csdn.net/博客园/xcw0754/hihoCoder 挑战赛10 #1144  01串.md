# hihoCoder 挑战赛10  #1144 : 01串 - xcw0754 - 博客园
# [hihoCoder 挑战赛10  #1144 : 01串](https://www.cnblogs.com/xcw0754/p/4438023.html)
思路：这只是逻辑测试题吧，考虑周全就行了。考虑n=m的情况，n>m的情况，m>n的情况。
（1）n>m的情况，0比1多几个都是行的，一共有m个“01”，后面补足够多个零即可。
（2）n=m的情况，一直输出“01”就行。
（3）n<m的情况，只允许m-n=1，其他情况都不行，否则肯定会有两个1在一起。
```
1 #include <iostream>
 2 #include <stdio.h>
 3 using namespace std;
 4 int main()
 5 {
 6     //freopen("input.txt", "r", stdin);
 7     int n, m;
 8     while(cin>>n>>m)
 9     {
10         if(n==m)        //一样多
11         {
12             for(int i=0; i<n+m; i++)
13             {
14                 if(i%2==0)    printf("0");
15                 else    printf("1");
16             }
17         }
18         else if(n>m)    //0多，都行
19         {
20             for(int i=0; i<m; i++)
21                 printf("01");
22             for(int i=0; i<n-m; i++)
23                 printf("0");
24         }
25         else if(n<m)    //1多
26         {
27             if(m-n==1 ) //刚好多1个
28             {
29                 for(int i=0; i<n+m; i++)
30                 {
31                     if(i%2==0)  printf("1");
32                     else        printf("0");
33                 }
34             }
35             else    printf("NO");   //多1个以上就是不行
36         }
37     }
38     return 0;
39 }
1144
```

