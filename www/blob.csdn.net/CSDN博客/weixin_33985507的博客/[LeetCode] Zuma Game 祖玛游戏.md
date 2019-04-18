# [LeetCode] Zuma Game 祖玛游戏 - weixin_33985507的博客 - CSDN博客
2017年04月25日 00:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
Think about Zuma Game. You have a row of balls on the table, colored red(R), yellow(Y), blue(B), green(G), and white(W). You also have several balls in your hand.
Each time, you may choose a ball in your hand, and insert it into the row (including the leftmost place and rightmost place). Then, if there is a group of 3 or more balls in the same color touching, remove these balls. Keep doing this until no more balls can be removed.
Find the minimal balls you have to insert to remove all the balls on the table. If you cannot remove all the balls, output -1.
**Examples:**
**Input:** "WRRBBW", "RB"
**Output:** -1
**Explanation:** WRRBBW -> WRR[R]BBW -> WBBW -> WBB[B]W -> WW
**Input:** "WWRRBBWW", "WRBRW"
**Output:** 2
**Explanation:** WWRRBBWW -> WWRR[R]BBWW -> WWBBWW -> WWBB[B]WW -> WWWW -> empty
**Input:**"G", "GGGGG"
**Output:** 2
**Explanation:** G -> G[G] -> GG[G] -> empty 
**Input:** "RBYYBBRRB", "YRBGB"
**Output:** 3
**Explanation:** RBYYBBRRB -> RBYY[Y]BBRRB -> RBBBRRB -> RRRB -> B -> B[B] -> BB[B] -> empty 
**Note:**
- - You may assume that the initial row of balls on the table won’t have any 3 or more consecutive balls with the same color.
- The number of balls on the table won't exceed 20, and the string represents these balls is called "board" in the input.
- The number of balls in your hand won't exceed 5, and the string represents these balls is called "hand" in the input.
- Both input strings will be non-empty and only contain characters 'R','Y','B','G','W'.
这道题说的就是著名的祖玛游戏了，让我想起了以前玩过的泡泡龙，也是一种祖玛游戏，在QQ上也有泡泡龙的游戏，还可以使用各种道具害其他玩家，相当有趣。那么这道题是一种简化版的祖玛游戏，只是一个一维数组，而且通过限定桌面上的球不超过20个，手里的球不超过5个来降低来难度，貌似是在暗示我们可以用暴力搜索法来做。这道题比较使用递归的方法来做，通过遍历所有可能的情况来找出最优解，题目希望我们用最少的球来消掉桌上所有的球，如果不能完全消掉，返回-1。我们使用哈希表来统计手中每种球的个数，然后我们遍历桌上的球，我们找连续相同球的个数，在没有可以消除的情况下，连续的个数只能是1个或2个，然后我们用3减去连续个数，就是我们需要补充的球数以使其可以被消除，那么我们在哈希表表中看我们手中的该类型的球够不够，如果够就表示可以消除，我们在哈希表中减去需要使用掉的球数，然后将消掉的球移除，对新的字符串调用递归，如果可以成功消除，会返回一个结果，该结果加上之前需要的球数用来更新结果res，注意调用完递归要恢复哈希表的状态。还有就是在刚进入递归函数时，我们要检测字符串，去除连续3个相同球的情况，这个去除函数也是个递归函数，写起来很简洁，但是很强大，参见代码如下：
解法一：
```
class Solution {
public:
    int findMinStep(string board, string hand) {
        int res = INT_MAX;
        unordered_map<char, int> m;
        for (char c : hand) ++m[c];
        res = helper(board, m);
        return res == INT_MAX ? -1 : res;
    }
    int helper(string board, unordered_map<char, int>& m) {
        board = removeConsecutive(board);
        if (board.empty()) return 0;
        int cnt = INT_MAX, j = 0;
        for (int i = 0; i <= board.size(); ++i) {
            if (i < board.size() && board[i] == board[j]) continue;
            int need = 3 - (i - j);
            if (m[board[j]] >= need) {
                m[board[j]] -= need;
                int t = helper(board.substr(0, j) + board.substr(i), m);
                if (t != INT_MAX) cnt = min(cnt, t + need);
                m[board[j]] += need;
            }
            j = i;
        }
        return cnt;
    }
    string removeConsecutive(string board) {
        for (int i = 0, j = 0; i <= board.size(); ++i) {
            if (i < board.size() && board[i] == board[j]) continue;
            if (i - j >= 3) return removeConsecutive(board.substr(0, j) + board.substr(i));
            else j = i;
        }
        return board;
    }
};
```
下面这种解法也是递归解法，但是思路和上面略有不同，这里我们不使用哈希表，而是使用一个集合，我们遍历手中的所有小球，如果某个小球已经在集合中存在了，说明我们已经处理过该小球了，直接跳过，否则就将该小球加入集合中。然后我们遍历桌上的小球，寻找和当前手中小球一样的位置，然后将手中小球加入当前位置，调用去除重复3个小球的函数，如果此时字符串为0了，说明当前桌上小球已经完全消掉了，返回1，因为我们此时只使用了一个小球；否则就将手中的当前小球去掉，对新的桌面和剩余手中的小球调用递归，如果得到的结果不是-1，我们用此结果加1来更新结果res，参见代码如下：
解法二：
```
class Solution {
public:
    int findMinStep(string board, string hand) {
        int res = INT_MAX;
        unordered_set<char> s;
        for (int i = 0; i < hand.size(); ++i) {
            if (s.count(hand[i])) continue;
            s.insert(hand[i]);
            for (int j = 0; j < board.size(); ++j) {
                if (board[j] != hand[i]) continue;
                string newBoard = board, newHand = hand;
                newBoard.insert(j, 1, hand[i]);
                newBoard = removeConsecutive(newBoard);
                if (newBoard.size() == 0) return 1;
                newHand.erase(i, 1);
                int cnt = findMinStep(newBoard, newHand);
                if (cnt != -1) res = min(res, cnt + 1);
            }
        }
        return res == INT_MAX ? -1 : res;
    }
    string removeConsecutive(string board) {
        for (int i = 0, j = 0; i <= board.size(); ++i) {
            if (i < board.size() && board[i] == board[j]) continue;
            if (i - j >= 3) return removeConsecutive(board.substr(0, j) + board.substr(i));
            else j = i;
        }
        return board;
    }
};
```
类似题目：
[Burst Balloons](http://www.cnblogs.com/grandyang/p/5006441.html)
[Remove Boxes](http://www.cnblogs.com/grandyang/p/6850657.html)[](http://www.cnblogs.com/grandyang/p/6759881.html)
[Strange Printer](http://www.cnblogs.com/grandyang/p/8319913.html)
参考资料：
[https://discuss.leetcode.com/topic/76360/bfs](https://discuss.leetcode.com/topic/76360/bfs)
[https://discuss.leetcode.com/topic/75578/simplest-method/2](https://discuss.leetcode.com/topic/75578/simplest-method/2)
[https://discuss.leetcode.com/topic/79820/short-java-solution-beats-98](https://discuss.leetcode.com/topic/79820/short-java-solution-beats-98)
[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)
