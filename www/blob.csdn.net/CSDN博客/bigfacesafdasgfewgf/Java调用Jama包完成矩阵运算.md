# Java调用Jama包完成矩阵运算 - bigfacesafdasgfewgf - CSDN博客





2015年03月10日 17:44:39[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：5453











    Java开发机器学习算法的时候，经常用到矩阵运算，我们可以调用Jama包来完成矩阵运算。




    Jama包下载地址：https://dl.pandaidea.com/jarfiles/j/Jama/Jama-1.0.2.jar.zip




    新建一个工程和类，导入Jama的jar包（右击，build path，add libraries），然后我们就可以使用了。如下：






```java
package testing;

import Jama.Matrix;  // 导入Jama包中的Matrix类

public class helloworld {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("调用jama包完成矩阵运算");
		
		double [][] array = {
				{-1, 1, 0},
				{-4, 3, 0},
				{1, 0, 2}
		};
		
		System.out.println("特征分解");
		// http://zhidao.baidu.com/link?url=KZY21A85_YfXLCw-4dZlES5AdgjXkQg4uuogjLsv6WvGV3VM9sBkTOQUofPpEzRqSO0WwlVrBMi8e-9hd4Rhoa
		Matrix A = new Matrix(array); 
		A.eig().getD().print(4, 2);   // 对角元素是特征值，4是列的宽度，2代表小数点后的位数
		A.eig().getV().print(4, 2);   // 特征向量

		System.out.println("矩阵维数");
		int rowNum = A.getRowDimension();  // 矩阵行数
		int colNum = A.getColumnDimension(); 
		System.out.println(rowNum + " " + colNum); 
		
		System.out.println("行列式");
		double detNum = A.det();   // 行列式
		System.out.println(detNum); 
	}

}
```













