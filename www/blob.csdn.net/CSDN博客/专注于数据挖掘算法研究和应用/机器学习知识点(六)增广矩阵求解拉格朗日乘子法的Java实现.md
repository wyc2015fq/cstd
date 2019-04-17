# 机器学习知识点(六)增广矩阵求解拉格朗日乘子法的Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年01月18日 09:49:26[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4109
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









基本的[拉格朗日乘子](http://baike.baidu.com/view/2415642.htm)法就是求函数f(x1,x2,...)在g(x1,x2,...)=0的[约束条件](http://baike.baidu.com/view/2167316.htm)下的极值的方法。其主要思想是将约束条件函数与[原函数](http://baike.baidu.com/view/152299.htm)联系到一起，使能配成与变量数量相等的[等式](http://baike.baidu.com/view/194373.htm)方程，从而求出得到原函数极值的各个变量的解。原函数加约束函数构成的一组方程组，用以求解变量组。

拉格朗日乘子（Lagrange multiplier）

假设需要求极值的目标函数(objective function)为f(x,y)，限制条件为φ(x,y)=M

![](https://img-blog.csdn.net/20170118094830224?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




求出x,y,λ的值，代入即可得到目标函数的极值

扩展为多个变量的式子为：F(x1,x2,...,xn,λ)=f(x1,x2)-λg(x1,x2,...,xn)

则求[极值点](http://baike.baidu.com/view/3237470.htm)的方程为：

∂F/∂xi=0（i∈[1,n]）

∂F/∂λ=g(x1,x2,...,xn)=0

参考代码如下：对求导后的构成三元一次线性方程组，采用增广矩阵来求解。



```java
package sk.ml;

import java.text.DecimalFormat;

/**
 * 功能：拉格朗日乘子法，求解 f(x,y)=x^2+3xy+y^2最大值，约束条件x+y=100
 *     构造函数：F(x,y,λ)=f(x,y)+λg(x,y)，其中g(x,y)=x+y-100
 *     求导：∂L/∂x=2x+3y+λ=0；∂L/∂y=2y+3x+λ=0；∂L/∂λ=x+y-100=0;
 *     用矩阵算法代码实现线性方程组求解x,y。
 * 作者：Jason.F
 * 时间：2017年1月18日
 */
public class LagrangeMultiplier {
	//增广矩阵计算三元一次线性方程组
	static DecimalFormat df = new DecimalFormat("0.##");
	/***
	 * 增广矩阵机型初等行变化的算法 
	 * @param value         需要算的增广矩阵
	 * @return 计算的结果
	 */
	public static double[][] mathDeterminantCalculation(double[][] value) throws Exception {
		// 当矩阵的行数大于2时
		for (int i = 0; i < value.length; i++) {
			// 检查数组对角线位置的数值是否是0，如果是零则对该数组进行调换，查找到一行不为0的进行调换
			if (value[i][i] == 0) {
				value = changeDeterminantNoZero(value, i, i);
			}
			for (int j = 0; j < i; j++) {
				// 让开始处理的行的首位为0处理为三角形式
				// 如果要处理的列为0则和自己调换一下位置，这样就省去了计算
				if (value[i][j] == 0) {
					continue;
				}
				// 如果要是要处理的行是0则和上面的一行进行调换
				if (value[j][j] == 0) {
					double[] temp = value[i];
					value[i] = value[i - 1];
					value[i - 1] = temp;
					continue;
				}
				double ratio = -(value[i][j] / value[j][j]);
				value[i] = addValue(value[i], value[j], ratio);
			}
		}
		return value;
	}

	/***
	 * 将i行之前的每一行乘以一个系数，使得从i行的第i列之前的数字置换为0
	 * @param currentRow 当前要处理的行
	 * @param frontRow i行之前的遍历的行
	 * @param ratio  要乘以的系数
	 * @return 将i行i列之前数字置换为0后的新的行
	 */
	public static double[] addValue(double[] currentRow, double[] frontRow,
			double ratio) throws Exception {
		for (int i = 0; i < currentRow.length; i++) {
			currentRow[i] += frontRow[i] * ratio;
			currentRow[i] = Double.parseDouble(df.format(currentRow[i]));
		}
		return currentRow;
	}

	/**
	 * 指定列的位置是否为0，查找第一个不为0的位置的行进行位置调换，如果没有则返回原来的值
	 * @param determinant  需要处理的行列式
	 * @param line  要调换的行
	 * @param row  要判断的列
	 */
	public static double[][] changeDeterminantNoZero(double[][] determinant,
			int line, int row) throws Exception {
		for (int j = line; j < determinant.length; j++) {
			// 进行行调换
			if (determinant[j][row] != 0) {
				double[] temp = determinant[line];
				determinant[line] = determinant[j];
				determinant[j] = temp;
				return determinant;
			}
		}
		return determinant;
	}

	/**
	 * 将系数矩阵和方程值的矩阵进行合并成增广矩阵
	 * @param coefficient   系数矩阵
	 * @param value  方程值
	 * @return 增广矩阵
	 */
	public static double[][] transferMatrix(double[][] coefficient,
			double[] value) {
		double[][] temp = new double[coefficient.length][coefficient[0].length + 1];
		if (coefficient.length != value.length) {
			return temp;
		}
		// 将方程值添加到系数矩阵中
		for (int i = 0; i < coefficient.length; i++) {
			for (int j = 0; j < coefficient[0].length; j++) {
				temp[i][j] = coefficient[i][j];
			}
		}
		for (int i = 0; i < value.length; i++) {
			temp[i][temp[i].length - 1] = value[i];
		}
		return temp;
	}

	/**
	 * 检查有效的行数，看非零行的个数
	 * @param value  需要检查的数组
	 * @return 非零行的个数
	 */
	public static int effectiveMatrix(double[][] value) {
		for (int i = value.length - 1; i > -1; i--) {
			for (int j = 0; j < value[i].length; j++) {
				if (value[i][j] != 0) {
					return i + 1;
				}
			}
		}
		return 0;
	}

	/**
	 * 当方程组有解的时候计算方程组的解
	 * @param mathMatrix  方程组的增广矩阵
	 * @return 方程组的解
	 */
	private static double[] calculationResult(double[][] mathMatrix) {
		// 有解时方程组的个数等于方程组的未知数
		double[] result = new double[mathMatrix.length];
		for (int i = mathMatrix.length - 1; i > -1; i--) {
			double temp = 0;
			for (int j = mathMatrix[i].length; j > 0; j--) {
				// 第一个为方程的解，需要将解赋值给临时变量
				if (mathMatrix[i][j - 1] != 0) {
					if (j == mathMatrix[i].length) {
						temp = mathMatrix[i][j - 1];
					} else if (j - 1 > -1 && result[j - 1] != 0) {
						temp -= mathMatrix[i][j - 1] * result[j - 1];
					} else {
						result[i] = temp / mathMatrix[i][j - 1];
						continue;
					}
				}
			}
		}
		return result;
	}

	public static void main(String[] args) {
		// 方程的未知数的个数 
		  int n = 3;
		  double x=0;
		  double y=0;//记录x,y值用于计算目标函数最大值
		  double[][] ratio = { { 2,3,1 },{ 3,2,1 }, {1,1,0}};// 系数矩阵
		  double[] value ={ 0,0,100 };// 方程的解
		  try {
			  // 转换成增广矩阵并进行初等行变化
			  double[][] mathMatrix = mathDeterminantCalculation(transferMatrix(ratio, value));
			  // 找出非零行的个数
			  int checkMatrixRow = effectiveMatrix(mathMatrix);
			  // 根据未知数的个数和方程组非零行的个数来判断当前方程组的解的情况
			  if (n > checkMatrixRow) {
				  System.out.println("未知数有" + n + "个，消元法后获取的阶梯方程组有"+ checkMatrixRow + "个方程,少于未知数个数，所以该方程有无数组解");
			  } else if (n < checkMatrixRow) {
				  System.out.println("未知数有" + n + "个，消元法后获取的阶梯方程组有"+ checkMatrixRow + "个方程,多于未知数个数，所以该方程有无解");
			  } else {
				  //System.out.println("未知数有" + n + "个，消元法后获取的阶梯方程组有"+ checkMatrixRow + "个方程,等于未知数个数，所以该方程有解");
				  double[] result = calculationResult(mathMatrix);
				  //for (int i = 0; i < result.length; i++) {
					  //System.out.println("方程组的解为x" + (i + 1) + "="+ df.format(result[i]));
					  x=result[0];
					  y=result[1];
				  //}
			  }
			  System.out.println("在x="+df.format(x)+"和y="+df.format(y)+"时，目标函数取得最大值:"+df.format(x*x+3*x*y+y*y));

		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
```


执行结果：`在x=50和y=50时，目标函数取得最大值:12500`









