# [CareerCup] 9.9 Eight Queens 八皇后问题 - Grandyang - 博客园







# [[CareerCup] 9.9 Eight Queens 八皇后问题](https://www.cnblogs.com/grandyang/p/4845827.html)







9.9 Write an algorithm to print all ways of arranging eight queens on an 8x8 chess board so that none of them share the same row, column or diagonal. In this case, "diagonal" means all diagonals, not just the two that bisect the board.



LeetCode上的原题，请参见我之前的博客[N-Queens N皇后问题](http://www.cnblogs.com/grandyang/p/4377782.html)和[N-Queens II N皇后问题之二](http://www.cnblogs.com/grandyang/p/4380706.html)。



```
class Solution {
public:
    vector<vector<int> > placeQueens(int n) {
        vector<vector<int> > res;
        vector<int> pos(n, -1);
        placeQueensDFS(pos, 0, res);
        return res;
    }
    void placeQueensDFS(vector<int> &pos, int row, vector<vector<int> > &res) {
        int n = pos.size();
        if (row == n) res.push_back(pos);
        else {
            for (int col = 0; col < n; ++col) {
                if (isValid(pos, row, col)) {
                    pos[row] = col;
                    placeQueensDFS(pos, row + 1, res);
                    pos[row] = -1;
                }
            }
        }
    }
    bool isValid(vector<int> &pos, int row, int col) {
        for (int i = 0; i < row; ++i) {
            if (col == pos[i] || abs(row - i) == abs(col - pos[i])) {
                return false;
            }
        }
        return true;
    }
};
```














