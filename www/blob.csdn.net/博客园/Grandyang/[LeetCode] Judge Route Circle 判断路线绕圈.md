# [LeetCode] Judge Route Circle 判断路线绕圈 - Grandyang - 博客园







# [[LeetCode] Judge Route Circle 判断路线绕圈](https://www.cnblogs.com/grandyang/p/7514416.html)








Initially, there is a Robot at position (0, 0). Given a sequence of its moves, judge if this robot makes a circle, which means it moves back to the original place.

The move sequence is represented by a string. And each move is represent by a character. The valid robot moves are `R` (Right), `L`(Left), `U` (Up) and `D` (down). The output should be true or false representing whether the robot makes a circle.

Example 1:
Input: "UD"
Output: true



Example 2:
Input: "LL"
Output: false



这道题让我们判断一个路径是否绕圈，就是说有多少个U，就得对应多少个D。同理，L和R的个数也得相等。这不就是之前那道[Valid Parentheses](http://www.cnblogs.com/grandyang/p/4424587.html)的变种么，这次博主终于举一反三了！这比括号那题还要简单，因为括号至少还有三种，这里就水平和竖直两种。比较简单的方法就是使用两个计数器，如果是U，则cnt1自增1；如果是D，cnt1自减1。同理，如果是L，则cnt1自增1；如果是R，cnt1自减1。最后只要看cnt1和cnt2是否同时为0即可，参见代码如下：



解法一：

```
class Solution {
public:
    bool judgeCircle(string moves) {
        int cnt1 = 0, cnt2 = 0;
        for (char move : moves) {
            if (move == 'U') ++cnt1;
            else if (move == 'D') --cnt1;
            else if (move == 'L') ++cnt2;
            else if (move == 'R') --cnt2;
        }
        return cnt1 == 0 && cnt2 == 0;
    }
};
```



下面这种解法使用了哈希表来建立字符和其出现的次数之间的映射，最后直接比较对应的字符出现的次数是否相等即可，参见代码如下：



解法二：

```
class Solution {
public:
    bool judgeCircle(string moves) {
        unordered_map<char, int> m;
        for (char c : moves) ++m[c];
        return m['L'] == m['R'] && m['U'] == m['D'];
    }
};
```



类似题目：

[Valid Parentheses](http://www.cnblogs.com/grandyang/p/4424587.html)



参考资料： 

[https://discuss.leetcode.com/topic/99256/c-counter-4-lines-solution](https://discuss.leetcode.com/topic/99256/c-counter-4-lines-solution)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












