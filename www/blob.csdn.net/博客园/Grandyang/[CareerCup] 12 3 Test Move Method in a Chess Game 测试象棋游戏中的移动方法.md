# [CareerCup] 12.3 Test Move Method in a Chess Game 测试象棋游戏中的移动方法 - Grandyang - 博客园







# [[CareerCup] 12.3 Test Move Method in a Chess Game 测试象棋游戏中的移动方法](https://www.cnblogs.com/grandyang/p/4908264.html)







12.3 We have the following method used in a chess game: boolean canMoveTo( int x, int y). This method is part of the Piece class and returns whether or not the piece can move to position (x, y). Explain howyou would test this method.



这道题让我们测试象棋游戏中的移动方法boolean canMoveTo( int x, int y)，这个方法判断能否把棋子移动到(x, y)位置。这类问题通常有两种测试方法，极值测试和常规测试。

极值测试：我们需要测试一些坏的情况，可能会引起程序崩溃的值，例如

1. 检测x和y为负值的情况

2. 检测当x超过棋盘宽度的情况

3. 检测当y超过棋盘高度的情况

4. 检测一个满棋盘

5. 检测一个空棋盘，或是近似为空的棋盘

6. 检测白棋子远大于黑棋子的情况

7. 检测黑棋子远大于白棋子的情况

常规测试：检测全部情况会非常耗时，所以我们只检测部分区域。














