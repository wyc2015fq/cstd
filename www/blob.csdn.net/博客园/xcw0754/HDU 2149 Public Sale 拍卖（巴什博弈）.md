# HDU 2149 Public Sale 拍卖（巴什博弈） - xcw0754 - 博客园
# [HDU 2149 Public Sale 拍卖（巴什博弈）](https://www.cnblogs.com/xcw0754/p/4496841.html)
思路：只要能给对方留下n+1，我就能胜，否则败。
```
1 #include <iostream>
 2 #include <cstdio>
 3 using namespace std;
 4 int main()
 5 {
 6     //freopen("input.txt", "r", stdin);
 7     int n, m;
 8     while(cin>>m>>n)
 9     {
10         if(m<=n)
11         {
12             for(int i=m; i<=n; i++)
13             {
14                 cout<<i;
15                 if(i!=n)    cout<<" ";
16             }
17         }
18         else if(!(m%(n+1)))
19             cout<<"none";
20         else
21             cout<<m%(n+1);
22         cout<<endl;
23 
24     }
25     return 0;
26 }
AC代码
```

