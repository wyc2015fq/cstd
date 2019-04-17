# [LeetCode] Output Contest Matches 输出比赛匹配对 - Grandyang - 博客园







# [[LeetCode] Output Contest Matches 输出比赛匹配对](https://www.cnblogs.com/grandyang/p/6828353.html)







During the NBA playoffs, we always arrange the rather strong team to play with the rather weak team, like make the rank 1 team play with the rank nth team, which is a good strategy to make the contest more interesting. Now, you're given n teams, you need to output their final contest matches in the form of a string.

The n teams are given in the form of positive integers from 1 to n, which represents their initial rank. (Rank 1 is the strongest team and Rank n is the weakest team.) We'll use parentheses('(', ')') and commas(',') to represent the contest team pairing - parentheses('(' , ')') for pairing and commas(',') for partition. During the pairing process in each round, you always need to follow the strategy of making the rather strong one pair with the rather weak one.

Example 1:
Input: 2
Output: (1,2)
Explanation: 
Initially, we have the team 1 and the team 2, placed like: 1,2.
Then we pair the team (1,2) together with '(', ')' and ',', which is the final answer.



Example 2:
Input: 4
Output: ((1,4),(2,3))
Explanation: 
In the first round, we pair the team 1 and 4, the team 2 and 3 together, as we need to make the strong team and weak team together.
And we got (1,4),(2,3).
In the second round, the winners of (1,4) and (2,3) need to play again to generate the final winner, so you need to add the paratheses outside them.
And we got the final answer ((1,4),(2,3)).



Example 3:
Input: 8
Output: (((1,8),(4,5)),((2,7),(3,6)))
Explanation: 
First round: (1,8),(2,7),(3,6),(4,5)
Second round: ((1,8),(4,5)),((2,7),(3,6))
Third round: (((1,8),(4,5)),((2,7),(3,6)))
Since the third round will generate the final winner, you need to output the answer (((1,8),(4,5)),((2,7),(3,6))).



Note:
- The n is in range [2, 212].
- We ensure that the input n can be converted into the form 2k, where k is a positive integer.



这道题讲的是NBA的季后赛对战顺序，对于一个看了十几年NBA的老粉来说，再熟悉不过了。这种对战顺序是为了避免强强之间过早对决，从而失去比赛的公平性，跟欧冠欧联那种八强就开始随机抽签匹配有本质上的区别。NBA的这种比赛机制基本弱队很难翻身，假如你是拿到最后一张季后赛门票进的，那么一上来就干联盟第一，肯定凶多吉少，很有可能就被横扫了。但是偶尔也会出现黑八的情况，但都是极其少见的，毕竟像勇士这么叼的球队毕竟不多。好了，不闲扯了，来做题吧。我们就拿NBA这种八个球队的情况来分析吧，八只球队的排名是按常规赛胜率来排的：

1 2 3 4 5 6 7 8

因为是最强和最弱来对决，其次是次强与次弱对决，以此类推可得到：

1-8  2-7  3-6  4-5

那么接下来呢，还是最强与最弱，次强与次弱这种关系：

(1-8  4-5)  (2-7  3-6)

最后胜者争夺冠军

((1-8  4-5)  (2-7  3-6))

这样一分析是不是就清楚了呢，由于n限定了是2的次方数，那么就是可以一直对半分的，比如开始有n队，第一拆分为n/2对匹配，然后再对半拆，就是n/2/2，直到拆到n为1停止，而且每次都是首与末配对，次首与次末配对，这样搞清楚了规律，代码应该就不难写了吧，参见代码如下：



解法一：

```
class Solution {
public:
    string findContestMatch(int n) {
        vector<string> v;
        for (int i = 1; i <= n; ++i) v.push_back(to_string(i));
        while (n > 1) {
            for (int i = 0; i < n / 2; ++i) {
                v[i] = "(" + v[i] + "," + v[n - i - 1] + ")";
            }
            n /= 2;
        }
        return v[0];
    }
};
```



下面这种方法是递归的写法，解题思路跟上面没有区别，参见代码如下：



解法二：

```
class Solution {
public:
    string findContestMatch(int n) {
        vector<string> v;
        for (int i = 1; i <= n; ++i) v.push_back(to_string(i));
        helper(n, v);
        return v[0];
    }
    void helper(int n, vector<string>& v) {
        if (n == 1) return;
        for (int i = 0; i < n; ++i) {
            v[i] = "(" + v[i] + "," + v[n - i - 1] + ")";
        }
        helper(n / 2, v);
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/83454/java-10-lines/2](https://discuss.leetcode.com/topic/83454/java-10-lines/2)

[https://discuss.leetcode.com/topic/83457/c-java-clean-code](https://discuss.leetcode.com/topic/83457/c-java-clean-code)

[https://discuss.leetcode.com/topic/83460/java-recursive-solution](https://discuss.leetcode.com/topic/83460/java-recursive-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












