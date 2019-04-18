# HDU 2147 kiki's game kiki的游戏（博弈，找规律） - xcw0754 - 博客园
# [HDU 2147 kiki's game kiki的游戏（博弈，找规律）](https://www.cnblogs.com/xcw0754/p/4503310.html)
题意:
　　给一个有n*m 个格子的棋盘，将一个硬币放在右上角一格，每次可以往左/下/左下移动一格，碰到不能移动的局面者输。
思路：
　　找P/N状态。先将(n,1)归为P状态，那么能一步到达此位置的有3个位置，分别是其上/右/右上的格子。根据这个规律来找，在整个棋盘的格子上标上P和N。可以发现，棋盘上是有规律的，若提供的n和m皆为奇数，则先手输；否则先手赢。
```
1 #include <iostream>
 2 #include <cstdio>
 3 using namespace std;
 4 int main()
 5 {
 6     //freopen("input.txt", "r", stdin);
 7     int a, b;
 8     while(cin>>a>>b)
 9     {
10         if(!a&&!b)  break;
11         if( a&1 && b&1 )    printf("What a pity!\n");
12         else    printf("Wonderful!\n");
13     }
14     return 0;
15 }
AC代码
```

