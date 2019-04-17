# [LeetCode] Flip Game 翻转游戏 - Grandyang - 博客园







# [[LeetCode] Flip Game 翻转游戏](https://www.cnblogs.com/grandyang/p/5224896.html)







You are playing the following Flip Game with your friend: Given a string that contains only these two characters: `+` and `-`, you and your friend take turns to flip twoconsecutive `"++"` into `"--"`. The game ends when a person can no longer make a move and therefore the other person will be the winner.

Write a function to compute all possible states of the string after one valid move.

For example, given `s = "++++"`, after one move, it may become one of the following states:
[
  "--++",
  "+--+",
  "++--"
]



If there is no valid move, return an empty list `[]`.



这道题让我们把相邻的两个++变成--，真不是一道难题，我们就从第二个字母开始遍历，每次判断当前字母是否为+，和之前那个字母是否为+，如果都为加，则将翻转后的字符串存入结果中即可，参见代码如下：



```
class Solution {
public:
    vector<string> generatePossibleNextMoves(string s) {
        vector<string> res;
        for (int i = 1; i < s.size(); ++i) {
            if (s[i] == '+' && s[i - 1] == '+') {
                res.push_back(s.substr(0, i - 1) + "--" + s.substr(i + 1));
            }
        }
        return res;
    }
};
```



类似题目：

[[Flip Game II](http://www.cnblogs.com/grandyang/p/5226206.html)](http://www.cnblogs.com/grandyang/p/4873248.html)



参考资料：

[https://leetcode.com/problems/flip-game/description/](https://leetcode.com/problems/flip-game/description/)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












