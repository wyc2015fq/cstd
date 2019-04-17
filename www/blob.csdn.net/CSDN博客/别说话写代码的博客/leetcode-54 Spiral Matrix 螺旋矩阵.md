# leetcode-54. Spiral Matrix 螺旋矩阵 - 别说话写代码的博客 - CSDN博客





2019年01月10日 20:20:54[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：26标签：[Spiral Matrix																[leetcode																[螺旋矩阵](https://so.csdn.net/so/search/s.do?q=螺旋矩阵&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



|Given a matrix of *m* x *n* elements (*m* rows, *n* columns), return all elements of the matrix in spiral order.**Example 1:****Input:**[ [ 1, 2, 3 ], [ 4, 5, 6 ], [ 7, 8, 9 ]]**Output:** [1,2,3,6,9,8,7,4,5]**Example 2:****Input:**[  [1, 2, 3, 4],  [5, 6, 7, 8],  [9,10,11,12]]**Output:** [1,2,3,4,8,12,11,10,9,5,6,7]|给定一个包含 *m* x *n* 个元素的矩阵（*m* 行, *n* 列），请按照顺时针螺旋顺序，返回矩阵中的所有元素。**示例 1:****输入:**[ [ 1, 2, 3 ], [ 4, 5, 6 ], [ 7, 8, 9 ]]**输出:** [1,2,3,6,9,8,7,4,5]**示例 2:****输入:**[  [1, 2, 3, 4],  [5, 6, 7, 8],  [9,10,11,12]]**输出:** [1,2,3,4,8,12,11,10,9,5,6,7]|
|----|----|

思路：4个指针。left,right,up,down；分别遍历后移动。 注意边界值情况。

```cpp
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        if(matrix.empty() || matrix[0].empty()) return {};
        int rowi=0,rowj=matrix.size()-1;
        int coli=0,colj=matrix[0].size()-1;
        vector<int> res;
        while(rowi<=rowj && coli<=colj)
        {
            for(int i=coli;i<=colj;++i)
                res.push_back(matrix[rowi][i]);
            if(++rowi>rowj) break;
            for(int i=rowi;i<=rowj;++i)
                res.push_back(matrix[i][colj]);
            if(--colj<coli) break;
            for(int i=colj;i>=coli;--i)
                res.push_back(matrix[rowj][i]);
            if(--rowj<rowi) break;
            for(int i=rowj;i>=rowi;--i)
                res.push_back(matrix[i][coli]);
            if(++coli>colj) break;
        }return res;
    }
};
```](https://so.csdn.net/so/search/s.do?q=Spiral Matrix&t=blog)




