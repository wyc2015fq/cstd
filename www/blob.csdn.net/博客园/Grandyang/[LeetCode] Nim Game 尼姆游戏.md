# [LeetCode] Nim Game 尼姆游戏 - Grandyang - 博客园







# [[LeetCode] Nim Game 尼姆游戏](https://www.cnblogs.com/grandyang/p/4873248.html)







You are playing the following Nim Game with your friend: There is a heap of stones on the table, each time one of you take turns to remove 1 to 3 stones. The one who removes the last stone will be the winner. You will take the first turn to remove the stones.

Both of you are very clever and have optimal strategies for the game. Write a function to determine whether you can win the game given the number of stones in the heap.

For example, if there are 4 stones in the heap, then you will never win the game: no matter 1, 2, or 3 stones you remove, the last stone will always be removed by your friend.

**Hint:**
- If there are 5 stones in the heap, could you figure out a way to remove the stones such that you will always be the winner?

**Credits:**
Special thanks to [@jianchao.li.fighter](https://leetcode.com/discuss/user/jianchao.li.fighter) for adding this problem and creating all test cases.



有史以来最少代码量的解法，虽然解法很简单，但是题目还是蛮有意思的，题目说给我们一堆石子，每次可以拿一个两个或三个，两个人轮流拿，拿到最后一个石子的人获胜，现在给我们一堆石子的个数，问我们能不能赢。那么我们就从最开始分析，由于是我们先拿，那么3个以内(包括3个)的石子，我们直接赢，如果共4个，那么我们一定输，因为不管我们取几个，下一个人一次都能取完。如果共5个，我们赢，因为我们可以取一个，然后变成4个让别人取，根据上面的分析我们赢，所以我们列出1到10个的情况如下：

1    Win

2    Win

3    Win

4    Lost

5    Win

6    Win

7    Win

8    Lost

9    Win

10   Win

由此我们可以发现规律，只要是4的倍数个，我们一定会输，所以对4取余即可，参见代码如下：



```
class Solution {
public:
    bool canWinNim(int n) {
        return n % 4;
    }
};
```



讨论：我们来generalize一下这道题，当可以拿1～n个石子时，那么个数为(n+1)的整数倍时一定会输，我们试着证明一下这个结论，若当前共有m*(n+1)个石子，那么：

当m=1时，即剩n+1个的时候，肯定会输，因为不管你取1～n中的任何一个数字，另一个人都可以取完。

当m>1时，即有m*(n+1)的时候，不管你先取1～n中的任何一个数字x，另外一个人一定会取n+1-x个，这样总数就变成了(m-1)*(n+1)，第二个人就一直按这个策略取，那么直到剩n+1个的时候，就便变成m=1的情况，一定会输。



类似题目：

[Flip Game II](http://www.cnblogs.com/grandyang/p/5226206.html)



参考资料：

[https://leetcode.com/problems/nim-game/solution/](https://leetcode.com/problems/nim-game/solution/)

[https://leetcode.com/problems/nim-game/discuss/73749/Theorem:-all-4s-shall-be-false](https://leetcode.com/problems/nim-game/discuss/73749/Theorem:-all-4s-shall-be-false)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












