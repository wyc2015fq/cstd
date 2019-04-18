# 【35-40】LeetCode：Python解题 - Joe的博客 - CSDN博客





2018年03月19日 17:47:09[Joe-Han](https://me.csdn.net/u010089444)阅读数：715








## **41. First Missing Positive**【Hard】

Given an unsorted integer array, find the first missing positive integer.

For example, 

Given **[1,2,0]** return **3**, 

and **[3,4,-1,1]** return **2**.
Your algorithm should run in O(n) time and uses constant space.

**Solution 1:**

```python
class Solution(object):
    def firstMissingPositive(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        if not nums:
            return 1
        m = max(nums)
        l = [1.1 for i in range(m+2)]

        for i in nums:
            if i >= 0:
                l[i] = 0 

        minNum = l.index(0)
        return l[minNum:].index(1.1)+minNum if minNum < 2 else 1
```

**Solution 2:**

```python
class Solution(object):
    def firstMissingPositive(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        for any list whose length is N, the first missing positive must be in range [1,...,N+1]
        """
        N = len(nums)
        # move every value to the position of its value
        for target in nums:
            while target < N+1 and target > 0 and target != nums[target-1]:
                nums[target-1], target= target, nums[target-1]
        # find first location where the index doesn't match the value
        for cursor in range(N):
            if nums[cursor] != cursor+1:
                return cursor+1
        return N+1
```

## **42. Trapping Rain Water**【Hard】

Given n non-negative integers representing an elevation map where the width of each bar is 1, compute how much water it is able to trap after raining.

For example, 

Given **[0,1,0,2,1,0,1,3,2,1,2,1]**, return **6**.

![image](https://leetcode.com/static/images/problemset/rainwatertrap.png)

**Solution:**

```python
class Solution(object):
    def trap(self, height):
        """
        :type height: List[int]
        :rtype: int
        """
        left, right = 0, len(height)-1
        left_max = right_max = 0
        ans = 0
        while left < right:
            if height[left] < height[right]:
                if height[left] > left_max:
                    left_max = height[left] 
                else:
                    ans += left_max - height[left]
                left += 1
            else:
                if height[right] > right_max:
                    right_max = height[right] 
                else:
                    ans += right_max - height[right]
                right -= 1

        return ans
```

**Discussion：**

Using 2 pointers：[https://leetcode.com/problems/trapping-rain-water/solution/](https://leetcode.com/problems/trapping-rain-water/solution/)

## **43. Multiply Strings**【Medium】

Given two non-negative integers **num1** and **num2** represented as strings, return the product of **num1** and **num2**.

**Note:**
- The length of both **num1** and **num2** is < 110.
- Both **num1** and **num2** contains only digits 0-9.
- Both **num1** and **num2** does not contain any leading zero.
- 
You **must not use any built-in BigInteger library** or **convert the inputs to integer** directly.

**Solution:**

```python
class Solution(object):
def multiply(self, num1, num2):
    """
    :type num1: str
    :type num2: str
    :rtype: str
    """
    dic = {'0':0, '1':1, '2':2, '3':3, '4':4, '5':5, '6':6, '7':7, '8':8, '9':9}
    lens = len(num1)+len(num2)
    product = [0]*lens
    for i in reversed(num2):
        lens = index = lens - 1
        for j in reversed(num1):
            product[index] += dic[i]*dic[j]
            product[index-1] += product[index] / 10
            product[index] = product[index] % 10
            index -= 1
    for i in range(len(product)):
        if product[i] != 0:
            return ''.join(map(str,product[i:]))

    return '0'
```


## **44. Multiply Strings**【Hard】

Implement wildcard pattern matching with support for ‘?’ and ‘*’.

```
'?' Matches any single character.
'*' Matches any sequence of characters (including the empty sequence).

The matching should cover the entire input string (not partial).

The function prototype should be:
bool isMatch(const char *s, const char *p)

Some examples:
isMatch("aa","a") → false
isMatch("aa","aa") → true
isMatch("aaa","aa") → false
isMatch("aa", "*") → true
isMatch("aa", "a*") → true
isMatch("ab", "?*") → true
isMatch("aab", "c*a*b") → false
```

**Solution:**

```python
class Solution(object):
    def isMatch(self, s, p):
        """
        :type s: str
        :type p: str
        :rtype: bool
        """
        len_s, len_p= len(s), len(p)
        dp = [True] + [False]*len_s      
        if len_p - p.count('*') > len_s:
            return False 

        for c in p:
            if c != '*':
                for i in range(len_s-1,-1,-1): 
                    dp[i+1] = dp[i] and (s[i] == c or c == '?')
            else:
                for i in range(len_s):
                    dp[i+1] = dp[i+1] or dp[i]
            dp[0] = dp[0] and c == '*'
        return dp[-1]
```




