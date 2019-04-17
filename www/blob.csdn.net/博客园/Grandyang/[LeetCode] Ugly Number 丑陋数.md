# [LeetCode] Ugly Number 丑陋数 - Grandyang - 博客园







# [[LeetCode] Ugly Number 丑陋数](https://www.cnblogs.com/grandyang/p/4741934.html)







Write a program to check whether a given number is an ugly number.

Ugly numbers are positive numbers whose prime factors only include `2, 3, 5`. For example, `6, 8` are ugly while `14` is not ugly since it includes another prime factor `7`.

Note that `1` is typically treated as an ugly number.



这道题让我们检测一个数是否为丑陋数，所谓丑陋数就是其质数因子只能是2,3,5。那么最直接的办法就是不停的除以这些质数，如果剩余的数字是1的话就是丑陋数了，有两种写法，如下所示：



解法一：

```
class Solution {
public:
    bool isUgly(int num) {
        while (num >= 2) {
            if (num % 2 == 0) num /= 2;
            else if (num % 3 == 0) num /= 3;
            else if (num % 5 == 0) num /= 5;
            else return false;
        }
        return num == 1;
    }
};
```



解法二：

```
class Solution {
public:
    bool isUgly(int num) {
        if (num <= 0) return false;
        while (num % 2 == 0) num /= 2;
        while (num % 3 == 0) num /= 3;
        while (num % 5 == 0) num /= 5;
        return num == 1;
    }
};
```



类似题目：

[Super Ugly Number](http://www.cnblogs.com/grandyang/p/5144918.html)

[Ugly Number II](http://www.cnblogs.com/grandyang/p/4743837.html)



参考资料：

[https://leetcode.com/discuss/60914/my-2ms-java-solution](https://leetcode.com/discuss/60914/my-2ms-java-solution)

[https://leetcode.com/discuss/52703/2-4-lines-every-language](https://leetcode.com/discuss/52703/2-4-lines-every-language)

[https://leetcode.com/discuss/78281/4ms-recursive-solution-in-c](https://leetcode.com/discuss/78281/4ms-recursive-solution-in-c)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












