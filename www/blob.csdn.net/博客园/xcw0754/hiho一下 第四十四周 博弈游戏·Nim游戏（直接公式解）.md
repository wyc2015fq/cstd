# hiho一下 第四十四周 博弈游戏·Nim游戏（直接公式解） - xcw0754 - 博客园
# [hiho一下 第四十四周 博弈游戏·Nim游戏（直接公式解）](https://www.cnblogs.com/xcw0754/p/4474212.html)
证明看这http://hihocoder.com/contest/hiho44/problem/1
思路：
　　设 sg=a[1]^a[2]^...a[n]，若sg=0，则先手Alice必败，否则必赢。
**所有值的异或和为0就先手必败**（仅需记住这句就够应付此类题了），否则先手胜。
```
1 #include <iostream>
 2 using namespace std;
 3 int stone[101], n, tmp;
 4 int main()
 5 {
 6     //freopen("input.txt","r",stdin);
 7     while(cin>>n){
 8         scanf("%d", &tmp);
 9         for(int i=1; i<n; i++)    scanf("%d",&stone[i]);
10         for(int i=1; i<n; i++)    tmp^=stone[i];
11         if(!tmp)    cout<<"Bob"<<endl;
12         else        cout<<"Alice"<<endl;
13     }
14     return 0;
15 }
AC代码
```

