# [LeetCode] To Lower Case 转为小写 - Grandyang - 博客园







# [[LeetCode] To Lower Case 转为小写](https://www.cnblogs.com/grandyang/p/10018055.html)







Implement function ToLowerCase() that has a string parameter str, and returns the same string in lowercase.




Example 1:
Input: "Hello"
Output: "hello"


Example 2:
Input: "here"
Output: "here"


Example 3:
Input: "LOVELY"
Output: "lovely"





这道题让我们将单词转为小写，是一道比较简单的题目，我们都知道小写字母比其对应的大写字母的ASCII码大32，所以我们只需要遍历字符串，对于所有的大写字母，统统加上32即可，参见代码如下：



```
class Solution {
public:
    string toLowerCase(string str) {
        for (char &c : str) {
            if (c >= 'A' && c <= 'Z') c += 32;
        }
        return str;
    }
};
```



参考资料：

[https://leetcode.com/problems/to-lower-case/](https://leetcode.com/problems/to-lower-case/)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












