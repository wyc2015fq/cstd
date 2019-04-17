# [LeetCode] Reverse Words in a String II 翻转字符串中的单词之二 - Grandyang - 博客园







# [[LeetCode] Reverse Words in a String II 翻转字符串中的单词之二](https://www.cnblogs.com/grandyang/p/5186294.html)







Given an input string, reverse the string word by word. 

Example:
Input:  ["t","h","e"," ","s","k","y"," ","i","s"," ","b","l","u","e"]
Output: ["b","l","u","e"," ","i","s"," ","s","k","y"," ","t","h","e"]
Note: 
- A word is defined as a sequence of non-space characters.
- The input string does not contain leading or trailing spaces.
- The words are always separated by a single space.

Follow up: Could you do it *in-place* without allocating extra space?



这道题让我们翻转一个字符串中的单词，跟之前那题 [Reverse Words in a String](http://www.cnblogs.com/grandyang/p/4606676.html) 没有区别，由于之前那道题我们就是用in-place的方法做的，而这道题反而更简化了题目，因为不考虑首尾空格了和单词之间的多空格了，方法还是很简单，先把每个单词翻转一遍，再把整个字符串翻转一遍，或者也可以调换个顺序，先翻转整个字符串，再翻转每个单词，参见代码如下：



解法一：

```
class Solution {
public:
    void reverseWords(vector<char>& str) {
        int left = 0, n = str.size();
        for (int i = 0; i <= n; ++i) {
            if (i == n || str[i] == ' ') {
                reverse(str, left, i - 1);
                left = i + 1;
            }
        }
        reverse(str, 0, n - 1);
    }
    void reverse(vector<char>& str, int left, int right) {
        while (left < right) {
            char t = str[left];
            str[left] = str[right];
            str[right] = t;
            ++left; --right;
        }
    }
};
```



我们也可以使用C++ STL中自带的reverse函数来做，我们先把整个字符串翻转一下，然后再来扫描每个字符，用两个指针，一个指向开头，另一个开始遍历，遇到空格停止，这样两个指针之间就确定了一个单词的范围，直接调用reverse函数翻转，然后移动头指针到下一个位置，在用另一个指针继续扫描，重复上述步骤即可，参见代码如下：



解法二：

```
class Solution {
public:
    void reverseWords(vector<char>& str) {
        reverse(str.begin(), str.end());
        for (int i = 0, j = 0; i < str.size(); i = j + 1) {
            for (j = i; j < str.size(); ++j) {
                if (str[j] == ' ') break;
            }
            reverse(str.begin() + i, str.begin() + j);
        }
    }
};
```



类似题目：

[Reverse Words in a String III](http://www.cnblogs.com/grandyang/p/6703311.html)

[Reverse Words in a String](http://www.cnblogs.com/grandyang/p/4606676.html)

[Rotate Array](http://www.cnblogs.com/grandyang/p/4298711.html)



参考资料：

[https://leetcode.com/problems/reverse-words-in-a-string-ii/](https://leetcode.com/problems/reverse-words-in-a-string-ii/)

[https://leetcode.com/problems/reverse-words-in-a-string-ii/discuss/53851/Six-lines-solution-in-C%2B%2B](https://leetcode.com/problems/reverse-words-in-a-string-ii/discuss/53851/Six-lines-solution-in-C%2B%2B)

[https://leetcode.com/problems/reverse-words-in-a-string-ii/discuss/53775/My-Java-solution-with-explanation](https://leetcode.com/problems/reverse-words-in-a-string-ii/discuss/53775/My-Java-solution-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












