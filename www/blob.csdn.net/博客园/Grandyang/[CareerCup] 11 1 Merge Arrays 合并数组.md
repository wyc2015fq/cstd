# [CareerCup] 11.1 Merge Arrays 合并数组 - Grandyang - 博客园







# [[CareerCup] 11.1 Merge Arrays 合并数组](https://www.cnblogs.com/grandyang/p/4881109.html)







11.1 You are given two sorted arrays, A and B, where A has a large enough buffer at the end to hold B. Write a method to merge B into A in sorted orde.



LeetCode上的原题，请参见我之前的博客[Merge Sorted Array 混合插入有序数组](http://www.cnblogs.com/grandyang/p/4059650.html)。



```
class Solution {
public:
    void merge(vector<int> &a, int m, vector<int> &b, int n) {
        int cnt = m + n - 1;
        --m; --n;
        while (m >= 0 && n >= 0) a[cnt--] = a[m] > b[n] ? a[m--] : b[n--];
        while (n >= 0) a[cnt--] = b[n--];
    }
};
```














