# [CareerCup] 5.1 Insert Bits 插入位 - Grandyang - 博客园







# [[CareerCup] 5.1 Insert Bits 插入位](https://www.cnblogs.com/grandyang/p/4717019.html)







5.1 You are given two 32-bit numbers, N and M, and two bit positions, land j. Write a method to insert M into N such that M starts at bit j and ends at bit i. You can assume that the bits j through i have enough space to fit all of M. That is, if M = 10011, you can assume that there are at least 5 bits between j and i. You would not, for example, have j = 3 and i = 2, because M could not fully fit between bit 3 and bit 2.
EXAMPLE
Input: N = 10000000000, M = 10011, i = 2, j = 6
Output: N = 10001001100



这道题给了我们两个二进制数N和M，又给了我们两个位置i和j，让我们在把N中的[i,j]替换为M。我最先想到的方法比较笨，就是先把N的后i位取出来存入结果中，然后再把M按对应位置存入结果中，最后把j之后的N的位存入结果中，参见代码如下：



解法一：

```
class Solution {
public:
    int insertBits(int n, int m, int i, int j) {
        int res = 0;
        for (int k = 0; k < i; ++k) {
            if (n & 1) res += 1 << k;
            n = n >> 1;
        }
        for (int k = i; k <= j; ++k) {
            if (m & 1) res += 1 << k;
            m = m >> 1;
            n = n >> 1;
        }
        res += n << (j + 1);
        return res;
    }
};
```



但是书上给出了一个更巧妙的解法，首先在N中把[i,j]对应的位清空，然后把M平移到对应的位置，然后用或来合并M和N即可，方法很巧妙，参见代码如下：



解法二：

```
class Solution {
public:
    int insertBits(int n, int m, int i, int j) {
        int allOnes = ~0;
        int left = allOnes << (j + 1);
        int right = (1 << i) - 1;
        int mask = left | right;
        int n_cleared = n & mask;
        int m_shifted = m << i;
        return n_cleared | m_shifted;
    }
};
```














