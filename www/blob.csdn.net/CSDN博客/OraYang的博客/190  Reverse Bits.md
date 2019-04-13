
# 190. Reverse Bits - OraYang的博客 - CSDN博客

2017年07月28日 12:59:56[OraYang](https://me.csdn.net/u010665216)阅读数：185所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Reverse bits of a given 32 bits unsigned integer.
For example, given input 43261596 (represented in binary as00000010100101000001111010011100), return 964176192 (represented in binary as00111001011110000010100101000000).
Follow up:
If this function is called many times, how would you optimize it?
思路：通过按位左移即可
代码：

```python
class Solution {
public:
    uint32_t reverseBits(uint32_t n) {
        uint32_t res=0;
        for(int i=0;i<32;i++)
        {
            if(n&1==1)
                res = (res<<1) +1;
            else
                res = res<<1;
            n = n>>1;
        }
        return res;
    }
};
```



