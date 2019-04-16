# Codeforces - 493D. Vasya and Chess - 博弈 - 紫芝的博客 - CSDN博客





2018年07月27日 16:06:25[紫芝](https://me.csdn.net/qq_40507857)阅读数：40
个人分类：[博弈论](https://blog.csdn.net/qq_40507857/article/category/7632032)









# D. Vasya and Chess

time limit per test

 2 seconds

memory limit per test

 256 megabytes

input

 standard input

output

 standard output

Vasya decided to learn to play chess. Classic chess doesn't seem interesting to him, so he plays his own sort of chess.

The queen is the piece that captures all squares on its vertical, horizontal and diagonal lines. If the cell is located on the same vertical, horizontal or diagonal line with queen, and the cell contains a piece of the enemy color, the queen is able to move to this square. After that the enemy's piece is removed from the board. The queen cannot move to a cell containing an enemy piece if there is some other piece between it and the queen.

There is an *n* × *n* chessboard. We'll denote a cell on the intersection of the *r*-th row and *c*-th column as (*r*, *c*). The square (1, 1) contains the white queen and the square (1, *n*) contains the black queen. All other squares contain green pawns that don't belong to anyone.

The players move in turns. The player that moves first plays for the white queen, his opponent plays for the black queen.

On each move the player has to capture some piece with his queen (that is, move to a square that contains either a green pawn or the enemy queen). The player loses if either he cannot capture any piece during his move or the opponent took his queen during the previous move.

Help Vasya determine who wins if both players play with an optimal strategy on the board *n* × *n*.

### Input

The input contains a single number *n* (2 ≤ *n* ≤ 109) — the size of the board.

### Output

On the first line print the answer to problem — string "white" or string "black", depending on who wins if the both players play optimally.

If the answer is "white", then you should also print two integers *r* and *c* representing the cell (*r*, *c*), where the first player should make his first move to win. If there are multiple such cells, print the one with the minimum *r*. If there are still multiple squares, print the one with the minimum *c*.

Examples

input

```
2
```

output

```
white
1 2
```

input

```
3
```

output

```
black
```

### Note

In the first sample test the white queen can capture the black queen at the first move, so the white player wins.

In the second test from the statement if the white queen captures the green pawn located on the central vertical line, then it will be captured by the black queen during the next move. So the only move for the white player is to capture the green pawn located at (2, 1).

Similarly, the black queen doesn't have any other options but to capture the green pawn located at (2, 3), otherwise if it goes to the middle vertical line, it will be captured by the white queen.

During the next move the same thing happens — neither the white, nor the black queen has other options rather than to capture green pawns situated above them. Thus, the white queen ends up on square (3, 1), and the black queen ends up on square (3, 3).

In this situation the white queen has to capture any of the green pawns located on the middle vertical line, after that it will be captured by the black queen. Thus, the player who plays for the black queen wins.

### 1.题意概述
- 一个n×nn×n的棋盘，白皇后在(1,1)(1,1)处，黑皇后在(1,n)(1,n)处，其余坐标为绿色棋子。皇后每次可以走行、列、对角线，但是要求**每一步必须吃掉棋子或者对方皇后**。问你这种策略下哪一方最终能够获胜？

### 2.解题思路
- 
我们先观察一下规律：
- 当n为2时候，先手（白）必胜
- 当n为3时候，后手（黑）必胜
- 当n为4时候，先手（白）可以往右侧行一格，那么情况就转变成了n为3时候黑白互换先后手

然后归纳出一般情况：n为奇数，黑必胜；n为偶数，白必胜！

- 
我们还可以这样考虑，以对称轴为中心，白如何走，黑就跟着走，最后状态一定是在对称轴结束，当n为偶数，一定是黑先过对称轴，而当n为奇数一定是白先到对称轴，后者赢。


![](https://img-blog.csdn.net/20180727155858297?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180727160256261?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```cpp
#include <cstdio>
#include <iostream>
using namespace std;
int main()
{
    int n;
    while(cin >> n){
        if(n&1)
        printf("black\n");
    else
        printf("white\n1 2\n");
    }
    return 0;
}
```





