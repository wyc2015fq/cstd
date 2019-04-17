# [LeetCode] Counting Bits 计数位 - Grandyang - 博客园







# [[LeetCode] Counting Bits 计数位](https://www.cnblogs.com/grandyang/p/5294255.html)







Given a non negative integer number **num**. For every numbers **i** in the range **0 ≤ i ≤ num** calculate the number of 1's in their binary representation and return them as an array.

**Example:**

For `num = 5` you should return `[0,1,1,2,1,2]`.


**Follow up:**
- It is very easy to come up with a solution with run time **O(n*sizeof(integer))**. But can you do it in linear time **O(n)** /possibly in a single pass?
- Space complexity should be **O(n)**.
- Can you do it like a boss? Do it without using any builtin function like **__builtin_popcount**  in c++ or in any other language.

**Hint:**
- You should make use of what you have produced already.
- Divide the numbers in ranges like [2-3], [4-7], [8-15] and so on. And try to generate new range from previous.
- Or does the odd/even status of the number help you in calculating the number of 1s?

**Credits:**
Special thanks to [@ syedee](https://leetcode.com/discuss/user/syedee) for adding this problem and creating all test cases.



这道题给我们一个整数n，然我们统计从0到n每个数的二进制写法的1的个数，存入一个一维数组中返回，题目中明确表示不希望我们一个数字一个数字，一位一位的傻算，而是希望我们找出规律，而且题目中也提示了我们注意[2-3], [4-7], [8-15]这些区间的规律，那么我们写出0到15的数的二进制和1的个数如下：

```
0    0000    0
-------------
1    0001    1
-------------
2    0010    1
3    0011    2
-------------
4    0100    1
5    0101    2
6    0110    2
7    0111    3
-------------
8    1000    1
9    1001    2
10   1010    2
11   1011    3
12   1100    2
13   1101    3
14   1110    3
15   1111    4
```

我最先看出的规律是这样的，除去前两个数字0个1，从2开始，2和3，是[21, 22)区间的，值为1和2。而4到7属于[22, 23)区间的，值为1,2,2,3，前半部分1和2和上一区间相同，2和3是上面的基础上每个数字加1。再看8到15，属于[23, 24)区间的，同样满足上述规律，所以可以写出代码如下：



解法一：

```
class Solution {
public:
    vector<int> countBits(int num) {
        if (num == 0) return {0};
        vector<int> res{0, 1};
        int k = 2, i = 2;
        while (i <= num) {
            for (i = pow(2, k - 1); i < pow(2, k); ++i) {
                if (i > num) break;
                int t = (pow(2, k) - pow(2, k - 1)) / 2;
                if (i < pow(2, k - 1) + t) res.push_back(res[i - t]);
                else res.push_back(res[i - t] + 1);
            }
            ++k;
        }
        return res;
    }
};
```



下面来看一种投机取巧的方法，直接利用了built-in的函数bitset的count函数可以直接返回1的个数，题目中说了不提倡用这种方法，写出来只是多一种思路而已：



解法二：

```
class Solution {
public:
    vector<int> countBits(int num) {
        vector<int> res;
        for (int i = 0; i <= num; ++i) {
            res.push_back(bitset<32>(i).count());
        }
        return res;
    }
};
```



下面这种方法相比第一种方法就要简洁很多了，这个规律找的更好，规律是，从1开始，遇到偶数时，其1的个数和该偶数除以2得到的数字的1的个数相同，遇到奇数时，其1的个数等于该奇数除以2得到的数字的1的个数再加1，参见代码如下：



解法三：

```
class Solution {
public:
    vector<int> countBits(int num) {
        vector<int> res{0};
        for (int i = 1; i <= num; ++i) {
            if (i % 2 == 0) res.push_back(res[i / 2]);
            else res.push_back(res[i / 2] + 1);
        }
        return res;
    }
};
```



下面这种方法就更加巧妙了，巧妙的利用了i&(i - 1)， 这个本来是用来判断一个数是否是2的指数的快捷方法，比如8，二进制位1000, 那么8&(8-1)为0，只要为0就是2的指数, 那么我们现在来看一下0到15的数字和其对应的i&(i - 1)值：

```
i    bin       '1'    i&(i-1)
0    0000    0
-----------------------
1    0001    1    0000
-----------------------
2    0010    1    0000
3    0011    2    0010
-----------------------
4    0100    1    0000
5    0101    2    0100
6    0110    2    0100
7    0111    3    0110
-----------------------
8    1000    1    0000
9    1001    2    1000
10   1010    2    1000
11   1011    3    1010
12   1100    2    1000
13   1101    3    1100
14   1110    3    1100
15   1111    4    1110
```

我们可以发现每个i值都是i&(i-1)对应的值加1，这样我们就可以写出代码如下：



解法四：

```
class Solution {
public:
    vector<int> countBits(int num) {
        vector<int> res(num + 1, 0);
        for (int i = 1; i <= num; ++i) {
            res[i] = res[i & (i - 1)] + 1;
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/discuss/92796/four-lines-c-time-o-n-space-o-1](https://leetcode.com/discuss/92796/four-lines-c-time-o-n-space-o-1)

[https://leetcode.com/discuss/92694/my-408-ms-c-solution-using-bitset](https://leetcode.com/discuss/92694/my-408-ms-c-solution-using-bitset)

[https://leetcode.com/discuss/92698/my-448ms-c-easy-solution-o-n-time-and-o-n-space](https://leetcode.com/discuss/92698/my-448ms-c-easy-solution-o-n-time-and-o-n-space)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












