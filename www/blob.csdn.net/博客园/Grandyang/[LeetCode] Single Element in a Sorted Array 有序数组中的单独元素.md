# [LeetCode] Single Element in a Sorted Array 有序数组中的单独元素 - Grandyang - 博客园







# [[LeetCode] Single Element in a Sorted Array 有序数组中的单独元素](https://www.cnblogs.com/grandyang/p/7679036.html)








Given a sorted array consisting of only integers where every element appears twice except for one element which appears once. Find this single element that appears only once.

Example 1:
Input: [1,1,2,3,3,4,4,8,8]
Output: 2



Example 2:
Input: [3,3,7,7,10,11,11]
Output: 10



Note: Your solution should run in O(log n) time and O(1) space.



这道题给我们了一个有序数组，说是所有的元素都出现了两次，除了一个元素，让我们找到这个元素。如果没有时间复杂度的限制，我们可以用多种方法来做，最straightforward的解法就是用个双指针，每次检验两个，就能找出落单的。也可以像[Single Number](http://www.cnblogs.com/grandyang/p/4130577.html)里的方法那样，将所有数字亦或起来，相同的数字都会亦或成0，剩下就是那个落单的数字。那么由于有了时间复杂度的限制，需要为O(logn)，而数组又是有序的，不难想到要用二分搜索法来做。二分搜索法的难点在于折半了以后，如何判断将要去哪个分支继续搜索，而这道题确实判断条件不明显，比如下面两个例子：

1  1  2  2  3

1  2  2  3  3

这两个例子初始化的时候left=0, right=4一样，mid算出来也一样为2，但是他们要去的方向不同，如何区分出来呢？仔细观察我们可以发现，如果当前数字出现两次的话，我们可以通过数组的长度跟当前位置的关系，计算出右边和当前数字不同的数字的总个数，如果是偶数个，说明落单数左半边，反之则在右半边。有了这个规律就可以写代码了，为啥我们直接就能跟mid+1比呢，不怕越界吗？当然不会，因为left如何跟right相等，就不会进入循环，所以mid一定会比right小，一定会有mid+1存在。当然mid是有可能为0的，所以此时当mid和mid+1的数字不等时，我们直接返回mid的数字就可以了，参见代码如下：



解法一：

```
class Solution {
public:
    int singleNonDuplicate(vector<int>& nums) {
        int left = 0, right = nums.size() - 1, n = nums.size();
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == nums[mid + 1]) {
                if ((n - 1 - mid) % 2 == 1) right = mid;
                else left = mid + 1;
            } else {
                if (mid == 0 || nums[mid] != nums[mid - 1]) return nums[mid];
                if ((n - 1 - mid) % 2 == 0) right = mid;
                else left = mid + 1;
            }
        }
        return nums[left];
    }
};
```



下面这种解法是对上面的分支进行合并，使得代码非常的简洁。使用到了亦或1这个小技巧，为什么要亦或1呢，原来我们可以将坐标两两归为一对，比如0和1，2和3，4和5等等。而亦或1可以直接找到你的小伙伴，比如对于2，亦或1就是3，对于3，亦或1就是2。如果你和你的小伙伴相等了，说明落单数在右边，如果不等，说明在左边，这方法，太叼了有木有，参见代码如下：



解法二：

```
class Solution {
public:
    int singleNonDuplicate(vector<int>& nums) {
        int left = 0, right = nums.size() - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] == nums[mid ^ 1]) left = mid + 1;
            else right = mid;
        }
        return nums[left];
    }
};
```



下面这种解法其实跟上面的方法其实有些类似，虽然没有亦或1，但是将right缩小了一倍，但是在比较的时候，是比较mid*2和mid*2+1的关系的，这样还是能正确的比较原本应该相等的两个小伙伴的值的，其实核心思路和上面一样，参见代码如下：



解法三：

```
class Solution {
public:
    int singleNonDuplicate(vector<int>& nums) {
        int left = 0, right = nums.size() / 2;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid * 2] == nums[mid * 2 + 1]) left = mid + 1;
            else right = mid;
        }
        return nums[left * 2];
    }
};
```



下面这种方法其实跟解法二很像，没有用亦或1，但是对mid进行了处理，强制使其成为小伙伴对儿中的第一个位置，然后跟另一个小伙伴比较大小，参见代码如下：



解法四：

```
class Solution {
public:
    int singleNonDuplicate(vector<int>& nums) {
        int left = 0, right = nums.size() - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (mid % 2 == 1) --mid;
            if (nums[mid] == nums[mid + 1]) left = mid + 2;
            else right = mid;
        }
        return nums[left];
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/83310/short-compare-nums-i-with-nums-i-1](https://discuss.leetcode.com/topic/83310/short-compare-nums-i-with-nums-i-1)

[https://discuss.leetcode.com/topic/82235/java-code-by-using-binary-search-o-log-n](https://discuss.leetcode.com/topic/82235/java-code-by-using-binary-search-o-log-n)

[https://discuss.leetcode.com/topic/82332/java-binary-search-o-log-n-shorter-than-others](https://discuss.leetcode.com/topic/82332/java-binary-search-o-log-n-shorter-than-others)

[https://discuss.leetcode.com/topic/87424/java-binary-search-short-7l-o-log-n-w-explanations](https://discuss.leetcode.com/topic/87424/java-binary-search-short-7l-o-log-n-w-explanations)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












