# [CareerCup] 11.6 Search a 2D Matrix 搜索一个二维矩阵 - Grandyang - 博客园







# [[CareerCup] 11.6 Search a 2D Matrix 搜索一个二维矩阵](https://www.cnblogs.com/grandyang/p/4889019.html)







11.6 Given an M x N matrix in which each row and each column is sorted in ascending order, write a method to find an element.



LeetCode上的原题，请参见我之前的博客[Search a 2D Matrix 搜索一个二维矩阵](http://www.cnblogs.com/grandyang/p/4323301.html)和[Search a 2D Matrix II 搜索一个二维矩阵之二](http://www.cnblogs.com/grandyang/p/4669134.html)。



```
class Solution {
public:
    bool findElement(vector<vector<int> > &matrix, int elem) {
        if (matrix.empty() || matrix[0].empty()) return false;
        int row = 0, col = matrix[0].size() - 1;
        while (row < matrix.size() && col >= 0) {
            if (matrix[row][col] == elem) return true;
            else if (matrix[row][col] < elem) ++row;
            else --col;
        }
        return false;
    }
};
```














