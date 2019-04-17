# [CareerCup] 5.4 Explain Expression ((n & (n-1)) == 0) 解释表达式 - Grandyang - 博客园







# [[CareerCup] 5.4 Explain Expression ((n & (n-1)) == 0) 解释表达式](https://www.cnblogs.com/grandyang/p/4728639.html)







5.4 Explain what the following code does: ((n & (n-1)) == 0).



这道题让我们解释一个表达式((n & (n-1)) == 0)，是说一个数跟比它小1的数字按位相与，结果全是0的情况，那么说明两个数每个位置上至少都有一个0，那么最有可能的情况是连个数的各个位置的数是相反的，而且还要相差1，那么最后可能的情况是一个数的最高位是1，其余位都是0，则其减1即为向最高位借1，其余位均为1，例如： 二进数的10000和1111就相差1，且满足表达式。那么我们知道最高位是1，其余位都是0的二进数就是2的次数。



LeetCode上的原题，请参见我之前的博客[Power of Two 判断2的次方数](http://www.cnblogs.com/grandyang/p/4623394.html)。












