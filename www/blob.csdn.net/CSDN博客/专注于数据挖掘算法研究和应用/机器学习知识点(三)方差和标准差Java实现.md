# 机器学习知识点(三)方差和标准差Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年01月10日 17:15:40[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：14882
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









1、方差：方差是各个数据与平均数之差的平方的平均数。

2、标准差：

标准差(Standard Deviation) 


各数据偏离平均数的距离（离均差）的平均数，它是离差平方和平均后的方根。用σ表示。因此，标准差也是一种平均数


标准差是方差的算术平方根。


标准差能反映一个数据集的离散程度。平均数相同的，标准差未必相同。


3、参考代码：



```java
package sk.ann;

import java.text.DecimalFormat;
import java.util.Random;

public class VarianceAndStandardDiviation {
	private final static double dmax=999;//Double.MAX_VALUE;//Double类型的最大值，太大的double值，相乘会达到无穷大
	private final static double dmin=Double.MIN_VALUE;//Double类型的最小值
	private final static int n=100;//假设求取100个doubl数的方差和标准差
	
	public static void main(String[] args){
		Random random = new Random();	
		double[] x=new double[n];
		for(int i=0;i<n;i++){//随机生成n个double数
			x[i]=Double.valueOf(Math.floor(random.nextDouble()*(dmax-dmin)));
			System.out.println(x[i]);
		}
		//设置doubl字符串输出格式，不以科学计数法输出	
		DecimalFormat df=new DecimalFormat("#,##0.00");//格式化设置
		//计算方差
		double dV=Variance(x);
		System.out.println("方差="+df.format(dV));
		//计算标准差
		double dS=StandardDiviation(x);
		System.out.println("标准差="+df.format(dS));
	}
	
	//方差s^2=[(x1-x)^2 +...(xn-x)^2]/n
	public static double Variance(double[] x) { 
		int m=x.length;
		double sum=0;
		for(int i=0;i<m;i++){//求和
		    sum+=x[i];
		}
		double dAve=sum/m;//求平均值
		double dVar=0;
		for(int i=0;i<m;i++){//求方差
			dVar+=(x[i]-dAve)*(x[i]-dAve);
		}
		return dVar/m;
	}
	
	//标准差σ=sqrt(s^2)
	public static double StandardDiviation(double[] x) { 
		int m=x.length;
		double sum=0;
		for(int i=0;i<m;i++){//求和
		    sum+=x[i];
		}
		double dAve=sum/m;//求平均值
		double dVar=0;
		for(int i=0;i<m;i++){//求方差
			dVar+=(x[i]-dAve)*(x[i]-dAve);
		}
		return Math.sqrt(dVar/m);	
	}
}
```
随机执行的结果：





```
方差=80,345.83
标准差=283.45
```








