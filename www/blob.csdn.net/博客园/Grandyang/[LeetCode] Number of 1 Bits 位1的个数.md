# [LeetCode] Number of 1 Bits 位1的个数 - Grandyang - 博客园







# [[LeetCode] Number of 1 Bits 位1的个数](https://www.cnblogs.com/grandyang/p/4325432.html)









Write a function that takes an unsigned integer and returns the number of ’1' bits it has (also known as the [Hamming weight](http://en.wikipedia.org/wiki/Hamming_weight)).

For example, the 32-bit integer ’11' has binary representation `00000000000000000000000000001011`, so the function should return 3.



很简单的一道位操作Bit Manipulation的题，最近新出的三道题都没有啥难度啊，这样会误导新人的，做了这三道得出个LeetCode没啥难度的结论，其实里面好题真的不少，难题也很多，经典题也多，反正就是赞赞赞，32个赞。



```
class Solution {
public:
    int hammingWeight(uint32_t n) {
        int res = 0;
        for (int i = 0; i < 32; ++i) {
            res += (n & 1);
            n = n >> 1;
        }
        return res;
    }
};
```



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












