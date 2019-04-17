# [LeetCode] Power of Two 判断2的次方数 - Grandyang - 博客园







# [[LeetCode] Power of Two 判断2的次方数](https://www.cnblogs.com/grandyang/p/4623394.html)







Given an integer, write a function to determine if it is a power of two.

Example 1:
Input: 1
Output: true
Example 2:
Input: 16
Output: true
Example 3:
Input: 218
Output: false


这道题让我们判断一个数是否为2的次方数，而且要求时间和空间复杂度都为常数，那么对于这种玩数字的题，我们应该首先考虑位操作 Bit Operation。在LeetCode中，位操作的题有很多，比如比如 [Repeated DNA Sequences](http://www.cnblogs.com/grandyang/p/4284205.html)，[Single Number](http://www.cnblogs.com/grandyang/p/4130577.html),  [Single Number II](http://www.cnblogs.com/grandyang/p/4263927.html)，[Grey Code](http://www.cnblogs.com/grandyang/p/4315649.html)，[Reverse Bits](http://www.cnblogs.com/grandyang/p/4321355.html)，[Bitwise AND of Numbers Range](http://www.cnblogs.com/grandyang/p/4431646.html)，[Number of 1 Bits](http://www.cnblogs.com/grandyang/p/4325432.html) 和 [Divide Two Integers](http://www.cnblogs.com/grandyang/p/4431949.html) 等等。那么我们来观察下2的次方数的二进制写法的特点：

1     2       4         8         16 　　....

1    10    100    1000    10000　....

那么我们很容易看出来2的次方数都只有一个1，剩下的都是0，所以我们的解题思路就有了，我们只要每次判断最低位是否为1，然后向右移位，最后统计1的个数即可判断是否是2的次方数，代码如下：



解法一：

```
class Solution {
public:
    bool isPowerOfTwo(int n) {
        int cnt = 0;
        while (n > 0) {
            cnt += (n & 1);
            n >>= 1;
        }
        return cnt == 1;
    } 
};
```



这道题还有一个技巧，如果一个数是2的次方数的话，根据上面分析，那么它的二进数必然是最高位为1，其它都为0，那么如果此时我们减1的话，则最高位会降一位，其余为0的位现在都为变为1，那么我们把两数相与，就会得到0，用这个性质也能来解题，而且只需一行代码就可以搞定，如下所示：



解法二：

```
class Solution {
public:
    bool isPowerOfTwo(int n) {
        return (n > 0) && (!(n & (n - 1)));
    } 
};
```



类似题目：

[Number of 1 Bits](http://www.cnblogs.com/grandyang/p/4325432.html)

[Power of Four](http://www.cnblogs.com/grandyang/p/5403783.html)

[Power of Three](http://www.cnblogs.com/grandyang/p/5138212.html)



参考资料：

[https://leetcode.com/problems/power-of-two/discuss/63974/Using-nand(n-1)-trick](https://leetcode.com/problems/power-of-two/discuss/63974/Using-nand(n-1)-trick)

[https://leetcode.com/problems/power-of-two/discuss/63972/One-line-java-solution-using-bitCount](https://leetcode.com/problems/power-of-two/discuss/63972/One-line-java-solution-using-bitCount)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












