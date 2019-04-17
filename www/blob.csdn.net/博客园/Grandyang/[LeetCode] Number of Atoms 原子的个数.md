# [LeetCode] Number of Atoms 原子的个数 - Grandyang - 博客园







# [[LeetCode] Number of Atoms 原子的个数](https://www.cnblogs.com/grandyang/p/8667239.html)







Given a chemical `formula` (given as a string), return the count of each atom.

An atomic element always starts with an uppercase character, then zero or more lowercase letters, representing the name.

1 or more digits representing the count of that element may follow if the count is greater than 1. If the count is 1, no digits will follow. For example, H2O and H2O2 are possible, but H1O2 is impossible.

Two formulas concatenated together produce another formula. For example, H2O2He3Mg4 is also a formula.

A formula placed in parentheses, and a count (optionally added) is also a formula. For example, (H2O2) and (H2O2)3 are formulas.

Given a formula, output the count of all elements as a string in the following form: the first name (in sorted order), followed by its count (if that count is more than 1), followed by the second name (in sorted order), followed by its count (if that count is more than 1), and so on.

Example 1:
Input: 
formula = "H2O"
Output: "H2O"
Explanation: 
The count of elements are {'H': 2, 'O': 1}.



Example 2:
Input: 
formula = "Mg(OH)2"
Output: "H2MgO2"
Explanation: 
The count of elements are {'H': 2, 'Mg': 1, 'O': 2}.



Example 3:
Input: 
formula = "K4(ON(SO3)2)2"
Output: "K4N2O14S4"
Explanation: 
The count of elements are {'K': 4, 'N': 2, 'O': 14, 'S': 4}.



Note:
- All atom names consist of lowercase letters, except for the first character which is uppercase.
- The length of `formula` will be in the range `[1, 1000]`.
- `formula` will only consist of letters, digits, and round parentheses, and is a valid formula as defined in the problem.



这道题给了我们一个化学式，让我们数其中原子的个数。比如水是H2O，里面有两个氢原子，一个氧原子，返回还是H2O。例子2给的是氢氧化镁（哈哈，想不到这么多年过去了，高中化学还没有完全还给老师，呀，暴露年龄了呢|||-.-），里面有一个镁原子，氧原子和氢原子各两个，我们返回H2MgO2，可以看到元素是按字母顺序排列的，这道题就是纯粹玩字符串，不需要任何的化学知识。再看第三个例子K4(ON(SO3)2)2，就算你不认识里面的钾，硫，氮，氧等元素，也不影响做题，这个例子的返回是K4N2O14S4，钾原子有4个，氮原子有2个，氧原子有14个，是3x2x2 + 2 = 14得来的，硫原子有4个，是2x2 = 4得来的。那么我们可以发现规律，先统计括号里的原子个数，然后如果括号外面有数字，那么括号里每个原子的个数乘以外面的数字即可，然后在外层若还有数字，那么就继续乘这个数字，这种带有嵌套形式的字符串，比较适合用递归来做。我们最终的目的是统计每个原子的数量，所以我们只要建立了每个元素和其出现次数的映射，就可以生成返回的字符串了，由于需要按元素的字母顺序排列，所以我们使用TreeMap来建立映射。我们使用一个变量pos，来记录我们遍历的位置，这是个全局的变量，在递归函数参数中需要设置引用。我们遍历的时候，需要分三种情况讨论，分别是遇到左括号，右括号，和其他。我们一个个来看：

如果当前是左括号，那么我们pos先自增1，跳过括号位置，然后我们可以调用递归函数，来处理这个括号中包括的所有内容，外加上后面的数字，比如Mg(OH)2，在pos=2处遇到左括号，调用完递归函数后pos指向了最后一个字符的后一位，即pos=7。而在K4(ON(SO3)2)2中，如果是遇到中间的那个左括号pos=5时，调用完递归函数后pos指向了第二个右括号，即pos=11。递归函数返回了中间部分所有原子跟其个数之间的映射，我们直接将其都加入到当前的映射中即可。

如果当前是右括号，说明一个完整的括号已经遍历完了，我们需要取出其后面的数字，如果括号存在，那么后面一定会跟数字，否则不需要括号。所以我们先让pos自增1，跳过括号的位置，然后用个变量i记录当前位置，再进行while循环，找出第一个非数字的位置，那么中间就都是数字啦，用substr将其提取出来，并转为整数，然后遍历当前的映射对，每个值都乘以这个倍数即可，然后返回。

