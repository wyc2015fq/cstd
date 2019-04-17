# [LeetCode] Reverse Words in a String 翻转字符串中的单词 - Grandyang - 博客园







# [[LeetCode] Reverse Words in a String 翻转字符串中的单词](https://www.cnblogs.com/grandyang/p/4606676.html)







Given an input string, reverse the string word by word.

 For example,

Given s = "`the sky is blue`",

return "`blue is sky the`".


**Update (2015-02-12):**

For C programmers: Try to solve it *in-place* in *O*(1) space.


[click to show clarification.](https://leetcode.com/problems/reverse-words-in-a-string/)

**Clarification:**
- What constitutes a word?

A sequence of non-space characters constitutes a word.
- Could the input string contain leading or trailing spaces?

Yes. However, your reversed string should not contain leading or trailing spaces.
- How about multiple spaces between two words?

Reduce them to a single space in the reversed string.



这道题让我们翻转字符串中的单词，题目中给了我们写特别说明，如果单词之间遇到多个空格，只能返回一个，而且首尾不能有单词，并且对C语言程序员要求空间复杂度为O(1)，所以我们只能对原字符串s之间做修改，而不能声明新的字符串。那么我们如何翻转字符串中的单词呢，我们的做法是，先整个字符串整体翻转一次，然后再分别翻转每一个单词（或者先分别翻转每一个单词，然后再整个字符串整体翻转一次），此时就能得到我们需要的结果了。那么这里我们需要定义一些变量来辅助我们解题，storeIndex表示当前存储到的位置，n为字符串的长度。我们先给整个字符串反转一下，然后我们开始循环，遇到空格直接跳过，如果是非空格字符，我们此时看storeIndex是否为0，为0的话表示第一个单词，不用增加空格；如果不为0，说明不是第一个单词，需要在单词中间加一个空格，然后我们要找到下一个单词的结束位置我们用一个while循环来找下一个为空格的位置，在此过程中继续覆盖原字符串，找到结束位置了，下面就来翻转这个单词，然后更新i为结尾位置，最后遍历结束，我们剪裁原字符串到storeIndex位置，就可以得到我们需要的结果，代码如下：



C++ 解法一：

```
class Solution {
public:
    void reverseWords(string &s) {
        int storeIndex = 0, n = s.size();
        reverse(s.begin(), s.end());
        for (int i = 0; i < n; ++i) {
            if (s[i] != ' ') {
                if (storeIndex != 0) s[storeIndex++] = ' ';
                int j = i;
                while (j < n && s[j] != ' ') s[storeIndex++] = s[j++];
                reverse(s.begin() + storeIndex - (j - i), s.begin() + storeIndex);
                i = j;
            }
        }
        s.resize(storeIndex);
    }
};
```



Java解法一：

```
public class Solution {
    public String reverseWords(String s) {
        int storeIndex = 0, n = s.length();
        StringBuilder sb = new StringBuilder(s).reverse();
        for (int i = 0; i < n; ++i) {
            if (sb.charAt(i) != ' ') {
                if (storeIndex != 0) sb.setCharAt(storeIndex++, ' ');
                int j = i;
                while (j < n && sb.charAt(j) != ' ') sb.setCharAt(storeIndex++, sb.charAt(j++));
                String t = new StringBuilder(sb.substring(storeIndex - (j - i), storeIndex)).reverse().toString();
                sb.replace(storeIndex - (j - i), storeIndex, t);
                i = j;
            }
        }
        sb.setLength(storeIndex);
        return sb.toString();
    }
}
```



下面我们来看使用字符串流类stringstream的解法，我们先把字符串装载入字符串流中，然后定义一个临时变量tmp，然后把第一个单词赋给s，这里需要注意的是，如果含有非空格字符，那么每次>>操作就会提取连在一起的非空格字符，那么我们每次将其加在s前面即可；如果原字符串为空，那么就不会进入while循环；如果原字符串为许多空格字符连在一起，那么第一个>>操作就会提取出这些空格字符放入s中，然后不进入while循环，这时候我们只要判断一下s的首字符是否为空格字符，是的话就将s清空即可，参见代码如下：



C++ 解法二：

```
class Solution {
public:
    void reverseWords(string &s) {
        istringstream is(s);
        string tmp;
        is >> s;
        while(is >> tmp) s = tmp + " " + s;
        if(!s.empty() && s[0] == ' ') s = "";
    }
};
```



下面这种方法也是使用stringstream来做，但是我们使用了getline来做，第三个参数是设定分隔字符，我们用空格字符来分隔，这个跟上面的>>操作是有不同的，每次只能过一个空格字符，如果有多个空格字符连在一起，那么t会赋值为空字符串，所以我们在处理t的时候首先要判断其是否为空，是的话直接跳过，参见代码如下：



C++ 解法三：

```
class Solution {
public:
    void reverseWords(string &s) {
        istringstream is(s);
        s = "";
        string t = "";
        while (getline(is, t, ' ')) {
            if (t.empty()) continue;
            s = (s.empty() ? t : (t + " " + s));
        }
    }
};
```



而如果我们使用Java的String的split函数来做的话就非常简单了，没有那么多的幺蛾子，简单明了，我们首先将原字符串调用trim()来去除冗余空格，然后调用split()来分隔，分隔符设为"\\s+"，这其实是一个正则表达式，\\s表示空格字符，+表示可以有一个或多个空格字符，那么我们就可以把单词分隔开装入一个字符串数组中，然后我们从末尾开始，一个个把单词取出来加入结果res中，并且单词之间加上空格字符，注意我们把第一个单词留着不取，然后返回的时候再加上即可，参见代码如下：



Java解法二：

```
public class Solution {
    public String reverseWords(String s) {
        String res = "";
        String[] words = s.trim().split("\\s+");
        for (int i = words.length - 1; i > 0; --i) {
            res += words[i] + " ";   
        }
        return res + words[0];
    }
}
```



下面这种方法就更加的简单了，疯狂的利用到了Java的内置函数，这也是Java的强大之处，注意这里的分隔符没有用正则表达式，而是直接放了个空格符进去，后面还是有+号，跟上面的写法得到的效果是一样的，然后我们对字符串数组进行翻转，然后调用join()函数来把字符串数组拼接成一个字符串，中间夹上空格符即可，参见代码如下：



Java解法三：

```
public class Solution {
    public String reverseWords(String s) {
        String[] words = s.trim().split(" +");
        Collections.reverse(Arrays.asList(words));
        return String.join(" ", words);
    }
}
```



类似题目：

[Reverse Words in a String II](http://www.cnblogs.com/grandyang/p/5186294.html)



参考资料：

[https://discuss.leetcode.com/topic/3298/in-place-simple-solution](https://discuss.leetcode.com/topic/3298/in-place-simple-solution)

[https://discuss.leetcode.com/topic/2742/my-accepted-java-solution](%20https://discuss.leetcode.com/topic/2742/my-accepted-java-solution)

[https://discuss.leetcode.com/topic/11785/java-3-line-builtin-solution](https://discuss.leetcode.com/topic/11785/java-3-line-builtin-solution)

[https://discuss.leetcode.com/topic/10199/5-lines-c-using-stringstream](https://discuss.leetcode.com/topic/10199/5-lines-c-using-stringstream)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












