# [LeetCode] Complex Number Multiplication 复数相乘 - Grandyang - 博客园







# [[LeetCode] Complex Number Multiplication 复数相乘](https://www.cnblogs.com/grandyang/p/6660437.html)







Given two strings representing two [complex numbers](https://en.wikipedia.org/wiki/Complex_number).

You need to return a string representing their multiplication. Note i2 = -1 according to the definition.

**Example 1:**

**Input:** "1+1i", "1+1i"
**Output:** "0+2i"
**Explanation:** (1 + i) * (1 + i) = 1 + i
2
 + 2 * i = 2i, and you need convert it to the form of 0+2i.

**Example 2:**

**Input:** "1+-1i", "1+-1i"
**Output:** "0+-2i"
**Explanation:** (1 - i) * (1 - i) = 1 + i
2
 - 2 * i = -2i, and you need convert it to the form of 0+-2i.

**Note:**
- The input strings will not have extra blank.
- The input strings will be given in the form of **a+bi**, where the integer **a** and **b** will both belong to the range of [-100, 100]. And **the output should be also in this form**.



这道题让我们求复数的乘法，有关复数的知识最早还是在本科的复变函数中接触到的，难起来还真是难。但是这里只是最简单的乘法，只要利用好定义i2=-1就可以解题，而且这道题的另一个考察点其实是对字符的处理，我们需要把字符串中的实部和虚部分离开并进行运算，那么我们可以用STL中自带的find_last_of函数来找到加号的位置，然后分别拆出实部虚部，进行运算后再变回字符串，参见代码如下：



解法一：

```
class Solution {
public:
    string complexNumberMultiply(string a, string b) {
        int n1 = a.size(), n2 = b.size();
        auto p1 = a.find_last_of("+"), p2 = b.find_last_of("+");
        int a1 = stoi(a.substr(0, p1)), b1 = stoi(b.substr(0, p2));
        int a2 = stoi(a.substr(p1 + 1, n1 - p1 - 2));
        int b2 = stoi(b.substr(p2 + 1, n2 - p2 - 2));
        int r1 = a1 * b1 - a2 * b2, r2 = a1 * b2 + a2 * b1;
        return to_string(r1) + "+" + to_string(r2) + "i";
    }
};
```



下面这种方法利用到了字符串流类istringstream来读入字符串，直接将实部虚部读入int变量中，注意中间也要把加号读入char变量中，然后再进行运算即可，参见代码如下：



解法二：

```
class Solution {
public:
    string complexNumberMultiply(string a, string b) {
        istringstream is1(a), is2(b);
        int a1, a2, b1, b2, r1, r2;
        char plus;
        is1 >> a1 >> plus >> a2;
        is2 >> b1 >> plus >> b2;
        r1 = a1 * b1 - a2 * b2, r2 = a1 * b2 + a2 * b1;
        return to_string(r1) + "+" + to_string(r2) + "i";
    }
};
```



下面这种解法实际上是C语言的解法，用到了sscanf这个读入字符串的函数，需要把string转为cost char*型，然后标明读入的方式和类型，再进行运算即可，参见代码如下：



解法三：

```
class Solution {
public:
    string complexNumberMultiply(string a, string b) {
        int a1, a2, b1, b2, r1, r2;
        sscanf(a.c_str(), "%d+%di", &a1, &a2);
        sscanf(b.c_str(), "%d+%di", &b1, &b2);
        r1 = a1 * b1 - a2 * b2, r2 = a1 * b2 + a2 * b1;
        return to_string(r1) + "+" + to_string(r2) + "i";
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/84261/java-3-liner](https://discuss.leetcode.com/topic/84261/java-3-liner)

[https://discuss.leetcode.com/topic/84382/c-using-stringstream](https://discuss.leetcode.com/topic/84382/c-using-stringstream)

[https://discuss.leetcode.com/topic/84323/java-elegant-solution](https://discuss.leetcode.com/topic/84323/java-elegant-solution)

[https://discuss.leetcode.com/topic/84508/cpp-solution-with-sscanf](https://discuss.leetcode.com/topic/84508/cpp-solution-with-sscanf)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












