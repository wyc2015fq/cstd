# LeetCode Find Minimum in Rotated Sorted Array 旋转序列找最小值（二分查找） - xcw0754 - 博客园
# [LeetCode Find Minimum in Rotated Sorted Array 旋转序列找最小值（二分查找）](https://www.cnblogs.com/xcw0754/p/4679282.html)
题意：有一个有序序列A，其内部可能有部分被旋转了，比如A[1...n]被转成A[mid...n]+A[1...mid-1]，如果被旋转，只有这种形式。问最小元素是？(假设没有重复元素)
思路：如果是序没乱，直接返回A[1]，如果乱了，二分查找还是可以的，O(1)可能就不行了。
　　二分要点：mid有可能就是所要找的最小元素，所以不能轻易写出l=mid+1这样的语句，可能最小值就被忽略过了，因为我们无法直接判断A[mid]是否就是最小值。所以尽量应该是l=mid这样写，但是要防止死循环。
　　具体来说，可以这样做，先判断A[1]和A[n]的大小情况，如果A[1]<A[n]，那么A[1]为答案。
　　如果A[L]<A[mid]，既然有比A[mid]更小的，那这个元素肯定是不要的，l=mid+1，因为A[L...mid]肯定是前面部分。
　　否则，A[mid]肯定小于A[R]，那么r=mid，注意这里A[mid]可能为最小值，不能跳过。
方法一：
```
1 class Solution {
 2 public:
 3     int findMin(vector<int>& nums) {
 4         if(nums.size()==1 ||  nums[0]<nums[ nums.size()-1 ]     )  return nums[0];//这里便知道A[1]是否为答案
 5 
 6         int l=0, r=nums.size()-1;
 7         while(l<r)
 8         {
 9             int mid=(l+r)/2;
10             if( nums[mid]<nums[r] ) r=mid;  //mid有可能就是要找的数，所以不能跳过
11             else    l=mid+1;
12         }
13         return nums[l];
14     }
15 
16 };
AC代码
```
方法二：
```
1 class Solution {
 2 public:
 3     int findMin(vector<int>& nums) {
 4         if(nums.size()==1 ||  nums[0]<nums[ nums.size()-1 ]     )  return nums[0];
 5 
 6         int l=0, r=nums.size()-1;
 7         while(l<r)
 8         {
 9             int mid=(l+r)/2;
10             if( nums[mid]>nums[l] ) l=mid;
11             else if( nums[mid]<nums[r] )    r=mid;
12             else l++;
13         }
14         return nums[l];
15     }
16 
17 };
AC代码
```

