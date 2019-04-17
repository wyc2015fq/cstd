# [LeetCode] Valid Palindrome 验证回文字符串 - Grandyang - 博客园







# [[LeetCode] Valid Palindrome 验证回文字符串](https://www.cnblogs.com/grandyang/p/4030114.html)







Given a string, determine if it is a palindrome, considering only alphanumeric characters and ignoring cases.

 For example,
`"A man, a plan, a canal: Panama"` is a palindrome.
`"race a car"` is *not* a palindrome.


**Note:**

Have you consider that the string might be empty? This is a good question to ask during an interview.


For the purpose of this problem, we define empty string as valid palindrome.




验证回文字符串是比较常见的问题，所谓回文，就是一个正读和反读都一样的字符串，比如“level”或者“noon”等等就是回文串。但是这里，加入了空格和非字母数字的字符，增加了些难度，但其实原理还是很简单：只需要建立两个指针，left和right, 分别从字符的开头和结尾处开始遍历整个字符串，如果遇到非字母数字的字符就跳过，继续往下找，直到找到下一个字母数字或者结束遍历，如果遇到大写字母，就将其转为小写。等左右指针都找到字母数字时，比较这两个字符，若相等，则继续比较下面两个分别找到的字母数字，若不相等，直接返回false. 

时间复杂度为O(n), 代码如下：



解法一：

```
class Solution {
public:
    bool isPalindrome(string s) {
        int left = 0, right = s.size() - 1 ;
        while (left < right) {
            if (!isAlphaNum(s[left])) ++left;
            else if (!isAlphaNum(s[right])) --right;
            else if ((s[left] + 32 - 'a') %32 != (s[right] + 32 - 'a') % 32) return false;
            else {
                ++left; --right;
            }
        }
        return true;
    }
    bool isAlphaNum(char &ch) {
        if (ch >= 'a' && ch <= 'z') return true;
        if (ch >= 'A' && ch <= 'Z') return true;
        if (ch >= '0' && ch <= '9') return true;
        return false;
    }
};
```



我们也可以用系统自带的判断是否是数母字符的判断函数isalnum，参见代码如下；



解法二：

```
class Solution {
public:
    bool isPalindrome(string s) {
        int left = 0, right = s.size() - 1 ;
        while (left < right) {
            if (!isalnum(s[left])) ++left;
            else if (!isalnum(s[right])) --right;
            else if ((s[left] + 32 - 'a') %32 != (s[right] + 32 - 'a') % 32) return false;
            else {
                ++left; --right;
            }
        }
        return true;
    }
};
```



对于该问题的扩展，还有利用Manacher算法来求解最长回文字符串问题，参见我的另一篇博文[Manacher's Algorithm 马拉车算法](http://www.cnblogs.com/grandyang/p/4475985.html)。



参考资料：

[https://discuss.leetcode.com/topic/5581/here-s-a-clean-c-solution/2](https://discuss.leetcode.com/topic/5581/here-s-a-clean-c-solution/2)

[https://discuss.leetcode.com/topic/25405/my-three-line-java-solution](https://discuss.leetcode.com/topic/25405/my-three-line-java-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












