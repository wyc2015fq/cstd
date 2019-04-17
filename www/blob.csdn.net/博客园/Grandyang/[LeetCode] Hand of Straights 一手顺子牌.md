# [LeetCode] Hand of Straights 一手顺子牌 - Grandyang - 博客园







# [[LeetCode] Hand of Straights 一手顺子牌](https://www.cnblogs.com/grandyang/p/10468523.html)







Alice has a `hand` of cards, given as an array of integers.

Now she wants to rearrange the cards into groups so that each group is size `W`, and consists of `W` consecutive cards.

Return `true` if and only if she can.



Example 1:
Input: hand = [1,2,3,6,2,3,4,7,8], W = 3
Output: true
Explanation: Alice's `hand` can be rearranged as `[1,2,3],[2,3,4],[6,7,8]`
Example 2:
Input: hand = [1,2,3,4,5], W = 4
Output: false
Explanation: Alice's `hand` can't be rearranged into groups of `4`
Note:
- `1 <= hand.length <= 10000`
- `0 <= hand[i] <= 10^9`
- `1 <= W <= hand.length`



这道题说是我们在打扑克牌，是否能将手里的牌都以顺子的形式出完。在我们打拐3挖坑或者斗地主的时候，顺子牌在后期的威力是蛮大的，某些人手里憋了一把牌，结果到后期找个机会上手了之后，直接一把甩完，看的手中只有一把单牌的博主是目瞪口呆。其实到了后期，大家手中都没啥牌了的时候，就算是很小的连牌，也不一定能要得起，而划单是最没前途的出法，所以我们要尽量将手中的牌都组成顺子丢出去。这里给了我们一个W，规定了顺子的最小长度，那么我们就拿例子1来模拟下打牌吧，首先摸到了牌之后，肯定要先整牌，按从小到大的顺序排列，这里我们就不考虑啥3最大，4最小啥的，就统一按原始数字排列吧：

1 2 2 3 3 4 6 7 8

好，下面要来组顺子，既然这里是3张可连，那么我们从最小的开始连呗。其实这道题还是简化了许多，真正打牌的时候，即便是3张起连，那么连4张5张都是可以的，可以这里限定了只能连W张，就使得题目变简单了。我们用贪婪算法就可以了，首先从1开始，那么一定得有2和3，才能起连，若少了任何一个，都可以直接返回false，好那么取出这三张后，手里还有：

2 3 4 6 7 8

那么从当前手里的最小的牌2开始起连，那么手里必须要有3和4，若少了任何一个，都可以直接返回false，好那么取出这三张后，手里还有：

6 7 8

那么从当前手里的最小的牌6开始起连，那么手里必须要有7和8，若少了任何一个，都可以直接返回false，好那么取出这三张后，手里没牌了，我们成功的连完了所有的牌。分析这个过程，我们不难发现，由于牌可以重复，所以我们要统计每张牌出现的次数，同时还要给牌按大小排序，那么用TreeMap来建立牌的大小和其出现次数之间的映射就最好不过了，利用了其可以按key值排序的特点。首先遍历手中牌，建立映射。然后开始while循环，条件是TreeMap不为空，然后去出最小的那张牌，然后遍历能组成顺子的W张牌，若没有直接返回true，有的话，则映射值自减1，若映射值为0了，则从TreeMap中移除该映射对儿即可，while循环退出后返回true，参见代码如下：



解法一：

```
class Solution {
public:
    bool isNStraightHand(vector<int>& hand, int W) {
        map<int, int> m;
        for (int i : hand) ++m[i];
        while (!m.empty()) {
            int start = m.begin()->first;
            for (int i = 0; i < W; ++i) {
                if (!m.count(start + i)) return false;
                if (--m[start + i] == 0) m.erase(start + i);
            }
        }
        return true;
    }
};
```



我们也可以不对TreeMap进行删除操作，而是直接修改其映射值，在建立好映射对儿之后，不用while循环，而是遍历所有的映射对儿，若某个映射值为0了，直接跳过。然后还是遍历能组成顺子的W张牌，若某张牌出现的次数小于顺子其实位置的牌的个数，则直接返回false，因为肯定会有起始牌剩余，无法全组成顺子，这样还避免了上面解法中一张一张减的操作，提高了运算效率。然后我们的映射值减去起始牌的个数，最后for循环退出后，返回true，参见代码如下：



解法二：

```
class Solution {
public:
    bool isNStraightHand(vector<int>& hand, int W) {
        map<int, int> m;
        for (int i : hand) ++m[i];
        for (auto a : m) {
            if (a.second == 0) continue;
            for (int i = a.first; i < a.first + W; ++i) {
                if (m[i] < a.second) return false;
                m[i] = m[i] - a.second;
            }
        }
        return true;
    }
};
```



参考资料：

[https://leetcode.com/problems/hand-of-straights/](https://leetcode.com/problems/hand-of-straights/submissions/)

[https://leetcode.com/problems/hand-of-straights/discuss/135700/Short-Java-solution!](https://leetcode.com/problems/hand-of-straights/discuss/135700/Short-Java-solution!)

[https://leetcode.com/problems/hand-of-straights/discuss/135598/C%2B%2BJavaPython-O(MlogM)-Complexity](https://leetcode.com/problems/hand-of-straights/discuss/135598/C%2B%2BJavaPython-O(MlogM)-Complexity)



[LeetCode All in One 题目讲解汇总(持续更新中...)](https://www.cnblogs.com/grandyang/p/4606334.html)












