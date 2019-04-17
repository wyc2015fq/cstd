# [LeetCode] Maximize Distance to Closest Person 离最近的人的最大距离 - Grandyang - 博客园







# [[LeetCode] Maximize Distance to Closest Person 离最近的人的最大距离](https://www.cnblogs.com/grandyang/p/10503789.html)








In a row of `seats`, `1` represents a person sitting in that seat, and `0` represents that the seat is empty. 

There is at least one empty seat, and at least one person sitting.

Alex wants to sit in the seat such that the distance between him and the closest person to him is maximized. 

Return that maximum distance to closest person.

Example 1:

```
Input: [1,0,0,0,1,0,1]
Output: 2
Explanation:
If Alex sits in the second open seat (seats[2]), then the closest person has distance 2.
If Alex sits in any other open seat, the closest person has distance 1.
Thus, the maximum distance to the closest person is 2.
```

Example 2:

```
Input: [1,0,0,0]
Output: 3
Explanation:
If Alex sits in the last seat, the closest person is 3 seats away.
This is the maximum distance possible, so the answer is 3.
```

Note:
- `1 <= seats.length <= 20000`
- `seats` contains only 0s or 1s, at least one `0`, and at least one `1`.




这道题给了我们一个只有0和1且长度为n的数组，代表n个座位，其中0表示空座位，1表示有人座。现在说是爱丽丝想找个位置坐下，但是希望能离最近的人越远越好，这个不难理解，就是想左右两边尽量跟人保持距离，让我们求这个距离最近的人的最大距离。来看题目中的例子1，有三个空位连在一起，那么爱丽丝肯定是坐在中间的位置比较好，这样跟左右两边人的距离都是2。例子2有些特别，当空位连到了末尾的时候，这里我们可以想像成靠墙，那么靠墙坐肯定离最远啦，所以例子2中爱丽丝坐在最右边的位子上距离左边的人距离最远为3。那么我们不难发现，爱丽丝肯定需要先找出最大的连续空位长度，若连续空位靠着墙了，那么就直接挨着墙坐，若两边都有人，那么就坐到空位的中间位置。如何能快速知道连续空位的长度呢，只要知道了两边人的位置，相减就是中间连续空位的个数。所以博主最先使用的方法是用一个数组来保存所有1的位置，即有人坐的位置，然后用相邻的两个位置相减，就可以得到连续空位的长度。当然，靠墙这种特殊情况要另外处理一下。当把所有1位置存入数组nums之后，开始遍历nums数组，第一个人的位置有可能不靠墙，那么他的位置坐标就是他左边靠墙的连续空位个数，直接更新结果res即可，因为靠墙连续空位的个数就是离右边人的最远距离。然后对于其他的位置，我们减去前一个人的位置坐标，然后除以2，更新结果res。还有最右边靠墙的情况也要处理一下，就用 n-1 减去最后一个人的位置坐标，然后更新结果res即可，参见代码如下：




解法一：

```
class Solution {
public:
    int maxDistToClosest(vector<int>& seats) {
        int n = seats.size(), res = 0;
        vector<int> nums;
        for (int i = 0; i < n; ++i) {
            if (seats[i] == 1) nums.push_back(i);
        }
        for (int i = 0; i < nums.size(); ++i) {
            if (i == 0) res = max(res, nums[0]);
            else res = max(res, (nums[i] - nums[i - 1]) / 2);
        }
        if (!nums.empty()) res = max(res, n - 1 - nums.back());
        return res;
    }
};
```




我们也可以只用一次遍历，那么就需要在遍历的过程中统计出连续空位的个数，即连续0的个数。那么我们采用双指针来做，start 指向连续0的起点，初始化为0，i为当前遍历到的位置。遍历 seats 数组，跳过0的位置，当遇到1的时候，此时我们先判断下 start 的值，若是0的话，表明当前这段连续的空位是靠着墙的，所以我们要用连续空位的长度 i-start 来直接更新结果res，否则的话就是两头有人的中间的空位，那么用长度加1除以2来更新结果res，此时 start 要更新为 i+1，指向下一段连续空位的起始位置。for循环退出后，还是要处理最右边靠墙的位置，用 n-start 来更新结果res即可，参见代码如下：




解法二：

```
class Solution {
public:
    int maxDistToClosest(vector<int>& seats) {
        int n = seats.size(), start = 0, res = 0;
        for (int i = 0; i < n; ++i) {
            if (seats[i] != 1) continue;
            if (start == 0) res = max(res, i - start);
            else res = max(res, (i - start + 1) / 2);
            start = i + 1;
        }
        res = max(res, n - start);
        return res;
    }
};
```




讨论：这道题的一个很好的follow up是让我们返回爱丽丝坐下的位置，那么我们要在结果res可以被更新的时候，同时还应该记录下连续空位的起始位置start，这样有了 start 和 最大距离 res，那么我们就可以定位出爱丽丝的座位了。




类似题目：

[Exam Room](https://www.cnblogs.com/grandyang/p/10618437.html)




参考资料：

[https://leetcode.com/problems/maximize-distance-to-closest-person/](https://leetcode.com/problems/maximize-distance-to-closest-person/)

[https://leetcode.com/problems/maximize-distance-to-closest-person/discuss/137912/C%2B%2BJava-1-Pass-Solution](https://leetcode.com/problems/maximize-distance-to-closest-person/discuss/137912/C%2B%2BJava-1-Pass-Solution)




[LeetCode All in One 题目讲解汇总(持续更新中...)](https://www.cnblogs.com/grandyang/p/4606334.html)












