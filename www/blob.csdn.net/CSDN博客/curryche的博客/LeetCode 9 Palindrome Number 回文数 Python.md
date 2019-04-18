# LeetCode 9. Palindrome Number 回文数 Python - curryche的博客 - CSDN博客





2018年06月04日 21:00:33[curryche](https://me.csdn.net/whwan11)阅读数：108








Determine whether an integer is a palindrome. An integer is a palindrome when it reads the same backward as forward.

Example 1: 

Input: 121 

Output: true
Example 2: 

Input: -121 

Output: false 

Explanation: From left to right, it reads -121. From right to left, it becomes 121-. Therefore it is not a palindrome.
Example 3: 

Input: 10 

Output: false 

Explanation: Reads 01 from right to left. Therefore it is not a palindrome.
题目：判断一个是否是回文数 

思路：很直观的做法，首先计算输入的number的位数，然后从两边分别比较，一旦发现不对称就返回false。

```python
class Solution(object):
    def isPalindrome(self, x):
        """
        :type x: int
        :rtype: bool
        """
        if x<0:
            return False

        digits=1
        while(x/digits>=10):
            digits*=10

        num=x
        while(digits>1):
            right=num%10
            left=num/digits
            if right!=left:
                return False
            else:
                num=(num%digits)/10
                digits/=100

        return True
```

运行结果：279ms，beat 78% 的solution 
![提交结果分布](https://img-blog.csdn.net/2018060420525032?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dod2FuMTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)






