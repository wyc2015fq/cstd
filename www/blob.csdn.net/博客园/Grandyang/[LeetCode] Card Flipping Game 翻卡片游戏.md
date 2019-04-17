# [LeetCode] Card Flipping Game 翻卡片游戏 - Grandyang - 博客园







# [[LeetCode] Card Flipping Game 翻卡片游戏](https://www.cnblogs.com/grandyang/p/10163255.html)









On a table are `N` cards, with a positive integer printed on the front and back of each card (possibly different).

We flip any number of cards, and after we choose one card. 

If the number `X` on the back of the chosen card is not on the front of any card, then this number X is good.

What is the smallest number that is good?  If no number is good, output `0`.

Here, `fronts[i]` and `backs[i]` represent the number on the front and back of card `i`. 

A flip swaps the front and back numbers, so the value on the front is now on the back and vice versa.

Example:
Input: fronts = [1,2,4,4,7], backs = [1,3,4,1,3]
Output: `2`
Explanation: If we flip the second card, the fronts are `[1,3,4,4,7]` and the backs are `[1,2,4,1,3]`.
We choose the second card, which has number 2 on the back, and it isn't on the front of any card, so `2` is good.


Note:
- `1 <= fronts.length == backs.length <= 1000`.
- `1 <= fronts[i] <= 2000`.
- `1 <= backs[i] <= 2000`.





这道题刚开始的时候博主一直没看懂题意，不知所云，后来逛了论坛才总算弄懂了题意，说是给了一些正反都有正数的卡片，可以翻面，让我们找到一个最小的数字，在卡的背面，且要求其他卡正面上均没有这个数字。简而言之，就是要在backs数组找一个最小数字，使其不在fronts数组中。我们想，既然不能在fronts数组中，说明卡片背面的数字肯定跟其正面的数字不相同，否则翻来翻去都是相同的数字，肯定会在fronts数组中。那么我们可以先把正反数字相同的卡片都找出来，将数字放入一个HashSet，也方便我们后面的快速查找。现在其实我们只需要在其他的数字中找到一个最小值即可，因为正反数字不同，就算fronts中其他卡片的正面还有这个最小值，我们可以将那张卡片翻面，使得相同的数字到backs数组，总能使得fronts数组不包含有这个最小值，就像题目中给的例子一样，数字2在第二张卡的背面，就算其他卡面也有数字2，只要其不是正反都是2，我们都可以将2翻到背面去，参见代码如下：



```
class Solution {
public:
    int flipgame(vector<int>& fronts, vector<int>& backs) {
        int res = INT_MAX, n = fronts.size();
        unordered_set<int> same;
        for (int i = 0; i < n; ++i) {
            if (fronts[i] == backs[i]) same.insert(fronts[i]);
        }
        for (int front : fronts) {
            if (!same.count(front)) res = min(res, front);
        }
        for (int back : backs) {
            if (!same.count(back)) res = min(res, back);
        }
        return (res == INT_MAX) ? 0 : res;
    }
};
```



参考资料：

[https://leetcode.com/problems/card-flipping-game/](https://leetcode.com/problems/card-flipping-game/)

[https://leetcode.com/problems/card-flipping-game/discuss/125791/C%2B%2BJavaPython-Easy-and-Concise-with-Explanation](https://leetcode.com/problems/card-flipping-game/discuss/125791/C%2B%2BJavaPython-Easy-and-Concise-with-Explanation)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













