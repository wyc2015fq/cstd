
# 88. Merge Sorted Array - OraYang的博客 - CSDN博客

2017年07月16日 11:09:11[OraYang](https://me.csdn.net/u010665216)阅读数：131标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given two sorted integer arraysnums1andnums2, mergenums2intonums1as one sorted array.
Note:
You may assume thatnums1has enough space (size that is greater or equal tom+n) to hold additional elements fromnums2. The number of elements initialized innums1andnums2aremandnrespectively.
思路：本题思路比较简单，核心思想就是建立三个索引，索引nums1,索引nums2,索引nums1+nums2，通过同时遍历nums1及nums2,然后将较大值添加到nums1末尾，本题需要注意的地方是，nums1为空的情况(**注意：本题leetcode testcase错误，与题意不符**）
![](https://img-blog.csdn.net/20170716111349634?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
代码：

```python
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
    if(m==0)  
        for(int j =0;j<n;j++)
            nums1[j] = nums2[j];
    int idx1 = m-1;  
    int idx2 = n-1;  
    int len = m+n-1;  
    while(idx1>=0 && idx2>=0)  
    {  
        if(nums1[idx1]>nums2[idx2])  
        {  
            nums1[len--] = nums2[idx1--];  
        }  
        else  
        {  
            nums1[len--] = nums2[idx2--];  
        }  
    }  
    while(idx2>=0)  
    {  
        nums1[len--] = nums2[idx2--];  
    } 
            
    }
};
```



