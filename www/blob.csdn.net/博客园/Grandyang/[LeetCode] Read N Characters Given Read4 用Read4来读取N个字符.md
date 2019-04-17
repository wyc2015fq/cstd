# [LeetCode] Read N Characters Given Read4 用Read4来读取N个字符 - Grandyang - 博客园







# [[LeetCode] Read N Characters Given Read4 用Read4来读取N个字符](https://www.cnblogs.com/grandyang/p/5174322.html)







The API: int read4(char *buf) reads 4 characters at a time from a file.
The return value is the actual number of characters read. For example, it returns 3 if there is only 3 characters left in the file.
By using the read4 API, implement the function int read(char *buf, int n) that reads n characters from the file.
Note:
The read function will only be called once for each test case.



这道题给了我们一个Read4函数，每次可以从一个文件中最多读出4个字符，如果文件中的字符不足4个字符时，返回准确的当前剩余的字符数。现在让我们实现一个最多能读取n个字符的函数。这题有迭代和递归的两种解法，我们先来看迭代的方法，思路是我们每4个读一次，然后把读出的结果判断一下，如果为0的话，说明此时的buf已经被读完，跳出循环，直接返回res和n之中的较小值。否则一直读入，直到读完n个字符，循环结束，最后再返回res和n之中的较小值，参见代码如下：



解法一：

```
// Forward declaration of the read4 API.
int read4(char *buf);

class Solution {
public:
    int read(char *buf, int n) {
        int res = 0;
        for (int i = 0; i <= n / 4; ++i) {
            int cur = read4(buf + res);
            if (cur == 0) break;
            res += cur;
        }
        return min(res, n);
    }
};
```



下面来看递归的解法，这个也不难，我们对buf调用read4函数，然后判断返回值t，如果返回值t大于等于n，说明此时n不大于4，直接返回n即可，如果此返回值t小于4，直接返回t即可，如果都不是，则直接返回调用递归函数加上4，其中递归函数的buf应往后推4个字符，此时n变成n-4即可，参见代码如下：



解法二：

```
// Forward declaration of the read4 API.
int read4(char *buf);

class Solution {
public:
    int read(char *buf, int n) {
        int t = read4(buf);
        if (t >= n) return n;
        if (t < 4) return t;
        return 4 + read(&buf[4], n - 4);
    }
};
```



类似题目：

[Read N Characters Given Read4 II - Call multiple times](http://www.cnblogs.com/grandyang/p/5181672.html)



参考资料：

[https://leetcode.com/discuss/61941/ap-solution-c-0ms-4lines](https://leetcode.com/discuss/61941/ap-solution-c-0ms-4lines)

[https://leetcode.com/discuss/65714/my-solution-using-recursion](https://leetcode.com/discuss/65714/my-solution-using-recursion)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












