# 剑指offer——顺时针打印矩阵 - 风雪夜归人 - CSDN博客
2018年05月09日 22:07:12[cdjccio](https://me.csdn.net/qq_34624951)阅读数：42
个人分类：[剑指offer](https://blog.csdn.net/qq_34624951/article/category/7623846)
Q：
输入一个矩阵，按照从外向里以顺时针的顺序依次打印出每一个数字，例如，如果输入如下矩阵：
1     2  3   4
5    6   7   8
9   10 11 12
13 14 15 16
则依次打印出数字1,2,3,4,8,12,16,15,14,13,9,5,6,7,11,10.
A：本题采取记录最外圈四个角的位置方法， 
当满足左右间距大于1，上下间距大于1时，每次剥下一圈，将四个角都往内移一位，再判断是否满足条件； 
当不满足上述条件时，判断是一行或者一列或者单独一个元素，将该行、该列、该元素加入buf。
```cpp
#include <iostream>
#include <vector>
using namespace std;
const int M = 4, N = 4;
class Solution {
public:
    vector<int> printMatrix(vector<vector<int> > matrix) {
        int top     = 0;
        int below   = matrix.size() - 1;    // 行数
        int left    = 0;
        int right   = matrix[0].size() - 1; // 列数
        vector<int > buf;
        while(right >left && top < below) // 从左到右
        {
            for(int i = left; i <= right; i++)
            {
                buf.push_back(matrix[top][i]);
            }
            for(int i = top + 1; i <= below; i++) // 从上到下
            {
                buf.push_back(matrix[i][right]);
            }
            for(int i = right - 1; i >= left; i--) // 从右到左
            {
                buf.push_back(matrix[below][i]);
            }
            for(int i = below - 1; i > top; i--) // 从下到上
            {
                buf.push_back(matrix[i][left]);
            }
            top     ++;
            below   --;
            left    ++;
            right   --;
        }
        if(top == below && left < right)  // 只有一行
        {
            for(int i = left; i <= right; i++)
            {
                buf.push_back(matrix[top][i]);
            }
        }
        if(left == right && top < below)  // 只有一列
        {
            for(int i = top; i <= below; i++)
            {
                buf.push_back(matrix[i][left]);
            }
        }
        if(left == right && top == below)   // 只有一个元素
        {
            buf.push_back(matrix[top][left]);
        }
        return buf;
    }
};
int main()
{
    vector<vector<int > > matrix;
    int k = 1;
    for(int i = 0; i<M; i++)
    {
        matrix.push_back(vector<int> (N));
    }
    for(int i = 0; i<M; i++)
    {
        for(int j = 0; j<N; j++)
        {
            matrix[i][j] = k++;
        }
    }
//    for(int i = 0; i<M; i++)
//    {
//        for(int j = 0; j<N; j++)
//        {
//            cout << matrix[i][j] << " ";
//        }
//        cout << endl;
//    }
//    cout << endl;
    Solution r;
    vector<int > result;
    result = r.printMatrix(matrix);
    int len = result.size();
    cout << len << endl;
    for(int i = 0; i<len; i++)
    {
        cout << result[i] << " " ;
    }
    return 0;
}
```
