# [LeetCode] Reverse Words in a String III 翻转字符串中的单词之三 - Grandyang - 博客园







# [[LeetCode] Reverse Words in a String III 翻转字符串中的单词之三](https://www.cnblogs.com/grandyang/p/6703311.html)







Given a string, you need to reverse the order of characters in each word within a sentence while still preserving whitespace and initial word order.

Example 1:
Input: "Let's take LeetCode contest"
Output: "s'teL ekat edoCteeL tsetnoc"



Note: In the string, each word is separated by single space and there will not be any extra space in the string.



这道题让我们翻转字符串中的每个单词，感觉整体难度要比之前两道[Reverse Words in a String II](http://www.cnblogs.com/grandyang/p/5186294.html)和[Reverse Words in a String](http://www.cnblogs.com/grandyang/p/4606676.html)要小一些，由于题目中说明了没有多余空格，使得难度进一步的降低了。首先我们来看使用字符流处理类stringstream来做的方法，相当简单，就是按顺序读入每个单词进行翻转即可，参见代码如下：



解法一：

```
class Solution {
public:
    string reverseWords(string s) {
        string res = "", t = "";
        istringstream is(s);
        while (is >> t) {
            reverse(t.begin(), t.end());
            res += t + " ";
        }
        res.pop_back();
        return res;
    }
};
```



下面我们来看不使用字符流处理类，也不使用STL内置的reverse函数的方法，那么就是用两个指针，分别指向每个单词的开头和结尾位置，确定了单词的首尾位置后，再用两个指针对单词进行首尾交换即可，有点像验证回文字符串的方法，参见代码如下：



解法二：

```
class Solution {
public:
    string reverseWords(string s) {
        int start = 0, end = 0, n = s.size();
        while (start < n && end < n) {
            while (end < n && s[end] != ' ') ++end;
            for (int i = start, j = end - 1; i < j; ++i, --j) {
                swap(s[i], s[j]);
            }
            start = ++end;
        }
        return s;
    }
};
```



类似题目：

[Reverse Words in a String II](http://www.cnblogs.com/grandyang/p/5186294.html)

[Reverse Words in a String](http://www.cnblogs.com/grandyang/p/4606676.html)



参考资料：

[https://discuss.leetcode.com/topic/85773/nothing-fancy-straight-java-stringbuilder](https://discuss.leetcode.com/topic/85773/nothing-fancy-straight-java-stringbuilder)

[https://discuss.leetcode.com/topic/85797/java-two-methods-3-line-using-built-in-and-char-array](https://discuss.leetcode.com/topic/85797/java-two-methods-3-line-using-built-in-and-char-array)


[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












