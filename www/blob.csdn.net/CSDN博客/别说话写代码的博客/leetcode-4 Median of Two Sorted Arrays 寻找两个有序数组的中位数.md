# leetcode-4. Median of Two Sorted Arrays 寻找两个有序数组的中位数 - 别说话写代码的博客 - CSDN博客





2019年02月21日 23:15:27[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：14标签：[寻找两个有序数组的中位数																[leetcode																[Median of Two Sorted Arrays](https://so.csdn.net/so/search/s.do?q=Median of Two Sorted Arrays&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



|There are two sorted arrays **nums1** and **nums2** of size m and n respectively.Find the median of the two sorted arrays. The overall run time complexity should be O(log (m+n)).You may assume **nums1** and **nums2** cannot be both empty.**Example 1:**```nums1 = [1, 3]nums2 = [2]The median is 2.0```**Example 2:**```nums1 = [1, 2]nums2 = [3, 4]The median is (2 + 3)/2 = 2.5```|给定两个大小为 m 和 n 的有序数组 `nums1` 和 `nums2`。请你找出这两个有序数组的中位数，并且要求算法的时间复杂度为 O(log(m + n))。你可以假设 `nums1` 和 `nums2` 不会同时为空。**示例 1:**```nums1 = [1, 3]nums2 = [2]则中位数是 2.0```**示例 2:**```nums1 = [1, 2]nums2 = [3, 4]则中位数是 (2 + 3)/2 = 2.5```|
|----|----|

思路：不知道奇偶的情况下求中位数可以用个策略就是求第(m+n+1)/2,(m+n+2)/2然后平均。注意这个题目条件是两个有序数组， 看到有序 就想到用二分查找的思想。假设要找两个数组排序后第k个数，那只需要分别在  两个数组中找k/2个位置比较，如果a[k/2-1] == b[k/2-1]，那就说明第k个数字找到了，就是a[k/2]或者b[k/2]。

如果a[k/2-1]<b[k/2-1]，那就说明k肯定不在a[0...k/2-1] 前面，删除这一段。所以k的范围缩小为a的k/2之后

如果a[k/2-1]>b[k/2-1]，那就说明k肯定不在b[0...k/2-1]前面，删除这一段，所以k的范围缩小为b的k/2之后

```cpp
class Solution {
public:
    int mid(vector<int> nums1,vector<int> nums2,int k)
    {
        if(!nums1.size()) return nums2[k-1];
        if(!nums2.size()) return nums1[k-1];
        if(k==1) return min(nums1[0],nums2[0]);
        int i=min((int)nums1.size(),k/2),j=min((int)nums2.size(),k/2);
        if(nums1[i-1]>nums2[j-1])
            return mid(nums1,vector<int>(nums2.begin()+j,nums2.end()),k-j);
        else return mid(vector<int>(nums1.begin()+i,nums1.end()),nums2,k-i);
        return 0;
    }
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int m=nums1.size(),n=nums2.size();
        return (mid(nums1,nums2,(m+n+1)/2)+mid(nums1,nums2,(m+n+2)/2))/2.0;
    }
};
```

参考：[http://www.cnblogs.com/grandyang/p/4465932.html](http://www.cnblogs.com/grandyang/p/4465932.html)](https://so.csdn.net/so/search/s.do?q=寻找两个有序数组的中位数&t=blog)




