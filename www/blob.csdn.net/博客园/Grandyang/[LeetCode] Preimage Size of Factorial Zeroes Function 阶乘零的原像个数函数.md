# [LeetCode] Preimage Size of Factorial Zeroes Function 阶乘零的原像个数函数 - Grandyang - 博客园







# [[LeetCode] Preimage Size of Factorial Zeroes Function 阶乘零的原像个数函数](https://www.cnblogs.com/grandyang/p/9214055.html)







Let `f(x)` be the number of zeroes at the end of `x!`. (Recall that `x! = 1 * 2 * 3 * ... * x`, and by convention, `0! = 1`.)

For example, `f(3) = 0` because 3! = 6 has no zeroes at the end, while `f(11) = 2` because 11! = 39916800 has 2 zeroes at the end. Given `K`, find how many non-negative integers `x` have the property that `f(x) = K`.
Example 1:
Input: K = 0
Output: 5
Explanation: 0!, 1!, 2!, 3!, and 4! end with K = 0 zeroes.

Example 2:
Input: K = 5
Output: 0
Explanation: There is no x such that x! ends in K = 5 zeroes.

Note:
- `K` will be an integer in the range `[0, 10^9]`.



这道题的题目名称非常的难懂，但是读了题目内容以后，就不难理解了，定义函数f(x)为x!的末尾0的个数，现在给了我们一个非负整数K，问使得f(x)=K成立的非负整数的个数。我们之前做过一道有关阶乘末尾零的个数的题[Factorial Trailing Zeroes](http://www.cnblogs.com/grandyang/p/4219878.html)，从那道里我们知道了末尾0其实是由2和5相乘为10得到的，而阶乘中2的数量远多于5的个数，所以10的个数就只取决于5的个数。需要注意的一点就是，像25，125，这样的不只含有一个5的数字需要考虑进去。比如，24的阶乘末尾有4个0，分别是5，10，15，20中的四个5组成的，而25的阶乘末尾就有6个0，分别是5，10，15，20中的各一个5，还有25中的两个5，所以共有六个5，那么就不存在其阶乘数末尾有5个0的数。还有一个很重要的规律需要发现，我们知道20，21，22，23，24，这五个数的阶乘数末尾零的个数其实是相同的，都是有4个，因为它们包含的5的个数相同。而19，18，17，16，15，这五个数末尾零个数相同，均为3。那么我们其实可以发现，每五个数，必会至少多出1个5，有可能更多。所以阶乘末尾零个数均为K个的x值，只有两种情况，要么是5，要么是0，这个规律得出来后，我们继续向着正确的解题方向前进。

基于之前那道题[Factorial Trailing Zeroes](http://www.cnblogs.com/grandyang/p/4219878.html)的解法，我们知道了如何快速求一个给定的数字阶乘末尾零的个数，那么我们只要找到了一个这样的数，其阶乘末尾零的个数等于K的话，那么就说明总共有5个这样的数，返回5，反之，如果找不到这样的数字，就返回0。那么像这种选一个candidate数字，再进行验证的操作，用二分搜索法就是极好的，属于博主的总结帖中[LeetCode Binary Search Summary 二分搜索法小结](http://www.cnblogs.com/grandyang/p/6854825.html)的第四类，用子函数当作判断关系。我们首先要确定二分搜索法的范围，左边界很好确定，为0就行了，关键是来确定右边界，我们来分析一下，一个数字的阶乘末尾零个数为K，那么这个数字能有多大，就拿前面举的例子来说吧，末尾有4个0的最大数字是24，有六个0的最大是29，那么我们发现它们都不会超过5*(K+1)这个范围，所以这就是我们的右边界，注意右边界可能会超过整型数范围，所以要用长整型来表示。那么之后就是经典的二分搜索法的步骤了，确定一个中间值mid，然后调用子函数来计算mid的阶乘数末尾零的个数，用来和K比较大小，如果想等了，直接返回5，如果小于K，那么更新left为mid+1，反之，更新right为mid即可，最终没找到的话，返回0即可，参见代码如下：



解法一：

```
class Solution {
public:
    int preimageSizeFZF(int K) {
        long left = 0, right = 5L * (K + 1);
        while (left < right) {
            long mid = left + (right - left) / 2;
            long cnt = numOfTrailingZeros(mid);
            if (cnt == K) return 5;
            else if (cnt < K) left = mid + 1;
            else right = mid;
        }
        return 0;
    }
    long numOfTrailingZeros(long x) {
        long res = 0;
        for (; x > 0; x /= 5) {
            res += x / 5;
        }
        return res;
    }
};
```



下面这种解法是把子函数融到了while循环内，使得看起来更加简洁一些，解题思路跟上面的解法一模一样，参见代码如下：



解法二：

```
class Solution {
public:
    int preimageSizeFZF(int K) {
        long left = 0, right = 5L * (K + 1);
        while (left < right) {
            long mid = left + (right - left) / 2, cnt = 0;
            for (long i = 5; mid / i > 0; i *= 5) {
                cnt += mid / i;
            }
            if (cnt == K) return 5;
            else if (cnt < K) left = mid + 1;
            else right = mid;
        }
        return 0;
    }
};
```



下面这种解法也挺巧妙的，也是根据观察规律推出来的，我们首先来x为1到25的情况：

```
x:    1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
f(x): 0 0 0 0 1 1 1 1 1 2  2  2  2  2  3  3  3  3  3  4  4  4  4  4  6
g(x): 0 0 0 0 1 0 0 0 0 1  0  0  0  0  1  0  0  0  0  1  0  0  0  0  2
```

这里，f(x)是表示x!末尾零的个数，而g(x) = f(x) - f(x-1)，那么我们其实还可以通过观察发现，f(x) = sum(g(x)).

我们再仔细观察上面的数字，发现g(x)有正值的时候都是当x是5的倍数的时候，那么我们来专门看一下x是5的倍数时的情况吧：

```
x:    5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100 105 110 115 120 125
g(x): 1 1  1  1  2  1  1  1  1  2  1  1  1  1  2  1  1  1  1   2   1   1   1   1   3
```

仔细观察上面的红色数字，g(x)=1时，是5的倍数，g(x)=2时，都是25的倍数，g(x)=3时，是125的倍数，那么我们就有：

g(x) = 0     if x % 5 != 0,
g(x) >= 1    if x % 5 == 0,
g(x) >= 2   if x % 25 == 0.

如果我们继续将上面的数字写下去，我们可以发现规律，g(x)按照 1 1 1 1 x 的规律重复五次，第五次的时候x自增1。我们再继续观察:

当x=25时，g(x)=2，此时K=5被跳过了。

当x=50时，g(x)=2，此时K=11被跳过了。

当x=75时，g(x)=2，此时K=17被跳过了。

当x=100时，g(x)=2，此时K=23被跳过了。

当x=125时，g(x)=3，此时K=29，30被跳过了。

进一步，我们可以发现如下规律：

5(=1*5), 11(=6*1+5), 17(=6*2+5), 23(=6*3+5), 29(=6*4+5), 30(=6*5), 36(=31+5), 42(=31+6+5), 48(=31+6*2+5)

这些使得x不存在的K，出现都是有规律的，它们减去一个特定的基数base后，都是余5，而余1，2，3，4的，都是返回5。那么这个基数base，实际是1，6，31，156，...，是由 base = base * 5 + 1，不断构成的，通过这种不断对基数取余的操作，我们可以最终将K降为小于等于5的数，就可以直接返回结果了，参见代码如下：



解法三：

```
class Solution {
public:
    int preimageSizeFZF(int K) {
        if (K < 5) return 5;
        int base = 1;
        while (base * 5 + 1 <= K) {
            base = base * 5 + 1;
        }
        if (K / base == 5) return 0;
        return preimageSizeFZF(K % base);
    }
};
```



类似题目：

[Factorial Trailing Zeroes](http://www.cnblogs.com/grandyang/p/4219878.html)



参考资料：

[https://leetcode.com/problems/preimage-size-of-factorial-zeroes-function/discuss/117649/4ms-Java-Math-Solution](https://leetcode.com/problems/preimage-size-of-factorial-zeroes-function/discuss/117649/4ms-Java-Math-Solution)

[https://leetcode.com/problems/preimage-size-of-factorial-zeroes-function/discuss/117631/C++-O(logn)-math-solution-with-explanation](https://leetcode.com/problems/preimage-size-of-factorial-zeroes-function/discuss/117631/C++-O(logn)-math-solution-with-explanation)

[https://leetcode.com/problems/preimage-size-of-factorial-zeroes-function/discuss/117821/Four-binary-search-solutions-based-on-different-ideas](https://leetcode.com/problems/preimage-size-of-factorial-zeroes-function/discuss/117821/Four-binary-search-solutions-based-on-different-ideas)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












