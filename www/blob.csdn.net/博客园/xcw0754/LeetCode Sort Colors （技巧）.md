# LeetCode Sort Colors （技巧） - xcw0754 - 博客园
# [LeetCode Sort Colors （技巧）](https://www.cnblogs.com/xcw0754/p/4931205.html)
 题意：
　　一个数组只可能含有3种数据，分别为1,2,3，请将数组排序（只能扫一遍）。
思路：
　　如果扫两遍的话，用个桶记录一下，再赋值上去就行了。
```
1 class Solution {
 2 public:
 3     void sortColors(vector<int>& nums) {
 4         int cnt[3]={0};
 5         for(int i=0; i<nums.size(); i++)
 6             cnt[nums[i]]++;
 7         for(int j=0,i=0; j<3; j++)
 8             while(cnt[j]-- > 0)
 9                 nums[i++]=j;
10     }
11 };
AC代码
```
　　还有这种傻瓜方法，扫一遍先将2移到末尾，再扫一遍将0移到前面。仍然无效率。
```
1 class Solution {
 2 public:
 3     void sortColors(vector<int>& nums) {
 4         int L=0, R=nums.size()-1;
 5         for(int i=0; i<nums.size(); i++)
 6         {
 7             while(L<R && nums[L]!=2)    L++;
 8             while(L<R && nums[R]==2)    R--;
 9             if(L<R)    swap(nums[L],nums[R]);
10         }
11         L=0, R=nums.size()-1;
12         for(int i=0; i<nums.size(); i++)
13         {
14             while(L<R && nums[L]!=1)    L++;
15             while(L<R && nums[R]!=0)    R--;
16             if(L<R)    swap(nums[L],nums[R]);
17         }
18     }
19 };
AC代码
```
　　还有一种吊吊的，扫一遍就搞定的。扫一遍数组，考虑nums[i]，如果nums[i]=2，立刻换到末尾，此时nums[i]有可能仍然是2，如果是2，一直继续换到末尾。这样就保证了区间(L,i)中不可能出现2，如果此时nums[i]为0，就与前面的换，此时nums[i]就只可能是0或1了，0就一直换，1就pass。
```
1 class Solution {
 2 public:
 3     void sortColors(vector<int>& nums) {
 4          int L=-1, R=nums.size();
 5         for(int i=0; i<R; i++)
 6         {
 7             while(i<R&&nums[i]==2)    
 8                 swap(nums[i],nums[--R]);    
 9             if(nums[i]==0)    
10                 swap(nums[i],nums[++L]);
11         }
12     }
13 };
AC代码
```

