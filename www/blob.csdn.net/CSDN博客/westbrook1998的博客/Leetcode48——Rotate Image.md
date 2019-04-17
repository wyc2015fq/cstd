# Leetcode48——Rotate Image - westbrook1998的博客 - CSDN博客





2018年02月24日 23:06:38[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：54








You are given an n x n 2D matrix representing an image.

Rotate the image by 90 degrees (clockwise).

Note: 

You have to rotate the image in-place, which means you have to modify the input 2D matrix directly. DO NOT allocate another 2D matrix and do the rotation.

Example 1:

Given input matrix =  

[ 

  [1,2,3], 

  [4,5,6], 

  [7,8,9] 

],
rotate the input matrix in-place such that it becomes: 

[ 

  [7,4,1], 

  [8,5,2], 

  [9,6,3] 

] 

Example 2:
Given input matrix = 

[ 

  [ 5, 1, 9,11], 

  [ 2, 4, 8,10], 

  [13, 3, 6, 7], 

  [15,14,12,16] 

], 
rotate the input matrix in-place such that it becomes: 

[ 

  [15,13, 2, 5], 

  [14, 3, 4, 1], 

  [12, 6, 8, 9], 

  [16, 7,10,11] 

]
`分析： 

首先通过观察看出二维数组的旋转可以看成是每四个数为一组的交换，这样只需要一个temp变量来作为临时变量 

观察得出四个数交换的规律：
```
int temp=matrix[i][j];
matrix[i][j]=matrix[n-j-1][i];
matrix[n-j-1][i]=matrix[n-i-1][n-j-1];
matrix[n-i-1][n-j-1]=matrix[j][n-i-1];
matrix[j][n-i-1]=temp;
```

然后再判断需要交换多少次四个数 

n=1 不需要交换 

n=2 交换一次 第一个数a[0][0] 

n=3 交换两次 第一个数a[0][0] a[0][1] 

n=4 交换四次 第一个数是a[0][0] a[0][1] a[0][2] a[1][1] 

n=5 交换六次 第一个数是a[0][0] a[0][1] a[0][2] a[0][3] a[0][4] a[1][1] a[1][2] 

由此可以得出，循环
```matlab
i<=n/2;
i<j<=n-i-1;
```

由此得解 

代码+测试主方法

```
import java.util.ArrayList;
import java.util.List;

class Solution {
    public void rotate(int[][] matrix) {
        int n=matrix.length;
        int l=(n-1)/2;
        for(int i=0;i<=l;i++){
            for(int j=i;j<n-1-i;j++){
                swap(matrix,i,j,n);
            }
        }
    }
    public static void swap(int[][] matrix,int i,int j,int n){
        int temp=matrix[i][j];
        matrix[i][j]=matrix[n-j-1][i];
        matrix[n-j-1][i]=matrix[n-i-1][n-j-1];
        matrix[n-i-1][n-j-1]=matrix[j][n-i-1];
        matrix[j][n-i-1]=temp;
    }
    public static void main(String[] args){
        int[][] matrix={{5,1,9,11},{2,4,8,10},{13,3,6,7},{15,14,12,16}};
        int len=matrix.length;
        for(int i=0;i<len;i++){
            for(int j=0;j<len;j++){
                System.out.print(matrix[i][j]+"  ");
            }
            System.out.println();
        }
        new Solution().rotate(matrix);
        for(int i=0;i<len;i++){
            for(int j=0;j<len;j++){
                System.out.print(matrix[i][j]+"  ");
            }
            System.out.println();
        }
    }
}
```









