
# 剑指offer——顺时针打印矩阵 - manduner的博客 - CSDN博客


2019年03月27日 18:47:17[Manduner_TJU](https://me.csdn.net/manduner)阅读数：27标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[剑指offer																](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)



# 1，问题描述
输入一个矩阵，按照从外向里以顺时针的顺序依次打印出每一个数字，例如，如果输入如下4 X 4矩阵： 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 则依次打印出数字1,2,3,4,8,12,16,15,14,13,9,5,6,7,11,10.
# 2，解题思路
（1）每次将矩阵的第一行元素依次添加至列表中，
（2）然后将剩余的矩阵逆时针旋转
（3）重复上述操作
例如：
![](https://img-blog.csdnimg.cn/20190327184607665.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy,size_16,color_FFFFFF,t_70)
# 3，源码
```python
import java.util.ArrayList;
public class Solution {
	    public ArrayList<Integer> printMatrix(int [][] matrix) {
	    	ArrayList<Integer> list = new ArrayList<Integer>();
	    	
	    	while(matrix.length != 0) {
		    	for(int i=0;i<matrix[0].length;i++) {
		    		list.add(matrix[0][i]);
		    	}
		    	if(matrix.length==1) break;//如果条件成立，说明matrix所有元素已经添加到list中。
		    	matrix = copyNeedElement(matrix);
		    	matrix = arrayRowColumnSwap(matrix);
	    	}
	       
	    	return list;
	    }
	    
	    //删除数组第一行并返回剩余的数组
		public static int[][] copyNeedElement(int[][] matrix) {
	    	int m = matrix.length;
	    	int n = matrix[0].length;
	    	//删除数组的第一行
        	int[][] temp = new int[m-1][n];
        	System.arraycopy(matrix, 1, temp, 0, temp.length);
	    	return temp;
		}
		//逆时针旋转数组
		public static int[][] arrayRowColumnSwap(int[][] matrix) {
			int m=matrix.length;
			int n=matrix[0].length;
			int[][] temp = new int[n][m];    
			for(int i=0;i<n;i++) {
				for(int j=0;j<m;j++) {
					temp[i][j] = matrix[j][n-1-i];
				}
			}
			return temp;
		}
}
```


