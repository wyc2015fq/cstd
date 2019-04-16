# LightOJ 1186／ LightOJ 1192【 Nim博弈变形】距离表示石子数量 - 紫芝的博客 - CSDN博客





2018年11月08日 10:24:10[紫芝](https://me.csdn.net/qq_40507857)阅读数：139
个人分类：[博弈论](https://blog.csdn.net/qq_40507857/article/category/7632032)









You are given an **n x n** chess board. Only pawn is used in the 'Incredible Chess' and they can move forward or backward. In each column there are two pawns, one white and one black. White pawns are placed in the lower part of the board and the black pawns are placed in the upper part of the board.

The game is played by two players. Initially a board configuration is given. One player uses white pieces while the other uses black. In each move, a player can move a pawn of his piece, which can go forward or backward any positive integer steps, but it cannot jump over any piece. White gives the first move.

The game ends when there is no move for a player and he will lose the game. Now you are given the initial configuration of the board. You have to write a program to determine who will be the winner.

![](https://vj.e949.cn/e013d8d58fc971cd42fd70fa43bbdee0?v=1541345707)

Example of a Board

Input

Input starts with an integer **T (****≤ 200)**, denoting the number of test cases.

Each case starts with an integer **n (3 ≤ n ≤ 100)** denoting the dimension of the board. The next line will contain **n** integers, **W0, W1, ..., Wn-1** giving the position of the white pieces. The next line will also contain **n** integers, **B0, B1, ... Bn-1** giving the position of the black pieces. **Wi** means the row position of the white piece of **ith** column. And **Bi** means the row position of the black piece of **ith** column. You can assume that **(0 ≤ Wi < Bi < n) **for** (0 ≤ i < n)** and at least one move is remaining.

Output

For each case, print the case number and **'white wins'** or **'black wins'** depending on the result.

Sample Input

2

6

1 3 2 2 0 1

5 5 5 3 1 2

7

1 3 2 2 0 4 0

3 4 4 3 1 5 6

Sample Output

Case 1: black wins

Case 2: white wins

分析：

把每一列的棋子间隔（可以走的）看成Nｉｍ博弈中的石子数量，将N堆石子数量异或，若结果为０，先手必败；

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
#include<iostream>
using namespace std;
typedef long long LL;
const int maxn=120;
int a[maxn],b[maxn];
int main(){
    int T;
    int ca=1;
    scanf("%d",&T);
    while(T--){
        int n;
        scanf("%d",&n);
        for(int i=1;i<=n;i++)
            scanf("%d",&a[i]);
        for(int j=1;j<=n;j++)
            scanf("%d",&b[j]);
        int ans=0;
        for(int i=1;i<=n;i++)
            ans^=abs(b[i]-a[i]-1);
            printf("Case %d: ",ca++);
        if(ans==0)
            printf("black wins\n");
        else
            printf("white wins\n");
    }
    return 0;
}
```

## Left Right

[LightOJ - 1192](https://cn.vjudge.net/problem/26905/origin)

Two players, Alice and Bob are playing a strange game in a **1 x n** board. The cells are numbered from **0** to **n-1**, where the left most cell is marked as cell **0**. Each cell can contain at most one piece.

![](https://vj.e949.cn/1399dde8cfda6248dfda3472115477bd?v=1541345707)

Fig 1: an example

There are two kinds of pieces, gray and white. Alice moves all gray pieces, and bob moves all white ones. The pieces alternate, that is, leftmost piece is gray, next is white, next to that is gray, then it's white again, and so on. There will always be equal number of black and gray pieces. Alice can only move pieces to the right. Bob can only move pieces to the left.

At each move, a player selects one piece and moves that piece, either to its left (Bob) or to its right (Alice), any number of cells (at least 1) but, it can neither jump over other pieces, nor it can move outside the board. The players alternate their turns.

For example, if Alice decides to move the left most gray piece, these two moves are available to her.

![](https://vj.e949.cn/f7b519d1703bc62e078dbdebe87da6c7?v=1541345707)

Fig 2: Moving the gray piece one cell to the right

![](https://vj.e949.cn/acf825ebaa1a7119c72fdf8d59046a7c?v=1541345707)

Fig 3: Moving the gray piece two cells to the right

Alice moves first. The game ends, when someone is unable to make any move, and loses the game. You can assume that, both of them play optimally (that is, if it is possible to apply a strategy that will ensure someone's win, he/she will always use that strategy).

Now you are given a configuration of a board, you have to find the winner.

Input

Input starts with an integer **T (****≤ 200)**, denoting the number of test cases.

Each case starts with a line containing an integer **k (1 ≤ k ≤ 100)** denoting the number of gray pieces in the board. The next line contains **2k** distinct integers (in ascending order) denoting the position of the pieces. The first integer denotes a gray piece, the second integer denotes a white piece, the next integer denotes a gray piece and so on. All the integers will lie in the range **[0, 10＾9]**. Assume that **n** is sufficiently large to contain all the pieces. And at least one move is remaining.

Output

For each case, print the case number and **'Alice'** or **'Bob'** depending on the winner of the game.

Sample Input

2

2

0 3 7 9

2

1 3 7 9

Sample Output

Case 1: Alice

Case 2: Bob

分析：

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
#include<iostream>
using namespace std;
typedef long long LL;
const int maxn=120;
int a[maxn],b[maxn];
int main(){
    int T;
    int ca=1;
    scanf("%d",&T);
    while(T--){
        int n;
        scanf("%d",&n);
        n*=2;
        for(int i=1;i<=n;i++)
            scanf("%d",&a[i]);

        int ans=0;
        for(int i=2;i<=n;i+=2)
            ans^=abs(a[i]-a[i-1]-1);
            printf("Case %d: ",ca++);
        if(ans==0)
            printf("Bob\n");
        else
            printf("Alice\n");
    }
    return 0;
}
```





