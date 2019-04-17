# [CareerCup] 9.3 Magic Index 魔法序号 - Grandyang - 博客园







# [[CareerCup] 9.3 Magic Index 魔法序号](https://www.cnblogs.com/grandyang/p/4818606.html)







9.3 A magic index in an array A[0.. .n-1] is defined to be an index such that A[i] = i. Given a sorted array of distinct integers, write a method to find a magic index, if one exists, in array A.
FOLLOW UP
What if the values are not distinct?



这道题定义了一个魔法序号，就是一个数组的序号等于该位置的值的时候，这个序号就是魔法序号，给了我们一个有序数组，让我们来找魔法序号。这里brute force的方法就不提了，因为没啥考察的目的，对于高效的查找方法我们就要首先考虑二分搜索法，首先我们来看这种方法，没啥特别的地方，套用一般的二分查找法的格式即可，参见代码如下：



```
class Solution {
public:
    int getMagicIdx(vector<int> &nums) {
        int left = 0, right = nums.size() - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (nums[mid] == mid) return mid;
            else if (nums[mid] > mid) right = mid - 1;
            else left = mid + 1;
        }
        return -1;
    }
};
```



这道题的Follow up是说如果数组由重复项怎么处理，那么传统的二分搜索法就会失效，因为下列这种情况可能存在：
|-10|-5|2|2|2|3|4|7|9|12|13|
|----|----|----|----|----|----|----|----|----|----|----|
|0|1|2|3|4|5|6|7|8|9|10|

这种情况符合题意，但是左右两边都会出现魔法序号，所以二分查找法会失效。那么我们难道又要用地毯式搜索了么，其实也不必，我们可以用一种类似于二分搜索法的递归方法来解决问题，就拿上面那个例子来说，第一次找到比较完中间点后，由于左右两边都会出现答案，所以我们左右半段要分别递归一下，这里我们可以加一个trick来优化算法，比如要递归左半段时，那么新的右边界就可以设为min(mid - 1, nums[mid])，同理递归右半段时，左边界可以设为max(mid + 1, nums[mid])。还有个小trick，就是如果左半段搜到了答案，那么直接返回即可，不用再搜右半段，因为题目让我们找一个就行了，没说要找出所有的Magic index，参见代码如下：



```
// Follow up
class Solution {
public:
    int getMagicIdx(vector<int> &nums) {
        return getMagicIdxDFS(nums, 0, nums.size() - 1);
    }
    int getMagicIdxDFS(vector<int> &nums, int start, int end) {
        if (end < start) return -1;
        int mid = (start + end) / 2;
        if (mid == nums[mid]) return mid;
        int left = getMagicIdxDFS(nums, start, min(mid - 1, nums[mid]));
        if (left >= 0) return left;
        int right = getMagicIdxDFS(nums, max(mid + 1, nums[mid]), end);
        return right;
    }
};
```














