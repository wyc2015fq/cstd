
# 191. Number of 1 Bits - OraYang的博客 - CSDN博客

2017年07月28日 13:30:42[OraYang](https://me.csdn.net/u010665216)阅读数：181所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Write a function that takes an unsigned integer and returns the number of ’1' bits it has (also known as the[Hamming
 weight](http://en.wikipedia.org/wiki/Hamming_weight)).
For example, the 32-bit integer ’11' has binary representation`00000000000000000000000000001011`,
 so the function should return 3.
思路：本题思路可以参考我的上一篇博客[190.Reverse Bits](http://blog.csdn.net/u010665216/article/details/76247776)，还是按位与操作
代码：

```python
class Solution {
public:
    int hammingWeight(uint32_t n) {
        int res = 0;
        for (int i=0;i<32;i++)
        {
            if(n&1==1)
                res++;
            n = n>>1;
        }
        return res;
    }
};
```



