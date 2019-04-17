# [LeetCode] Count Primes 质数的个数 - Grandyang - 博客园







# [[LeetCode] Count Primes 质数的个数](https://www.cnblogs.com/grandyang/p/4462810.html)







Count the number of prime numbers less than a non-negative number, *n*.

Example:
Input: 10
Output: 4
Explanation: There are 4 prime numbers less than 10, they are 2, 3, 5, 7.
References:

[How Many Primes Are There?](https://primes.utm.edu/howmany.html)

[Sieve of Eratosthenes](http://en.wikipedia.org/wiki/Sieve_of_Eratosthenes)


Credits:
Special thanks to [@mithmatt](https://leetcode.com/discuss/user/mithmatt) for adding this problem and creating all test cases.



这道题给定一个非负数n，让我们求小于n的质数的个数，题目中给了充足的提示，解题方法就在第二个提示[埃拉托斯特尼筛法Sieve of Eratosthenes](http://zh.wikipedia.org/wiki/%E5%9F%83%E6%8B%89%E6%89%98%E6%96%AF%E7%89%B9%E5%B0%BC%E7%AD%9B%E6%B3%95)中，这个算法的过程如下图所示，我们从2开始遍历到根号n，先找到第一个质数2，然后将其所有的倍数全部标记出来，然后到下一个质数3，标记其所有倍数，一次类推，直到根号n，此时数组中未被标记的数字就是质数。我们需要一个n-1长度的bool型数组来记录每个数字是否被标记，长度为n-1的原因是题目说是小于n的质数个数，并不包括n。 然后我们来实现埃拉托斯特尼筛法，难度并不是很大，代码如下所示：



![埃拉托斯特尼筛法](http://upload.wikimedia.org/wikipedia/commons/b/b9/Sieve_of_Eratosthenes_animation.gif)



```
class Solution {
public:
    int countPrimes(int n) {
        int res = 0;
        vector<bool> prime(n, true);
        for (int i = 2; i < n; ++i) {
            if (prime[i]) {
                ++res;
                for (int j = 2; i * j < n; ++j) {
                    prime[i * j] = false;
                }
            }
        }
        return res;
    }
};
```



类似题目：

[Ugly Number II](http://www.cnblogs.com/grandyang/p/4743837.html)

[Ugly Number](http://www.cnblogs.com/grandyang/p/4741934.html)

[Perfect Squares](http://www.cnblogs.com/grandyang/p/4800552.html)



参考资料：

[https://leetcode.com/problems/count-primes/](https://leetcode.com/problems/count-primes/)

[https://leetcode.com/problems/count-primes/discuss/57588/My-simple-Java-solution](https://leetcode.com/problems/count-primes/discuss/57588/My-simple-Java-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












