# LeetCode  Find the Duplicate Number 找重复出现的数（技巧） - xcw0754 - 博客园
# [LeetCode  Find the Duplicate Number 找重复出现的数（技巧）](https://www.cnblogs.com/xcw0754/p/4924298.html)
题意：
　　有一个含有n+1个元素的数组，元素值是在1～n之间的整数，请找出其中出现超过1次的数。（保证仅有1个出现次数是超过1的数）
思路：
　　方法一：O(nlogn)。根据鸽笼原理及题意，每次如果<=k的数超过了k个，那么答案必定在[1，k]。可以用二分枚举答案来解决。
```
1 bool left(vector<int>& nums,int tar)//是否在左边
 2 {
 3     int cnt=0;
 4     for(int i=0; i<nums.size(); i++)    
 5         if(nums[i]<=tar)    
 6             cnt++;
 7     return cnt>tar;
 8 }
 9 
10 int findDuplicate(vector<int>& nums)
11 {
12     int L=1, R=nums.size()-1;
13     while(L<R)
14     {
15         int mid=R-(R-L+1)/2;
16         if( left(nums,mid)==true )    R=mid;
17         else    L=mid+1;
18     }
19     return R;
20 }
AC代码
```
　　方法二：O(n)。将数组nums看成是一个链表，next[i]表示点i的后继（0也是一个点，因为0也是下标）。根据题意，此链表必定有且仅有一个简单环存在，这样就类似于[Linked List Cycle II](https://leetcode.com/problems/linked-list-cycle-ii/) ，只是会多余出部分的链，但是这不会影响到这个模型，从0点出发依然存在这样的一个模型，只是环的接口处不会是0而已。要注意两个指针的起始位置，必须保证fast=2*slow。
```
1 class Solution {
 2 public:
 3     int findDuplicate(vector<int>& nums)
 4     {
 5         int slow=0, fast=nums[0];
 6         while(fast!=slow)
 7         {
 8             slow=nums[slow];
 9             fast=nums[nums[fast]];
10         }
11         fast=0;
12         slow=nums[slow];
13         while(fast!=slow)
14         {
15             slow=nums[slow];
16             fast=nums[fast];
17         }
18         return fast;
19     }
20 };
AC代码
```

