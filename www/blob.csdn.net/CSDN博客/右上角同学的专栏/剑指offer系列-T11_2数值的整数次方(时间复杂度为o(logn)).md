# 剑指offer系列-T11_2数值的整数次方(时间复杂度为o(logn)) - 右上角同学的专栏 - CSDN博客
2016年07月02日 22:44:17[右上角徐](https://me.csdn.net/u011032983)阅读数：433
本人对java语言更熟悉，所以剑指offer代码都是通过Java实现，且涉及的核心代码全部通过牛客网的测试用例检查，感谢牛客网为我检验程序提供了极大帮助！main函数是为了在自己运行程序时，运行结果更直观化。
```java
/**
 @author xhl 数值的整数次方
 *  题目描述
 *  给定一个double类型的浮点数base和int类型的整数exponent。求base的exponent次方。
 *  本次实现的时间复杂度为o(logn)
 */
public class offerT11_2 {
	/**
	 * @param args
	 * 递归执行，复杂度为o(logn)
	 */
	public double Power(double base, int exponent) {
		double res;
		if (equal(base, 0.0) && exponent < 0)
			return 0.0;
		if (exponent==0)
			return 1.0;
		
		/*先按照指数为正计算。判断指数为偶数时，先求指数/2次方的值，再平方；奇数时先求（指数-1）/2次方的值。再平方并乘以基数*/
		int absexponent = Math.abs(exponent);
		if (absexponent == 1)
			res=base;
		else {
			if ((absexponent &0x1) == 0) {
				res = Power(base, absexponent>>1);
				res =res * res;
			} else {
				res = Power(base, (absexponent - 1)>>1);
				res =res * res * base;
			}
		}
		
		/*判断指数正负*/
		if(exponent<0)
			res=1/res;
		return res;
	}
	boolean equal(double x, double y) {
		if (x - y > 0.0000000001 && x - y < -0.0000000001)
			return true;
		return false;
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		offerT11_2 o = new offerT11_2();
		double result = o.Power(3.1,0);
		System.out.print(result);//double精度丢失，例如计算0.05+0.01
	}
}
```
