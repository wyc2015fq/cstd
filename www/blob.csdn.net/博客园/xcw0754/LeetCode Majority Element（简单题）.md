# LeetCode Majority Element（简单题） - xcw0754 - 博客园
# [LeetCode Majority Element（简单题）](https://www.cnblogs.com/xcw0754/p/5056524.html)
题意：
　　给一个数组，其中有一个元素的出现次数已经超过数组的一半大小，请找出这个元素？
思路：
　　可以扫一遍数组，将这个出现次数过多的元素抵消其他的元素，最后必定留下1个以上的元素，就是它自己了。
python3
扫一遍
```
1 class Solution(object):
 2     def majorityElement(self, nums):
 3         """
 4         :type nums: List[int]
 5         :rtype: int
 6         """
 7         idx,cnt,cur=0,0,0
 8         while idx<len(nums):
 9             idx+=1
10             if cnt==0:
11                 cnt=1
12                 cur=nums[idx-1]
13                 continue
14             if nums[idx-1]!=cur:
15                 cnt-=1
16             else:
17                 cnt+=1    
18         return cur
AC代码
```
排序
```
1 class Solution(object):
2     def majorityElement(self, nums):
3         """
4         :type nums: List[int]
5         :rtype: int
6         """
7         return sorted(nums)[int(len(nums)/2)]
AC代码
```
dict
```
1 class Solution(object):
 2     def majorityElement(self, nums):
 3         """
 4         :type nums: List[int]
 5         :rtype: int
 6         """
 7         dic={}
 8         for x in nums:
 9             dic[x]=dic.get(x,0)+1
10             if dic[x]*2>len(nums):
11                 return x
AC代码
```

