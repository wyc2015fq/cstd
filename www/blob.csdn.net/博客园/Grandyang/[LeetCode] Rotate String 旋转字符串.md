# [LeetCode] Rotate String 旋转字符串 - Grandyang - 博客园







# [[LeetCode] Rotate String 旋转字符串](https://www.cnblogs.com/grandyang/p/9251578.html)







We are given two strings, `A` and `B`.

A *shift on `A`* consists of taking string `A` and moving the leftmost character to the rightmost position. For example, if `A = 'abcde'`, then it will be `'bcdea'` after one shift on `A`. Return `True` if and only if `A` can become `B` after some number of shifts on `A`.
Example 1:
Input: A = 'abcde', B = 'cdeab'
Output: true

Example 2:
Input: A = 'abcde', B = 'abced'
Output: false

Note:
- `A` and `B` will have length at most `100`.



这道题给了我们两个字符串A和B，定义了一种偏移操作，以某一个位置将字符串A分为两截，并将两段调换位置，如果此时跟字符串B相等了，就说明字符串A可以通过偏移得到B。现在就是让我们判断是否存在这种偏移，那么最简单最暴力的方法就是遍历所有能将A分为两截的位置，然后用取子串的方法将A断开，交换顺序，再去跟B比较，如果相等，返回true即可，遍历结束后，返回false，参见代码如下：



解法一：

```
class Solution {
public:
    bool rotateString(string A, string B) {
        if (A.size() != B.size()) return false;
        for (int i = 0; i < A.size(); ++i) {
            if (A.substr(i, A.size() - i) + A.substr(0, i) == B) return true;
        }      
        return false;
    }
};
```



还有一种一行完成碉堡了的方法，就是我们其实可以在A之后再加上一个A，这样如果新的字符串(A+A)中包含B的话，说明A一定能通过偏移得到B。就比如题目中的例子，A="abcde", B="bcdea"，那么A+A="abcdeabcde"，里面是包括B的，所以返回true即可，参见代码如下：



解法二：

```
class Solution {
public:
    bool rotateString(string A, string B) {
        return A.size() == B.size() && (A + A).find(B) != string::npos;
    }
};
```



参考资料：

[https://leetcode.com/problems/rotate-string/solution/](https://leetcode.com/problems/rotate-string/solution/)

[https://leetcode.com/problems/rotate-string/discuss/118696/C++-Java-Python-1-Line-Solution](https://leetcode.com/problems/rotate-string/discuss/118696/C++-Java-Python-1-Line-Solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












