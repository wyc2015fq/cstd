# LeetCode  Kth Largest Element in an Array （快速排序） - xcw0754 - 博客园
# [LeetCode  Kth Largest Element in an Array （快速排序）](https://www.cnblogs.com/xcw0754/p/4973075.html)
题意：
　　在一个无序的数组中第k大的数是多少？
思路：
　　按照快排的思路，如果每次分成两段后，设为L和R。如果R>=k ，则答案在右边集合，否则在左边集合。
　　这里用了3位取中法。注意快排别给写死循环了。
```
1 class Solution {
 2 public:
 3     int findKthLargest(vector<int>& nums, int k) {
 4         if(k>nums.size())    return 0;
 5            return DFS(nums,0,nums.size()-1,k);
 6     }
 7 
 8     int DFS(vector<int>& nums,int s,int e,int k)
 9     {
10         int L=s, R=e;
11         //三位取中法
12         if(nums[e]>nums[s])    swap(nums[s],nums[e]);    
13         if(nums[s]>nums[(s+e)/2])    swap(nums[s],nums[(s+e)/2]); 
14 
15         int mid=nums[s];
16         while(L<R)
17         {
18             while(L<R && nums[R]>=mid)    R--;    //找小
19             nums[L]=nums[R];
20             while(L<R && nums[L]<=mid)    L++;    //找大
21             nums[R]=nums[L];
22         }
23         nums[L]=mid;
24         int len=e-L;//右边部分的元素个数
25         if(len+1==k)    return mid;
26         if(len>=k)    return DFS(nums,L+1,e,k);
27         else        return DFS(nums,s,L-1,k-len-1);
28     }
29 };
AC代码
```

