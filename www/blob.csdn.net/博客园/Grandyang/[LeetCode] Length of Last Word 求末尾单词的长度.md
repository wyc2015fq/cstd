# [LeetCode] Length of Last Word 求末尾单词的长度 - Grandyang - 博客园







# [[LeetCode] Length of Last Word 求末尾单词的长度](https://www.cnblogs.com/grandyang/p/4086298.html)







Given a string *s* consists of upper/lower-case alphabets and empty space characters `' '`, return the length of last word in the string.

If the last word does not exist, return 0.

Note: A word is defined as a character sequence consists of non-space characters only.

For example, 
Given *s* = `"Hello World"`,
return `5`.



这道题难度不是很大。先对输入字符串做预处理，去掉开头和结尾的空格，然后用一个计数器来累计非空格的字符串的长度，遇到空格则将计数器清零。代码如下：



解法一：

```
class Solution {
public:
    int lengthOfLastWord(const char *s) {
        int len = strlen(s);
        int left = 0;
        int right = len - 1;
        int count = 0;
        while (s[left] == ' ') ++left;
        while (s[right] == ' ') --right;
        for (int i = left; i <= right; ++i) {
            if (s[i] == ' ') count = 0;
            else ++count;
        }
        return count;
    }
};
```



昨晚睡觉前又想到了一种解法，其实不用上面那么复杂的，我们关心的主要是非空格的字符，那么我们实际上在遍历字符串的时候，如果遇到非空格的字符，我们只需要判断其前面一个位置的字符是否为空格，如果是的话，那么当前肯定是一个新词的开始，将计数器重置为1，如果不是的话，说明正在统计一个词的长度，计数器自增1即可。但是需要注意的是，当i=0的时候，无法访问前一个字符，所以这种情况要特别判断一下，归为计数器自增1那类。参见代码如下：



解法二：

```
class Solution {
public:
    int lengthOfLastWord(string s) {
        int res = 0;
        for (int i = 0; i < s.size(); ++i) {
            if (s[i] != ' ') {
                if (i != 0 && s[i - 1] == ' ') res = 1;
                else ++res;
            }
        }
        return res;
    }
};
```



下面这种方法是第一种解法的优化版本，由于我们只关于最后一个单词的长度，所以开头有多少个空格起始我们并不在意，我们从字符串末尾开始，先将末尾的空格都去掉，然后开始找非空格的字符的长度即可，参见代码如下：



解法三：

```
class Solution {
public:
    int lengthOfLastWord(string s) {
        int right = s.size() - 1, res = 0;
        while (right >= 0 && s[right] == ' ') --right;
        while (right >= 0 && s[right] != ' ' ) {
            --right; 
            ++res;
        }
        return res;
    }
};
```



这道题用Java来做可以一行搞定，请参见[这个帖子](https://discuss.leetcode.com/topic/6055/a-single-line-of-code-in-java).



参考资料：

[https://discuss.leetcode.com/topic/17312/7-lines-4ms-c-solution](https://discuss.leetcode.com/topic/17312/7-lines-4ms-c-solution)

[https://discuss.leetcode.com/topic/6055/a-single-line-of-code-in-java](https://discuss.leetcode.com/topic/6055/a-single-line-of-code-in-java)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












