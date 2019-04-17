# [LeetCode] Merge Sorted Array 混合插入有序数组 - Grandyang - 博客园







# [[LeetCode] Merge Sorted Array 混合插入有序数组](https://www.cnblogs.com/grandyang/p/4059650.html)







Given two sorted integer arrays *nums1* and *nums2*, merge *nums2* into *nums1* as one sorted array.

Note:
- The number of elements initialized in *nums1*and *nums2* are *m* and *n* respectively.
- You may assume that *nums1* has enough space (size that is greater or equal to *m* + *n*) to hold additional elements from *nums2*.

Example:
Input:
nums1 = [1,2,3,0,0,0], m = 3
nums2 = [2,5,6],       n = 3

Output: [1,2,2,3,5,6]


混合插入有序数组，由于两个数组都是有序的，所有只要按顺序比较大小即可。题目中说了nums1数组有足够大的空间，说明我们不用resize数组，又给了我们m和n，那就知道了混合之后的数组的大小，这样我们就从nums1和nums2数组的末尾开始一个一个比较，把较大的数，按顺序从后往前加入混合之后的数组末尾。需要三个变量i，j，k，分别指向nums1，nums2，和混合数组的末尾。进行while循环，如果i和j都大于0，再看如果nums1[i] > nums2[j]，说明要先把nums1[i]加入混合数组的末尾，加入后k和i都要自减1；反之就把nums2[j]加入混合数组的末尾，加入后k和j都要自减1。循环结束后，有可能i或者j还大于等于0，若j大于0，那么我们还需要继续循环，将nums2中的数字继续拷入nums1。若是i大于等于0，那么就不用管，因为混合数组本身就放在nums1中，参见代码如下：



解法一： 

```
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int i = m - 1, j = n - 1, k = m + n - 1;
        while (i >= 0 && j >= 0) {
            if (nums1[i] > nums2[j]) nums1[k--] = nums1[i--];
            else nums1[k--] = nums2[j--];
        }
        while (j >= 0) nums1[k--] = nums2[j--];
    }
};
```



我们还可以写的更简洁一些，将两个while循环融合到一起，我们只要加上i>=0且nums1[i] > nums2[j]的判断条件，就可以从nums1中取数，否则就一直从nums2中取数，参见代码如下:



解法二：

```
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int i = m - 1, j = n - 1, k = m + n - 1;
        while (j >= 0) {
            nums1[k--] = (i >= 0 && nums1[i] > nums2[j]) ? nums1[i--] : nums2[j--];
        }
    }
};
```



类似题目：

[Merge Sorted Array](http://www.cnblogs.com/grandyang/p/4059650.html)



参考资料：

[https://leetcode.com/problems/merge-sorted-array/](https://leetcode.com/problems/merge-sorted-array/)

[https://leetcode.com/problems/merge-sorted-array/discuss/29572/My-simple-solution](https://leetcode.com/problems/merge-sorted-array/discuss/29572/My-simple-solution)

[https://leetcode.com/problems/merge-sorted-array/discuss/29515/4ms-C%2B%2B-solution-with-single-loop](https://leetcode.com/problems/merge-sorted-array/discuss/29515/4ms-C%2B%2B-solution-with-single-loop)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












