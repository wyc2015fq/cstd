# [LeetCode] Additive Number 加法数 - Grandyang - 博客园







# [[LeetCode] Additive Number 加法数](https://www.cnblogs.com/grandyang/p/4974115.html)







Additive number is a string whose digits can form additive sequence.

A valid additive sequence should contain at least three numbers. Except for the first two numbers, each subsequent number in the sequence must be the sum of the preceding two.

Given a string containing only digits `'0'-'9'`, write a function to determine if it's an additive number.

Note: Numbers in the additive sequence cannot have leading zeros, so sequence `1, 2, 03` or `1, 02, 3`is invalid.

Example 1:
Input: `"112358"`
Output: true 
Explanation: The digits can form an additive sequence: `1, 1, 2, 3, 5, 8`. 
             1 + 1 = 2, 1 + 2 = 3, 2 + 3 = 5, 3 + 5 = 8

Example 2:
Input: `"199100199"`
Output: true 
Explanation: The additive sequence is: `1, 99, 100, 199`. 
             1 + 99 = 100, 99 + 100 = 199
Follow up:
How would you handle overflow for very large input integers?

**Credits:**
Special thanks to [@jeantimex](https://leetcode.com/discuss/user/jeantimex) for adding this problem and creating all test cases.



这道题定义了一种加法数，就是至少含有三个数字，除去前两个数外，每个数字都是前面两个数字的和，题目中给了许多例子，也限定了一些不合法的情况，比如两位数以上不能以0开头等等，让我们来判断一个数是否是加法数。目光犀利的童鞋应该一眼就能看出来，这尼玛不就是斐波那契数组么，跟另一道 [Split Array into Fibonacci Sequence](https://www.cnblogs.com/grandyang/p/10434771.html) 简直不要太像啊。只不过那道题要返回一种组合方式，而这道题只是问能否拆成斐波那契数列。

开始我还想是否能用动态规划来解，可是发现不会写状态转移方程，只得作罢。其实这题可用Brute Force的思想来解，我们让第一个数字先从一位开始，第二个数字从一位，两位，往高位开始搜索，前两个数字确定了，相加得到第三位数字，三个数组排列起来形成一个字符串，和原字符串长度相比，如果小于原长度，那么取出上一次计算的第二个和第三个数，当做新一次计算的前两个数，用相同的方法得到第三个数，再加入当前字符串，再和原字符串长度相比，以此类推，直到当前字符串长度不小于原字符串长度，比较两者是否相同，相同返回true，不相同则继续循环。如果所有情况都遍历完了还是没有返回true，则说明不是Additive Number，返回false，参见代码如下：



解法一：

```
class Solution {
public:
    bool isAdditiveNumber(string num) {
        for (int i = 1; i < num.size(); ++i) {
            string s1 = num.substr(0, i);
            if (s1.size() > 1 && s1[0] == '0') break;
            for (int j = i + 1; j < num.size(); ++j) {
                string s2 = num.substr(i, j - i);
                long d1 = stol(s1), d2 = stol(s2);
                if ((s2.size() > 1 && s2[0] == '0')) break;
                long next = d1 + d2;
                string nextStr = to_string(next);
                if (nextStr != num.substr(j, nextStr.length())) continue; // optimization here
                string allStr = s1 + s2 + nextStr;
                while (allStr.size() < num.size()) {
                    d1 = d2;
                    d2 = next;
                    next = d1 + d2;
                    nextStr = to_string(next);
                    allStr += nextStr;
                }
                if (allStr == num) return true;
            }
        }
        return false;
    }
};
```



此题还有递归解法，博主最先尝试的是跟那道 [Split Array into Fibonacci Sequence](https://www.cnblogs.com/grandyang/p/10434771.html) 一样的递归方法，虽然这道题不用返回组合方式，但是我们仍可以使用一样的递归来做，只不过这里的结果res是一个布尔型的全局变量，当我们找到一组符合题意的组合了之后，就将结果res设置为true，这样一旦再进入递归函数时，只要res为true了，就直接返回即可。之后的部分就基本相同了，可以参见 [Split Array into Fibonacci Sequence](https://www.cnblogs.com/grandyang/p/10434771.html)  中的讲解，注意稍有不同的地方是，这里拆分出来的数字是可以超过整型int范围的，但是貌似不会超过长整型long的范围，所以我们可以加一个检测str的长度大于19就break，因为long的十进制数长度是19位，参见代码如下：



解法二：

```
class Solution {
public:
    bool isAdditiveNumber(string num) {
        bool res = false;
        vector<long> out;
        helper(num, 0, out, res);
        return res;
    }
    void helper(string& num, int start, vector<long>& out, bool& res) {
        if (res) return;
        if (start >= num.size()) {
            if (out.size() >= 3) res = true; 
            return;
        }
        for (int i = start; i < num.size(); ++i) {
            string str = num.substr(start, i - start + 1);
            if ((str.size() > 1 && str[0] == '0') || str.size() > 19) break;
            long curNum = stol(str), n = out.size();
            if (out.size() >= 2 && curNum != out[n - 1] + out[n - 2]) continue;
            out.push_back(curNum);
            helper(num, i + 1, out, res);
            out.pop_back();
        }
    }
};
```



由于这道题并不需要我们返回具体的组合方式，所以也可以不使用上面的写法，而是不停的用当前的两个数，去拼后面的数字，一旦拼不出来了，直接返回false。跟解法一类似，首先用两个for循环来确定前两个数字，然后将后面的整体提取出来，和当前的两个数字一起调用递归，若递归返回true了，则直接返回true，否则继续遍历。

在递归函数中，还是首先检验是否存在leading zeros，然后将 num1 和 num2 分别转为长整型long，相加后再转回字符串，存入sumStr，这时候看sumStr是否和num相等，是的话直接返回true。否则再来检验，若sumStr的长度大于等于num了，返回false，或者在num中取跟sumStr长度相等的子串，若不等于sumStr，说明无法拼出来，也返回false。若都没返回的话，就再次调用递归，不过这次num要去掉和sumStr长度相等的子串，留下后面的部分，此时带入递归的两个数字要变成 num2 和 sumStr，继续跟后面的比较，参见代码如下：



解法三：

```
class Solution {
public:
    bool isAdditiveNumber(string num) {
        for (int i = 1; i < num.size(); ++i) {
            string s1 = num.substr(0, i);
            if (s1.size() > 1 && s1[0] == 0) break;
            for (int j = i + 1; j < num.size(); ++j){
                string s2 = num.substr(i, j - i);
                if (s2.size() > 1 && s2[0] == 0) break;
                if(helper(num.substr(j), s1, s2)) return true;
            }
        }
        return false;
    }
    bool helper(string num, string num1, string num2){
        if ((num1.size() > 1 && num1[0] == '0') || (num2.size() > 1 && num2[0] == '0')) return false;
        string sumStr = to_string(stol(num1) + stol(num2));
        if (sumStr == num) return true;
        if (sumStr.size() >= num.size() || num.substr(0, sumStr.size()) != sumStr) return false;
        return helper(num.substr(sumStr.size()), num2, sumStr);
    }
};
```



题目中有个 follow up 说是让我们handle超大整数的溢出情况，那么我们想，上面的解法中哪块可能溢出啊，当然只有将子串转为长整型long的时候，假如此时字符串特别长，甚至超出了长整型的范围，那么我们就不能用stol了，此时就不能转换了，那么我们只能强行将两个字符串相加了，这里用的又是另外一道题目 [Add Strings](http://www.cnblogs.com/grandyang/p/5944311.html) 的知识点了，这样我们就完美的避开了可能溢出的情况了，参见代码如下：



解法四：

```
// Follow up, handle overflow for very large input integers.
class Solution {
public:
    bool isAdditiveNumber(string num) {
        for (int i = 1; i < num.size(); ++i) {
            string s1 = num.substr(0, i);
            if (s1.size() > 1 && s1[0] == 0) break;
            for (int j = i + 1; j < num.size(); ++j){
                string s2 = num.substr(i, j - i);
                if (s2.size() > 1 && s2[0] == 0) break;
                if(helper(num.substr(j), s1, s2)) return true;
            }
        }
        return false;
    }
    bool helper(string num, string num1, string num2){
        if ((num1.size() > 1 && num1[0] == '0') || (num2.size() > 1 && num2[0] == '0')) return false;
        string sumStr = add(num1, num2);
        if (sumStr == num) return true;
        if (sumStr.size() >= num.size() || num.substr(0, sumStr.size()) != sumStr) return false;
        return helper(num.substr(sumStr.size()), num2, sumStr);
    }
    string add(string num1, string num2) {
        string res;
        int i = (int)num1.size() - 1, j = (int)num2.size() - 1, carry = 0;
        while (i >= 0 || j >= 0) {
            int val1 = (i >= 0) ? (num1[i--] - '0') : 0;
            int val2 = (j >= 0) ? (num2[j--] - '0') : 0;
            int sum = val1 + val2 + carry;
            res.push_back(sum % 10 + '0');
            carry = sum / 10;
        }
        if (carry) res.push_back(carry + '0');
        reverse(res.begin(), res.end());
        return res;
    }
};
```



类似题目：

[Add Strings](http://www.cnblogs.com/grandyang/p/5944311.html)

[Split Array into Fibonacci Sequence](https://www.cnblogs.com/grandyang/p/10434771.html)



参考资料：

[https://leetcode.com/problems/additive-number/](https://leetcode.com/problems/additive-number/)

[https://leetcode.com/problems/additive-number/discuss/75567/Java-Recursive-and-Iterative-Solutions](https://leetcode.com/problems/additive-number/discuss/75567/Java-Recursive-and-Iterative-Solutions)

[https://leetcode.com/problems/additive-number/discuss/75704/My-Simple-C%2B%2B-Non-recursion-Solution](https://leetcode.com/problems/additive-number/discuss/75704/My-Simple-C%2B%2B-Non-recursion-Solution)

[https://leetcode.com/problems/additive-number/discuss/75576/0ms-concise-C%2B%2B-solution-(perfectly-handles-the-follow-up-and-leading-0s)](https://leetcode.com/problems/additive-number/discuss/75576/0ms-concise-C%2B%2B-solution-(perfectly-handles-the-follow-up-and-leading-0s))



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












