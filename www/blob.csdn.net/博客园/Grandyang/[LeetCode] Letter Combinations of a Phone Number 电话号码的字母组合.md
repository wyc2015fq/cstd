# [LeetCode] Letter Combinations of a Phone Number 电话号码的字母组合 - Grandyang - 博客园







# [[LeetCode] Letter Combinations of a Phone Number 电话号码的字母组合](https://www.cnblogs.com/grandyang/p/4452220.html)







Given a digit string, return all possible letter combinations that the number could represent.

A mapping of digit to letters (just like on the telephone buttons) is given below.

![](http://upload.wikimedia.org/wikipedia/commons/thumb/7/73/Telephone-keypad2.svg/200px-Telephone-keypad2.svg.png)
**Input:**Digit string "23"
**Output:** ["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"].

**Note:**

Although the above answer is in lexicographical order, your answer could be in any order you want.




这道题让我们求电话号码的字母组合，即数字2到9中每个数字可以代表若干个字母，然后给一串数字，求出所有可能的组合，相类似的题目有[Path Sum II](http://www.cnblogs.com/grandyang/p/4042156.html)，[Subsets II](http://www.cnblogs.com/grandyang/p/4310964.html)，[Permutations](http://www.cnblogs.com/grandyang/p/4358848.html)，[Permutations II](http://www.cnblogs.com/grandyang/p/4359825.html)，[Combinations](http://www.cnblogs.com/grandyang/p/4332522.html)，[Combination Sum](http://www.cnblogs.com/grandyang/p/4419259.html) 和[Combination Sum II](http://www.cnblogs.com/grandyang/p/4419386.html) 等等。我们用递归Recursion来解，我们需要建立一个字典，用来保存每个数字所代表的字符串，然后我们还需要一个变量level，记录当前生成的字符串的字符个数，实现套路和上述那些题十分类似。在递归函数中我们首先判断level，如果跟digits中数字的个数相等了，我们将当前的组合加入结果res中，然后返回。否则我们通过digits中的数字到dict中取出字符串，然后遍历这个取出的字符串，将每个字符都加到当前的组合后面，并调用递归函数即可，参见代码如下：



解法一：

```
class Solution {
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) return {};
        vector<string> res;
        string dict[] = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
        letterCombinationsDFS(digits, dict, 0, "", res);
        return res;
    }
    void letterCombinationsDFS(string digits, string dict[], int level, string out, vector<string> &res) {
        if (level == digits.size()) {res.push_back(out); return;}
        string str = dict[digits[level] - '0'];
        for (int i = 0; i < str.size(); ++i) {
            letterCombinationsDFS(digits, dict, level + 1, out + string(1, str[i]), res);
        }
    }
};
```



这道题我们也可以用迭代Iterative来解，在遍历digits中所有的数字时，我们先建立一个临时的字符串数组t，然后跟上面解法的操作一样，通过数字到dict中取出字符串str，然后遍历取出字符串中的所有字符，再遍历当前结果res中的每一个字符串，将字符加到后面，并加入到临时字符串数组t中。取出的字符串str遍历完成后，将临时字符串数组赋值给结果res，具体实现参见代码如下：



解法二：

```
class Solution {
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) return {};
        vector<string> res{""};
        string dict[] = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
        for (int i = 0; i < digits.size(); ++i) {
            vector<string> t;
            string str = dict[digits[i] - '0'];
            for (int j = 0; j < str.size(); ++j) {
                for (string s : res) t.push_back(s + str[j]);
            }
            res = t;
        }
        return res;
    }
};
```



类似题目：

[Generate Parentheses](http://www.cnblogs.com/grandyang/p/4444160.html)

[Combination Sum](http://www.cnblogs.com/grandyang/p/4419259.html)

[Binary Watch](http://www.cnblogs.com/grandyang/p/5896454.html)



参考资料：

[https://leetcode.com/problems/letter-combinations-of-a-phone-number/](https://leetcode.com/problems/letter-combinations-of-a-phone-number/)

[https://leetcode.com/problems/letter-combinations-of-a-phone-number/discuss/8109/My-recursive-solution-using-Java](https://leetcode.com/problems/letter-combinations-of-a-phone-number/discuss/8109/My-recursive-solution-using-Java)

[https://leetcode.com/problems/letter-combinations-of-a-phone-number/discuss/8097/My-iterative-sollution-very-simple-under-15-lines](https://leetcode.com/problems/letter-combinations-of-a-phone-number/discuss/8097/My-iterative-sollution-very-simple-under-15-lines)

[https://leetcode.com/problems/letter-combinations-of-a-phone-number/discuss/8207/Concise-15-line-Java-Iterative-Solution-very-Straight-Forward-with-Brief-Explanation](https://leetcode.com/problems/letter-combinations-of-a-phone-number/discuss/8207/Concise-15-line-Java-Iterative-Solution-very-Straight-Forward-with-Brief-Explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












