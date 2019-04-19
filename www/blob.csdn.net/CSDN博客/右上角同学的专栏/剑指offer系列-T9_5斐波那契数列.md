# 剑指offer系列-T9_5斐波那契数列 - 右上角同学的专栏 - CSDN博客
2016年07月02日 22:36:09[右上角徐](https://me.csdn.net/u011032983)阅读数：178
个人分类：[剑指offer](https://blog.csdn.net/u011032983/article/category/6290473)
```java
/**
 * @author xhl 斐波那契数列 题目描述 大家都知道斐波那契数列， 现在要求输入一个整数n，请你输出斐波那契数列的第n项。 n>=0
 *  本次实现的时间复杂度是o(logn)
 */
public class offerT9_5 {
    /**
     * @param args
     */
    int Fibonacci(int n) {
        if (n == 0)
            return 0;
        if (n == 1)
            return 1;
        return Fibonacci2(n)[0][0];
    }
    /*n>=2时调用矩阵相乘形式*/
    int[][] Fibonacci2(int n) {
        int[][] matrix = { { 1, 1 }, { 1, 0 } };
        int[][] result = new int[matrix.length][matrix[0].length];
        /*
         * result={{Fibonacci(n),Fibonacci(n-1)}{Fibonacci(n-1),Fibonacci(n-2)}}=
         * matrix^(n-1),n>=2
         */
        if ((n - 1) == 1)
            result = matrix;
        else {
            int[][] temp;
            if ((n - 1) % 2 == 0) {
                temp = Fibonacci2((n + 1) / 2);/* 此处注意，n-1为偶数时，拆分为matrix^((n-1)/ 2)的平方，matrix^((n-1)/2)
                                                * 对应的Fibonacci2函数的参数却是（(n + 1)/ 2）*/
                result = MatrixMul(temp, temp);
            } else {
                temp = Fibonacci2((n) / 2);
                result = MatrixMul(temp, temp);
                result = MatrixMul(result, matrix);
            }
        }
        return result;
    }
  /*两矩阵乘法计算*/
    int[][] MatrixMul(int[][] a, int[][] b) {
        int[][] result = new int[a.length][b[0].length];
        for (int i = 0; i < a.length; i++)
            for (int j = 0; j < b[0].length; j++)
                for (int k = 0; k < a[0].length; k++)
                    result[i][j] = a[i][k] * b[k][j] + result[i][j];
        return result;
    }
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        offerT9_5 o = new offerT9_5();
        int res = o.Fibonacci(4);
        System.out.print(res);
    }
}
```
