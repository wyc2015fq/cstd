# [LeetCode] Perfect Number 完美数字 - Grandyang - 博客园







# [[LeetCode] Perfect Number 完美数字](https://www.cnblogs.com/grandyang/p/6636879.html)







We define the Perfect Number is a positive integer that is equal to the sum of all its positive divisors except itself.

Now, given an integer n, write a function that returns true when it is a perfect number and false when it is not.



Example:
Input: 28
Output: True
Explanation: 28 = 1 + 2 + 4 + 7 + 14



Note: The input number n will not exceed 100,000,000. (1e8)



这道题让我们判断给定数字是否为完美数字，并给来完美数字的定义，就是一个整数等于除其自身之外的所有的因子之和。那么由于不能包含自身，所以n必定大于1。其实这道题跟之前的判断质数的题蛮类似的，都是要找因子。由于1肯定是因子，可以提前加上，那么我们找其他因子的范围是[2, sqrt(n)]。我们遍历这之间所有的数字，如果可以被n整除，那么我们把i和num/i都加上，对于n如果是平方数的话，那么我们此时相同的因子加来两次，所以我们要判断一下，如果相等，就不再加 num/i。实际上，符合要求的完美数字很少，根本就没有完全平方数，我们根本就不用担心会加两次，当然，这都是从结果分析的，为了严格按照题目的要求，还是加上判断吧。还有就是我们也可以在遍历的过程中如果累积和sum大于n了，直接返回false，但是感觉加了也没咋提高运行时间，所以干脆就不加了。在循环结束后，我们首先判断num是否为1，因为题目中说了不能加包括本身的因子，然后我们再看sum是否和num相等，参见代码如下：



解法一：

```
class Solution {
public:
    bool checkPerfectNumber(int num) {
        int sum = 1;
        for (int i = 2; i * i <= num; ++i) {
            if (num % i == 0) {
                sum += i + (num / i == i ? 0 : num / i);
            }
        }
        return num != 1 && sum == num;
    }
};
```



下面这种方法叼的不行，在给定的n的范围内其实只有五个符合要求的完美数字，于是就有这种枚举的解法，那么套用一句诸葛孔明的名言就是，我从未见过如此厚颜无耻之解法。哈哈，开个玩笑。写这篇博客的时候，国足正和伊朗进行十二强赛，上半场0比0，希望国足下半场能进球，好运好运，不忘初心，方得始终～



解法二：

```
class Solution {
public:
    bool checkPerfectNumber(int num) {
        return num == 6 || num == 28 || num == 496 || num == 8128 || num == 33550336;
    }
};
```



类似题目：

[Self Dividing Numbers](http://www.cnblogs.com/grandyang/p/7906786.html)



参考资料：

[https://leetcode.com/problems/perfect-number/](https://leetcode.com/problems/perfect-number/)

[https://leetcode.com/problems/perfect-number/discuss/98594/simple-java-solution](https://leetcode.com/problems/perfect-number/discuss/98594/simple-java-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












