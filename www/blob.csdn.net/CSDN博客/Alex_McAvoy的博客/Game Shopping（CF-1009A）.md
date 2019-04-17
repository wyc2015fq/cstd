# Game Shopping（CF-1009A） - Alex_McAvoy的博客 - CSDN博客





2018年08月23日 20:07:49[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：41








# Problem Description

Maxim wants to buy some games at the local game shop. There are nn games in the shop, the i-th game costs cici.

Maxim has a wallet which can be represented as an array of integers. His wallet contains mm bills, the j-th bill has value ajaj.

Games in the shop are ordered from left to right, Maxim tries to buy every game in that order.

When Maxim stands at the position i in the shop, he takes the first bill from his wallet (if his wallet is empty then he proceeds to the next position immediately) and tries to buy the ii-th game using this bill. After Maxim tried to buy the nn-th game, he leaves the shop.

Maxim buys the ii-th game if and only if the value of the first bill (which he takes) from his wallet is greater or equal to the cost of the i-th game. If he successfully buys the ii-th game, the first bill from his wallet disappears and the next bill becomes first. Otherwise Maxim leaves the first bill in his wallet (this bill still remains the first one) and proceeds to the next game.

For example, for array c=[2,4,5,2,4] and array a=[5,3,4,6] the following process takes place: Maxim buys the first game using the first bill (its value is 5), the bill disappears, after that the second bill (with value 3) becomes the first one in Maxim's wallet, then Maxim doesn't buy the second game because c2>a2, the same with the third game, then he buys the fourth game using the bill of value a2 (the third bill becomes the first one in Maxim's wallet) and buys the fifth game using the bill of value a3.

Your task is to get the number of games Maxim will buy.

# Input

The first line of the input contains two integers n and m (1≤n,m≤1000) — the number of games and the number of bills in Maxim's wallet.

The second line of the input contains nn integers c1,c2,…,cn (1≤ci≤1000), where cici is the cost of the i-th game.

The third line of the input contains mm integers a1,a2,…,am (1≤aj≤1000), where ajaj is the value of the j-th bill from the Maxim's wallet.

# Output

Print a single integer — the number of games Maxim will buy.

# Examples

**Input**

5 4

2 4 5 2 4

5 3 4 6

**Output**

3

**Input**

5 2

20 40 50 20 40

19 20

**Output**

0

**Input**

6 4

4 8 15 16 23 42

1000 1000 1000 1000

**Output**

4

————————————————————————————————————————————

题意：有 n 个游戏，m 张钞票，如果当前钞票价值能买游戏，就会买游戏用掉钞票，不能买就跳到下一张钞票，直到能够买为止，求最多能买多少游戏

思路：模拟

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1001
#define MOD 10007
#define E 1e-6
#define LL long long
using namespace std;
bool vis[27];
int a[1005],b[1005];
int main()
{

     int n,m;
     cin>>n>>m;
     for(int i=1;i<=n;i++)
         scanf("%d",&a[i]);

     for(int i=1;i<=m;i++)
        scanf("%d",&b[i]);

     int ans=0;
     for(int i=1,j=1;i<=n&&j<=m;)
     {
         if(b[j]>=a[i])
         {
             ans++;
             i++;
             j++;
         }
         else 
             i++;
     }
     cout<<ans<<endl;

	return 0;
}
```





