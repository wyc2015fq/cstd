# 最长公共子序列Java代码实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月19日 11:08:43[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：9789
所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)









最长公共子序列定义：两个或多个已知数列的子序列集合中最长的就是最长公共子序列。

比如数列A = “abcdef”和B = “adefcb”，那么两个数列的公共子序列集合有{”a","ab","abc","adef",等等}，其中最长的就是adef，这就是最长公共子序列。


注意：最长公共子序列的公共子序列里的元素可以不相邻，但是公共子字符串必须是连接在一起的，比如A和B的公共子字符串是“def”。

用动态规划法来求解最长公共子序列，因为最长公共子序列具有最有子结构性质，可以分成子问题来递归求最优解，最后组合子问题求解出问题。用c[i][j]记录X[i]与Y[j]
 的LCS 的长度，求解问题c[i,j]，可以分成c[i-1][j-1]、c[i-1][j]、c[i][j-1]子问题来求解，依次递堆到最小子问题，动态规划的递归式描述为：

![](https://img-blog.csdn.net/20160719110614857?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


计算c[i][j]矩阵，利用矩阵可以输出最长公共子序列字符，具体代码如下：



```java
package cn.hm;

public class LongestCommonSub {
	
	public static void main(String[] args) {
        // TODO Auto-generated method stub
        String str1 = "fjssharpsword";
        String str2 = "helloworld";
        //计算lcs递归矩阵
        int[][] re = longestCommonSubsequence(str1, str2);
        //打印矩阵
        for (int i = 0; i <= str1.length(); i++) {
                for (int j = 0; j <= str2.length(); j++) {
                        System.out.print(re[i][j] + "   ");
                }
                System.out.println();
        }

        System.out.println();
        System.out.println();
        //输出LCS
        print(re, str1, str2, str1.length(), str2.length());
	}

	// 假如返回两个字符串的最长公共子序列的长度
	public static int[][] longestCommonSubsequence(String str1, String str2) {
        int[][] matrix = new int[str1.length() + 1][str2.length() + 1];//建立二维矩阵
        // 初始化边界条件
        for (int i = 0; i <= str1.length(); i++) {
                matrix[i][0] = 0;//每行第一列置零
        }
        for (int j = 0; j <= str2.length(); j++) {
                matrix[0][j] = 0;//每列第一行置零
        }
        // 填充矩阵
        for (int i = 1; i <= str1.length(); i++) {
                for (int j = 1; j <= str2.length(); j++) {
                        if (str1.charAt(i - 1) == str2.charAt(j - 1)) {
                                matrix[i][j] = matrix[i - 1][j - 1] + 1;
                        } else {
                                matrix[i][j] = (matrix[i - 1][j] >= matrix[i][j - 1] ? matrix[i - 1][j]
                                                : matrix[i][j - 1]);
                        }
                }
        }
        return matrix;
	}
	//根据矩阵输出LCS
	public static void print(int[][] opt, String X, String Y, int i, int j) {
        if (i == 0 || j == 0) {
                return;
        }
        if (X.charAt(i - 1) == Y.charAt(j - 1)) {
                print(opt, X, Y, i - 1, j - 1);
                System.out.print(X.charAt(i - 1));
        } else if (opt[i - 1][j] >= opt[i][j]) {
                print(opt, X, Y, i - 1, j);
        } else {
                print(opt, X, Y, i, j - 1);
        }
	}
}
```


输出结果如下：



```
0   0   0   0   0   0   0   0   0   0   0   
0   0   0   0   0   0   0   0   0   0   0   
0   0   0   0   0   0   0   0   0   0   0   
0   0   0   0   0   0   0   0   0   0   0   
0   0   0   0   0   0   0   0   0   0   0   
0   1   1   1   1   1   1   1   1   1   1   
0   1   1   1   1   1   1   1   1   1   1   
0   1   1   1   1   1   1   1   2   2   2   
0   1   1   1   1   1   1   1   2   2   2   
0   1   1   1   1   1   1   1   2   2   2   
0   1   1   1   1   1   2   2   2   2   2   
0   1   1   1   1   2   2   3   3   3   3   
0   1   1   1   1   2   2   3   4   4   4   
0   1   1   1   1   2   2   3   4   4   5   


hword
```
通过代码的步骤去理解动态规划的最有子结构以及递归解性质，再而理解最长公共子序列是如何通过矩阵来输出。







