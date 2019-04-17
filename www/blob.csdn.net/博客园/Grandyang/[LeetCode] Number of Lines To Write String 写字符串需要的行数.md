# [LeetCode] Number of Lines To Write String 写字符串需要的行数 - Grandyang - 博客园







# [[LeetCode] Number of Lines To Write String 写字符串需要的行数](https://www.cnblogs.com/grandyang/p/9348596.html)







We are to write the letters of a given string `S`, from left to right into lines. Each line has maximum width 100 units, and if writing a letter would cause the width of the line to exceed 100 units, it is written on the next line. We are given an array `widths`, an array where widths[0] is the width of 'a', widths[1] is the width of 'b', ..., and widths[25] is the width of 'z'.

Now answer two questions: how many lines have at least one character from `S`, and what is the width used by the last such line? Return your answer as an integer list of length 2.


Example :
Input: 
widths = [10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10]
S = "abcdefghijklmnopqrstuvwxyz"
Output: [3, 60]
Explanation: 
All letters have the same length of 10. To write all 26 letters,
we need two full lines and one line with 60 units.
Example :
Input: 
widths = [4,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10]
S = "bbbcccdddaaa"
Output: [2, 4]
Explanation: 
All letters except 'a' have the same length of 10, and 
"bbbcccdddaa" will cover 9 * 10 + 2 * 4 = 98 units.
For the last 'a', it is written on the second line because
there is only 2 units left in the first line.
So the answer is 2 lines, plus 4 units in the second line.



Note:
- The length of `S` will be in the range [1, 1000].
- `S` will only contain lowercase letters.
- `widths` is an array of length `26`.
- `widths[i]` will be in the range of `[2, 10]`.



这道题给了我们一个字符串，让我们把里面的字母写下来，规定了每一行的长度为100，然后每个字母的长度可以在widths数组中查询，说是如果某一个字母加上后超过了长度100的限制，那么就移动到下一行，问我们最终需要多少行，和最后一行的长度。这道题并没有太大的难度和技巧，就是楞头写呗，遍历所有的字母，然后查表得到其宽度，然后看加上这个新宽度是否超了100，超了的话，行数计数器自增1，并且当前长度为这个字母的长度，因为另起了一行。如果没超100，那么行长度就直接加上这个字母的长度。遍历完成后返回行数和当前行长度即可，参见代码如下：



```
class Solution {
public:
    vector<int> numberOfLines(vector<int>& widths, string S) {
        int cnt = 1, cur = 0;
        for (char c : S) {
            int t = widths[c - 'a'];
            if (cur + t > 100) ++cnt;
            cur = (cur + t > 100) ? t : cur + t;
        }
        return {cnt, cur};
    }
};
```



参考资料：

[https://leetcode.com/problems/number-of-lines-to-write-string/solution/](https://leetcode.com/problems/number-of-lines-to-write-string/solution/)

[https://leetcode.com/problems/number-of-lines-to-write-string/discuss/120666/Easy-Solution-6-lines-C++JavaPython](https://leetcode.com/problems/number-of-lines-to-write-string/discuss/120666/Easy-Solution-6-lines-C++JavaPython)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












