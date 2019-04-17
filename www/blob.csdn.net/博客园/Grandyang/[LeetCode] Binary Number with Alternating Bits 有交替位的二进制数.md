# [LeetCode] Binary Number with Alternating Bits 有交替位的二进制数 - Grandyang - 博客园







# [[LeetCode] Binary Number with Alternating Bits 有交替位的二进制数](https://www.cnblogs.com/grandyang/p/7696387.html)








Given a positive integer, check whether it has alternating bits: namely, if two adjacent bits will always have different values.

Example 1:
Input: 5
Output: True
Explanation:
The binary representation of 5 is: 101



Example 2:
Input: 7
Output: False
Explanation:
The binary representation of 7 is: 111.



Example 3:
Input: 11
Output: False
Explanation:
The binary representation of 11 is: 1011.



Example 4:
Input: 10
Output: True
Explanation:
The binary representation of 10 is: 1010.



这道题让我们判断一个二进制数的1和0是否是交替出现的，博主开始也美想到啥简便方法，就一位一位来检测呗，用个变量bit来记录上一个位置的值，初始化为-1，然后我们用‘与’1的方法来获取最低位的值，如果是1，那么当此时bit已经是1的话，说明两个1相邻了，返回false，否则bit赋值为1。同理，如果是0，那么当此时bit已经是0的话，说明两个0相邻了，返回false，否则bit赋值为0。判断完别忘了将n向右移动一位。如果while循环退出了，返回true，参见代码如下：



解法一：

```
class Solution {
public:
    bool hasAlternatingBits(int n) {
        int bit = -1;
        while (n > 0) {
            if (n & 1 == 1) {
                if (bit == 1) return false;
                bit = 1;
            } else {
                if (bit == 0) return false;
                bit = 0;
            }
            n >>= 1;
        }
        return true;
    }
};
```



下面这种解法写的更加简洁了，我们不需要用if条件来判断，而是可以通过‘亦或’1的方式来将0和1互换，当然我们也可以通过d = 1 - d 来达到同样的效果，但还是写成‘亦或’1比较叼，while循环的条件是最低位等于d，而d不停的在0和1之间切换，n每次也向右平移一位，这样能交替检测0和1，循环退出后，如果n为0，则返回true，反之则返回false，参见代码如下：



解法二：

```
class Solution {
public:
    bool hasAlternatingBits(int n) {
        int d = n & 1;
        while ((n & 1) == d) {
            d ^= 1;
            n >>= 1;
        }
        return n == 0;
    }
};
```



下面这种解法就十分的巧妙了，利用了0和1的交替的特性，进行错位相加，从而组成全1的二进制数，然后再用一个检测全1的二进制数的trick，就是‘与’上加1后的数，因为全1的二进制数加1，就会进一位，并且除了最高位，其余位都是0，跟原数相‘与’就会得0，所以我们可以这样判断。比如n是10101，那么n>>1就是1010，二者相加就是11111，再加1就是100000，二者相‘与’就是0，参见代码如下：



解法三：

```
class Solution {
public:
    bool hasAlternatingBits(int n) {
        return ((n + (n >> 1) + 1) & (n + (n >> 1))) == 0;
    }
};
```



下面这种解法也很巧妙，先将n右移两位，再和原来的n亦或，得到的新n其实就是除了最高位，其余都是0的数，然后再和自身减1的数相‘与’，如果是0就返回true，反之false。比如n是10101，那么n/4是101，二者相‘亦或’，得到10000，此时再减1，为1111，二者相‘与’得0，参见代码如下：



解法四：

```
class Solution {
public:
    bool hasAlternatingBits(int n) {
        return ((n ^= n / 4) & (n - 1)) == 0;
    }
};
```



类似题目：

[Number of 1 Bits](http://www.cnblogs.com/grandyang/p/4325432.html)



参考资料：

[https://discuss.leetcode.com/topic/106280/c-concise-code](https://discuss.leetcode.com/topic/106280/c-concise-code)

[https://discuss.leetcode.com/topic/106356/oneliners-c-java-ruby-python](https://discuss.leetcode.com/topic/106356/oneliners-c-java-ruby-python)


[https://discuss.leetcode.com/topic/106295/java-c-very-simple-solution-1-line](https://discuss.leetcode.com/topic/106295/java-c-very-simple-solution-1-line)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












