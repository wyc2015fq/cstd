# 【1-5】LeetCode：Python解题 - Joe的博客 - CSDN博客





2017年12月09日 11:42:04[Joe-Han](https://me.csdn.net/u010089444)阅读数：1758








## **1. Two Sum**【Easy】

Given an array of integers, return **indices** of the two numbers such that they add up to a specific target.

You may assume that each input would have **exactly** one solution, and you may not use the same element twice.

**Example:**

```bash
Given nums = [2, 7, 11, 15], target = 9,

Because nums[0] + nums[1] = 2 + 7 = 9,
return [0, 1].
```

**Solution:**

```python
class Solution(object):
    def twoSum(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: List[int]
        """
        num_map = {}
        for i in range(len(nums)):
            result = target - nums[i]
            if result in num_map:
                return [num_map[result],i]
            num_map[nums[i]] = i
```

**Discussion:**

使用字典，key是差，value是index。比如例子中的[2,7,11,15]，target是9。那么在2的时候就存入7：0，下一位找到7的时候，之前有个差值是7，那么就返回7对应的index，0，以及当前这个7的index，就是1。算法时间复杂度为O(n)

## **2. Add Two Numbers**【Medium】

You are given two **non-empty** linked lists representing two non-negative integers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.

**Input:** (2 -> 4 -> 3) + (5 -> 6 -> 4)，**Output:** 7 -> 0 -> 8

**Solution:**

```python
# Definition for singly-linked list.
# class ListNode(object):
#     def __init__(self, x):
#         self.val = x
#         self.next = None

class Solution(object):
    def addTwoNumbers(self, l1, l2):
        """
        :type l1: ListNode
        :type l2: ListNode
        :rtype: ListNode
        """
        head = p = ListNode(0)
        carry = 0
        while l1 or l2 or carry:
            if l1:
                carry += l1.val
                l1 = l1.next
            if l2:
                carry += l2.val
                l2 = l2.next
            carry, val = divmod(carry, 10)
            p.next = p = ListNode(val)


        return head.next
```

## **3. Longest Substring Without Repeating Characters**【Medium】

Given a string, find the length of the **longest substring** without repeating characters.

**Examples:**

Given **“abcabcbb”**, the answer is **“abc”**, which the length is 3.

Given **“bbbbb”**, the answer is **“b”**, with the length of 1.

Given **“pwwkew”**, the answer is **“wke”**, with the length of 3. Note that the answer must be a substring, “pwke” is a subsequence and not a substring.

**Solution:**

```python
class Solution(object):
    def lengthOfLongestSubstring(self, s):
        """
        :type s: str
        :rtype: int
        """
        start = maxlen = 0
        dic = {}
        lens= len(s)
        for i in range(lens):
            if start + maxlen >= lens:
                return maxlen
            if s[i] in dic and start <= dic[s[i]]:
                start = dic[s[i]]+1
            else:
                maxlen = max(maxlen, i - start +1)
            dic[s[i]] = i
        return maxlen
```

**Discussion:**

声明一个变量start,记录初始索引，一个变量mxlen记录最大长度，然后遍历字串中字符，用字典dic记录该字符和其对应的索引index，若该字符串出现在dic中，且索引位置不小于start，那么将index 减去 start当做该不同字符子串的长度，并与maxlen比较取其大者。否则将start后移，最后更新dic

## **4. Median of Two Sorted Arrays**【Hard】

There are two sorted arrays **nums1** and **nums2** of size m and n respectively.

Find the median of the two sorted arrays. The overall run time complexity should be O(log (m+n)).

**Examples:**

```
nums1 = [1, 3]
nums2 = [2]

The median is 2.0
```

```
nums1 = [1, 2]
nums2 = [3, 4]

The median is (2 + 3)/2 = 2.5
```

**Solution:**

```python
class Solution(object):
    def findMedianSortedArrays(self, nums1, nums2):
        """
        :type nums1: List[int]
        :type nums2: List[int]
        :rtype: float
        """
        result = []
        i = j = 0
        len1, len2 = len(nums1),len(nums2)
        while i <len1 and j < len2:
            if nums1[i] <= nums2[j]:
                result.append(nums1[i])
                i += 1
            else:
                result.append(nums2[j])
                j += 1
        if j < len2:
            result.extend(nums2[j:])
        elif i < len1:
            result.extend(nums1[i:])

        mid = (len1+len2) / 2
        if (len1+len2) % 2:
            return result[mid]
        else:
            return (result[mid]+result[mid-1])/2.0
```

**Discussion:**

合并两个有序链表，并分奇偶情况得到中间值；另一种思路是分别将nums1和nums2划分成left和right，使得len(left)=len(right)且max(left)<= min(right)。 
[https://discuss.leetcode.com/topic/4996/share-my-o-log-min-m-n-solution-with-explanation](https://discuss.leetcode.com/topic/4996/share-my-o-log-min-m-n-solution-with-explanation)

## **5. Longest Palindromic Substring**【Medium】

Given a string **s**, find the longest palindromic substring in **s**. You may assume that the maximum length of **s** is 1000.

**Examples:**

```
Input: "babad"

Output: "bab"

Note: "aba" is also a valid answer.
```

```
Input: "cbbd"

Output: "bb"
```

**Solution:**

```python
class Solution(object):
    def __init__(self):
        self.maxlen = 1
        self.index =  0

    def extendPalindrome(self, start, end, s, lens):     
        while start >=0 and end < lens and s[start] == s[end]:
            start -=1
            end += 1
        if self.maxlen < end - start - 1:
            self.maxlen = end - start - 1
            self.index = start +1

    def longestPalindrome(self, s):
        """
        :type s: str
        :rtype: str
        """
        lens = len(s)
        if lens < 2 :
            return s 
        for i in range(lens-1):
            # if i*2 +1 > self.maxlen and (lens - i - 1)*2 + 1 > self.maxlen:
                self.extendPalindrome(i, i, s, lens)
            # if i*2 +2> self.maxlen and (lens - i - 1)*2 + 2 > self.maxlen:
                self.extendPalindrome(i, i+1, s, lens)
        return s[self.index : self.index + self.maxlen]
```

**Discussion:**

主要有两种思路，思路一：动态规划，S[i~j] 是回文的条件是S[i]=S[j]且S[i+1~j-1]是回文，基本思路是外层循环i从后往前扫，内层循环j从i当前字符扫到结尾处。过程中使用的历史信息是从i+1到n之间的任意子串是否是回文已经被记录下来。思路二：遍历字符串，以每个字符为中心往两边扩张，直到不是回文串为止（主要区分奇偶，即“aabaa”和“abba”）。



