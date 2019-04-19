# 剑指offer系列-T11数值的整数次方 - 右上角同学的专栏 - CSDN博客
2016年07月02日 22:38:54[右上角徐](https://me.csdn.net/u011032983)阅读数：124
个人分类：[剑指offer](https://blog.csdn.net/u011032983/article/category/6290473)
```java
/**
 * @author xhl 数值的整数次方
 *  题目描述
 *  给定一个double类型的浮点数base和int类型的整数exponent。求base的exponent次方。
 */
public class offerT11 {
	/**
	 * @param args
	 */
	public double Power(double base, int exponent) {
		if (equal(base,0.0)&& exponent < 0)
			return 0.0;
		int absexponent = Math.abs(exponent);
		double result = 1.0;
		for (int i = 0; i < absexponent; i++)
			result = result * base;
		if (exponent < 0)
			result = 1 / result;
		return result;
	}
	
	boolean equal(double x,double y){
		if(x-y>0.0000000001&&x-y<-0.0000000001)
			return true;
		return false;
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT11 o = new offerT11();
		double result = o.Power(1.1,2);
		System.out.print(result);//double精度丢失，例如计算0.05+0.01
	}
}
```
