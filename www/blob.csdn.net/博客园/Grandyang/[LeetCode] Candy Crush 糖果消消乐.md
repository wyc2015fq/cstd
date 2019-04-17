# [LeetCode] Candy Crush 糖果消消乐 - Grandyang - 博客园







# [[LeetCode] Candy Crush 糖果消消乐](https://www.cnblogs.com/grandyang/p/7858414.html)







This question is about implementing a basic elimination algorithm for Candy Crush.

Given a 2D integer array `board` representing the grid of candy, different positive integers `board[i][j]` represent different types of candies. A value of `board[i][j] = 0` represents that the cell at position `(i, j)` is empty. The given board represents the state of the game following the player's move. Now, you need to restore the board to a *stable state* by crushing candies according to the following rules:
- If three or more candies of the same type are adjacent vertically or horizontally, "crush" them all at the same time - these positions become empty.
- After crushing all candies simultaneously, if an empty space on the board has candies on top of itself, then these candies will drop until they hit a candy or bottom at the same time. (No new candies will drop outside the top boundary.)
- After the above steps, there may exist more candies that can be crushed. If so, you need to repeat the above steps.
- If there does not exist more candies that can be crushed (ie. the board is *stable*), then return the current board.

You need to perform the above rules until the board becomes stable, then return the current board.

Example 1:
Input:
board = 
[[110,5,112,113,114],[210,211,5,213,214],[310,311,3,313,314],[410,411,412,5,414],[5,1,512,3,3],[610,4,1,613,614],[710,1,2,713,714],[810,1,2,1,1],[1,1,2,2,2],[4,1,4,4,1014]]
Output:
[[0,0,0,0,0],[0,0,0,0,0],[0,0,0,0,0],[110,0,0,0,114],[210,0,0,0,214],[310,0,0,113,314],[410,0,0,213,414],[610,211,112,313,614],[710,311,412,613,714],[810,411,512,713,1014]]
Explanation: 

![](https://leetcode.com/static/images/problemset/candy_crush_example_2.png)



Note:
- The length of `board` will be in the range [3, 50].
- The length of `board[i]` will be in the range [3, 50].
- Each `board[i][j]` will initially start as an integer in the range [1, 2000].



这道题就是糖果消消乐，博主刚开始做的时候，没有看清楚题意，以为就像游戏中的那样，每次只能点击一个地方，然后消除后糖果落下，这样会导致一个问题，就是原本其他可以消除的地方在糖果落下后可能就没有了，所以博主在想点击的顺序肯定会影响最终的stable的状态，可是题目怎么没有要求返回所剩糖果最少的状态？后来发现，其实这道题一次消除table中所有可消除的糖果，然后才下落，形成新的table，这样消除后得到的结果就是统一的了，这样也大大的降低了难度。下面就来看如何找到要消除的糖果，可能有人会想像之前的岛屿的题目一样找连通区域，可是这道题的有限制条件，只有横向或竖向相同的糖果数达到三个才能消除，并不是所有的连通区域都能消除，所以找连通区域不是一个好办法。最好的办法其实是每个糖果单独检查其是否能被消除，然后把所有能被删除的糖果都标记出来统一删除，然后在下落糖果，然后再次查找，直到无法找出能够消除的糖果时达到稳定状态。好，那么我们用一个数组来保存可以被消除的糖果的位置坐标，判断某个位置上的糖果能否被消除的方法就是检查其横向和纵向的最大相同糖果的个数，只要有一个方向达到三个了，当前糖果就可以被消除。所以我们对当前糖果的上下左右四个方向进行查看，用四个变量x0, x1, y0, y1，其中x0表示上方相同的糖果的最大位置，x1表示下方相同糖果的最大位置，y0表示左边相同糖果的最大位置，y1表示右边相同糖果的最大位置，均初始化为当前糖果的位置，然后使用while循环向每个方向遍历，注意我们并不需要遍历到头，而是只要遍历三个糖果就行了，因为一旦查到了三个相同的，就说明当前的糖果已经可以消除了，没必要再往下查了。查的过程还要注意处理越界情况，好，我们得到了上下左右的最大的位置，分别让相同方向的做差，如果水平和竖直方向任意一个大于3了，就说明可以消除，将坐标加入数组del中。注意这里一定要大于3，是因为当发现不相等退出while循环时，坐标值已经改变了，所以已经多加了或者减了一个，所以差值要大于3。遍历完成后，如果数组del为空，说明已经stable了，直接break掉，否则将要消除的糖果位置都标记为0，然后进行下落处理。下落处理实际上是把数组中的0都移动到开头，那么就从数组的末尾开始遍历，用一个变量t先指向末尾，然后然后当遇到非0的数，就将其和t位置上的数置换，然后t自减1，这样t一路减下来都是非0的数，而0都被置换到数组开头了，参见代码如下：



```
class Solution {
public:
    vector<vector<int>> candyCrush(vector<vector<int>>& board) {
        int m = board.size(), n = board[0].size();
        while (true) {
            vector<pair<int, int>> del;
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (board[i][j] == 0) continue;
                    int x0 = i, x1 = i, y0 = j, y1 = j;
                    while (x0 >= 0 && x0 > i - 3 && board[x0][j] == board[i][j]) --x0;
                    while (x1 < m && x1 < i + 3 && board[x1][j] == board[i][j]) ++x1;
                    while (y0 >= 0 && y0 > j - 3 && board[i][y0] == board[i][j]) --y0;
                    while (y1 < n && y1 < j + 3 && board[i][y1] == board[i][j]) ++y1;
                    if (x1 - x0 > 3 || y1 - y0 > 3) del.push_back({i, j});
                }
            }
            if (del.empty()) break;
            for (auto a : del) board[a.first][a.second] = 0;
            for (int j = 0; j < n; ++j) {
                int t = m - 1;
                for (int i = m - 1; i >= 0; --i) {
                    if (board[i][j]) swap(board[t--][j], board[i][j]);   
                }
            }
        }
        return board;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/109635/simple-c-brute-force](https://discuss.leetcode.com/topic/109635/simple-c-brute-force)

[https://discuss.leetcode.com/topic/109952/ac-java-solution-easy-to-understand](https://discuss.leetcode.com/topic/109952/ac-java-solution-easy-to-understand)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












