# 【Java】【算法练习】对于一个矩阵，请设计一个算法从左上角(mat[0][0])开始，顺时针打印矩阵元素。 给定int矩阵mat,以及它的维数nxm，请返回一个数组，数组中的元素为矩阵元素的顺时针输 - csdn_baotai的博客 - CSDN博客

2018年09月06日 09:13:39[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：124


## 题目描述：

对于一个矩阵，请设计一个算法从左上角(mat[0][0])开始，顺时针打印矩阵元素。

给定int矩阵mat,以及它的维数nxm，请返回一个数组，数组中的元素为矩阵元素的顺时针输出。

测试样例：

[[1,2],[3,4]],2,2

返回：[1,2,4,3]

## 解题思路：

对于一个矩阵，顺时针打印时一定要保证边界处理的正确性；

## 解题代码：

```java
public class Printer {
    public int[] clockwisePrint(int[][] mat, int n, int m) {
        // write code here
        // 定义一个返回结果的数组，将顺时针遍历的所有值存下来返回
        int[] ret = new int[m*n];   // n为行，m为列
        // 判断非法输入的情况
        if(n <= 0 || m <= 0)
            return ret;

        // 定义控制行的下标
        int i = 0;
        // 定义控制列的下标
        int j = 0;
        // 定义控制要返回的数组ret的下标
        int index = 0;
        // 定义圈数，每遍历完一层就 +1
        int N = 0;
        // 当要返回的数组的下标超过 n*m 时，说明数组已经满了，也就是给定的mat数组中所有的数都已 
        //经遍历完了，此时结束
        while(index < ret.length) {

            // 向右打印，列++，行不动，列 < 总列数-圈数，可以有效的控制打印的列下标
            while(j < m-N && index < ret.length) {
                ret[index] = mat[i][j];
                index++;
                j++;
            }

            // 向下打印
            // 要竖着打印的第一个元素上一个横着向右打印已经遍历过了，因此 i++,来到下一行
            i++;
            // 由于上面的横着向右打印最后结束的位置在 mat[i][m - N],这里已经越界，所以要进行j--
            j--;
            // 行++，列不动，行 < 总行数-圈数
            while(i < n-N && index < ret.length) {
                ret[index] = mat[i][j];
                index++;
                i++;
            }

            // 向左打印
            j--;
            i--;
            // j >= N 和上面的道理是一样的，可以带个值进行验证一下，比第一圈(N = 0)， 所以向左 
            //遍历要到第一列(下标是0)的位置
            while(j >= N && index < ret.length) {
                ret[index] = mat[i][j];
                j--;
                index++;
            }

            // 向上打印
            j++;
            i--;
            // 注意这里没有等号，比如 第一圈(N = 0)，第一行(下标为0)的位置已经打印过了
            while(i > N && index < ret.length) {
                ret[index] = mat[i][j];
                i--;
                index++;
            }

            // 一圈已经遍历完了，就来到下一圈
            N++;
            // 行和列的下标都要 +1，往里走一圈
            i++;
            j++;
        }
        // 返回结果
        return ret;
    }
}
```

