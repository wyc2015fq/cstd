
# leetcode小白解题记录——第四题 - lin_tuer的博客 - CSDN博客


2016年08月22日 02:51:49[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：312


Median of Two Sorted Arrays
There are two sorted arraysnums1andnums2of size m and n respectively.
Find the median of the two sorted arrays. The overall run time complexity should be O(log (m+n)).
Example 1:
nums1 = [1, 3]
nums2 = [2]
The median is 2.0
Example 2:
nums1 = [1, 2]
nums2 = [3, 4]
The median is (2 + 3)/2 = 2.5
》看到这道题的时候，相信大家都有思路，最直观的方法就是合并数组，排序，找中值，over。可是一旦有sort，时间复杂度就不会低于O(Nlog(N))，这明显是不符合题目的复杂度要求的~

```cpp
class Solution {
public:
    double findMedianSortedArrays(vector<int> &num1,vector<int> &num2) {
        //这是最直接的方法，但是复杂度不满足要求
        
        vector<int> hearr;
        int m=num1.size();
        int n=num2.size();
        //合并两个数组
        for(int i=0;i<m;i++)
        {
            hearr.push_back(num1[i]);
        }
        for(int j=0;j<n;j++)
        {
            hearr.push_back(num2[j]);
        }
        //对数组排序
        sort(hearr.begin(),hearr.end()); //O(NlogN)  使用sort排序：需要头文件#include<algorithm>，  默认从小到大排序
          
        double median=(double) ((n+m)%2? hearr[((m+n-1)/2)]:(hearr[(m+n)/2-1]+hearr[(m+n)/2])/2.0);  
          
        hearr.clear();  
          
        return median;  
        
    }
};
```

》leetcode给出的方法是将原问题转变成一个寻找第k小数的问题（假设两个原序列升序排列）这样中位数实际上是第(m+n)/2小的数。
所以只要解决了第k小数的问题，原问题也得以解决。
算法流程：
>首先假设数组A和B的元素个数都大于k/2，我们比较A[k/2-1]和B[k/2-1]两个元素，这两个元素分别表示A的第k/2小的元素和B的第k/2小的元素。
> 这两个元素比较共有三种情况：>、<和=。如果A[k/2-1]<B[k/2-1]，这表示A[0]到A[k/2-1]的元素都在A和B合并之后的前k小的元素中。即，A[k/2-1]不可能大于两数组合并之后的第k小值，弃之。

》》通过上面的分析，我们即可以采用递归的方式实现寻找第k小的数。此外我们还需要考虑几个边界条件：
》如果A或者B为空，则直接返回B[k-1]或者A[k-1]；
》如果k为1，我们只需要返回A[0]和B[0]中的较小值；
》如果A[k/2-1]=B[k/2-1]，返回其中一个；

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
```
long way to go~


