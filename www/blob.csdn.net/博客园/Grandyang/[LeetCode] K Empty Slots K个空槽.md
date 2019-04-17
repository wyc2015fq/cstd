# [LeetCode] K Empty Slots K个空槽 - Grandyang - 博客园







# [[LeetCode] K Empty Slots K个空槽](https://www.cnblogs.com/grandyang/p/8415880.html)







There is a garden with `N` slots. In each slot, there is a flower. The `N` flowers will bloom one by one in `N` days. In each day, there will be `exactly` one flower blooming and it will be in the status of blooming since then.

Given an array `flowers` consists of number from `1` to `N`. Each number in the array represents the place where the flower will open in that day.

For example, `flowers[i] = x` means that the unique flower that blooms at day `i` will be at position `x`, where `i` and `x` will be in the range from `1` to `N`.

Also given an integer `k`, you need to output in which day there exists two flowers in the status of blooming, and also the number of flowers between them is `k` and these flowers are not blooming.

If there isn't such day, output -1.

Example 1:
Input: 
flowers: [1,3,2]
k: 1
Output: 2
Explanation: In the second day, the first and the third flower have become blooming.



Example 2:
Input: 
flowers: [1,2,3]
k: 1
Output: -1



Note:
- The given array will be in the range [1, 20000].



这道题给了我们这样一个场景，说是花园里有N个空槽，可以放花，每天放一朵开着的花，而且一旦放了就会一直开下去。不是按顺序放花，而是给了我们一个数组flowers，其中flowers[i] = x表示第i天放的花会在位置x。其实题目这里有误，数组是从0开始的，而天数和位置都是从1开始的，所以正确的应该是第i+1天放的花会在位置x。然后给了我们一个整数k，让我们判断是否正好有两朵盛开的花中间有k个空槽，如果有，返回当前天数，否则返回-1。博主刚开始想的是先用暴力破解来做，用一个状态数组，如果该位置有花为1，无花为0，然后每增加一朵花，就遍历一下状态数组，找有没有连续k个0，结果TLE了。这说明，应该等所有花都放好了，再来找才行，但是这样仅用0和1的状态数组是不行的，我们得换个形式。

我们用一个days数组，其中days[i] = t表示在i+1位置上会在第t天放上花，那么如果days数组为[1 3 2]，就表示第一个位置会在第一天放上花，第二个位置在第三天放上花，第三个位置在第二天放上花。我们想，在之前的状态数组中，0表示没放花，1表示放了花，而days数组中的数字表示放花的天数，那么就是说数字大的就是花放的时间晚，那么在当前时间i，所有大于i的是不是也就是可以看作是没放花呢，这样问题就迎刃而解了，我们来找一个k+2大小的子数组，除了首尾两个数字，中间的k个数字都要大于首尾两个数字即可，那么首尾两个数字中较大的数就是当前的天数。left和right是这个大小为k+2的窗口，初始化时left为0，right为k+1，然后i从0开始遍历，这里循环的条件时right小于n，当窗口的右边界越界后，循环自然需要停止。如果当days[i]小于days[left]，或者days[i]小于等于days[right]的时候，有两种情况，一种是i在[left, right]范围内，说明窗口中有数字小于边界数字，这不满足我们之前限定的条件，至于days[i]为何可以等于days[right]，是因为当i遍历到right到位置时，说明中间的数字都是大于左右边界数的，此时我们要用左右边界中较大的那个数字更新结果res。不管i是否等于right，只要进了这个if条件，说明当前窗口要么是不合题意，要么是遍历完了，我们此时要重新给left和right赋值，其中left赋值为i，right赋值为k+1+i，还是大小为k+2的窗口，继续检测。最后我们看结果res，如果还是INT_MAX，说明无法找到，返回-1即可，参见代码如下：



解法一：

```
class Solution {
public:
    int kEmptySlots(vector<int>& flowers, int k) {
        int res = INT_MAX, left = 0, right = k + 1, n = flowers.size();
        vector<int> days(n, 0);
        for (int i = 0; i < n; ++i) days[flowers[i] - 1] = i + 1;
        for (int i = 0; right < n; ++i) {
            if (days[i] < days[left] || days[i] <= days[right]) {
                if (i == right) res = min(res, max(days[left], days[right]));
                left = i; 
                right = k + 1 + i;
            }
        }
        return (res == INT_MAX) ? -1 : res;
    }
};
```



下面这种方法用到了TreeSet来做，利用其自动排序的特点，然后用lower_bound和upper_bound进行快速的二分查找。 题目中的flowers[i] = x表示第i+1天放的花会在位置x。所以我们遍历flowers数组，其实就是按照时间顺序进行的，我们取出当前需要放置的位置cur，然后在集合set中查找第一个大于cur的数字，如果存在的话，说明两者中间点位置都没有放花，而如果中间正好有k个空位的话，那么当前天数就即为所求。这是当cur为左边界的情况，同样，我们可以把cur当右边界来检测，在集合set中查找第一个小于cur的数字，如果二者中间有k个空位，也返回当前天数。需要注意的是，C++和Java中的upper_bound和higher是相同作用的，但是lower_bound和lower却不太一样。C++中的lower_bound找的是第一个不小于目标值的数字，所以可能会返回和目标值相同或者大于目标值的数字。只要这个数字不是第一个数字，然后我们往前退一位，就是要求的第一个小于目标值的数字，这相当于Java中的lower函数，参见代码如下：



解法二：

```
class Solution {
public:
    int kEmptySlots(vector<int>& flowers, int k) {
        set<int> s;
        for (int i = 0; i < flowers.size(); ++i) {
            int cur = flowers[i];
            auto it = s.upper_bound(cur);
            if (it != s.end() && *it - cur == k + 1) {
                return i + 1;
            }
            it = s.lower_bound(cur);
            if (it != s.begin() && cur - *(--it) == k + 1) {
                return i + 1;
            }
            s.insert(cur);
        }
        return -1;
    }
};
```



讨论：这道题有一个很好的follow up，就是改为最后的有k盆连续开花的是哪一天，就是k个连续不空的槽，博主没有想出特别好的解法，只能采用暴力搜索的解法。比如就像解法一，求出了days数组后。我们可以遍历每个长度为k的子数组，然后找出该子数组中最大的数字，然后找出所有的这些最大数字中的最小的一个，就是所求。或者，我们可以使用类似于合并区间的思想，遍历flowers数组，每遍历一个数字，如果跟现有的区间连续，就加入当前区间，直到出现某个区间的长度大于等于k了，则当前天数即为所求。如果各位看官大神们有更好的解法，一定要留言告知博主哈～



参考资料：

[https://discuss.leetcode.com/topic/104738/java-accepted-solution](https://discuss.leetcode.com/topic/104738/java-accepted-solution)

[https://discuss.leetcode.com/topic/104771/java-c-simple-o-n-solution](https://discuss.leetcode.com/topic/104771/java-c-simple-o-n-solution)

[https://discuss.leetcode.com/topic/106088/iterate-over-time-vs-iterate-over-position](https://discuss.leetcode.com/topic/106088/iterate-over-time-vs-iterate-over-position)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












