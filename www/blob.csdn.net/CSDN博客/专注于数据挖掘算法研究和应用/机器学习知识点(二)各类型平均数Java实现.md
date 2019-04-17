# 机器学习知识点(二)各类型平均数Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年01月06日 11:20:46[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3079
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)











![](https://img-blog.csdn.net/20170106112523792?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170106112532646?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170106112541802?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





代码如下：



```java
package sk.ann;

import java.text.DecimalFormat;
import java.util.Random;

/*
 * 功能：计算并比较各类型平均数，调和<=几何<=算术<=平方
 * 作者：Jason.F
 * 时间：2017年1月6日
 */
public class Average {
	private final static double dmax=999;//Double.MAX_VALUE;//Double类型的最大值，太大的double值，相乘会达到无穷大
	private final static double dmin=Double.MIN_VALUE;//Double类型的最小值
	private final static int n=100;//假设求取100个doubl数的各类平均值
	public static void main(String[] args){
		Random random = new Random();	
		double[] x=new double[n];
		for(int i=0;i<n;i++){//随机生成n个double数
			x[i]=Double.valueOf(Math.floor(random.nextDouble()*(dmax-dmin)));
		}
		//设置doubl字符串输出格式，不以科学计数法输出	
		DecimalFormat df=new DecimalFormat("#,##0.00");//格式化设置
		//计算调和平均数
		double dHM=HarmonicMean(x);
		System.out.println("调和平均数="+df.format(dHM)); 
		//计算几何平均数
		double dGM=GeometricMean(x);
		System.out.println("几何平均数="+df.format(dGM)); 
		//计算算术平均数
		double dAM=ArithmeticMean(x);
		System.out.println("算术平均数="+df.format(dAM)); 
		//计算平方平均数
		double dMS=MeanSquare(x);
		System.out.println("平方平均数="+df.format(dMS));
		//计算中位数，用冒泡排序
		double dMN=MedianNumber(x);
		System.out.println("中位数="+df.format(dMN));
	}
	
	//中位数，排序后去中间值，用冒泡排序
	public static double MedianNumber(double[] x) {   
	    double temp; // 记录临时中间值   
	    int m = x.length; // 数组大小   
	    for (int i = 0; i < m - 1; i++) {   
	        for (int j = i + 1; j < m; j++) {   
	            if (x[i] < x[j]) { // 交换两数的位置   
	                temp = x[i];   
	                x[i] = x[j];   
	                x[j] = temp;   
	            }   
	        }   
	    }   
	    if(m%2==0) //偶数
	    	return (x[m/2]+x[m/2+1])/2;//偶数
	    else //奇数
	    	return x[m/2+1]/2;
	}
	
	//算术平均数：是一组数据的代数和除以数据的项数所得的平均数
	public static double ArithmeticMean(double[] x){
		int m=x.length;
		double sum=0;
		for(int i=0;i<m;i++){//计算x值的倒数
		    sum+=x[i];
		}
		return sum/m;
	}
	//平方平均数：是一组数据的平方和除以数据的项数的开方
	public static double MeanSquare(double[] x){
		int m=x.length;
		double sum=0;
		for(int i=0;i<m;i++){//计算x值的倒数
		    sum+=x[i]*x[i];
		}
		return Math.sqrt(sum/m);
	}
	
	//几何平均数：是n个数据的连乘积的开n次方根。
	public static double GeometricMean(double[] x){
		int m=x.length;
		double sum=1;
		for(int i=0;i<m;i++){//计算x值的倒数
		    sum*=x[i];
		}
		return Math.pow(sum,1.0/m);//返回sum的m次方根
	}
	//调和平均数：一组数据的倒数和除数据的项数的倒数。
	public static double HarmonicMean(double[] x){
		int m=x.length;
		double sum=0;
		for(int i=0;i<m;i++){//计算x值的倒数
		    sum+=1/x[i];
		}
		return m/sum;
	}
}
```


随机执行的一次结果：各类型平均数排序调和<=几何<=算术<=中位<=平方





```
调和平均数=218.12
几何平均数=407.75
算术平均数=534.11
平方平均数=610.11
中位数=535.00
```










