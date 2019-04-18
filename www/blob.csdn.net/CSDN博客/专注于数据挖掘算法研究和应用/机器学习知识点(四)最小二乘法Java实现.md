# 机器学习知识点(四)最小二乘法Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年01月17日 11:27:08[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4907
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









最小二乘法（又称最小平方法）是一种数学优化技术。它通过最小化误差的平方和寻找数据的最佳函数匹配。利用最小二乘法可以简便地求得未知的数据，并使得这些求得的数据与实际数据之间误差的平方和为最小。最小二乘法还可用于曲线拟合。通过一元线性模型应用来理解最小二乘法。

监督学习任务中，预测离散结果的是分类任务，预测连续结果的是回归任务。在回归任务中，预测结果y和x的函数关系中，一元线性回归只包含一个属性的，对应的线性关系；二元线性回归包含两个属性，对应的平面关系；d元线性回归就包括d个属性，对应的超平面关系。

在一元线性回归任务中，给定数据集{(x1,y1),(x2,y2),…,(xn,yn)}，有n个(xi,yi)数据对，在坐标中对应n个点，要拟合这n个点为一条直线的线性关系，自然是直线在n个点中间最好。但显然有很多直线满足，怎么衡量呢？选择怎样的直线最好呢？标准是什么？选择最佳直线的标准是：使总的拟合误差（即总残差）达到最小。

1）用“残差和最小”确定直线位置，存在相互抵消的问题。

2）用“残差绝对值和最小”确定直线位置，但绝对值的计算比较麻烦。

3）最小二乘法的原则是以“残差平方和最小”确定直线位置，用最小二乘法除了计算比较方便外，得到的估计量还具有优良特性、对异常值非常敏感。

综上，我们选用最小二乘法的误差平方和最小作为标准来选出一条直线作为n个点的拟合直线。最常用的是普通最小二乘法（ Ordinary  Least Square，OLS）：所选择的回归模型应该使所有观察值的残差平方和达到最小。（Q为残差平方和）- 即采用平方损失函数。

数学形式定义直线为：

f(xi)=yi=axi+b+ei;

其中，i∈[1,n]，ei是样本(xi,yi)的真实值yi=axi+b+ei和拟合值y’i= axi+b的误差，即ei= yi-axi-b。

![](https://img-blog.csdn.net/20170117112640507?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


最小二乘法一元线性回归模型的Java实现，参考代码如下：



```java
package sk.ml;

import java.text.DecimalFormat;
import java.util.Random;

/*
 * 功能：一元线性回归模型最小二乘法Java实现
 * 作者：Jason.F
 * 时间：2017-01-16
 */
public class LeastSquares {
	private final static int n=20;//随机生成10个点的(x,y)
	public static void main(String[] args){
		//随机生成20个坐标点
		Random random = new Random();	
		double[] x=new double[n];
		double[] y=new double[n];
		for(int i=0;i<n;i++){//随机生成n个double数
			x[i]=Double.valueOf(Math.floor(random.nextDouble()*(99-1)));
			y[i]=Double.valueOf(Math.floor(random.nextDouble()*(999-1)));
		}
        /* y = a x + b
		 * b = sum( y ) / n - a * sum( x ) / n
		 * a = ( n * sum( xy ) - sum( x ) * sum( y ) ) / ( n * sum( x^2 ) - sum(x) ^ 2 )
		 * */
		estimate(x, y, x.length );
	}
	/**
	  * 预测
	  * @param x,y,i
	  */
	public static void estimate( double[] x , double[] y , int i ) {
		double a = getA( x , y ) ;
		double b = getB( x , y , a ) ;
		//设置doubl字符串输出格式，不以科学计数法输出	
		DecimalFormat df=new DecimalFormat("#,##0.00");//格式化设置
		System.out.println("y="+df.format(a)+"x"+"+"+df.format(b));
	}
	 
	 /**
	  * 计算 x的系数a
	  * @param x, y
	  * @return a
	  */
	 public static double getA( double[] x , double[] y ){
		 int n = x.length ;
		 return ( n * pSum( x , y ) - sum( x ) * sum( y ) )/ ( n * sqSum( x ) - Math.pow(sum(x), 2) ) ;
	 }
	 
	 /**
	  * 计算常量系数b
	  * @param x,y,a
	  * @returnb
	  */
	 public static double getB( double[] x , double[] y , double a ){
		 int n = x.length ;
		 return sum( y ) / n - a * sum( x ) / n ;
	 }
	 
	 /**
	  * 计算常量系数b
	  * @param x, y
	  * @return b
	  */
	 public static double getC( double[] x , double[] y ){
		 int n = x.length ;
		 double a = getA( x , y ) ;
		 return sum( y ) / n - a * sum( x ) / n ;
	 }
	 
	 //计算和值
	 private static double sum(double[] ds) {
		 double s = 0 ;
		 for( double d : ds ) s = s + d ;
		 return s ;
	 }
	 //计算开平方和值
	 private static double sqSum(double[] ds) {
		 double s = 0 ;
		 for( double d : ds ) s = s + Math.pow(d, 2) ;
		 return s ;
	 }
	 //计算x和y积的和值
	 private static double pSum( double[] x , double[] y ) {
		 double s = 0 ;
		 for( int i = 0 ; i < x.length ; i++ ) s = s + x[i] * y[i] ;
		 return s ;
	 }
}
```


随机生成的一次执行结果如下：


`y=-0.29x+541.23`




