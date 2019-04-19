# leetcode_304. Range Sum Query 2D - Immutable ? 待解决 - wjheha的博客 - CSDN博客
2017年11月01日 10:15:25[wjheha](https://me.csdn.net/wjheha)阅读数：65
Given a 2D matrix matrix, find the sum of the elements inside the rectangle defined by its upper left corner (row1, col1) and lower right corner (row2, col2).
Range Sum Query 2D 
The above rectangle (with the red border) is defined by (row1, col1) = (2, 1) and (row2, col2) = (4, 3), which contains sum = 8.
Example: 
Given matrix = [ 
  [3, 0, 1, 4, 2], 
  [5, 6, 3, 2, 1], 
  [1, 2, 0, 1, 5], 
  [4, 1, 0, 1, 7], 
  [1, 0, 3, 0, 5] 
]
sumRegion(2, 1, 4, 3) -> 8 
sumRegion(1, 1, 2, 2) -> 11 
sumRegion(1, 2, 2, 4) -> 12 
Note: 
You may assume that the matrix does not change. 
There are many calls to sumRegion function. 
You may assume that row1 ≤ row2 and col1 ≤ col2.
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/range-sum-query-2d-immutable/description/](https://leetcode.com/problems/range-sum-query-2d-immutable/description/)
***思路***：
