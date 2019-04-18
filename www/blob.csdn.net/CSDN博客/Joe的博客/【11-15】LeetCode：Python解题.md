# 【11-15】LeetCode：Python解题 - Joe的博客 - CSDN博客





2017年12月09日 11:52:57[Joe-Han](https://me.csdn.net/u010089444)阅读数：859








## **11. Container With Most Water**【Medium】

Given n non-negative integers a1, a2, …, an, where each represents a point at coordinate (i, ai). n vertical lines are drawn such that the two endpoints of line i is at (i, ai) and (i, 0). Find two lines, which together with x-axis forms a container, such that the container contains the most water.

**Note:** You may not slant the container and n is at least 2.

**Solution:**

```python
class Solution(object):
    def maxArea(self, height):
        """
        :type height: List[int]
        :rtype: int
        """
        maxs = l = 0
        r = len(height)-1
        while l < r:
            if height[l] < height[r]:
                maxs, l = max(maxs, height[l]*(r-l)), l+1
            else:
                maxs, r = max(maxs, height[r]*(r-l)), r-1
        return maxs
```

**Discussion:**

思路一：线段之间两两进行比较，x坐标之差为宽度，y坐标中较小的值为高度。 思路二：用两个指针从两端开始向中间靠拢，如果左端线段短于右端，那么左端右移，反之右端左移，直到左右两端移到中间重合。合理性在于：木桶的短边决定其高，当左端线段L小于右端线段R时，我们把L右移，这时舍弃的是L与右端其他线段（R-1, R-2,…）组成的木桶，这些木桶是没必要判断的，因为这些木桶的容积肯定都没有L和R组成的木桶容积大（木桶高为L的高，底变短了）。

## **12. Integer to Roman**【Medium】

Given an integer, convert it to a roman numeral.

Input is guaranteed to be within the range from 1 to 3999.

**Solution:**

```python
class Solution(object):
    def intToRoman(self, num):
        """
        :type num: int
        :rtype: str
        """
        n = [1000,100,10,1]
        s1 = ['M','C','X','I']
        s2 = ['' ,'D','L','V']
        s = ''
        for i in range(4):
            w = num / n[i]
            if w == 9:
                s =s + s1[i] + s1[i-1]
            elif w >= 5:
                s += s2[i]
                for j in range(w-5):
                    s += s1[i]
            elif w == 4:
                s = s + s1[i] + s2[i]
            else:
                for j in range(w):
                    s += s1[i]
            num = num % n[i]
        return s
```

**Discussion:**

```
罗马字符    I   V   X   L   C   D   M
整数数字    1   5   10  50  100 500 1000
```
- 相同的数字连写，所表示的数等于这些数字相加得到的数，如：III = 3；
- 小的数字在大的数字的右边，所表示的数等于这些数字相加得到的数， 如：VIII = 8；XII = 12； 

小的数字，（限于I、X 和C）在大的数字的左边，所表示的数等于大数减小数得到的数，如：IV= 4；IX= 9；
- 正常使用时，连写的数字重复不得超过三次。（表盘上的四点钟“IIII”例外）
- 在一个数的上面画一条横线，表示这个数扩大1000倍。（本题用不到这点）

有几条须注意掌握：
- 基本数字I、X、C中的任何一个，自身连用构成数目，或者放在大数的右边连用构成数目，都不能超过三个；放在大数的左边只能用一个。
- 不能把基本数字V、L、D中的任何一个作为小数放在大数的左边采用相减的方法构成数目；放在大数的右边采用相加的方式构成数目，只能使用一个。
- V和X左边的小数字只能用I，且只能有1个。
- L和C左边的小数字只能用X，且只能有1个。 

D和M左边的小数字只能用C，且只能有1个。

## **13. Roman to Integer** 【Easy】

Given a roman numeral, convert it to an integer.

Input is guaranteed to be within the range from 1 to 3999.

**Solution:**

```python
class Solution(object):
    def romanToInt(self, s):
        """
        :type s: str
        :rtype: int
        """
        roman = {'M': 1000,'D': 500 ,'C': 100,'L': 50,'X': 10,'V': 5,'I': 1}
        sum, lens = 0, len(s)
        for i in range(lens-1):
            if roman[s[i]] < roman[s[i+1]]:
                sum -= roman[s[i]]
            else:
                sum += roman[s[i]]
        return sum +roman[s[-1]]
```

## **14. Longest Common Prefix**【Easy】

Write a function to find the longest common prefix string amongst an array of strings.

**Solution:**

```python
class Solution(object):
    def longestCommonPrefix(self, strs):
        """
        :type strs: List[str]
        :rtype: str
        """
        if not strs:
            return ''
        lens = len(strs[0])
        s = strs[0]
        while lens > 0:
            flag = True
            for i in strs[1:]:
                if s != i[:lens]:
                    s, lens, flag = s[:-1], lens-1, False
                    break
            if flag:
                break
        return s
```

## **15. 3Sum**【Medium】

Given an array S of n integers, are there elements a, b, c in S such that a + b + c = 0? Find all unique triplets in the array which gives the sum of zero.

**Note:** The solution set must not contain duplicate triplets.

```
For example, given array S = [-1, 0, 1, 2, -1, -4],

A solution set is:
[
  [-1, 0, 1],
  [-1, -1, 2]
]
```

**Solution:**

```python
class Solution(object):
    def threeSum(self, nums):
        """
        :type nums: List[int]
        :rtype: List[List[int]]
        """
        nums.sort()
        result = []
        lenn = len(nums)
        for i in range(lenn-2):
            if i > 0 and nums[i] == nums[i-1]:
                continue
            l, r = i+1, lenn - 1
            while l < r:
                s = nums[i] + nums[l] + nums[r]
                if  s < 0:
                    l += 1    
                elif s > 0:
                    r -= 1
                else:
                    result.append([nums[i], nums[l], nums[r]])
                    while l < r and nums[l] == nums[l+1]:
                        l += 1
                    while l < r and nums[r] == nums[r-1]:
                        r -= 1
                    l, r = l+1, r-1
        return result
```

**Discussion:**

该问题可以归结为Ksum求和问：即给你一组N个数字，然后给你一个常数(比如target) ，目标是在这一堆数里面找到K个数字，使得这K个数字的和等于target。思路一是枚举法，复杂度是O(N^K)。思路二是先对数组排序，对于2sum问题，设置两个指针r，l分别指向头和尾，若这两个数的和等于target，则r++ 且l–；若小于target，则r++；若大于target，则l–。对于3sum问题，先取出一个数，接下来在剩下的数字里面找到两个数字使得他们的和等于(target – 那个取出的数)就可以。另一种是采用字典的方法：[http://blog.csdn.net/doc_sgl/article/details/12462151#comments](http://blog.csdn.net/doc_sgl/article/details/12462151#comments)



