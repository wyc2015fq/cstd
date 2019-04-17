# [LeetCode] Shortest Palindrome 最短回文串 - Grandyang - 博客园







# [[LeetCode] Shortest Palindrome 最短回文串](https://www.cnblogs.com/grandyang/p/4523624.html)







Given a string S, you are allowed to convert it to a palindrome by adding characters in front of it. Find and return the shortest palindrome you can find by performing this transformation.

For example:

Given `"aacecaaa"`, return `"aaacecaaa"`.

Given `"abcd"`, return `"dcbabcd"`.

**Credits:**
Special thanks to [@ifanchu](https://leetcode.com/discuss/user/ifanchu) for adding this problem and creating all test cases. Thanks to [@Freezen](https://leetcode.com/discuss/user/Freezen) for additional test cases.



这道题让我们求最短的回文串，LeetCode中关于回文串的其他的题目有[Palindrome Number 验证回文数字](http://www.cnblogs.com/grandyang/p/4125510.html)，[Validate Palindrome 验证回文字符串](http://www.cnblogs.com/grandyang/p/4030114.html)，[Palindrome Partitioning 拆分回文串](http://www.cnblogs.com/grandyang/p/4270008.html)，[Palindrome Partitioning II 拆分回文串之二](http://www.cnblogs.com/grandyang/p/4271456.html)和[Longest Palindromic Substring 最长回文串](http://www.cnblogs.com/grandyang/p/4464476.html)。题目让我们在给定字符串s的前面加上最少个字符，使之变成回文串，那么我们来看题目中给的两个例子，最坏的情况下是s中没有相同的字符，那么最小需要添加字符的个数为s.size() - 1个，第一个例子的字符串包含一个回文串，只需再在前面添加一个字符即可，还有一点需要注意的是，前面添加的字符串都是从s的末尾开始，一位一位往前添加的，那么我们只需要知道从s末尾开始需要添加到前面的个数。这道题如果用brute force无法通过OJ，所以我们需要用一些比较巧妙的方法来解。这里我们用到了KMP算法，KMP算法是一种专门用来匹配字符串的高效的算法，具体方法可以参见这篇博文[从头到尾彻底理解KMP](http://blog.csdn.net/v_july_v/article/details/7041827)。我们把s和其转置r连接起来，中间加上一个其他字符，形成一个新的字符串t，我们还需要一个和t长度相同的一位数组next，其中next[i]表示从t[i]到开头的子串的相同前缀后缀的个数，具体可参考KMP算法中解释。最后我们把不相同的个数对应的字符串添加到s之前即可，代码如下：



C++ 解法一：

```
class Solution {
public:
    string shortestPalindrome(string s) {
        string r = s;
        reverse(r.begin(), r.end());
        string t = s + "#" + r;
        vector<int> next(t.size(), 0);
        for (int i = 1; i < t.size(); ++i) {
            int j = next[i - 1];
            while (j > 0 && t[i] != t[j]) j = next[j - 1];
            next[i] = (j += t[i] == t[j]);
        }
        return r.substr(0, s.size() - next.back()) + s;
    }
};
```



Java 解法一：

```
public class Solution {
    public String shortestPalindrome(String s) {
        String r = new StringBuilder(s).reverse().toString();
        String t = s + "#" + r;
        int[] next = new int[t.length()];
        for (int i = 1; i < t.length(); ++i) {
            int j = next[i - 1];
            while (j > 0 && t.charAt(i) != t.charAt(j)) j = next[j - 1];
            j += (t.charAt(i) == t.charAt(j)) ? 1 : 0;
            next[i] = j;
        }
        return r.substring(0, s.length() - next[t.length() - 1]) + s;
    }
}
```



从上面的Java和C++的代码中，我们可以看出来C++和Java在使用双等号上的明显的不同，感觉Java对于双等号对使用更加苛刻一些，比如Java中的双等号只对primitive类数据结构(比如int, char等)有效，但是即便有效，也不能将结果直接当1或者0来用。而对于一些从Object派生出来的类，比如Integer或者String等，不能直接用双等号来比较，而是要用其自带的equals()函数来比较，因为双等号判断的是不是同一个对象，而不是他们所表示的值是否相同。同样需要注意的是，Stack的peek()函数取出的也是对象，不能直接和另一个Stack的peek()取出的对象直接双等，而是使用equals或者先将其中一个强行转换成primitive类，再和另一个强行比较。

下面这种方法的写法比较简洁，虽然不是明显的KMP算法，但是也有其的影子在里面，首先我们还是先将其的翻转字符串搞出来，然后比较原字符串s的前缀后翻转字符串t的对应位置的后缀是否相等，起始位置是比较s和t是否相等，如果相等，说明s本身就是回文串，不用添加任何字符，直接返回即可；如果不想等，s去掉最后一位，t去掉第一位，继续比较，以此类推直至有相等，或者循环结束，这样我们就能将两个字符串在正确的位置拼接起来了。很有意思的是，这种方法对应Java写法却会TLE，无法通过OJ。



C++ 解法二：

```
class Solution {
public:
    string shortestPalindrome(string s) {
        string t = s;
        reverse(t.begin(), t.end());
        int n = s.size(), i = 0;
        for (i = n; i >= 0; --i) {
            if (s.substr(0, i) == t.substr(n - i)) {
                break;
            }
        }
        return t.substr(0, n - i) + s;
    }
};
```



下面这种Java写法也是在找相同的前缀后缀，但是并没有每次把前缀后缀取出来比较，而是用两个指针分别指向对应的位置比较，然后用end指向相同后缀的起始位置，最后再根据end的值来拼接两个字符串。有意思的是这种方法对应的C++写法会TLE，跟上面正好相反，那么我们是否能得出Java的substring操作略慢，而C++的reverse略慢呢，我也仅仅是猜测而已。



Java 解法三：

```
public class Solution {
    public String shortestPalindrome(String s) {
        int i = 0, end = s.length() - 1, j = end;
        char arr = s.toCharArray();
        while (i < j) {
            if (arr[i] == arr[j]) {
                ++i; --j;
            } else {
                i = 0; --end; j = end;
            }
        }
        return new StringBuilder(s.substring(end + 1)).reverse().toString() + s;
    }
}
```



参考资料：

[http://blog.csdn.net/v_july_v/article/details/7041827](http://blog.csdn.net/v_july_v/article/details/7041827)

[https://leetcode.com/discuss/36807/c-8-ms-kmp-based-o-n-time-%26-o-n-memory-solution](https://leetcode.com/discuss/36807/c-8-ms-kmp-based-o-n-time-%26-o-n-memory-solution)

[https://discuss.leetcode.com/topic/14770/my-easily-understandable-but-time-consuming-c-solution](https://discuss.leetcode.com/topic/14770/my-easily-understandable-but-time-consuming-c-solution)

[https://discuss.leetcode.com/topic/25860/my-9-lines-three-pointers-java-solution-with-explanation](https://discuss.leetcode.com/topic/25860/my-9-lines-three-pointers-java-solution-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












