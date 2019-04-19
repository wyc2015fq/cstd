# C++ 实现矩阵转换函数算法 - fanyun的博客 - CSDN博客
2019年02月23日 22:07:17[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：187标签：[矩阵转置](https://so.csdn.net/so/search/s.do?q=矩阵转置&t=blog)
个人分类：[C++																[算法](https://blog.csdn.net/fanyun_01/article/category/7113106)](https://blog.csdn.net/fanyun_01/article/category/6102444)
1.题目要求
 给出一个简单的矩阵：
1、2、3
4、5、6
7、8、9
实现算法使得矩阵转置输出：
2.实现思路
     使用二维数组存放矩阵数据，再通过循环转置数据。
3.代码实现
```cpp
#include <iostream.h>
void move(int matrix[3][3])
{
       int i, j, k;
       for (i = 0; i < 3; i++)
              for (j = 0; j < i; j++)
              {
                     k = matrix[i][j];
                     matrix[i][j] = matrix[j][i];
                     matrix[j][i] = k;
              }
}
void main()
{
       int i, j;
       int data[3][3];
       cout << “输入矩阵的元素” << endl;
       for (i = 0; i < 3; i++)
              for (j = 0; j < 3; j++)
              {
                     cout << “第” << i + 1 << “行第” << j + 1
                     << ”个元素为：“;
                     cin >> data[i][j];
              }
       cout << “输入的矩阵的为：” << endl;
       for (i = 0; i < 3; i++)
       {
              for (j = 0; j < 3; j++)
              cout << data[i][j] << ” “;
              cout << endl;
       }
       move(data);
       cout << “转置后的矩阵的为：” << endl;
       for (i = 0; i < 3; i++)
       {
              for (j = 0; j < 3; j++)
              cout << data[i][j] << ” “;
              cout << endl;
       }
}
```
4.程序输出
输入矩阵的元素
第1 行第1 个元素为：1
第1 行第2 个元素为：2
第1 行第3 个元素为：3
第2 行第1 个元素为：4
第2 行第2 个元素为：5
第2 行第3 个元素为：6
第3 行第1 个元素为：7
第3 行第2 个元素为：8
第3 行第3 个元素为：9
**输入的矩阵的为：**
1 2 3
4 5 6
7 8 9
**转置后的矩阵的为：**
1 4 7
2 5 8
3 6 9
