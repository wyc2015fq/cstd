# leetcode题库：4.两个排序数组的中位数 - Koma Hub - CSDN博客
2018年04月23日 09:52:18[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：129
***题目：***
/** 
 *leetcode题库：4. 求两个排序数组的中位数
 *
 *  给定两个大小为 m 和 n 的有序数组 nums1 和 nums2 。
 *  请找出这两个有序数组的中位数。
 *  要求算法的时间复杂度为 O(log (m+n))。
 *
 *  示例 1: nums1 = [1, 3]   nums2 = [2]   中位数是 2.0
 *  示例 2: nums1 = [1, 2]   nums2 = [3, 4]   中位数是 (2 + 3)/2 = 2.5
 */
***思路***：[http://www.cnblogs.com/grandyang/p/4465932.html](http://www.cnblogs.com/grandyang/p/4465932.html)
这道题让我们求两个有序数组的中位数，而且限制了时间复杂度为O(log (m+n))，看到这个时间复杂度，自然而然的想到了应该使用二分查找法来求解。但是这道题被定义为Hard也是有其原因的，难就难在要在两个未合并的有序数组之间使用二分法，这里我们需要定义一个函数来找到第K个元素，由于两个数组长度之和的奇偶不确定，因此需要分情况来讨论，对于奇数的情况，直接找到最中间的数即可，偶数的话需要求最中间两个数的平均值。下面重点来看如何实现找到第K个元素，首先我们需要让数组1的长度小于或等于数组2的长度，那么我们只需判断如果数组1的长度大于数组2的长度的话，交换两个数组即可，然后我们要判断小的数组是否为空，为空的话，直接在另一个数组找第K个即可。还有一种情况是当K = 1时，表示我们要找第一个元素，只要比较两个数组的第一个元素，返回较小的那个即可。
***完整的程序：***
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include <string.h>
/** 
 *leetcode题库：4. 求两个排序数组的中位数
 *
 *  给定两个大小为 m 和 n 的有序数组 nums1 和 nums2 。
 *  请找出这两个有序数组的中位数。
 *  要求算法的时间复杂度为 O(log (m+n))。
 *
 *  示例 1: nums1 = [1, 3]   nums2 = [2]   中位数是 2.0
 *  示例 2: nums1 = [1, 2]   nums2 = [3, 4]   中位数是 (2 + 3)/2 = 2.5
 */
 /*
 
 */
double findKth(int *nums1, int nums1Size, int i,
               int *nums2, int nums2Size, int j, int k)
{
    if(nums1Size -i > nums2Size - j)
    {
        return findKth(nums2, nums2Size, j, nums1, nums1Size, i, k);
    }
    /* 判断小的数组是否为空,为空的话,直接在另一个数组找第K个即可 */
    if(nums1Size == i)
    {
        return nums2[j + k - 1];
    }
    /*当K = 1时,表示我们要找第一个元素,只要比较两个数组的第一个元素，
      返回较小的那个即可*/
    if(k == 1)
    {
        return nums1[i]<nums2[j]?nums1[i]:nums2[j];
    }
    int pa = (i + k / 2)<nums1Size?(i + k / 2):nums1Size;
    int pb = j + k - pa + i;
    if(nums1[pa - 1] < nums2[pb - 1])
    {
        return findKth(nums1, nums1Size, pa, nums2, nums2Size, j, k-pa+i);
    }
    else if(nums1[pa - 1] > nums2[pb - 1])
    {
        return findKth(nums1, nums1Size, i, nums2, nums2Size, pb, k - pb + j);
    }
    else{
        return nums1[pa - 1];
    }
}
double findMedianSortedArrays(int* nums1, int nums1Size, 
                              int* nums2, int nums2Size) 
/* 题干中给出的函数 */
{
    if (nums1Size <= 0 && nums2Size <= 0) 
    {
        return 0;
    }
    int total = nums1Size + nums2Size;
    if(total % 2 == 1)
    {
        return findKth(nums1, nums1Size, 0, nums2, nums2Size, 0, total/2+1);
    }
    else 
    {
        return (findKth(nums1, nums1Size,0, nums2, nums2Size,0, total/2)
               +findKth(nums1, nums1Size,0, nums2, nums2Size,0, total/2+1))/2.0;
    }
}
int main( int argc, char **argv)
{
    int *v1, *v2;
    int n1 = 10, n2 = 2;
    int array1[] = {1,2,4,5,6,7,8,9,10,11};
    int array2[] = {3,4};
    v1 = array1;
    v2 = array2;
    
    printf("%f\n",findMedianSortedArrays(v1, n1, v2, n2));
    
    return 1;
}
```
运行测试：
```
D:\test>gcc leetcode4.c
D:\test>a.exe
5.500000
```
修改测试数据：
```
int n1 = 5, n2 = 2;
int array1[] = {1,4,8,9,16};
int array2[] = {3,10};
D:\test>gcc leetcode4.c
D:\test>a.exe
8.000000
```
