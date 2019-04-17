# lintcode - 落单的数 III - guoziqing506的博客 - CSDN博客





2016年08月17日 15:53:45[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1075
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给出2*n + 2个的数字，除其中两个数字之外其他每个数字均出现两次，找到这两个数字。

样例：给出 [1,2,2,3,4,4,5,3]，返回 1和5

挑战 ：O(n)时间复杂度，O(1)的额外空间复杂度




我们之前已经做过两道类似的题目，分别是[落单的数](http://blog.csdn.net/guoziqing506/article/details/52200303)，[落单的数 II](http://blog.csdn.net/guoziqing506/article/details/52203014)，思路都是位运算。这道题也不例外。




不过这道题想出方法来倒还真不太容易，至少我当时没想出来，也是后来查了别人的做法，才知道的，在此，我将别人的方法用我的话再说一遍，努力让它更好理解。




当然，首先想到的就是跟之前2*n + 1个数时的情况一样（详见：[落单的数](http://blog.csdn.net/guoziqing506/article/details/52200303)），先将所有的数异或一遍，这样，我们就将数组中那两个不同的数异或到了一个结果中（此处不懂的话看刚才给的链接）。现在的难处在于无法将这个结果拆开，拆成我们想要的那两个不同的数。




怎么办呢？我们如果对二进制足够熟悉，就不难得出这样一个结论，这个异或的结果（为方便描述，记为Xor）的二进制位中为1的位，必然是这两个不同的数（方便起见，记为first 和 second）不同的位，也就是说，first和second在这些位中一个是1，一个是0。不失一般性，我们就找Xor中第一个为1的位，将这个位数记为k.




那么，一定隐含了这样一个逻辑：在成对的2*n个数当中，一定有2x个数的第k位是1，而有2y个数的第k位是0，其中，x + y = n，所以，




换个说法，既然Xor的第k为是1，那我们不妨假设first的第k位是0，而second的第k位是1。那么，如果令x个数第k位为1的数，和second一起，与Xor异或，就能得到first，这个道理与2n + 1时是一样的。而再令first与Xor异或，就能得到second.




于是，可以按以下步骤操作：

1. 将数组中所有的数异或，得到一个结果，记为Xor

2. 查出Xor中第一个为1的位（也就是为1的最小的位），记为k

3. 查出数组中所有第k位为1的数（这里面当然包括second）与Xor异或，得到first

4. 将first与Xor异或，得到second




写出代码即可：



```python
class Solution:
    """
    @param A : An integer array
    @return : Two integer
    """
    def singleNumberIII(self, A):

        first, second = 0, 0
        Xor = 0

        # for循环求出不同的两个数异或的结果
        for i in A:
            Xor ^= i

        temp = Xor
        k = 1

        # 检查temp的二进制位中第一个为1的位置，记为k
        while temp & 1 != 1:
            k += 1
            temp >>= 1

        # 设置一个整数，此数的第k位是1，其他位是0
        firstHelper = pow(2, k - 1)

        # 令A中所有第k位为1的数异或，并将这个异或的结果与Xor异或，可得到第一个数
        for i in A:
            if i & firstHelper == firstHelper:
                first ^= i
        first ^= Xor

        # 第一个数与Xor异或，得到第二个数
        second = first ^ Xor
        return first, second
        # write your code here
```







