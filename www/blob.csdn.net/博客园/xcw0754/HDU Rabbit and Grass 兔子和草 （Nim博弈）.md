# HDU Rabbit and Grass 兔子和草 （Nim博弈） - xcw0754 - 博客园
# [HDU Rabbit and Grass 兔子和草 （Nim博弈）](https://www.cnblogs.com/xcw0754/p/4496932.html)
思路：简单Nim博弈，只需要将所给的数字全部进行异或，结果为0，则先手必败。否则必胜。
```
1 #include <iostream>
 2 using namespace std;
 3 int main()
 4 {
 5     //freopen("input.txt", "r", stdin);
 6     int t, n, m;
 7     while(cin>>t,t)
 8     {
 9         int ans,tmp;
10         cin>>ans;
11         for(int i=1; i<t; i++)
12         {
13             cin>>tmp;
14             ans^=tmp;
15         }
16         if(ans)
17             cout<<"Rabbit Win!"<<endl;
18         else
19             cout<<"Grass Win!"<<endl;
20     }
21     return 0;
22 }
AC代码
```

