# Palindrome Number ——解题笔记 - bigfacesafdasgfewgf - CSDN博客





2015年05月06日 10:47:16[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：716











    【题目】 Determine whether an integer is a palindrome. Do this without extra space.


click to show spoilers.


Some hints:

Could negative integers be palindromes? (ie, -1)


If you are thinking of converting the integer to string, note the restriction of using extra space.


You could also try reversing an integer. However, if you have solved the problem "Reverse Integer", you know that the reversed integer might overflow. How would you handle such case?


There is a more generic way of solving this problem.







    解法：把int类型的数逐个分解，所有元素放在一个vector中，然后判断这个vector是否是左右对称的。

    注意：vector中push_back添加元素的用法；vector可以直接用下表取数。




    代码如下：



```cpp
class Solution {
public:
    bool isPalindrome(int x) {
        if(x < 0)
            return false;
            
        vector<int> nums;
        while(x)
        {
            nums.push_back(x%10);
            x /= 10;
        }
        
        int len = nums.size();
        for(int i = 0; i < len/2; i++)
        {
            if(nums[i] != nums[len - 1 - i])  % 下标要注意
                return false;
        }
        return true;
    }
};
```





