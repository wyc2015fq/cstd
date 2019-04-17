# [LeetCode] Find the Duplicate Number 寻找重复数 - Grandyang - 博客园







# [[LeetCode] Find the Duplicate Number 寻找重复数](https://www.cnblogs.com/grandyang/p/4843654.html)







Given an array *nums* containing *n* + 1 integers where each integer is between 1 and *n* (inclusive), prove that at least one duplicate number must exist. Assume that there is only one duplicate number, find the duplicate one.

Example 1:
Input: `[1,3,4,2,2]`
Output: 2

Example 2:
Input: [3,1,3,4,2]
Output: 3
Note:
- You must not modify the array (assume the array is read only).
- You must use only constant, *O*(1) extra space.
- Your runtime complexity should be less than *O*(*n*2).
- There is only one duplicate number in the array, but it could be repeated more than once.



这道题给了我们n+1个数，所有的数都在[1, n]区域内，首先让我们证明必定会有一个重复数，这不禁让我想起了小学华罗庚奥数中的抽屉原理(又叫[鸽巢原理](https://zh.wikipedia.org/wiki/%E9%B4%BF%E5%B7%A2%E5%8E%9F%E7%90%86)), 即如果有十个苹果放到九个抽屉里，如果苹果全在抽屉里，则至少有一个抽屉里有两个苹果，这里就不证明了，直接来做题吧。题目要求我们不能改变原数组，即不能给原数组排序，又不能用多余空间，那么哈希表神马的也就不用考虑了，又说时间小于O(n2)，也就不能用brute force的方法，那我们也就只能考虑用二分搜索法了，我们在区间[1, n]中搜索，首先求出中点mid，然后遍历整个数组，统计所有小于等于mid的数的个数，如果个数小于等于mid，则说明重复值在[mid+1, n]之间，反之，重复值应在[1, mid-1]之间，然后依次类推，直到搜索完成，此时的low就是我们要求的重复值，参见代码如下：



解法一：

```
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int left = 0, right = nums.size();
        while (left < right){
            int mid = left + (right - left) / 2, cnt = 0;
            for (int num : nums) {
                if (num <= mid) ++cnt;
            }
            if (cnt <= mid) left = mid + 1;
            else right = mid;
        }    
        return right;
    }
};
```



经过热心网友[waruzhi](http://www.cnblogs.com/waruzhi/)的留言提醒还有一种O(n)的解法，并给了[参考帖子](http://bookshadow.com/weblog/2015/09/28/leetcode-find-duplicate-number/)，发现真是一种不错的解法，其核心思想快慢指针在之前的题目[Linked List Cycle II](http://www.cnblogs.com/grandyang/p/4137302.html)中就有应用，这里应用的更加巧妙一些，由于题目限定了区间[1,n]，所以可以巧妙的利用坐标和数值之间相互转换，而由于重复数字的存在，那么一定会形成环，我们用快慢指针可以找到环并确定环的起始位置，确实是太巧妙了！



解法二：

```
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int slow = 0, fast = 0, t = 0;
        while (true) {
            slow = nums[slow];
            fast = nums[nums[fast]];
            if (slow == fast) break;
        }
        while (true) {
            slow = nums[slow];
            t = nums[t];
            if (slow == t) break;
        }
        return slow;
    }
};
```



这道题还有一种位操作Bit Manipulation的解法，也十分的巧妙。思路是遍历每一位，然后对于32位中的每一个位bit，我们都遍历一遍从0到n-1，我们将0到n-1中的每一个数都跟bit相‘与’，若大于0，则计数器cnt1自增1。同时0到n-1也可以当作nums数组的下标，从而让nums数组中的每个数字也跟bit相‘与’，若大于0，则计数器cnt2自增1。最后比较若cnt2大于cnt1，则将bit加入结果res中。这是为啥呢，因为对于每一位，0到n-1中所有数字中该位上的1的个数应该是固定的，如果nums数组中所有数字中该位上1的个数多了，说明重复数字在该位上一定是1，这样我们把重复数字的所有为1的为都累加起来，就可以还原出了这个重复数字，参见代码如下：



解法三：

```
class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int res = 0, n = nums.size();
        for (int i = 0; i < 32; ++i) {
            int bit = (1 << i), cnt1 = 0, cnt2 = 0;
            for (int k = 0; k < n; ++k) {
                if ((k & bit) > 0) ++cnt1;
                if ((nums[k] & bit) > 0) ++cnt2;
            }
            if (cnt2 > cnt1) res += bit;
        }
        return res;
    }
};
```



类似题目：

[First Missing Positive](http://www.cnblogs.com/grandyang/p/4395963.html)

[Missing Number](http://www.cnblogs.com/grandyang/p/4756677.html)

[Single Number](http://www.cnblogs.com/grandyang/p/4130577.html)

[Find All Numbers Disappeared in an Array](http://www.cnblogs.com/grandyang/p/6222149.html)

[Set Mismatch](http://www.cnblogs.com/grandyang/p/7324242.html)

[Array Nesting](http://www.cnblogs.com/grandyang/p/6932727.html)

[Linked List Cycle II](http://www.cnblogs.com/grandyang/p/4137302.html)



参考资料：

[https://leetcode.com/discuss/60830/python-solution-explanation-without-changing-input-array](https://leetcode.com/discuss/60830/python-solution-explanation-without-changing-input-array)

[https://leetcode.com/problems/find-the-duplicate-number/discuss/72872/O(32*N)-solution-using-bit-manipulation-in-10-lines](https://leetcode.com/problems/find-the-duplicate-number/discuss/72872/O(32*N)-solution-using-bit-manipulation-in-10-lines)

[https://leetcode.com/problems/find-the-duplicate-number/discuss/73045/Simple-C%2B%2B-code-with-O(1)-space-and-O(nlogn)-time-complexity](https://leetcode.com/problems/find-the-duplicate-number/discuss/73045/Simple-C%2B%2B-code-with-O(1)-space-and-O(nlogn)-time-complexity)

[https://leetcode.com/problems/find-the-duplicate-number/discuss/72846/My-easy-understood-solution-with-O(n)-time-and-O(1)-space-without-modifying-the-array.-With-clear-explanation.](https://leetcode.com/problems/find-the-duplicate-number/discuss/72846/My-easy-understood-solution-with-O(n)-time-and-O(1)-space-without-modifying-the-array.-With-clear-explanation.)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












