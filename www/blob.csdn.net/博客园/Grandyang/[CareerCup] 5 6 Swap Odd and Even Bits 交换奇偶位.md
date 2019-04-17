# [CareerCup] 5.6 Swap Odd and Even Bits 交换奇偶位 - Grandyang - 博客园







# [[CareerCup] 5.6 Swap Odd and Even Bits 交换奇偶位](https://www.cnblogs.com/grandyang/p/4743995.html)







5.6 Write a program to swap odd and even bits in an integer with as few instructions as possible (e.g., bit 0 and bit 1 are swapped, bit 2 and bit 3 are swapped, and soon).



这道题让我们交换奇偶位，那么我们首先还是要考虑用位操作Bit Manipulation来做，我们知道由于奇偶位是相邻的，奇数位平移一位就是偶数位，反过来偶数位平移一位就是奇数位，那么这题我们可以分别将原来的奇数位和偶数位分别提取出来，各自平移一位，再将其混合成结果即可。提取的方法我们用mask来，对于一个32位的整型数，其奇数位的二进制的mask为10101010101010101010101010101010，换成十六进制数为0xaaaaaaaa，同理偶数位的二进制的mask为01010101010101010101010101010101，换成十六进制数为0x55555555，平移完将其或起来即可，参见代码如下：



```
class Solution {
public:
    int swapOddEvenBits(int x) {
        return (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
    }
};
```














