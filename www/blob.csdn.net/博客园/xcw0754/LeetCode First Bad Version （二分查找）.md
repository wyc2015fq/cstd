# LeetCode First Bad Version （二分查找） - xcw0754 - 博客园
# [LeetCode First Bad Version （二分查找）](https://www.cnblogs.com/xcw0754/p/4934023.html)
题意：
　　有一个bool序列表示对应下标的版本是否出问题（下标从1开始），如果一个版本出了问题，那么其后面全部版本必定出问题。现在给出判断任意版本是否出问题的API，请找到第一个出问题的版本。
思路：
　　明显的二分查找。
```
1 // Forward declaration of isBadVersion API.
 2 bool isBadVersion(int version);
 3 
 4 class Solution {
 5 public:
 6     int firstBadVersion(int n) {
 7         int L=1, R=n;
 8         while(L<R)
 9         {
10             int mid=R-(R-L+1)/2;
11             if(isBadVersion(mid))    R=mid;
12             else    L=mid+1;
13         }
14         return R;
15     }
16 };
AC代码
```

