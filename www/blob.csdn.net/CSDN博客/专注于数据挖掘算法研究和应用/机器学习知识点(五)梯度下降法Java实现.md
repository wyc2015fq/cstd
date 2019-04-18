# 机器学习知识点(五)梯度下降法Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年01月17日 15:57:45[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5418
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









梯度下降法，就是利用负梯度方向来决定每次迭代的新的搜索方向，使得每次迭代（步长）能使待优化的目标函数逐步减小。梯度下降法是2范数下的最速下降法，用来求解函数的最小值，无约束优化。

看数学定义都晦涩，网上发现一个比较通俗的说法：想象你站在一座高山上，你想要用最短的时间下山，但是你每次只能走一步。那你需要做的就是查看你周围360度的范围，找到一个最陡峭的(下降的最快的)方向，然后转移到那个点上；转移到新的位置之后，重复相应的步骤，环顾360度，找到最陡峭的(下降的最快的)方向，然后转移过去，这样每次都是选择最陡峭的方向走，那么很快就能到达山下了。

一个多元函数的梯度方向是该函数值增大最陡的方向，在一元函数中，梯度方向是沿着切线方向；而在多元函数中，梯度向量是函数值f对每个变量的导数，向量的方向就是梯度方向。

梯度下降法的计算过程就是沿梯度下降的方向求解极小值，其迭代公式为：

![](https://img-blog.csdn.net/20170117155445807?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

梯度方向通过对函数求导得到，步长的确定是由线性搜索算法来确定，即把下一个点的坐标作为ak+1的函数，然后求满足f(ak+1)的最小值的即可。

一般情况下，梯度向量为0的明是到了一个极值点，此时梯度的幅值也为0。采用梯度下降算法进行最优化求解时，算法迭代的终止条件是梯度向量的幅值接近0即可，可以设置个非常小的常数阈值。

总结来说，梯度下降法就是在没有约束的情况下求解函数的最小值，通过对函数求导沿着最陡梯度下降，直到梯度向量的增值接近0为止。这里面有两个关键参数要定义，一个是迭代步长；另一个是终止迭代的误差值设置。

![](https://img-blog.csdn.net/20170117155600198?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





回到上文梯度下降法定义，求一个函数最小值的梯度算法。下面代码用梯度下降法来求解函数的最小值。



```java
package sk.ml;

import java.text.DecimalFormat;

/**
 * 功能：梯度下降算法，求解 f(x)=x^4-3x^3+2 最小值
 * 导数为: f'(x)=4x^3-9x^2
 * 作者：Jason.F
 * 时间：2017年1月17日
 */
public class GradientDescent {
	static double y_cur = 0;//用于每次迭代后的值记录，循环终止时就是最小值
	static double x = 6; // 从 x=6开始迭代
	double step = 0.01; // 每次迭代的步长
	double precision = 0.00001;//误差
	static int iter = 0;//迭代次数
	
	//目标函数的导数
	private double  derivative(double x) {//导数
		return 4 * Math.pow(x, 3) - 9 *Math.pow(x, 2);
	}
	//目标函数，要求解最下值
	private double function(double x){//函数
		return Math.pow(x, 4)-3 * Math.pow(x, 3)+2;
	}
	
	private void getmin() {
		y_cur=function(x);
		double y_div=function(x);//初始y值
		while (y_div > precision){//下降梯度的幅度变化大于误差，继续迭代
			//System.out.println("当前y="+y_cur+",x="+x);
			x=x-step*derivative(x);//沿梯度负方向移动
			y_div=y_cur-function(x);//移动后计算y的变化幅度值
			y_cur=function(x);	//y值跟着x移动变化，计算下一轮迭代	
			iter++;
		}
	}
	
	public static void main(String[] args) {
		GradientDescent gd = new GradientDescent();
		gd.getmin();
		DecimalFormat df=new DecimalFormat("#,##0.00");//格式化设置
		System.out.println("迭代"+iter+"次，函数最小值："+df.format(y_cur)+"，对应的x值："+df.format(x));
	}
}
```


执行结果：`迭代55次，函数最小值：-6.54，对应的x值：2.25`



