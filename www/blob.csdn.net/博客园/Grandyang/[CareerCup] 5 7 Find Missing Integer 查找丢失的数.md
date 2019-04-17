# [CareerCup] 5.7 Find Missing Integer 查找丢失的数 - Grandyang - 博客园







# [[CareerCup] 5.7 Find Missing Integer 查找丢失的数](https://www.cnblogs.com/grandyang/p/4747429.html)







5.7 An array A contains all the integers from 0 to n, except for one number which is missing. In this problem, we cannot access an entire integer in A with a single operation. The elements of A are represented in binary, and the only operation we can use to access them is "fetch the jth bit of A[i]," which takes constant time. Write code to find the missing integer. Can you do it in 0(n) time?



这道题给我们一个0到n的数组，然后其中一个数丢失了，让我们找到这个数。一个比较容易想到的方法是先用高斯求和公式求出等差数列0到n的和，然后遍历数组求所有数字之和，那么差值就是结果。但是这个解法就没有用到高大上的位操作Bit Manipulation，也许也不是出题人想考察的方法。题目中说了所有的整型数都是以二进制数表示的，而且可以以O(1)的时间来访问任意一位，这也很明显的提示了我们用位操作。我们先来看一个n=13的例子，那么0到13的二进制数表示如下：

00000        00100        01000        01100
00001        00101        01001        01101
00010        00110        01010
00011        00111        01011

然后我们观察上面的数字的最低有效位Least Significant Bit (LSB)，然后我们统计1和0的个数，我们可以得出一个规律，当n是奇数时，0和1的个数相等，当n是偶数时，0比1多一个。那么当我们移除一个数的话，就有下面四种情况：
||N为偶数，0比1多一个|N为奇数，0和1个数相等|
|----|----|----|
|移除0|0和1个数相等|0比1个数少|
|移除1|0比1个数多|0比1个数多|

从上表可以看出来我们比较LSB的1和0的个数就可以知道移除的是1还是0，只要0的个数小于等于1，就是移除0，若0的个数大于1，就是移除1.我们一旦知道最低有效位移除的值，那么把所有不符合要求的都去掉，然后再用相同方法来判断倒数第二低有效位，以此类推直到所有的数字都被移除了，具体过程参见如下，当n=13，且移除了一个数：

00000        00100        01000        01100
00001        00101        01001        01101
00010        00110        01010
------           00111        01011

那么我们来统计最低有效位的0和1的个数，发现0比1个数多，由此我们知道移除的数的最低有效位是1，那么我们把所有最低有效位是0的数都去掉：

00000001000100001100
00001        00101        01001        01101
000100011001010
------           00111        01011

我们再来统计倒数第二低有效位的0和1的个数，我们发现0比1个数多，那么移除数的倒数第二低有效位还是1，同样把其他的去掉：

00000001000100001100
00001001010100101101
000100011001010
------           00111        01011

我们再来统计倒数第三低有效位的0和1的个数，我们发现0比1个数相等，那么移除数的倒数第三低有效位是0，同样把其他的去掉，那么就只剩下一个了:

01011

那么倒数第四低有效位的0比1个数小，移除的是0，在把这个不是0的删掉，则数组为空了，我们就可以停止了，把所有移除的组合起来就得到0011，也就是最后的答案了，参见代码如下：



```
class Solution {
public:
    int findMissing(vector<int> nums) {
        return findMissing(nums, 0);
    }
    int findMissing(vector<int> nums, int col) {
        if (nums.empty()) return 0;
        vector<int> oneBits, zeroBits;
        for (auto &a : nums) {
            if (fetch(a, col) == 0) zeroBits.push_back(a);
            else oneBits.push_back(a);
        }
        if (zeroBits.size() <= oneBits.size()) {
            int v = findMissing(zeroBits, col + 1);
            return (v << 1) | 0;
        } else {
            int v = findMissing(oneBits, col + 1);
            return (v << 1) | 1;
        }
    }
    int fetch(int n, int col) {
        return n & (int)pow(2, col);
    }
};
```














