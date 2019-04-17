# [LeetCode] Remove 9 移除9 - Grandyang - 博客园







# [[LeetCode] Remove 9 移除9](https://www.cnblogs.com/grandyang/p/8261714.html)









Start from integer 1, remove any integer that contains 9 such as 9, 19, 29...

So now, you will have a new integer sequence: 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, ...

Given a positive integer `n`, you need to return the n-th integer after removing. Note that 1 will be the first integer.

**Example 1:**

**Input:** 9
**Output:** 10

** Hint**: n will not exceed `9 x 10^8`.





这道题让我们移除所有包含数字9的数字，然后得到一个新的数列，给我们一个数字n，让我们求在这个新的数组中第n个数字。我们多写些数字来看看：

0，1，2，3，4，5，6，7，8 （移除了9）

10，11，12，13，14，15，16，17，18 （移除了19）

.....

80，81，82，83，84，85，86，87，88 （移除了89）

（移除了 90 - 99 ）

100，101，102，103，104，105，106，107，108 （移除了109）

我们可以发现，8的下一位就是10了，18的下一位是20，88的下一位是100，实际上这就是九进制的数字的规律，那么这道题就变成了将十进制数n转为九进制数，这个就没啥难度了，就每次对9取余，然后乘以base，n每次自除以9，base每次扩大10倍，参见代码如下：



解法一：

```
class Solution {
public:
   int newInteger(int n) {
        int res = 0, base = 1;
        while (n > 0) {
            res += n % 9 * base;
            n /= 9;
            base *= 10;
        }
        return res;
   }
};
```



我们也可以写的更简洁一些，不用base变量，将结果res先当作字符串来处理，最后再转回整型数，参见代码如下：



解法二：

```
class Solution {
public:
   int newInteger(int n) {
        string res = "";
        while (n > 0) {
            res = to_string(n % 9) + res;
            n /= 9;
        }
        return stoi(res);
   }
};
```



将十进制数转为九进制只能算Easy的题目，既然这道题标记了Hard，我们就不应该只满足于此。因为数字9是个特例，可以用上面的巧妙的解法，但如果要移除1到8中间的任意一个呢？上面的方法就不好使了，我们还是要来看看通用的解法。又来读[fun4LeetCode大神的paper](https://discuss.leetcode.com/topic/99481/alternative-solution-applicable-to-the-general-case)了，这次大神收着写的，不算太长，还是可以好好读一读的，首先我们知道，不管是移出那个数字，新数组中的第n个数字的值m，都是要大于n本身的，我们将多出的数的个数用f(1, m)表示，则有：

```
m - f(1, m) = n
```

要求m的话，我们就要先求出 f(1, m) 的值，然后加上n的值，就能得到m了。这道题无法直接求出m的值，而是采用一种迭代逼近的方法来算m。最开始的时候，我们让m为n，先求 f(1, n) 的值，比如说结果为k，然后我们再算 f(1, n + k)的值，用得到的结果k'来更新k，再带入算 f(1, n + k)，直到 k == f(1, n + k) 为止，那么此时的 n + k 就是我们要求的m。

下面来看我们如何计算 f(1, m)，我们当然不可能遍历所有的数字，一位一位来查看有没有要移除的数字了，太不高效了。我们再来看看开头列举的前99个数字中移除9后剩下的数字，统计一下，总共去掉了19个包含9的数字。那我们想，如果前99个数字中要移除所有包含2的数字，会去掉多少个？其实还是19个，我们发现，前99个数字，不论去掉哪个数字，都会去掉19个数字。这是一个很重要的发现，我们再来看看这19个数是怎么分布的，首先每10个数都一定会包含一个要移除的数，比如要移除的是9，每10个数都会有一个9出现，而在90几那一行，10个数都会包含9，所以都要移除，那么我们可以总结出规律，非移除数开头的其他9行，各移除1个，移除数开头的10个都要移除，所以就有10+9=19个。好，那么这是前99个数的情况，那么前999个数又是什么情况呢？其实很类似，非移除数开头的9行各有19个，移除数开头的有10x19个，所以整个就是19x19个，所以19这个基数很重要。

好，下面来看看各位上的数字a跟要移除数d之间的关系。有三种关系，分别是小于，等于，大于：

1）当 a < d 时，比如说我们要移除的数字是6，那么a就是1到5中的数，我们知道，每10个数中只含有一个6，所以就要移除a个6就行了，如果a在百位上，就是是a * 19个，然后再加上下一位上移除的值，用等式来写就是：

```
T(1, m) = a_i * (10^i - 9^i) + T(1, m % 10^i)
```

2）当 a = d 时，那么a此时为6，如果a是十位上的数，那么前面[1, 59]中的5个6要先移除掉，然后此时下一位有多少个数移除多个数，还要加上1。比如m如果是63，那么60, 61, 62, 63这四个数要移除，怎么算的，通过m%10 + 1来计算，所以整个用等式来写就是：

```
T(1, m) = a_i * (10^i - 9^i) + m % 10^i + 1
```

3）当 a > d 时，比如此时a为8，要移除的数字还是6，那么[60, 69]这10个数都要移除，那么实际上还要再移除7个6，分别是[1,9], [10,19], [21,29], [31,39], [41,49], [51,59], [71,79] 这7个区间中的6，那么是怎么算的，通过a - 1来算，实际上是情况1的值再加上 10^i 个数，用等式来写就是：

```
T(1, m) = (a_i - 1) * (10^i - 9^i) + 10^i + T(1, m % 10^i) = a_i * (10^i - 9^i) + 9^i + T(1, m % 10^i)
```

参见代码如下：



解法三：

```
class Solution {
public:
    int newInteger(int n) {
        long d = 9, pre = 0, cur = 0;
        while (true) {
            pre = cur;
            cur = helper(n + cur, d);
            if (cur == pre) break;
        }
        return n + cur;
    }
    long helper(long m, long d) {
        long res = 0, p = 1, q = 1;
        for (long i = m; i >= 10; i /= 10) {
            p *= 10;
            q *= 9;
        }
        for (long i = m; i >= d; i %= p, p /= 10, q /= 9) {
            long a = i / p;
            res += a * (p - q);
            if (a == d) {
                res += i % p + 1; break;
            } else if (a > d) {
                res += q;
            }
        }
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/99195/one-line-java-solution](https://discuss.leetcode.com/topic/99195/one-line-java-solution)

[https://discuss.leetcode.com/topic/99240/this-problem-nonary-base-9-short-java-solution](https://discuss.leetcode.com/topic/99240/this-problem-nonary-base-9-short-java-solution)

[https://discuss.leetcode.com/topic/99481/alternative-solution-applicable-to-the-general-case](https://discuss.leetcode.com/topic/99481/alternative-solution-applicable-to-the-general-case)

[https://discuss.leetcode.com/topic/99401/share-my-o-logn-c-solution-with-thinking-process-and-explanation](https://discuss.leetcode.com/topic/99401/share-my-o-logn-c-solution-with-thinking-process-and-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












