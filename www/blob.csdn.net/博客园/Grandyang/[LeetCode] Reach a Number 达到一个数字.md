# [LeetCode] Reach a Number 达到一个数字 - Grandyang - 博客园







# [[LeetCode] Reach a Number 达到一个数字](https://www.cnblogs.com/grandyang/p/8456022.html)







You are standing at position `0` on an infinite number line. There is a goal at position `target`.

On each move, you can either go left or right. During the *n*-th move (starting from 1), you take *n* steps.

Return the minimum number of steps required to reach the destination.

Example 1:
Input: target = 3
Output: 2
Explanation:
On the first move we step from 0 to 1.
On the second step we step from 1 to 3.



Example 2:
Input: target = 2
Output: 3
Explanation:
On the first move we step from 0 to 1.
On the second move we step  from 1 to -1.
On the third move we step from -1 to 2.



Note:
- `target` will be a non-zero integer in the range `[-10^9, 10^9]`.



这道题让我们从起点0开始，每次可以向数轴的左右两个方向中的任意一个走，第一步走距离1，第二步走距离2，以此类推，第n步走距离n，然后给了我们一个目标值target，问我们最少用多少步可以到达这个值。博主分析了给的两个例子后，开始想的是用贪婪算法来做，就是如果加上距离大于目标值的话，就减去这个距离，到是当目标值是4的话，贪婪算法会fail。一般贪婪算法不行的话，很自然的博主就会想想能否用DP来做，但这道题感觉很难很难重现为子问题，因为每一步走的步数都不一样，这个步数又跟最小步数息息相关，所以很难写出状态转移方程啊，只得放弃。后来博主尝试用BFS来做，就是每次都把当前能到大的所有的点，都加上和减去当前距离，形成新的位置，加入数组中，当某个新的位置达到目标值时返回，但是这种解法会TLE，当目标值很大的话，相当的不高效

其实这道题的正确解法用到了些数学知识，还有一些小trick，首先来说说小trick，第一个trick是到达target和-target的步数相同，因为数轴是对称的，只要将到达target的没步的距离都取反，就能到达-target。下面来说第二个trick，这个是解题的关键，比如说目标值是4，那么如果我们一直累加步数，直到其正好大于等于target时，有：

0 + 1 = 1

1 + 2 = 3

3 + 3 = 6

第三步加上3，得到了6，超过了目标值4，超过了的距离为2，是偶数，那么实际上我们只要将加上距离为1的时候，不加1，而是加-1，那么此时累加和就损失了2，那么正好能到目标值4，如下：

0 - 1 = -1

-1 + 2 = 1

1 + 3 = 4

那么，我们的第二个trick就是，当超过目标值的差值d为偶数时，只要将第d/2步的距离取反，就能得到目标值，此时的步数即为到达目标值的步数。那么，如果d为奇数时，且当前为第n步，那么我们看下一步n+1的奇偶，如果n+1为奇数，那么加上n+1再做差，得到的差值就为偶数了，问题解决，如果n+1为偶数，那么还得加上n+2这个奇数，才能让差值为偶数，这样就多加了两步。分析到这里，我们的解题思路也就明晰了吧：

我们先对target取绝对值，因为正负不影响最小步数。然后我们求出第n步，使得从1累加到n刚好大于等于target，我们可以反向利用求和公式，来求解出n，然后算出当前n的累加和sum，如果此时sum和target正好相等，perfect！直接返回n，否则就是计算差值，如果差值时偶数，那么也直接返回n，如果是奇数，判断此时n的奇偶，如果n是奇数，则返回n+2，若n是偶数，返回n+1，参见代码如下：



解法一：

```
class Solution {
public:
    int reachNumber(int target) {
        target = abs(target);
        long n = ceil((-1.0 + sqrt(1 + 8.0 * target)) / 2);
        long sum = n * (n + 1) / 2;
        if (sum == target) return n;
        long res = sum - target;
        if ((res & 1) == 0) return n;
        else return n + ((n & 1) ? 2 : 1);
    }
};
```



我们也可以不用求和公式来快速定位n，而是通过累加来做，res为我们的当前步数，也是最终需要返回的结果，sum是加上每步距离的累加值，如果sum小于target，或者sum减去target的差值为奇数，我们进行循环，步数res自增1，然后sum加上步数res，最后跳出循环的条件就是差值为偶数，也符合我们上的分析，参见代码如下：



解法二：

```
class Solution {
public:
    int reachNumber(int target) {
        target = abs(target);
        int res = 0, sum = 0;
        while (sum < target || (sum - target) % 2 == 1) {
            ++res;
            sum += res;
        }
        return res;
    }
};
```



下面这种解法是解法一的精简版，两行搞定碉堡了！



解法三：

```
class Solution {
public:
    int reachNumber(int target) {
        int n = ceil((sqrt(1 + 8.0 * abs(target)) - 1) / 2), d = n * (n + 1) / 2 - target;
        return n + (d % 2) * (n % 2 + 1);
    }
};
```



参考资料：

[https://leetcode.com/problems/reach-a-number/](https://leetcode.com/problems/reach-a-number/)

[https://leetcode.com/problems/reach-a-number/discuss/112969/C++-O(1)-Solution-without-loop](https://leetcode.com/problems/reach-a-number/discuss/112969/C++-O(1)-Solution-without-loop)

[https://leetcode.com/problems/reach-a-number/discuss/112992/Learn-from-other-with-my-explanations](https://leetcode.com/problems/reach-a-number/discuss/112992/Learn-from-other-with-my-explanations)

[https://leetcode.com/problems/reach-a-number/discuss/112982/2-liner-a-math-problem-with-explanation](https://leetcode.com/problems/reach-a-number/discuss/112982/2-liner-a-math-problem-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












