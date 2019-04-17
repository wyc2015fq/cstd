# [LeetCode] Excel Sheet Column Number 求Excel表列序号 - Grandyang - 博客园







# [[LeetCode] Excel Sheet Column Number 求Excel表列序号](https://www.cnblogs.com/grandyang/p/4225316.html)





Related to question [Excel Sheet Column Title](https://oj.leetcode.com/problems/excel-sheet-column-title/)

Given a column title as appear in an Excel sheet, return its corresponding column number.

For example:
    A -> 1
    B -> 2
    C -> 3
    ...
    Z -> 26
    AA -> 27
    AB -> 28 
**Credits:**
Special thanks to [@ts](https://oj.leetcode.com/discuss/user/ts) for adding this problem and creating all test cases.



这题实际上相当于一种二十六进制转十进制的问题，并不难，只要一位一位的转换即可。代码如下：



```
class Solution {
public:
    int titleToNumber(string s) {
        int n = s.size();
        int res = 0;
        int tmp = 1;
        for (int i = n; i >= 1; --i) {
            res += (s[i - 1] - 'A' + 1) * tmp; 
            tmp *= 26;
        }
        return res;
    }
};
```



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