如果当前是字母，那么需要将元素名提取出来了，题目中说了元素名只有第一个字母是大写，后面如果有的话，都是小写字母。所以我们用个while循环找到第一个非小写字母的位置，用substr取出中间的字符串，即元素名。由于元素名后也可能跟数字，所以在用个while循环，来找之后第一个非数字的位置，用substr提取出数字字符串。当然也可能元素名后没有数字，提取出来的数字字符串就是空的，我们加的时候判断一下，如果为空就只加1，否则就加上转化后的整数，参见代码如下：



解法一：

```
class Solution {
public:
    string countOfAtoms(string formula) {
        string res = "";
        int pos = 0;
        map<string, int> m = parse(formula, pos);
        for (auto a : m) {
            res += a.first + (a.second == 1 ? "" : to_string(a.second));
        }
        return res;
    }
    map<string, int> parse(string& str, int& pos) {
        map<string, int> res;
        while (pos < str.size()) {
            if (str[pos] == '(') {
                ++pos;
                for (auto a : parse(str, pos)) res[a.first] += a.second;
            } else if (str[pos] == ')') {
                int i = ++pos;
                while (pos < str.size() && isdigit(str[pos])) ++pos;
                int multiple = stoi(str.substr(i, pos - i));
                for (auto a : res) res[a.first] *= multiple;
                return res;
            } else {
                int i = pos++;
                while (pos < str.size() && islower(str[pos])) ++pos;
                string elem = str.substr(i, pos - i);
                i = pos;
                while (pos < str.size() && isdigit(str[pos])) ++pos;
                string cnt = str.substr(i, pos - i);
                res[elem] += cnt.empty() ? 1 : stoi(cnt);
            }
        }
        return res;
    }
};
```



下面这种解法是迭代形式，根据上面的递归解法改写而来。使用栈来代替递归函数，本身之上基本没有任何区别。需要注意的是，在遇到左括号时，我们将当前映射集cur加入了栈，这里用了个自带的move函数，表示将cur中所有的映射对移出并加入栈，之后cur就为空了。还有就是在处理右括号时，算出了倍数后，我们把当前的映射值乘以倍数后加到栈顶映射集中，然后用栈顶映射集来更新cur，并移除栈顶元素，参见代码如下：



解法二：

```
class Solution {
public:
    string countOfAtoms(string formula) {
        string res = "";
        stack<map<string, int>> st;
        map<string, int> cur;
        int n = formula.size(), pos = 0;
        while (pos < n) {
            if (formula[pos] == '(') {
                ++pos;
                st.push(move(cur));
            } else if (formula[pos] == ')') {
                int i = ++pos;
                while (pos < n && isdigit(formula[pos])) ++pos;
                int multiple = stoi(formula.substr(i, pos - i));
                for (auto a : cur) st.top()[a.first] += a.second * multiple;
                cur = move(st.top());
                st.pop();
            } else {
                int i = pos++;
                while (pos < n && islower(formula[pos])) ++pos;
                string elem = formula.substr(i, pos - i);
                i = pos;
                while (pos < n && isdigit(formula[pos])) ++pos;
                string cnt = formula.substr(i, pos - i);
                cur[elem] += cnt.empty() ? 1 : stoi(cnt);
            }
        }
        for (auto a : cur) {
            res += a.first + (a.second == 1 ? "" : to_string(a.second));
        }
        return res;
    }
};
```



类似题目：

[Decode String](http://www.cnblogs.com/grandyang/p/5849037.html)

[Encode String with Shortest Length](http://www.cnblogs.com/grandyang/p/6194403.html)

[Parse Lisp Expression](http://www.cnblogs.com/grandyang/p/8045255.html)



参考资料：

[https://leetcode.com/problems/number-of-atoms/discuss/109328/C++-iterative-solution](https://leetcode.com/problems/number-of-atoms/discuss/109328/C++-iterative-solution)

[https://leetcode.com/problems/number-of-atoms/discuss/112740/Concise-C++-recursive-parser](https://leetcode.com/problems/number-of-atoms/discuss/112740/Concise-C++-recursive-parser)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












