# 剑指offer——顺时针打印矩阵 - 刘炫320的博客 - CSDN博客
2017年07月09日 15:13:33[刘炫320](https://me.csdn.net/qq_35082030)阅读数：221标签：[顺时针打印矩阵																[剑指offer-java实现																[旋转魔方																[贪吃蛇](https://so.csdn.net/so/search/s.do?q=贪吃蛇&t=blog)](https://so.csdn.net/so/search/s.do?q=旋转魔方&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer-java实现&t=blog)](https://so.csdn.net/so/search/s.do?q=顺时针打印矩阵&t=blog)
个人分类：[剑指offer](https://blog.csdn.net/qq_35082030/article/category/6807625)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 问题描述
> 
输入一个矩阵，按照从外向里以顺时针的顺序依次打印出每一个数字，例如，如果输入如下矩阵： 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 则依次打印出数字1,2,3,4,8,12,16,15,14,13,9,5,6,7,11,10.
# 2. 解决方法
## 2.1 level1
这也是一道非常经典的题目，记得曾经是使用C语言写的，最简单的方法就是按照题目要求这样打印出来，从左到右，从上到下，从右到左，从下到上，分4个循环，再依靠上下左右边界即可完成：
```
public static ArrayList<Integer> printMatrix(int [][] matrix) {
          ArrayList<Integer> al = new ArrayList<>();
                int row = matrix.length;
                if (row == 0)
                    return al;
                int col = matrix[0].length;
                // 短的边/2，向上取整
                int circle = ((row > col ? col : row) + 1) / 2;
                for (int i = 0; i < circle; i++) {
                    // 从左向右打印，j=i; j<col-i,
                    // 这一行的前i个已经在第i圈从下往上被打印，故j=i
                    // 倒数i个都已经在第i圈从上往下被打印，故j=col-i-1<col-i
                    for (int j = i; j < col - i; j++)
                        al.add(matrix[i][j]);
                    // 从上往下打印，j=i+1;j<row-i,
                    // 这一列的前i+1个已经在从左向右打印时被打印,故j=i+1
                    // 倒数i个已经在第i圈从右往左被打印,故j=row-i-1<row-i
                    for (int j = i + 1; j < row - i; j++)
                        al.add(matrix[j][col - i - 1]);
                    // 从右往左打印，j=col-i-2;j>=i&&row-i-1!=i;，
                    // 这一行倒数i个已经在第i圈从上往下被打印
                    // 这一行倒数第i+1个已经在从上往下时被打印，故j=col-1-(i+1)=col-i-2
                    // 这一行的前i个已经在从下往上时被打印，故j=i>=i
                    // 当第i圈为0时即从未由上往下打印时，col有多列时，会造成重复打印，故判断row-i-1!=i以避免
                    for (int j = col - i - 2; j >= i && row - i - 1 != i; j--)
                        al.add(matrix[row - i - 1][j]);
                    // 从下往上打印，j=row-i-2;j>i&&col-i-1!=i，
                    // 这一列倒数i个已经在第i圈从右往作被打印
                    // 这一列倒数第i+1个已经在从右往左时被打印，故j=row-1-(i+1)=row-i-2
                    // 这一列的前i个已经在第i圈从左往右时被打印，
                    // 这一列的第i+1个已经在本圈从左往右被打印，故j=i+1>i
                    // 当第i圈为0时即从未由右向左打印时，row有多行时，会造成重复打印，故判断col-i-1!=i以避免
                    for (int j = row - i - 2; j > i && col - i - 1 != i; j--)
                        al.add(matrix[j][i]);
                }
                return al;
        }
```
## 2.2 level2
但是事实上，如果使用旋转魔方的方法，会减轻不少工作量，每次只取一行，然后逆时针旋转矩阵并去掉已经读取过的那一行，然后依次循环知道无矩阵可旋转：
```
public static ArrayList<Integer> printMatrix(int[][] matrix) {
        ArrayList<Integer> al = new ArrayList<>();
        int row = matrix.length;
        while (row != 0) {
            for (int i = 0; i < matrix[0].length; i++) {
                al.add(matrix[0][i]);
            }
            if (row == 1)
                break;
            matrix = turn(matrix);
            row = matrix.length;
        }
        return al;
    }
    private static int[][] turn(int[][] matrix) {
        // TODO 自动生成的方法存根
        int col = matrix[0].length;
        int row = matrix.length;
        int[][] newMatrix = new int[col][row - 1];
        for (int j = col - 1; j >= 0; j--) {
            for (int i = 1; i < row; i++) {
                newMatrix[col - 1 - j][i - 1] = matrix[i][j];
            }
        }
        return newMatrix;
    }
```
## 2.3 level3
上述方法尽管巧妙，但是还不够生动，如果把它看作是一个贪吃蛇的游戏的话，那么整个步骤就是分为2步，第一步吃掉当前所在地的数字，第二步探索有没有碰到墙，如果碰到墙，先记录该面墙的位置，然后把墙拉近一步，再向右转九十度走一步，不然的话就按原方向走一步。
```
public static ArrayList<Integer> printMatrix(int [][] matrix) {
           ArrayList<Integer> result=new ArrayList<Integer>();
           if(matrix==null||matrix.length==0){
               return result;
           }
           int left=-1;
           int right=matrix[0].length;
           int top=0;
           int buttom=matrix.length;
           int maxcount=right*buttom;
           int x=0,y=0;
           //flag决定方向，0向右行，1向下行，2向左行，3向上行。
           int flag=0;
           while(result.size()<maxcount){
               //先把当前的加入
               result.add(matrix[x][y]);
               //判断是否到头
               switch(flag){
                    case 0:{
                        //如果是碰到墙了，改方向行走。
                        if(y==right-1){
                            flag=(flag+1)%4;
                            right--;
                            x++;
                        }
                        //不然往前走一步。
                        else{
                            y++;
                        }
                        break;
                    }
                    case 1:{
                        if(x==buttom-1){
                            flag=(flag+1)%4;
                            buttom--;
                            y--;
                        }
                        else{
                            x++;
                        }
                        break;
                    }
                    case 2:{
                        if(y==left+1){
                            flag=(flag+1)%4;
                            left++;
                            x--;
                        }
                        else{
                            y--;
                        }
                        break;
                    }
                    case 3:{
                        if(x==top+1){
                            flag=(flag+1)%4;
                            top++;
                            y++;
                        }
                        else{
                            x--;
                        }
                        break;
                    }
               }
           }
           return result;
     }
```
毕竟这样似乎看起来会生动一些，有点人工智能的味道在里面。
# 3. 小结
通过上面的例子我们可以看到，把抽象枯燥的问题，完全可以化解为一个非常有趣的模型，然后再继续建模的话，会好很多。
