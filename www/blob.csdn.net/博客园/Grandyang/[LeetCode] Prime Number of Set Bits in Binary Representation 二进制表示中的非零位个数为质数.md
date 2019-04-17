# [LeetCode] Prime Number of Set Bits in Binary Representation 二进制表示中的非零位个数为质数 - Grandyang - 博客园







# [[LeetCode] Prime Number of Set Bits in Binary Representation 二进制表示中的非零位个数为质数](https://www.cnblogs.com/grandyang/p/8642157.html)








Given two integers `L` and `R`, find the count of numbers in the range `[L, R]` (inclusive) having a prime number of set bits in their binary representation.

(Recall that the number of set bits an integer has is the number of `1`s present when written in binary. For example, `21` written in binary is `10101` which has 3 set bits. Also, 1 is not a prime.)



Example 1:
Input: L = 6, R = 10
Output: 4
Explanation:
6 -> 110 (2 set bits, 2 is prime)
7 -> 111 (3 set bits, 3 is prime)
9 -> 1001 (2 set bits , 2 is prime)
10->1010 (2 set bits , 2 is prime)



Example 2:
Input: L = 10, R = 15
Output: 5
Explanation:
10 -> 1010 (2 set bits, 2 is prime)
11 -> 1011 (3 set bits, 3 is prime)
12 -> 1100 (2 set bits, 2 is prime)
13 -> 1101 (3 set bits, 3 is prime)
14 -> 1110 (3 set bits, 3 is prime)
15 -> 1111 (4 set bits, 4 is not prime)



Note:
- `L, R` will be integers `L <= R` in the range `[1, 10^6]`.
- `R - L` will be at most 10000.




这道题给了我们一个整数范围[L, R]，让我们统计其中有多个整数，其二进制表示中非零位个数为质数。参考题目中的例子不难理解题意，那么博主最先想到的就是暴力搜索啊，毕竟是到Easy题嘛，可能不需要太多的技巧。我们遍历整数范围[L, R]中的每一个数字，然后先统计出所有非零位个数cnt，通过和1相与，再右移一位的方式。然后就是来判断这个cnt是否是质数，判断的方法就是就是从其平方开始，一个一个的除，如果一直到2都没有约数，那么就是质数啦，结果res累加1，参见代码如下：



解法一：

```
class Solution {
public:
    int countPrimeSetBits(int L, int R) {
        int res = 0;
        for (int i = L; i <= R; ++i) {
            int t = i, cnt = 0;
            while (t > 0) {
                if (t & 1 == 1) ++cnt;
                t >>= 1;
            }
            bool succ = true;
            for (int j = sqrt(cnt); j > 1; --j) {
                if (cnt % j == 0) {
                    succ = false; break;
                }
            }
            if (succ && cnt != 1) ++res;
        }
        return res;
    }
};
```



好，下面我们来优化一下上面的解法，由于题目中给了数的大小范围 R <= 106 < 220，那么我们统计出来的非零位个数cnt只需要检测是否是20以内的质数即可，所以我们将20以内的质数都放入一个HashSet中，然后统计出来cnt后，直接在HashSet中查找有没有即可，参见代码如下：



解法二：

```
class Solution {
public:
    int countPrimeSetBits(int L, int R) {
        int res = 0;
        unordered_set<int> primes{2, 3, 5, 7, 11, 13, 17, 19};
        for (int i = L; i <= R; ++i) {
            int cnt = 0;
            for (int j = i; j > 0; j >>= 1) {
                cnt += j & 1;
            }
            res += primes.count(cnt);
        }
        return res;
    }
};
```



下面这种写法就更简洁啦，直接使用了C++的内置函数__builtin_popcount来快速的求出非零位的个数cnt，然后又利用到了20以内的数，只要不能被2和3的一定是质数，又可以快速判断了质数了，参见代码如下：



解法三：

```
class Solution {
public:
    int countPrimeSetBits(int L, int R) {
        int res = 0;
        for (int i = L; i <= R; ++i) {
            int cnt = __builtin_popcount(i);
            res += cnt < 4 ? cnt > 1 : (cnt % 2 && cnt % 3);
        }
        return res;
    }
};
```



类似题目：

[Number of 1 Bits](http://www.cnblogs.com/grandyang/p/4325432.html)



参考资料：

[https://leetcode.com/problems/prime-number-of-set-bits-in-binary-representation/discuss/113225/Short-C++-12-ms](https://leetcode.com/problems/prime-number-of-set-bits-in-binary-representation/discuss/113225/Short-C++-12-ms)

[https://leetcode.com/problems/prime-number-of-set-bits-in-binary-representation/discuss/113227/JavaC++-Clean-Code](https://leetcode.com/problems/prime-number-of-set-bits-in-binary-representation/discuss/113227/JavaC++-Clean-Code)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












