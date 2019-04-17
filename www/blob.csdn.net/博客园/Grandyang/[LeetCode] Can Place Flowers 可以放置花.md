# [LeetCode] Can Place Flowers 可以放置花 - Grandyang - 博客园







# [[LeetCode] Can Place Flowers 可以放置花](https://www.cnblogs.com/grandyang/p/6983982.html)







Suppose you have a long flowerbed in which some of the plots are planted and some are not. However, flowers cannot be planted in adjacent plots - they would compete for water and both would die.

Given a flowerbed (represented as an array containing 0 and 1, where 0 means empty and 1 means not empty), and a number n, return if n new flowers can be planted in it without violating the no-adjacent-flowers rule.

Example 1:
Input: flowerbed = [1,0,0,0,1], n = 1
Output: True



Example 2:
Input: flowerbed = [1,0,0,0,1], n = 2
Output: False



Note:
- The input array won't violate no-adjacent-flowers rule.
- The input array size is in the range of [1, 20000].
- n is a non-negative integer which won't exceed the input array size.



这道题给了我们一个01数组，其中1表示已经放了花，0表示可以放花的位置，但是有个限制条件是不能有相邻的花。那么我们来看如果是一些简单的例子，如果有3个连续的零，000，能放几盆花呢，其实是要取决约左右的位置的，如果是10001，那么只能放1盆，如果左右是边界的花，那么就能放两盆，101，所以如果我们想通过计算连续0的个数，然后直接算出能放花的个数，就必须要对边界进行处理，处理方法是如果首位置是0，那么前面再加上个0，如果末位置是0，就在最后面再加上个0。这样处理之后我们就默认连续0的左右两边都是1了，这样如果有k个连续0，那么就可以通过(k-1)/2来快速计算出能放的花的数量，参见代码如下：



解法一：

```
class Solution {
public:
    bool canPlaceFlowers(vector<int>& flowerbed, int n) {
        if (flowerbed.empty()) return false;
        if (flowerbed[0] == 0) flowerbed.insert(flowerbed.begin(), 0);
        if (flowerbed.back() == 0) flowerbed.push_back(0);
        int len = flowerbed.size(), cnt = 0, sum = 0;
        for (int i = 0; i <= len; ++i) {
            if (i < len && flowerbed[i] == 0) ++cnt;
            else {
                sum += (cnt - 1) / 2;
                cnt = 0;
            }
        }
        return sum >= n;
    }
};
```



我们也可以直接通过修改flowerbed的值来做，我们遍历花床，如果某个位置为0，我们就看其前面一个和后面一个位置的值，注意处理首位置和末位置的情况，如果pre和next均为0，那么说明当前位置可以放花，我们修改flowerbed的值，并且n自减1，最后看n是否小于等于0，参见代码如下：



解法二：

```
class Solution {
public:
    bool canPlaceFlowers(vector<int>& flowerbed, int n) {
        for (int i = 0; i < flowerbed.size(); ++i) {
            if (n == 0) return true;
            if (flowerbed[i] == 0) {
                int next = (i == flowerbed.size() - 1 ? 0 : flowerbed[i + 1]);
                int pre = (i == 0 ? 0 : flowerbed[i - 1]);
                if (next + pre == 0) {
                    flowerbed[i] = 1;
                    --n;
                }
            }
        }
        return n <= 0;
    }
};
```



下面这种方法跟上面的方法类似，为了不特殊处理首末位置，直接先在首尾各加了一个0，然后就三个三个的来遍历，如果找到了三个连续的0，那么n自减1，i自增1，这样相当于i一下向后跨了两步，可以自行带例子检验，最后还是看n是否小于等于0，参见代码如下：



解法三：

```
class Solution {
public:
    bool canPlaceFlowers(vector<int>& flowerbed, int n) {
        flowerbed.insert(flowerbed.begin(), 0);
        flowerbed.push_back(0);
        for (int i = 1; i < flowerbed.size() - 1; ++i) {
            if (n == 0) return true;
            if (flowerbed[i - 1] + flowerbed[i] + flowerbed[i + 1] == 0) {
                --n;
                ++i;
            }
        }
        return n <= 0;
    }
};
```



类似题目：

[Teemo Attacking](http://www.cnblogs.com/grandyang/p/6399408.html)



参考资料：

[https://discuss.leetcode.com/topic/91376/simplest-c-code](https://discuss.leetcode.com/topic/91376/simplest-c-code)

[https://discuss.leetcode.com/topic/91303/java-greedy-solution-o-flowerbed-beats-100](https://discuss.leetcode.com/topic/91303/java-greedy-solution-o-flowerbed-beats-100)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












