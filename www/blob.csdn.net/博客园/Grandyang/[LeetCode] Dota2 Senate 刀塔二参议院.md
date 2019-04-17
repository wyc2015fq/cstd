# [LeetCode] Dota2 Senate 刀塔二参议院 - Grandyang - 博客园







# [[LeetCode] Dota2 Senate 刀塔二参议院](https://www.cnblogs.com/grandyang/p/7439222.html)







In the world of Dota2, there are two parties: the `Radiant` and the `Dire`.

The Dota2 senate consists of senators coming from two parties. Now the senate wants to make a decision about a change in the Dota2 game. The voting for this change is a round-based procedure. In each round, each senator can exercise `one` of the two rights:
- `Ban one senator's right`: 
A senator can make another senator lose all his rights in this and all the following rounds.
- `Announce the victory`: 
If this senator found the senators who still have rights to vote are all from the same party, he can announce the victory and make the decision about the change in the game.



Given a string representing each senator's party belonging. The character 'R' and 'D' represent the `Radiant` party and the `Dire` party respectively. Then if there are `n` senators, the size of the given string will be `n`.

The round-based procedure starts from the first senator to the last senator in the given order. This procedure will last until the end of voting. All the senators who have lost their rights will be skipped during the procedure.

Suppose every senator is smart enough and will play the best strategy for his own party, you need to predict which party will finally announce the victory and make the change in the Dota2 game. The output should be `Radiant` or `Dire`.

Example 1:
Input: "RD"
Output: "Radiant"
Explanation: The first senator comes from Radiant and he can just ban the next senator's right in the round 1. 
And the second senator can't exercise any rights any more since his right has been banned. 
And in the round 2, the first senator can just announce the victory since he is the only guy in the senate who can vote.



Example 2:
Input: "RDD"
Output: "Dire"
Explanation: 
The first senator comes from Radiant and he can just ban the next senator's right in the round 1. 
And the second senator can't exercise any rights anymore since his right has been banned. 
And the third senator comes from Dire and he can ban the first senator's right in the round 1. 
And in the round 2, the third senator can just announce the victory since he is the only guy in the senate who can vote.



Note:
- The length of the given string will in the range [1, 10,000].



该来的总会来！！！自从上次LeetCode拿提莫出题[Teemo Attacking](http://www.cnblogs.com/grandyang/p/6399408.html)后，我就知道刀塔早晚也难逃魔掌，这道题直接就搞起了刀塔二。不过话说如果你是从魔兽3无缝过渡到刀塔，那么应该熟悉了两个阵营的叫法，近卫和天灾。刀塔二里面不知道搞什么鬼，改成了光辉和梦魇，不管了，反正跟这道题的解法没什么关系。这道题模拟了刀塔类游戏开始之前的BP过程，两个阵营按顺序Ban掉对方的英雄，看最后谁剩下来了，就返回哪个阵营。那么博主能想到的简单暴力的方法就是先统计所有R和D的个数，然后从头开始遍历，如果遇到了R，就扫描之后所有的位置，然后还要扫描R前面的位置，这就要用到数组的环形遍历的知识了，其实就是坐标对总长度取余，使其不会越界，如果我们找到了下一个D，就将其标记为B，然后对应的计数器cntR自减1。对于D也是同样处理，我们的while循环的条件是cntR和cntD都要大于0，当有一个等于0了的话，那么推出循环，返回那个不为0的阵营即可，参见代码如下：



解法一：

```
class Solution {
public:
    string predictPartyVictory(string senate) {
        int n = senate.size(), cntR = 0, cntD = 0;
        for (char c : senate) {
            c == 'R' ? ++cntR : ++cntD;
        }
        if (cntR == 0) return "Dire";
        if (cntD == 0) return "Radiant";
        while (cntR > 0 && cntD > 0) {
            for (int i = 0; i < n; ++i) {
                if (senate[i] == 'R') {
                    for (int j = i + 1; j < i + n; ++j) {
                        if (senate[j % n] == 'D') {
                            senate[j % n] = 'B';
                            --cntD;
                            break;
                        }
                    }
                } else if (senate[i] == 'D') {
                    for (int j = i + 1; j < i + n; ++j) {
                        if (senate[j % n] == 'R') {
                            senate[j % n] = 'B';
                            --cntR;
                            break;
                        }
                    }
                }
            }
        }
        return cntR != 0 ? "Radiant" : "Dire";
    }
};
```



上面的暴力搜索的方法略显复杂，我们其实有更好的方法来做，我们可以用两个队列queue，把各自阵营的位置存入不同的队列里面，然后进行循环，每次从两个队列各取一个位置出来，看其大小关系，小的那个说明在前面，就可以把后面的那个Ban掉，所以我们要把小的那个位置要加回队列里面，但是不能直接加原位置，因为下一轮才能再轮到他来Ban，所以我们要加上一个n，再排入队列。这样当某个队列为空时，推出循环，我们返回不为空的那个阵营，参见代码如下：



解法二：

```
class Solution {
public:
    string predictPartyVictory(string senate) {
        int n = senate.size();
        queue<int> q1, q2;
        for (int i = 0; i < n; ++i) {
            (senate[i] == 'R') ? q1.push(i) : q2.push(i);
        }
        while (!q1.empty() && !q2.empty()) {
            int i = q1.front(); q1.pop();
            int j = q2.front(); q2.pop();
            (i < j) ? q1.push(i + n) : q2.push(j + n);
        }
        return (q1.size() > q2.size()) ? "Radiant" : "Dire";
    }
};
```



类似题目：

[Teemo Attacking](http://www.cnblogs.com/grandyang/p/6399408.html)



参考资料：

[https://discuss.leetcode.com/topic/97671/java-c-very-simple-greedy-solution-with-explanation](https://discuss.leetcode.com/topic/97671/java-c-very-simple-greedy-solution-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












