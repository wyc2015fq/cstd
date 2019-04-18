# Median of Two Sorted Arrays——解题笔记 - bigfacesafdasgfewgf - CSDN博客





2015年05月06日 10:13:41[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：956标签：[LeetCode																[arrays																[merge																[median																[vector](https://so.csdn.net/so/search/s.do?q=vector&t=blog)
个人分类：[LeetCode](https://blog.csdn.net/puqutogether/article/category/2832605)








    【题目】

There are two sorted arrays nums1 and nums2 of size m and n respectively. Find the median of the two sorted arrays. The overall run time complexity should be O(log (m+n)).





    解法1：

    直接把两个vector合并，然后排序，取中位数。注意对合并后vector长度的奇偶数的处理。此解法比较好想到，但是时间复杂度是O((m+n)log(m+n)).

    代码如下：



```cpp
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        vector<int> mergedNums;
        mergedNums.assign(nums1.begin(), nums1.end());
        mergedNums.insert(mergedNums.end(), nums2.begin(), nums2.end());
        sort(mergedNums.begin(), mergedNums.end());
        
        int len1 = nums1.size();
        int len2 = nums2.size();
        
        double median = (len1 + len2)%2 ? mergedNums[(len1 + len2)>>1] : (mergedNums[(len1 + len2 - 1)>>1] + mergedNums[(len1 + len2)>>1])/2.0;
        
        return median;
    }
};
```






    解法2：

    时间复杂度能达到O(log(m+n))，但是思路比较复杂。我们可以把这个问题转化为寻找第k小的数，只不过这里的k是中位数。

    参考解释：

    http://blog.csdn.net/yutianzuijin/article/details/11499917


    http://blog.csdn.net/zxzxy1988/article/details/8587244





    代码如下：



```cpp
double findKth(int a[], int m, int b[], int n, int k) 
{ 
//always assume that m is equal or smaller than n 
if (m > n) 
return findKth(b, n, a, m, k); 
if (m == 0) 
return b[k - 1]; 
if (k == 1) 
return min(a[0], b[0]); 
//divide k into two parts 
int pa = min(k / 2, m), pb = k - pa; 
if (a[pa - 1] < b[pb - 1]) 
return findKth(a + pa, m - pa, b, n, k - pa); 
else if (a[pa - 1] > b[pb - 1]) 
return findKth(a, m, b + pb, n - pb, k - pb); 
else 
return a[pa - 1]; 
} 

class Solution 
{ 
public: 
double findMedianSortedArrays(int A[], int m, int B[], int n) 
{ 
int total = m + n; 
if (total & 0x1) 
return findKth(A, m, B, n, total / 2 + 1); 
else 
return (findKth(A, m, B, n, total / 2) 
+ findKth(A, m, B, n, total / 2 + 1)) / 2; 
} 
};
```](https://so.csdn.net/so/search/s.do?q=median&t=blog)](https://so.csdn.net/so/search/s.do?q=merge&t=blog)](https://so.csdn.net/so/search/s.do?q=arrays&t=blog)](https://so.csdn.net/so/search/s.do?q=LeetCode&t=blog)




