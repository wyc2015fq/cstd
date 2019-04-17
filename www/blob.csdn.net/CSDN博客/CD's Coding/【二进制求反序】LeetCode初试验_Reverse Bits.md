# 【二进制求反序】LeetCode初试验_Reverse Bits - CD's Coding - CSDN博客





2015年04月03日 14:57:16[糖果天王](https://me.csdn.net/okcd00)阅读数：1943








今天在网上看到有人说LeetCode很适合刷题，于是我就随手点了一道题看看。







### Reverse Bits
Total Accepted: 12829Total
 Submissions: 47004[My Submissions](https://leetcode.com/problems/reverse-bits/submissions/)
Question
 Solution 







Reverse bits of a given 32 bits unsigned integer.

For example, given input 43261596 (represented in binary as 00000010100101000001111010011100), return 964176192 (represented in binary as00111001011110000010100101000000).

Follow up:

If this function is called many times, how would you optimize it?

Related problem: [Reverse Integer](https://leetcode.com/problems/reverse-integer/)










这是一道很普通的二进制求反序二进制的问题，脑中想到的自然还是和十进制一样取余放到新的数里去，然后乘以十循环到底。

然后在Discuss看到一个3ms的算法，对哦~毕竟是二进制，那么因材制宜采用二进制的各种运算自然会快得多！

二进制有哪些在底层来说是基础运算呢？ 逻辑运算，左右移。

又因为uint32_t是一个32位的数，所以不管这数原先是多少都呆胶布~

res=(res<<1)^(n&1)  容器左移一格然后把移出来的末位放上原数n的末尾

n>>=1 然后原数的末尾就不需要咯，右移扔掉~

Code：



```cpp
class Solution {
public:
    uint32_t reverseBits(uint32_t n) {
        int i;
        uint32_t res=0;
        for(i=0;i<32;i++)
        {
            res=(res<<1)^(n&1);
            n>>=1;
        }
        return res;
    }
};
```














