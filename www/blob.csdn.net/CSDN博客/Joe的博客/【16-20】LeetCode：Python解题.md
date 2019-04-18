# 【16-20】LeetCode：Python解题 - Joe的博客 - CSDN博客





2017年12月09日 12:00:01[Joe-Han](https://me.csdn.net/u010089444)阅读数：941








## **16. 3Sum Closest**【Medium】

Given an array S of n integers, find three integers in S such that the sum is closest to a given number, target. Return the sum of the three integers. You may assume that each input would have exactly one solution.

```
For example, given array S = {-1 2 1 -4}, and target = 1.

The sum that is closest to the target is 2. (-1 + 2 + 1 = 2).
```

**Solution:**

```python
class Solution(object):
    def threeSumClosest(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: int
        """
        nums.sort()
        lens = len(nums)
        result = 100000000
        for i in range(lens-2):
            if i > 0 and nums[i-1] == nums[i]:
                continue
            l, r = i+1, lens-1
            while l < r:
                s = nums[i] + nums[l] + nums[r]
                if s == target:
                    return target
                if abs(s-target) < abs(result-target):
                    result = s
                if s > target:
                    r -= 1
                    while l < r and nums[r] == nums[r+1]:
                        r -= 1 
                elif s < target:
                    l += 1
                    while l < r and nums[l] == nums[l-1]:
                        l += 1 
        return result
```

## **17. Letter Combinations of a Phone Number**【Medium】

Given a digit string, return all possible letter combinations that the number could represent.

A mapping of digit to letters (just like on the telephone buttons) is given below.

![image](http://upload.wikimedia.org/wikipedia/commons/thumb/7/73/Telephone-keypad2.svg/200px-Telephone-keypad2.svg.png)

```
Input:Digit string "23"
Output: ["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"].
```

**Solution:**

```python
class Solution(object):
    def letterCombinations(self, digits):
        """
        :type digits: str
        :rtype: List[str]
        """
        dic = {'2':'abc', '3':'def', '4':'ghi', '5':'jkl', '6':'mno', '7':'pqrs', '8':'tuv', '9':'wxyz'}
        return [] if not digits else reduce(lambda x, y : [i + j for i in x for j in dic[y]], digits, [''])
```

## **18. 4Sum**【Medium】

Given an array S of n integers, are there elements a, b, c, and d in S such that a + b + c + d = target? Find all unique quadruplets in the array which gives the sum of target.

**Note:** The solution set must not contain duplicate quadruplets.

```
For example, given array S = [1, 0, -1, 0, -2, 2], and target = 0.

A solution set is:
[
  [-1,  0, 0, 1],
  [-2, -1, 1, 2],
  [-2,  0, 0, 2]
]
```

**Solution:**

```python
class Solution(object):
    def fourSum(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: List[List[int]]
        """
        nums.sort()
        lenn = len(nums)
        result = []
        for i in range(lenn-3):
            if i > 0 and nums[i] == nums[i-1]:
                continue
            for j in range(i+1, lenn):
                if j > i+1 and nums[j] == nums[j-1]:
                    continue
                l, r = j+1, lenn-1
                while l < r:
                    s = nums[i] + nums[j] + nums[l] +nums[r]
                    if s < target:
                        l += 1
                    elif s > target:
                        r -= 1
                    else:
                        result.append([nums[i], nums[j], nums[l], nums[r]]) 
                        l, r = l+1, r-1
                        while l < r and nums[l-1] == nums[l]:
                            l += 1
                        while l < r and nums[r+1] == nums[r]:
                            r -= 1
        return result
```

**Discussion:**

参考：[https://discuss.leetcode.com/topic/22705/python-140ms-beats-100-and-works-for-n-sum-n-2/2](https://discuss.leetcode.com/topic/22705/python-140ms-beats-100-and-works-for-n-sum-n-2/2)

## **19. Remove Nth Node From End of List**【Medium】

Given a linked list, remove the nth node from the end of list and return its head.

For example,

```
Given linked list: 1->2->3->4->5, and n = 2.

   After removing the second node from the end, the linked list becomes 1->2->3->5.
```

**Note:**

Given n will always be valid. 

Try to do this in one pass.
**Solution:**

```python
# Definition for singly-linked list.
# class ListNode(object):
#     def __init__(self, x):
#         self.val = x
#         self.next = None

class Solution(object):
    def recursion(self, parent, node, n):
        if node == None:
            return 1
        num = self.recursion(node, node.next, n)
        if num == n:
            if parent == node:
                nextNone = node.next
                node.val = nextNone.val
                node.next = nextNone.next
                nextNone = None
            else:
                parent.next = node.next
                node.next = None
        return num + 1

    def removeNthFromEnd(self, head, n):
        """
        :type head: ListNode
        :type n: int
        :rtype: ListNode
        """
        if head.next == None:
            return []
        self.recursion(head, head, n)
        return head
```

## **20. Valid Parentheses**【Easy】

Given a string containing just the characters ‘(‘, ‘)’, ‘{‘, ‘}’, ‘[’ and ‘]’, determine if the input string is valid.

The brackets must close in the correct order, “()” and “()[]{}” are all valid but “(]” and “([)]” are not.

**Solution:**

```python
class Solution(object):
    def isValid(self, s):
        """
        :type s: str
        :rtype: bool
        """
        dic = {')': '(', ']': '[', '}': '{'}
        l = []
        for i in s:
            if not l or i not in dic or (i in dic and l[-1] != dic[i]):
                l.append(i)
            else:
                l.pop(-1)
        return False if l else True
```




