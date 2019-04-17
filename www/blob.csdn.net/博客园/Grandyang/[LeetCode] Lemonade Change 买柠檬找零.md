# [LeetCode] Lemonade Change 买柠檬找零 - Grandyang - 博客园







# [[LeetCode] Lemonade Change 买柠檬找零](https://www.cnblogs.com/grandyang/p/10663469.html)








At a lemonade stand, each lemonade costs `$5`. 

Customers are standing in a queue to buy from you, and order one at a time (in the order specified by `bills`).

Each customer will only buy one lemonade and pay with either a `$5`, `$10`, or `$20` bill.  You must provide the correct change to each customer, so that the net transaction is that the customer pays $5.

Note that you don't have any change in hand at first.

Return `true` if and only if you can provide every customer with correct change.

Example 1:

```
Input: [5,5,5,10,20]
Output: true
Explanation:
From the first 3 customers, we collect three $5 bills in order.
From the fourth customer, we collect a $10 bill and give back a $5.
From the fifth customer, we give a $10 bill and a $5 bill.
Since all customers got correct change, we output true.
```

Example 2:

```
Input: [5,5,10]
Output: true
```

Example 3:

```
Input: [10,10]
Output: false
```

Example 4:

```
Input: [5,5,10,10,20]
Output: false
Explanation:
From the first two customers in order, we collect two $5 bills.
For the next two customers in order, we collect a $10 bill and give back a $5 bill.
For the last customer, we can't give change of $15 back because we only have two $10 bills.
Since not every customer received correct change, the answer is false.
```

Note:
- `0 <= bills.length <= 10000`
- `bills[i]` will be either `5`, `10`, or `20`.




这道题说是有很多柠檬，每个卖5刀，顾客可能会提供5刀，10刀，15刀的钞票，我们刚开始的时候并没有零钱，只有收到顾客的5刀，或者10刀可以用来给顾客找钱，当然如果第一个顾客就给10刀或者20刀，那么我们是无法找零的，这里就问我们最终是否能够都成功找零。博主最先想到的方法是首先用一个 HashMap 来分别统计出5刀，10刀，和20刀钞票的个数，然后再来统一分析是否能成功找零。由于10刀的钞票需要5刀的找零，20刀的钞票可以用1张10刀和1张5刀，或者3张5刀的钞票，所以至少需要1张5刀，那么当前5刀的个数一定不能小于10刀和20刀的个数之后，否则无法成功找零。由于20刀可以用10刀来找零，每个10刀可以节省两个5刀，但是为了获得每张10刀，我们还得付出一张5刀的找零，所以实际上用10刀来找零只能省下1张5刀钞票，但是假如10刀的个数不够，那么每张20刀的钞票还是需要3张5刀的钞票来找零的，所以我们判断若5刀钞票的个数小于20刀钞票个数的三倍减去10刀钞票的个数，直接返回 false。for 循环退出后返回 true，参见代码如下：




解法一：

```
class Solution {
public:
    bool lemonadeChange(vector<int>& bills) {
        unordered_map<int, int> cnt;
        for (int bill : bills) {
            ++cnt[bill];
            if (cnt[5] < cnt[20] + cnt[10]) return false;
            if (cnt[5] < 3 * cnt[20] - cnt[10]) return false;
        }
        return true;
    }
};
```




实际上我们并不需要一直保留所有钞票的个数，当某些钞票被当作零钱给了，就没有必要继续留着它们的个数了。其实上我们只关心当前还剩余的5刀和10刀钞票的个数，用两个变量 five 和 ten 来记录。然后遍历所有的钞票，假如遇到5刀钞票，则 five 自增1，若遇到10刀钞票，则需要找零5刀，则 five 自减1，ten 自增1。否则遇到的就是20刀的了，若我们还有10刀的钞票话，就先用10刀找零，则 ten 自减1，再用一张5刀找零，five 自减1。若没有10刀了，则用三张5刀找零，five 自减3。找零了后检测若此时5刀钞票个数为负数了，则直接返回 false，参见代码如下：




解法二：

```
class Solution {
public:
    bool lemonadeChange(vector<int>& bills) {
        int five = 0, ten = 0;
        for (int bill : bills) {
            if (bill == 5) ++five;
            else if (bill == 10) { --five; ++ten; }
            else if (ten > 0) { --ten; --five; }
            else five -= 3;
            if (five < 0) return false;
        }
        return true;
    }
};
```




参考资料：

[https://leetcode.com/problems/lemonade-change/](https://leetcode.com/problems/lemonade-change/)

[https://leetcode.com/problems/lemonade-change/discuss/143719/C%2B%2BJavaPython-Straight-Forward](https://leetcode.com/problems/lemonade-change/discuss/143719/C%2B%2BJavaPython-Straight-Forward)



[LeetCode All in One 题目讲解汇总(持续更新中...)](https://www.cnblogs.com/grandyang/p/4606334.html)












