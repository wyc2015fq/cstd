# [LeetCode] Non-negative Integers without Consecutive Ones 非负整数不包括连续的1 - Grandyang - 博客园







# [[LeetCode] Non-negative Integers without Consecutive Ones 非负整数不包括连续的1](https://www.cnblogs.com/grandyang/p/6959585.html)







Given a positive integer n, find the number of non-negative integers less than or equal to n, whose binary representations do NOT contain consecutive ones.

Example 1:
Input: 5
Output: 5
Explanation: 
Here are the non-negative integers <= 5 with their corresponding binary representations:
0 : 0
1 : 1
2 : 10
3 : 11
4 : 100
5 : 101
Among them, only integer 3 disobeys the rule (two consecutive ones) and the other 5 satisfy the rule. 



Note: 1 <= n <= 109



这道题给了我们一个数字，让我们求不大于这个数字的所有数字中，其二进制的表示形式中没有连续1的个数。根据题目中的例子也不难理解题意。我们首先来考虑二进制的情况，对于1来说，有0和1两种，对于11来说，有00，01，10，三种情况，那么有没有规律可寻呢，其实是有的，我们可以[参见这个帖子](http://www.geeksforgeeks.org/count-number-binary-strings-without-consecutive-1s/)，这样我们就可以通过DP的方法求出长度为k的二进制数的无连续1的数字个数。由于题目给我们的并不是一个二进制数的长度，而是一个二进制数，比如100，如果我们按长度为3的情况计算无连续1点个数个数，就会多计算101这种情况。所以我们的目标是要将大于num的情况去掉。下面从头来分析代码，首先我们要把十进制数转为二进制数，将二进制数存在一个字符串中，并统计字符串的长度。然后我们利用[这个帖子](http://www.geeksforgeeks.org/count-number-binary-strings-without-consecutive-1s/)中的方法，计算该字符串长度的二进制数所有无连续1的数字个数，然后我们从倒数第二个字符开始往前遍历这个二进制数字符串，如果当前字符和后面一个位置的字符均为1，说明我们并没有多计算任何情况，不明白的可以带例子来看。如果当前字符和后面一个位置的字符均为0，说明我们有多计算一些情况，就像之前举的100这个例子，我们就多算了101这种情况。我们怎么确定多了多少种情况呢，假如给我们的数字是8，二进制为1000，我们首先按长度为4算出所有情况，共8种。仔细观察我们十进制转为二进制字符串的写法，发现转换结果跟真实的二进制数翻转了一下，所以我们的t为"0001"，那么我们从倒数第二位开始往前遍历，到i=1时，发现有两个连续的0出现，那么i=1这个位置上能出现1的次数，就到one数组中去找，那么我们减去1，减去的就是0101这种情况，再往前遍历，i=0时，又发现两个连续0，那么i=0这个位置上能出1的次数也到one数组中去找，我们再减去1，减去的是1001这种情况，参见代码如下：



解法一：

```
class Solution {
public:
    int findIntegers(int num) {
        int cnt = 0, n = num;
        string t = "";
        while (n > 0) {
            ++cnt;
            t += (n & 1) ? "1" : "0"; 
            n >>= 1;
        }
        vector<int> zero(cnt), one(cnt);
        zero[0] = 1; one[0] = 1;
        for (int i = 1; i < cnt; ++i) {
            zero[i] = zero[i - 1] + one[i - 1];
            one[i] = zero[i - 1];
        }
        int res = zero[cnt - 1] + one[cnt - 1];
        for (int i = cnt - 2; i >= 0; --i) {
            if (t[i] == '1' && t[i + 1] == '1') break;
            if (t[i] == '0' && t[i + 1] == '0') res -= one[i];
        }
        return res;
    }
};
```



下面这种解法其实蛮有意思的，其实长度为k的二进制数字符串没有连续的1的个数是一个斐波那契数列f(k)。比如当k=5时，二进制数的范围是00000-11111，我们可以将其分为两个部分，00000-01111和10000-10111，因为任何大于11000的数字都是不成立的，因为有开头已经有了两个连续1。而我们发现其实00000-01111就是f(4)，而10000-10111就是f(3)，所以f(5) = f(4) + f(3)，这就是一个斐波那契数列啦。那么我们要做的首先就是建立一个这个数组，方便之后直接查值。我们从给定数字的最高位开始遍历，如果某一位是1，后面有k位，就加上f(k)，因为如果我们把当前位变成0，那么后面k位就可以直接从斐波那契数列中取值了。然后标记pre为1，再往下遍历，如果遇到0位，则pre标记为0。如果当前位是1，pre也是1，那么直接返回结果。最后循环退出后我们要加上数字本身这种情况，参见代码如下： 



解法二：

```
class Solution {
public:
    int findIntegers(int num) {
        int res = 0, k = 31, pre = 0;
        vector<int> f(32, 0);
        f[0] = 1; f[1] = 2;
        for (int i = 2; i < 31; ++i) {
            f[i] = f[i - 2] + f[i - 1];
        }
        while (k >= 0) {
            if (num & (1 << k)) {
                res += f[k];
                if (pre) return res;
                pre = 1;
            } else pre = 0;
            --k;
        }
        return res + 1;
    }
};
```



类似题目：

[House Robber II](http://www.cnblogs.com/grandyang/p/4518674.html)

[House Robber](http://www.cnblogs.com/grandyang/p/4383632.html)

[Ones and Zeroes](http://www.cnblogs.com/grandyang/p/6188893.html)



参考资料：

[https://discuss.leetcode.com/topic/90571/java-solution-dp](https://discuss.leetcode.com/topic/90571/java-solution-dp)

[https://discuss.leetcode.com/topic/90639/c-non-dp-o-32-fibonacci-solution](https://discuss.leetcode.com/topic/90639/c-non-dp-o-32-fibonacci-solution)

[https://discuss.leetcode.com/topic/90671/java-o-1-time-o-1-space-dp-solution](https://discuss.leetcode.com/topic/90671/java-o-1-time-o-1-space-dp-solution)

[http://www.geeksforgeeks.org/count-number-binary-strings-without-consecutive-1s/](http://www.geeksforgeeks.org/count-number-binary-strings-without-consecutive-1s/)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












