# lintcode - 矩阵归零 - guoziqing506的博客 - CSDN博客





2016年08月28日 08:55:51[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1234
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)








题目描述：矩阵归零。给定一个m×n矩阵，如果一个元素是0，则将其所在行和列全部元素变成0。需要在原矩阵上完成操作。

样例：给出一个矩阵

[
  [1, 2],
  [0, 3]
]

返回

[
  [0, 2],
  [0, 0]

]




难点在与我们需要对矩阵中为0的元素的位置做一个记录，然后在根据这个记录将相应的位置全部变为0. 而如果先不做记录，而是直接在矩阵原地进行调整，那显然就乱套了，因为我们将不知道当前看到的0是本来就有的，还是后来我们自己把它变成0的。




那么如何记录这些0的位置呢？思路可以循序渐进。




首先，可以建立一个等大的矩阵，然后按行按列遍历原矩阵。在新建的矩阵中记录每一个0出现的位置，然后将原矩阵中这些位置所在的行和列全部变为0. 这样一来，算法的空间复杂度较高，为O(mn)




这个空间复杂度是可以优化的。我们用两个列表就可以解决：一个长为m，一个长为n。分别记录对应的行跟列有没有0元素，最后，对矩阵做修改。空间复杂度O(m + n)




到此，好像很难对空间复杂度做更深层次的优化了。那么其实还有一种非常巧妙的思路，能将空间复杂度降为O(1). 具体是这样：我们先遍历矩阵的第一行，第一列。记录一下这个两组元素是否有0（当然只是记录，不做任何修改），然后遍历矩阵其余的元素，若出现0，则将这个0元素所在的行列数在矩阵第一行，第一列中做记录。例如，遍历发现矩阵的第 i 行，第 j 列是0，那么将矩阵第一行的第 j 个元素变为0，再将矩阵第一列的第 i 个元素变为0。这样一来，确实是改变了矩阵中的元素，但只是第一行与第一列的，而第一行，第一列含不含0这件事我们之前已经记录过了，所以不影响。接下来，按照现在第一行，第一列中0的分布调整矩阵中其余元素的值，最后，按照一开始时第一行，第一列的含0情况，调整第一行，第一列的值。




这样做，是不会增加额外的空间复杂度的，可以说是最优的方法。




代码如下：



```python
class Solution:
    """
    @param matrix: A list of lists of integers
    @return: Nothing
    """
    def setZeroes(self, matrix):

        # row_0, col_0表示第一行，第一列是否为0
        row_0, col_0 = False, False
        m, n = len(matrix), len(matrix[0])

        # 检查第一行
        for j in range(n):
            if matrix[0][j] == 0:
                row_0 = True
                break

        # 检查第一列
        for i in range(m):
            if matrix[i][0] == 0:
                col_0 = 0
                break

        # 扫描其余元素，并在第一行，第一列记录
        for i in range(m):
            for j in range(n):
                if matrix[i][j] == 0:
                    matrix[0][j] = 0
                    matrix[i][0] = 0

        # 按照第一行，第一列的记录，修改矩阵中其余元素
        for i in range(m):
            for j in range(n):
                if matrix[0][j] == 0 or matrix[i][0] == 0:
                    matrix[i][j] = 0

        # 修改第一行，第一列
        if row_0 == 0:
            for j in range(n):
                matrix[0][j] = 0

        if col_0 == 0:
            for i in range(m):
                matrix[i][0] = 0
        # write your code here
```







