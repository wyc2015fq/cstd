# [LeetCode] Flip Game II 翻转游戏之二 - Grandyang - 博客园







# [[LeetCode] Flip Game II 翻转游戏之二](https://www.cnblogs.com/grandyang/p/5226206.html)







You are playing the following Flip Game with your friend: Given a string that contains only these two characters: `+` and `-`, you and your friend take turns to flip two consecutive `"++"` into `"--"`. The game ends when a person can no longer make a move and therefore the other person will be the winner.

Write a function to determine if the starting player can guarantee a win.

Example:
Input: `s = "++++"`
Output: true 
Explanation: The starting player can guarantee a win by flipping the middle `"++"` to become `"+--+"`.

Follow up:
Derive your algorithm's runtime complexity.



这道题是之前那道[Flip Game](http://www.cnblogs.com/grandyang/p/5224896.html)的拓展，让我们判断先手的玩家是否能赢，那么我们可以穷举所有的情况，用回溯法来解题，我们的思路跟上面那题类似，也是从第二个字母开始遍历整个字符串，如果当前字母和之前那个字母都是+，那么我们递归调用将这两个位置变为--的字符串，如果返回false，说明当前玩家可以赢，结束循环返回false，参见代码如下：



解法一：

```
class Solution {
public:
    bool canWin(string s) {
        for (int i = 1; i < s.size(); ++i) {
            if (s[i] == '+' && s[i - 1] == '+' && !canWin(s.substr(0, i - 1) + "--" + s.substr(i + 1))) {
                return true;
            }
        }
        return false;
    }
};
```



第二种解法和第一种解法一样，只是用find函数来查找++的位置，然后把位置赋值给i，然后还是递归调用canWin函数，参见代码如下：



解法二：

```
class Solution {
public:
    bool canWin(string s) {
        for (int i = -1; (i = s.find("++", i + 1)) >= 0;) {
            if (!canWin(s.substr(0, i) + "--" + s.substr(i + 2))) {
                return true;
            }
        }
        return false;
    }
};
```



类似题目：

[Nim Game](http://www.cnblogs.com/grandyang/p/4873248.html)

[Flip Game](http://www.cnblogs.com/grandyang/p/5224896.html)

[Guess Number Higher or Lower II](http://www.cnblogs.com/grandyang/p/5677550.html)

[Can I Win](http://www.cnblogs.com/grandyang/p/6103525.html)



参考资料：

[https://leetcode.com/problems/flip-game-ii/](https://leetcode.com/problems/flip-game-ii/)

[https://leetcode.com/problems/flip-game-ii/discuss/74033/4-line-Java-Solution](https://leetcode.com/problems/flip-game-ii/discuss/74033/4-line-Java-Solution)

[https://leetcode.com/problems/flip-game-ii/discuss/74010/Short-Java-and-Ruby](https://leetcode.com/problems/flip-game-ii/discuss/74010/Short-Java-and-Ruby)

[https://leetcode.com/problems/flip-game-ii/discuss/73962/Share-my-Java-backtracking-solution](https://leetcode.com/problems/flip-game-ii/discuss/73962/Share-my-Java-backtracking-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












