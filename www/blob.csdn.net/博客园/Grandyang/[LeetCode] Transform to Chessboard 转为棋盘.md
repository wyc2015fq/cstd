# [LeetCode] Transform to Chessboard 转为棋盘 - Grandyang - 博客园







# [[LeetCode] Transform to Chessboard 转为棋盘](https://www.cnblogs.com/grandyang/p/9053705.html)







An N x N `board` contains only `0`s and `1`s. In each move, you can swap any 2 rows with each other, or any 2 columns with each other.

What is the minimum number of moves to transform the board into a "chessboard" - a board where no `0`s and no `1`s are 4-directionally adjacent? If the task is impossible, return -1.
Examples:
Input: board = [[0,1,1,0],[0,1,1,0],[1,0,0,1],[1,0,0,1]]
Output: 2
Explanation:
One potential sequence of moves is shown below, from left to right:

0110     1010     1010
0110 --> 1010 --> 0101
1001     0101     1010
1001     0101     0101

The first move swaps the first and second column.
The second move swaps the second and third row.


Input: board = [[0, 1], [1, 0]]
Output: 0
Explanation:
Also note that the board with 0 in the top left corner,
01
10

is also a valid chessboard.

Input: board = [[1, 0], [1, 0]]
Output: -1
Explanation:
No matter what sequence of moves you make, you cannot end with a valid chessboard.

Note:
- `board` will have the same number of rows and columns, a number in the range `[2, 30]`.
- `board[i][j]` will be only `0`s or `1`s.



这道题给了我们一个二维数组，里面都是由0和1组成的，让我们通过交换行或者列来形成一个棋盘。棋盘我们都见过吧，就是国际象棋的那种棋盘，黑白相间的那种，用数组表示就是0和1交替出现，相邻位置上的数字必定不是一样的。这道题默认的棋盘的起始位置可以是1或者0，然后依次类推可得到所有位置上的值。这道题最大的难点是在于判断给定的数组最终能否组成棋盘，因为能通过交换组成棋盘的数组其实是有很多苛刻条件需要满足的，只有这些条件都满足了，才能到计算交换数到那一步。首先我们先来看长度为4的棋盘：

1 0 1 0

0 1 0 1

1 0 1 0

0 1 0 1

或者：

0 1 0 1

1 0 1 0

0 1 0 1

1 0 1 0

我们发现对于长度为偶数的棋盘，每一行0和1的个数都是相等的，不管我们如何交换行和列，0和1的个数都是不会变化的，再看看长度为奇数的棋盘，比如3:

1 0 1

0 1 0

1 0 1

或者：

0 1 0

1 0 1

0 1 0

我们发现对于长度为奇数的棋盘，各行的0和1个数不同，但是还是有规律的，每行的1的个数要么为 n/2，要么为 (n+1)/2，这个规律一定要保证，不然无法形成棋盘。

还有一个很重要的规律，我们观察题目给的第一个例子，如果我们只看行，我们发现只有两种情况 0110 和 1001，如果只看列，只有 0011 和 1100，我们发现不管棋盘有多长，都只有两种情况，而这两种情况上各位上是相反的，只有这样的矩阵才有可能转换为棋盘。那么这个规律可以衍生出一个规律，就是任意一个矩形的四个顶点只有三种情况，要么四个0，要么四个1，要么两个0两个1，不会有其他的情况。那么四个顶点亦或在一起一定是0，所以我们判断只要亦或出了1，一定是不对的，直接返回-1。之后我们来统计首行和首列中的1个数，因为我们要让其满足之前提到的规律。统计完了首行首列1的个数，我们判断如果其小于 n/2 或者大于 (n+1) / 2，那么一定无法转为棋盘。我们还需要算下首行和首列跟棋盘位置的错位的个数，虽然 01010 和 10101 都可以是正确的棋盘，我们先默认跟 10101 比较好了，之后再做优化处理。

最后的难点就是计算最小的交换步数了，这里要分n的奇偶来讨论。如果n是奇数，我们必须得到偶数个，为啥呢，因为我们之前统计的是跟棋盘位置的错位的个数，而每次交换行或者列，会修改两个错位，所以如果是奇数就无法还原为棋盘。举个例子，比如首行是 10001，如果我们跟棋盘 10101 比较，只有一个错位，但是我们是无法通过交换得到 10101的，所以我们必须要交换得到 01010，此时的错位是4个，而我们通过 n - rowDiff 正好也能得到4，这就是为啥我们需要偶数个错位。如果n是偶数，那么就不会出现这种问题，但是会出现另一个问题，比如我们是 0101，这本身就是正确的棋盘排列了，但是由于我们默认是跟 1010 比较，那么我们会得到4个错位，所以我们应该跟 n - rowDiff 比较取较小值。列的处理跟行的处理完全一样。最终我们把行错位个数跟列错位个数相加，再除以2，就可以得到最小的交换次数了，之前说过了每交换一次，可以修复两个错位，参见代码如下：



```
class Solution {
public:
    int movesToChessboard(vector<vector<int>>& board) {
        int n = board.size(), rowSum = 0, colSum = 0, rowDiff = 0, colDiff = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (board[0][0] ^ board[i][0] ^ board[0][j] ^ board[i][j]) return -1;
            }
        }
        for (int i = 0; i < n; ++i) {
            rowSum += board[0][i];
            colSum += board[i][0];
            rowDiff += (board[i][0] == i % 2);
            colDiff += (board[0][i] == i % 2);
        }
        if (n / 2 > rowSum || rowSum > (n + 1) / 2) return -1;
        if (n / 2 > colSum || colSum > (n + 1) / 2) return -1;
        if (n % 2) {
            if (rowDiff % 2) rowDiff = n - rowDiff;
            if (colDiff % 2) colDiff = n - colDiff;
        } else {
            rowDiff = min(n - rowDiff, rowDiff);
            colDiff = min(n - colDiff, colDiff);
        }
        return (rowDiff + colDiff) / 2;
    }
};
```



参考资料：

[https://leetcode.com/problems/transform-to-chessboard/](https://leetcode.com/problems/transform-to-chessboard/)

[https://leetcode.com/problems/transform-to-chessboard/discuss/132113/Java-Clear-Code-with-Detailed-Explanations](https://leetcode.com/problems/transform-to-chessboard/discuss/132113/Java-Clear-Code-with-Detailed-Explanations)

[https://leetcode.com/problems/transform-to-chessboard/discuss/114847/Easy-and-Concise-Solution-with-Explanation-C%2B%2BJavaPython](https://leetcode.com/problems/transform-to-chessboard/discuss/114847/Easy-and-Concise-Solution-with-Explanation-C%2B%2BJavaPython)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












