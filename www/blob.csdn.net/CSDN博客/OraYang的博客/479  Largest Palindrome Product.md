
# 479. Largest Palindrome Product - OraYang的博客 - CSDN博客

2017年08月20日 09:42:38[OraYang](https://me.csdn.net/u010665216)阅读数：250所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Find the largest palindrome made from the product of two n-digit numbers.
Since the result could be very large, you should return the largest palindrome mod 1337.
Example:
Input: 2
Output: 987
Explanation: 99 x 91 = 9009, 9009 % 1337 = 987
Note:
The range of n is [1,8].
思路：本题属于简单题，求最大回文，直接的思路是构造回文并比较，但是感觉这样做不简单，于是思考，回文特征，发现两数成绩构造的最大回文是有限个数：9, 9009, 906609, 99000099, 9966006699, 999000000999,99956644665999, 9999000000009999，所以本题使用枚举法：
代码：

```python
class Solution {
public:
    int largestPalindrome(int n) {
        vector<long> res = {9, 9009, 906609, 99000099, 9966006699, 999000000999, \
                    99956644665999, 9999000000009999};
        return int(res[n - 1] % 1337);    
    }
};
```


