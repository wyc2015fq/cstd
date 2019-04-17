# [LeetCode] Valid Tic-Tac-Toe State 验证井字棋状态 - Grandyang - 博客园







# [[LeetCode] Valid Tic-Tac-Toe State 验证井字棋状态](https://www.cnblogs.com/grandyang/p/9223105.html)







A Tic-Tac-Toe board is given as a string array `board`. Return True if and only if it is possible to reach this board position during the course of a valid tic-tac-toe game.

The `board` is a 3 x 3 array, and consists of characters `" "`, `"X"`, and `"O"`.  The " " character represents an empty square.

Here are the rules of Tic-Tac-Toe:
- Players take turns placing characters into empty squares (" ").
- The first player always places "X" characters, while the second player always places "O" characters.
- "X" and "O" characters are always placed into empty squares, never filled ones.
- The game ends when there are 3 of the same (non-empty) character filling any row, column, or diagonal.
- The game also ends if all squares are non-empty.
- No more moves can be played if the game is over.
Example 1:
Input: board = ["O  ", "   ", "   "]
Output: false
Explanation: The first player always plays "X".

Example 2:
Input: board = ["XOX", " X ", "   "]
Output: false
Explanation: Players take turns making moves.

Example 3:
Input: board = ["XXX", "   ", "OOO"]
Output: false

Example 4:
Input: board = ["XOX", "O O", "XOX"]
Output: true

Note:
- `board` is a length-3 array of strings, where each string `board[i]` has length 3.
- Each `board[i][j]` is a character in the set `{" ", "X", "O"}`.



这道题又是关于井字棋游戏的，之前也有一道类似的题[Design Tic-Tac-Toe](http://www.cnblogs.com/grandyang/p/5467118.html)，不过那道题是模拟游戏进行的，而这道题是让我们验证当前井字棋的游戏状态是否正确。这题的例子给的比较好，cover了很多种情况：

情况一：

```
0 _ _
_ _ _
_ _ _
```

这是不正确的状态，因为先走的使用X，所以只出现一个O，是不对的。

情况二：

```
X O X
_ X _
_ _ _
```

这个也是不正确的，因为两个player交替下棋，X最多只能比O多一个，这里多了两个，肯定是不对的。

情况三：

```
X X X
_ _ _ 
O O O
```

这个也是不正确的，因为一旦第一个玩家的X连成了三个，那么游戏马上结束了，不会有另外一个O出现。

情况四：

```
X O X
O _ O
X O X
```

这个状态没什么问题，是可以出现的状态。

好，那么根据给的这些例子，我们可以分析一下规律，根据例子1和例子2我们得出下棋顺序是有规律的，必须是先X后O，不能破坏这个顺序，那么我们可以使用一个turns变量，当是X时，turns自增1，反之若是O，则turns自减1，那么最终turns一定是0或者1，其他任何值都是错误的，比如例子1中，turns就是-1，例子2中，turns是2，都是不对的。根据例子3，我们得出结论，只能有一个玩家获胜，那么我们可以用两个变量xwin和owin，来记录两个玩家的获胜状态，由于井字棋的制胜规则是横竖斜任意一个方向有三个连续的就算赢，那么我们分别在各个方向查找3个连续的X，有的话xwin赋值为true，还要查找3个连续的O，有的话owin赋值为true，例子3中xwin和owin同时为true了，是错误的。还有一种情况，例子中没有cover到的是：

情况五：

```
X X X
O O _
O _ _
```

我们看到虽然只有xwin为true，但是这种状态还是错误的，因为一旦第三个X放下后，游戏立即结束，不会有第三个O放下，这么检验这种情况呢？这是我们的turns变量就非常的重要了，当第三个O放下后，turns自减1，此时turns为0了，而正确的应该是当xwin为true的时候，第三个O不能放下，那么turns不减1，则还是1，这样就可以区分情况五了。当然，我们可以交换X和O的位置，即当owin为true时，turns一定要为0。现在我们已经覆盖了搜索的情况了，参见代码如下：



```
class Solution {
public:
    bool validTicTacToe(vector<string>& board) {
        bool xwin = false, owin = false;
        vector<int> row(3), col(3);
        int diag = 0, antidiag = 0, turns = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == 'X') {
                    ++row[i]; ++col[j]; ++turns;
                    if (i == j) ++diag;
                    if (i + j == 2) ++antidiag;
                } else if (board[i][j] == 'O') {
                    --row[i]; --col[j]; --turns;
                    if (i == j) --diag;
                    if (i + j == 2) --antidiag;
                }
            }
        }
        xwin = row[0] == 3 || row[1] == 3 || row[2] == 3 ||
               col[0] == 3 || col[1] == 3 || col[2] == 3 ||
               diag == 3 || antidiag == 3;
        owin = row[0] == -3 || row[1] == -3 || row[2] == -3 ||
               col[0] == -3 || col[1] == -3 || col[2] == -3 ||
               diag == -3 || antidiag == -3;
        if ((xwin && turns == 0) || (owin && turns == 1)) return false;
        return (turns == 0 || turns == 1) && (!xwin || !owin);
    }
};
```



类似题目：

[Design Tic-Tac-Toe](http://www.cnblogs.com/grandyang/p/5467118.html)



参考资料：

[https://leetcode.com/problems/valid-tic-tac-toe-state/discuss/117580/Straightforward-Java-solution-with-explaination](https://leetcode.com/problems/valid-tic-tac-toe-state/discuss/117580/Straightforward-Java-solution-with-explaination)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












