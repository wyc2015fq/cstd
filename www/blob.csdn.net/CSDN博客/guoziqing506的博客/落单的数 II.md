# 落单的数 II - guoziqing506的博客 - CSDN博客





2016年08月14日 09:31:31[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：709
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给出3*n + 1 个的数字，除其中一个数字之外其他每个数字均出现三次，找到这个数字。

样例：给出 [1,1,2,3,3,3,2,2,4,1] ，返回 4




题目的挑战还是在于一次遍历，常数级的额外空间复杂度。之前当有2 * n + 1 个数的时候的做法我们已经探讨过了（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/52200303)），用的是位运算的异或，这里，还是用相同的思路。只不过对处理三个相同元素的情况，好像没有类似于异或这样现成的位运算直接解决。




不过，换个角度思考这个问题，其实异或就是把整数的32位的0,1串当做整数0,1，对应位加和后，对2取余的结果，那么对于这样三个数的情况，一样的，可以用相同的方法，对应位的0,1相加，最后对3取余。得到的结果就是那个落单的数的二进制表示。然后恢复成整数即可。




代码如下：



```python
class Solution:
    """
    @param A : An integer array
    @return : An integer
    """
    def singleNumberII(self, A):
        result = 0
        record = [0 for i in range(32)]
        for i in A:

            # 对32位二进制串从右往左扫描
            for index in range(-1, -32, -1):
                # temp为每一位代表的数字（0或1）
                temp = (i & 1)
                record[index] += temp
                record[index] %= 3
                i = i >> 1

        # 此时的record是一个由整数0或1组成的数组，代表结果的二进制表示
        # 将数组恢复成整数
        for i in range(32):
            result += record[31 - i] * pow(2, i)
        return result
        # write your code here
```


细节参见注释吧






