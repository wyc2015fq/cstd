# leetcode-48. Rotate Image 旋转图像，矩阵旋转90度 - 别说话写代码的博客 - CSDN博客





2019年01月09日 23:19:29[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：31
所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)








|You are given an *n* x *n* 2D matrix representing an image.Rotate the image by 90 degrees (clockwise).**Note:**You have to rotate the image [**in-place**](https://en.wikipedia.org/wiki/In-place_algorithm), which means you have to modify the input 2D matrix directly. **DO NOT** allocate another 2D matrix and do the rotation.**Example 1:**Given **input matrix** = [  [1,2,3],  [4,5,6],  [7,8,9]],rotate the input matrix **in-place** such that it becomes:[  [7,4,1],  [8,5,2],  [9,6,3]]|给定一个 *n *× *n* 的二维矩阵表示一个图像。将图像顺时针旋转 90 度。**说明：**你必须在**[原地](https://baike.baidu.com/item/%E5%8E%9F%E5%9C%B0%E7%AE%97%E6%B3%95)**旋转图像，这意味着你需要直接修改输入的二维矩阵。**请不要**使用另一个矩阵来旋转图像。**示例 1:**给定 **matrix** = [  [1,2,3],  [4,5,6],  [7,8,9]],**原地**旋转输入矩阵，使其变为:[  [7,4,1],  [8,5,2],  [9,6,3]]**示例 2:**给定 **matrix** =[  [ 5, 1, 9,11],  [ 2, 4, 8,10],  [13, 3, 6, 7],  [15,14,12,16]], **原地**旋转输入矩阵，使其变为:[  [15,13, 2, 5],  [14, 3, 4, 1],  [12, 6, 8, 9],  [16, 7,10,11]]|
|----|----|

思路：

**第一种**方法：先按 主对角线旋转矩阵，然后每一行元素顺序逆置（或者按副对角线旋转，然后每列元素逆置）。逆时针旋转类似

1  2  3　　　 　　 1  4  77  4  1

4  5  6　　-->　　 2  5  8　　 -->  　  8  5  2

7  8  9 　　　 　　3  6  9　　　　      9  6  3

```cpp
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        int n=matrix.size();
        for(int i=0;i<n;++i)
        {
            for(int j=i+1;j<n;++j)
                swap(matrix[i][j],matrix[j][i]);
            reverse(matrix[i].begin(),matrix[i].end());
        }
    }
};
```

**第二种**方法：计算旋转后的新位置，然后再计算下一个新位置，第四个位置又变成当前位置了，所以这个方法每次循环换四个数字，如下所示：

1  2  3                 7  2  1                  7  4  1

4  5  6      -->      4  5  6　　 -->  　 8  5  2

7  8  9                 9  8  3　　　　　 9  6  3

```cpp
class Solution {
public:
    void rotate(vector<vector<int> > &matrix) {
        int n = matrix.size();
        for (int i = 0; i < n / 2; ++i) {
            for (int j = i; j < n - 1 - i; ++j) {
                int tmp = matrix[i][j];
                matrix[i][j] = matrix[n - 1 - j][i];
                matrix[n - 1 - j][i] = matrix[n - 1 - i][n - 1 - j];
                matrix[n - 1 - i][n - 1 - j] = matrix[j][n - 1 - i];
                matrix[j][n - 1 - i] = tmp;
            }
        }
    }
};
```





