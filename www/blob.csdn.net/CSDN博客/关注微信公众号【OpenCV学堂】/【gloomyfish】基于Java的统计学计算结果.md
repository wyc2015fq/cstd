# 【gloomyfish】基于Java的统计学计算结果 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2011年03月20日 16:52:00[gloomyfish](https://me.csdn.net/jia20003)阅读数：4416标签：[java																[input																[math																[apache																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)
个人分类：[Swing桌面开发](https://blog.csdn.net/jia20003/article/category/871637)





最近经常跟数值计算的东西打交道，特别是大量样本的统计计算，在常见的描述统计结果中，最常用到的是一下几种：

 - 一组样本的平均价值

 - 一组样本的中值

 - 一组样本中的最小值

 - 一组样本中的最大值

 - 一组样本的和

 - 一组样本的标准方差



其中，样本N（X[1].....X[n]）的中值的计算与样本的总数有一定的关系：

当样本数量为奇数（odd）时，中值
 median=X[n/2]

当样本数量为偶数（even）时，中值
 median=(X[n/2] + X[n/2])/2



标准方差的计算公式可以参考维基百科[http://en.wikipedia.org/wiki/Standard_deviation](http://en.wikipedia.org/wiki/Standard_deviation),
其中样

本的标准方差是指以N为分母（denominator
）计算结果，标准方差样本是指以N-1作为分母，
 N-1

又叫自由度数。



在标准的apache common math的组件中，已经包含了一组统计学计算的package，可以很好的计算

上面的结果。而它的标准方差的计算公式正是用N-1作为分母计算出来的结果。这个组件的下载URL：

[http://commons.apache.org/math/](http://commons.apache.org/math/)



下面是本人基于Java实现的代码，计算结果与apache
 common math中的DescriptiveStatistics的

结果完全一致





/**
 * @author gloomyfish
 * @date 2011-03-20
 */
package com.java.mathutil;

import java.util.Arrays;

import org.apache.commons.math.stat.descriptive.DescriptiveStatistics;

public class StatisticsDemo {
	private double[] inputData; // input data
	private double medianValue; // median value
	private double meanValue; // mean of input data array
	private double maxValue; // max value of array
	private double minValue; // min value of array
	private double sdValue; // standard deviation of array
	private double sumValue; // sum of array
	
	public StatisticsDemo(double[] input) {
		this.inputData = input;
		Arrays.sort(inputData);
		double N = inputData.length;
		for(int i=0; i<inputData.length; i++) 
		{
			if(i == 0) {
				maxValue = inputData[i];
				minValue = inputData[i];
			}
			
			if(maxValue < inputData[i]) {
				maxValue = inputData[i];
			}
			
			if(minValue > inputData[i]) {
				minValue = inputData[i];
			}
			
			sumValue += inputData[i];
		}
		meanValue = sumValue/N;
		
		// if total number is odd
		// calculate standard deviation and median value
		// http://en.wikipedia.org/wiki/Standard_deviation
		if(isOdd(inputData.length)) 
		{
			medianValue = inputData[inputData.length/2];
			
		} 
		else 
		{
			double temp = inputData[inputData.length/2] + inputData[(inputData.length/2 -1)];
			medianValue = temp/2.0d;
		}
		
		double powSum = 0.0d;
		for(int k=0; k<inputData.length; k++) {
			powSum += Math.pow((inputData[k] - meanValue), 2);
		}

		// This correction (the use of N − 1 instead of N) is known as Bessel's correction
		sdValue = Math.sqrt(powSum/(N-(double)1.0d));
		
		
	}
	
	private boolean isOdd(int n) {
		if((n & 0x1) == 1)
			return true;
		else 
			return false;
	}

	public double getMedianValue() {
		return medianValue;
	}

	public double getMeanValue() {
		return meanValue;
	}

	public double getMaxValue() {
		return maxValue;
	}

	public double getMinValue() {
		return minValue;
	}

	public double getSdValue() {
		return sdValue;
	}

	public double getSumValue() {
		return sumValue;
	}
	
	public static void main(String[] args)  {
		double[] data = new double[]{15.23,12.11,7,88,17,89,6.578,13.456,9.1235,20.5678};
		// Arrays.sort(data);
		
		StatisticsDemo dsd = new StatisticsDemo(data);
		DescriptiveStatistics ds = new DescriptiveStatistics();
		for(int i=0; i<data.length; i++) 
		{
			ds.addValue(data[i]);
		}
		System.out.println("Demo sum = " + dsd.getSumValue());
		System.out.println("Demo mean = " + dsd.getMeanValue());
		System.out.println("Demo median = " + dsd.getMedianValue());
		System.out.println("Demo standard deviation = " + dsd.getSdValue());
		System.out.println("DS sum = " + ds.getSum());
		System.out.println("DS mean = " + ds.getMean());
		System.out.println("DS median = " + ds.getPercentile(50));
		System.out.println("DS standard deviation = " + ds.getStandardDeviation());
	}
	
}




如有误导，后果自负！](https://so.csdn.net/so/search/s.do?q=apache&t=blog)](https://so.csdn.net/so/search/s.do?q=math&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




