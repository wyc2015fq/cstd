# [LeetCode] Fraction Addition and Subtraction 分数加减法 - Grandyang - 博客园







# [[LeetCode] Fraction Addition and Subtraction 分数加减法](https://www.cnblogs.com/grandyang/p/6954197.html)







Given a string representing an expression of fraction addition and subtraction, you need to return the calculation result in string format. The final result should be [irreducible fraction](https://en.wikipedia.org/wiki/Irreducible_fraction). If your final result is an integer, say `2`, you need to change it to the format of fraction that has denominator `1`. So in this case, `2` should be converted to `2/1`.

Example 1:
Input:"-1/2+1/2"
Output: "0/1"



Example 2:
Input:"-1/2+1/2+1/3"
Output: "1/3"



Example 3:
Input:"1/3-1/2"
Output: "-1/6"



Example 4:
Input:"5/3+1/3"
Output: "2/1"



Note:
- The input string only contains `'0'` to `'9'`, `'/'`, `'+'` and `'-'`. So does the output.
- Each fraction (input and output) has format `±numerator/denominator`. If the first input fraction or the output is positive, then `'+'` will be omitted.
- The input only contains valid irreducible fractions, where the numerator and denominator of each fraction will always be in the range [1,10]. If the denominator is 1, it means this fraction is actually an integer in a fraction format defined above.
- The number of given fractions will be in the range [1,10].
- The numerator and denominator of the final result are guaranteed to be valid and in the range of 32-bit int.



这道题让我们做分数的加减法，给了我们一个分数加减法式子的字符串，然我们算出结果，结果当然还是用分数表示了。那么其实这道题主要就是字符串的拆分处理，再加上一点中学的数学运算的知识就可以了。这里我们使用字符流处理类来做，每次按顺序读入一个数字，一个字符，和另一个数字。分别代表了分子，除号，分母。我们初始化分子为0，分母为1，这样就可以进行任何加减法了。中学数学告诉我们必须将分母变为同一个数，分子才能相加，为了简便，我们不求最小公倍数，而是直接乘上另一个数的分母，然后相加。不过得到的结果需要化简一下，我们求出分子分母的最大公约数，记得要取绝对值，然后分子分母分别除以这个最大公约数就是最后的结果了，参见代码如下：



```
class Solution {
public:
    string fractionAddition(string expression) {
        istringstream is(expression);
        int num = 0, dem = 0, A = 0, B = 1; 
        char c;
        while (is >> num >> c >> dem) {
            A = A * dem + num * B;
            B *= dem;
            int g = abs(gcd(A, B));
            A /= g;
            B /= g;
        }
        return to_string(A) + "/" + to_string(B);
    }
    int gcd(int a, int b) {
        return (b == 0) ? a : gcd(b, a % b);
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/89991/concise-java-solution](https://discuss.leetcode.com/topic/89991/concise-java-solution)

[https://discuss.leetcode.com/topic/90061/small-simple-c-java-python](https://discuss.leetcode.com/topic/90061/small-simple-c-java-python)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












