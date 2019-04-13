
# 166. Fraction to Recurring Decimal - OraYang的博客 - CSDN博客

2018年01月02日 18:17:20[OraYang](https://me.csdn.net/u010665216)阅读数：87标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[hashtable																](https://so.csdn.net/so/search/s.do?q=hashtable&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=hashtable&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 166. Fraction to Recurring Decimal
标签（空格分隔）： leetcode hashtable medium
---题目

---Given two integers representing the numerator and denominator of a fraction, return the fraction in string format.

---If the fractional part is repeating, enclose the repeating part in parentheses.

---For example,

---Given numerator = 1, denominator = 2, return “0.5”.

---Given numerator = 2, denominator = 1, return “2”.

---Given numerator = 2, denominator = 3, return “0.(6)”.

---思路

---本题，题目并不是很难，就是需要考虑一些边界条件：

---分子为0

---是正数还是负数

---是否会发生溢出（被-2147483648坑了好久）

---如何判断无限循环小数

---考虑完了这些情况后，我们只要按照除法法则，先求出整数部分，然后求小数部分，具体代码注释中有写。（这里我们借助hashtable来存储余数从而来判断是否为无限循环小数）

---代码

---class

---Solution {

---public

---:

---string

---fractionToDecimal(

---int

---numerator,

---int

---denominator) {

---//判断是否为0

---if

---(numerator==

---0

---)

---return

---"0"

---;

---string

---res;

---//判断符号

---if

---(numerator<

---0

---^denominator<

---0

---)
            res+=

---"-"

---;

---//取绝对值

---long

---n =

---labs

---(

---long

---(numerator));

---long

---d =

---labs

---(

---long

---(denominator));

---//添加整数部分

---cout

---<<n<<endl;
        res+=to_string(n/d);

---//判断余数是否为0

---if

---(n%d==

---0

---)

---return

---res;
        res+=

---"."

---;

---unordered_map

---<

---long

---,

---long

--->

---mp;

---//对余数部分进行判断

---for

---(

---long

---reminder = n%d;reminder;reminder %=d)
        {

---if

---(mp.count(reminder))
            {
                res.insert(mp[reminder],

---1

---,

---'('

---);
                res+=

---")"

---;

---return

---res;
            }
            mp[reminder] = res.size();
            reminder *=

---10

---;
            res +=to_string(reminder/d);
        }

---return

---res;
    }
};


