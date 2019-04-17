# [LeetCode] Find Minimum in Rotated Sorted Array  寻找旋转有序数组的最小值 - Grandyang - 博客园







# [[LeetCode] Find Minimum in Rotated Sorted Array  寻找旋转有序数组的最小值](https://www.cnblogs.com/grandyang/p/4032934.html)







Suppose a sorted array is rotated at some pivot unknown to you beforehand.

(i.e., `0 1 2 4 5 6 7` might become `4 5 6 7 0 1 2`).

Find the minimum element.

You may assume no duplicate exists in the array.



这道寻找旋转有序数组的最小值肯定不能通过直接遍历整个数组来寻找，这个方法过于简单粗暴，这样的话，旋不旋转就没有意义。应该考虑将时间复杂度从简单粗暴的O(n)缩小到O(lgn)，这时候二分查找法就浮现在脑海。



首先要判断这个有序数组是否旋转了，通过比较第一个和最后一个数的大小，如果第一个数小，则没有旋转，直接返回这个数。如果第一个数大，就要进一步搜索。我们定义left和right两个指针分别指向开头和结尾，还要找到中间那个数，然后和left指的数比较，如果中间的数大，则继续二分查找右半段数组，反之查找左半段。终止条件是当左右两个指针相邻，返回小的那个。代码如下：



```
class Solution {
public:
    int findMin(vector<int> &num) {
        int left = 0, right = num.size() - 1;
        if (num[left] > num[right]) {
            while (left != (right - 1)) {
                int mid = (left + right) / 2;
                if (num[left] < num[mid]) left = mid;
                else right = mid;
            }
            return min(num[left], num[right]);
        }
        return num[0];
    }
};
```



对于数组中有重复数字的情况，请参见我的另一篇博文：

[http://www.cnblogs.com/grandyang/p/4040438.html](http://www.cnblogs.com/grandyang/p/4040438.html)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












