# [LeetCode] Pascal's Triangle II 杨辉三角之二 - Grandyang - 博客园







# [[LeetCode] Pascal's Triangle II 杨辉三角之二](https://www.cnblogs.com/grandyang/p/4031536.html)







Given a non-negative index *k* where *k* ≤ 33, return the *k*th index row of the Pascal's triangle.

Note that the row index starts from 0.

![](https://upload.wikimedia.org/wikipedia/commons/0/0d/PascalTriangleAnimated2.gif)
In Pascal's triangle, each number is the sum of the two numbers directly above it.

Example:
Input: 3
Output: [1,3,3,1]

Follow up:

Could you optimize your algorithm to use only *O*(*k*) extra space?



杨辉三角想必大家并不陌生，应该最早出现在初高中的数学中，其实就是二项式系数的一种写法。
　　　　　　　　１
　　　　　　　１　１
　　　　　　１　２　１
　　　　　１　３　３　１
　　　　１　４　６　４　１
　　　１　５　10　10　５　１
　　１　６　15　20　15　６　１
　１　７　21　35　35　21　７　１
１　８　28　56　70　56　28　８　１
杨辉三角形第n层（顶层称第0层，第1行，第n层即第n+1行，此处n为包含0在内的自然数）正好对应于二项式![\left(a+b\right)^{n}](http://upload.wikimedia.org/math/4/b/9/4b96ccc1bce6fcd32bc5bd2fd2de4b7a.png)展开的系数。例如第二层1 2 1是幂指数为2的二项式![\left(a+b\right)^{2}](http://upload.wikimedia.org/math/8/2/0/820a2947ed4432430a12c5cb107561ed.png)展开形式![a^{2}+2ab+b^{2}](http://upload.wikimedia.org/math/5/c/2/5c2982db60ef27dfb6249933d1be73a1.png)的系数。



杨辉三角主要有下列五条性质：
- 杨辉三角以正整数构成，数字左右对称，每行由1开始逐渐变大，然后变小，回到1。
- 第![n](http://upload.wikimedia.org/math/7/b/8/7b8b965ad4bca0e41ab51de7b31363a1.png)行的数字个数为![n](http://upload.wikimedia.org/math/7/b/8/7b8b965ad4bca0e41ab51de7b31363a1.png)个。
- 第![n](http://upload.wikimedia.org/math/7/b/8/7b8b965ad4bca0e41ab51de7b31363a1.png)行的第![k](http://upload.wikimedia.org/math/8/c/e/8ce4b16b22b58894aa86c421e8759df3.png)个数字为组合数![C_{n-1}^{k-1}](http://upload.wikimedia.org/math/7/c/9/7c90e60eb2d8eaec74a30a81b1b4f1d9.png)。
- 第![n](http://upload.wikimedia.org/math/7/b/8/7b8b965ad4bca0e41ab51de7b31363a1.png)行数字和为![2^{n-1}](http://upload.wikimedia.org/math/4/2/e/42ef218254478d61d6f2afb986f4b88c.png)。
- 除每行最左侧与最右侧的数字以外，每个数字等于它的左上方与右上方两个数字之和（也就是说，第![n](http://upload.wikimedia.org/math/7/b/8/7b8b965ad4bca0e41ab51de7b31363a1.png)行第![k](http://upload.wikimedia.org/math/8/c/e/8ce4b16b22b58894aa86c421e8759df3.png)个数字等于第![n-1](http://upload.wikimedia.org/math/a/4/3/a438673491daae8148eae77373b6a467.png)行的第![k-1](http://upload.wikimedia.org/math/1/4/4/14464ac1dfe6fa8ad8fda94bb6f01571.png)个数字与第![k](http://upload.wikimedia.org/math/8/c/e/8ce4b16b22b58894aa86c421e8759df3.png)个数字的和）。这是因为有组合恒等式：![C_{n}^{i}=C_{n-1}^{i-1}+C_{n-1}^{i}](http://upload.wikimedia.org/math/c/7/0/c702e84dfa4142f435d9c2bbbbfeeae0.png)。可用此性质写出整个杨辉三角形。



由于题目有额外限制条件，程序只能使用O(k)的额外空间，那么这样就不能把每行都算出来，而是要用其他的方法, 我最先考虑用的是第三条性质，算出每个组合数来生成第n行系数，代码参见评论区一楼。本地调试输出前十行，没啥问题，拿到OJ上测试，程序在第18行跪了，中间有个系数不正确。那么问题出在哪了呢，仔细找找，原来出在计算组合数那里，由于算组合数时需要算连乘，而整形数int的数值范围只有-32768到32768之间，那么一旦n值过大，连乘肯定无法计算。而丧心病狂的OJ肯定会测试到成百上千行，所以这个方法不行。那么我们再来考虑利用第五条性质，除了第一个和最后一个数字之外，其他的数字都是上一行左右两个值之和。那么我们只需要两个for循环，除了第一个数为1之外，后面的数都是上一次循环的数值加上它前面位置的数值之和，不停地更新每一个位置的值，便可以得到第n行的数字，具体实现代码如下：



```
class Solution {
public:
    vector<int> getRow(int rowIndex) {
        vector<int> res(rowIndex + 1);
        res[0] = 1;
        for (int i = 1; i <= rowIndex; ++i) {
            for (int j = i; j >= 1; --j) {
                res[j] += res[j - 1];
            }
        }
        return res;
    }
};
```



类似题目：

[Pascal's Triangle](http://www.cnblogs.com/grandyang/p/4032449.html)



参考资料：

[https://leetcode.com/problems/pascals-triangle-ii/](https://leetcode.com/problems/pascals-triangle-ii/)

[https://leetcode.com/problems/pascals-triangle-ii/discuss/38420/Here-is-my-brief-O(k)-solution](https://leetcode.com/problems/pascals-triangle-ii/discuss/38420/Here-is-my-brief-O(k)-solution)

[https://leetcode.com/problems/pascals-triangle-ii/discuss/38478/My-accepted-java-solution-any-better-code](https://leetcode.com/problems/pascals-triangle-ii/discuss/38478/My-accepted-java-solution-any-better-code)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












