# Java 使用BigDecimal实现精确计算 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年11月28日 14:16:18[boonya](https://me.csdn.net/boonya)阅读数：792








前言：程序中很多业务都会涉及到双精度Double数的计算，在计算过程中有很多问题是潜在的陷阱，使得程序不会按照我们预想的那样能够精确输出。

# 1.错误示例

**先来看一个测试例子：**



```java
package com.boonya.program.lang;

import java.math.BigDecimal;

import org.junit.Test;

public class BigDecimalTest {
	
	private static double a = 2.00;
	
	private static double b = 1.10; 
	
	@Test
	public void testSubstract0(){
		System.out.println("method=testSubstract0>:"+(a - b));// 输出0.8999999999999999
	}
	
	@Test
	public void testSubstract1(){
		BigDecimal a1=new BigDecimal(a);
		BigDecimal b1=new BigDecimal(b);
		double result=a1.subtract(b1).doubleValue();
		System.out.println("method=testSubstract1>:"+result);// 输出0.8999999999999999
	}
	
	@Test
	public void testSubstract2(){
		BigDecimal a1=new BigDecimal(Double.valueOf(a));
		BigDecimal b1=new BigDecimal(Double.valueOf(b));
		double result=a1.subtract(b1).doubleValue();
		System.out.println("method=testSubstract2>:"+result);// 输出0.8999999999999999
	}
	
	@Test
	public void testSubstract3(){
		BigDecimal a1=new BigDecimal(Double.toString(a));
		BigDecimal b1=new BigDecimal(Double.toString(b));
		double result=a1.subtract(b1).doubleValue();
		System.out.println("method=testSubstract3>:"+result);// 输出0.9
	}
	

}
```



从上面可以看出，testSubstract3 方法输出的结果才是我们所预期的。


# 2.Double计算原理

《Effactive [Java](http://lib.csdn.net/base/javaee)》书中说，float和double类型的主要设计目标是为了科学计算和工程计算。他们执行二进制浮点运算，这是为了在广域数值范围上提供较为精确的快速近似计算而精心设计的。然而，它们没有提供完全精确的结果，所以不应该被用于要求精确结果的场合。但是，商业计算往往要求结果精确，这时候BigDecimal就派上大用场啦。


在JDK的描述中：

1、参数类型为double的构造方法的结果有一定的不可预知性。有人可能认为在Java中写入newBigDecimal(0.1)所创建的BigDecimal正好等于 0.1（非标度值 1，其标度为 1），但是它实际上等于0.1000000000000000055511151231257827021181583404541015625。这是因为0.1无法准确地表示为
 double（或者说对于该情况，不能表示为任何有限长度的二进制小数）。这样，传入到构造方法的值不会正好等于 0.1（虽然表面上等于该值）。



2、另一方面，String 构造方法是完全可预知的：写入 newBigDecimal("0.1") 将创建一个 BigDecimal，它正好等于预期的 0.1。因此，比较而言，**通常建议优先使用String构造方法**。

3、当double必须用作BigDecimal的源时，请注意，此构造方法提供了一个准确转换；它不提供与以下操作相同的结果：先使用`Double.toString(double)`方法，然后使用`BigDecimal(String)`构造方法，将double转换为String。要获取该结果，请使用static `valueOf(double)`方法。

# 3.BigDecimal计算工具

此工具类参考过网上的例子，但是其大部分都是错误的，其人使用的是：


BigDecimal b1 = new BigDecimal(Double.valueOf(value1));


在实际的计算中是错误的。下面是工具类的代码：




```java
package com.boonya.program.lang;

import java.math.BigDecimal;
import java.math.BigInteger;
/**
 * BigDecimal精确计算:参考http://docs.oracle.com/javase/7/docs/api/java/math/BigDecimal.html
 * 
 * <li>在大多数的商业计算中，一般采用java.math.BigDecimal类来进行精确计算。</li>
 * @package com.boonya.program.lang.BBigDecimal
 * @date   2016年11月28日  上午11:33:44
 * @author pengjunlin
 * @comment   
 * @update
 */
public class BBigDecimal extends BigDecimal{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -8187328246570289879L;

	public BBigDecimal(BigInteger val) {
		super(val);
	}

	/**
	 * 提供精确加法计算的add方法
	 * 
	 * @param value1
	 *            被加数
	 * @param value2
	 *            加数
	 * @return 两个参数的和
	 */
	public static double add(double value1, double value2) {
		BigDecimal a = new BigDecimal(Double.toString(value1));
		BigDecimal b = new BigDecimal(Double.toString(value2));
		return a.add(b).doubleValue();
	}
	
	/**
	 * 提供精确加法计算的add方法
	 * 
	 * @param value1
	 *            被加数
	 * @param value2
	 *            加数
	 * @param scale
	 *            精确范围
	 * @return 两个参数的和
	 * @throws IllegalAccessException 
	 */
	public static double add(double value1, double value2,int scale) throws IllegalAccessException {
		BigDecimal a = new BigDecimal(Double.toString(value1));
		BigDecimal b = new BigDecimal(Double.toString(value2));
		// 如果精确范围小于0，抛出异常信息
		if(scale<0){
			throw new IllegalAccessException("精确度不能小于0");
		}
		a=a.setScale(scale);//必须重新赋值
		b=b.setScale(scale);//必须重新赋值
		return a.add(b).setScale(scale).doubleValue();
	}

	/**
	 * 提供精确减法运算的subtract方法
	 * 
	 * @param value1
	 *            被减数
	 * @param value2
	 *            减数
	 * @return 两个参数的差
	 */
	public static double subtract(double value1, double value2) {
		BigDecimal a = new BigDecimal(Double.toString(value1));
		BigDecimal b = new BigDecimal(Double.toString(value2));
		return a.subtract(b).doubleValue();
	}
	
	/**
	 * 提供精确减法运算的subtract方法
	 * 
	 * @param value1
	 *            被减数
	 * @param value2
	 *            减数
	 * @param scale
	 *            精确范围
	 * @return 两个参数的差
	 * @throws IllegalAccessException 
	 */
	public static double subtract(double value1, double value2,int scale) throws IllegalAccessException {
		BigDecimal a = new BigDecimal(Double.toString(value1));
		BigDecimal b = new BigDecimal(Double.toString(value2));
		// 如果精确范围小于0，抛出异常信息
		if(scale<0){
			throw new IllegalAccessException("精确度不能小于0");
		}
		a=a.setScale(scale);//必须重新赋值
		b=b.setScale(scale);//必须重新赋值
		return a.subtract(b).setScale(scale).doubleValue();
	}

	/**
	 * 提供精确乘法运算的multiply方法
	 * 
	 * @param value1
	 *            被乘数
	 * @param value2
	 *            乘数
	 * @return 两个参数的积
	 */
	public static double multiply(double value1, double value2) {
		BigDecimal a = new BigDecimal(Double.toString(value1));
		BigDecimal b = new BigDecimal(Double.toString(value2));
		return a.multiply(b).doubleValue();
	}
	
	/**
	 * 提供精确乘法运算的multiply方法
	 * 
	 * @param value1
	 *            被乘数
	 * @param value2
	 *            乘数
	 * @param scale
	 *            精确范围
	 * @return 两个参数的积 
	 * @throws IllegalAccessException 
	 */
	public static double multiply(double value1, double value2,int scale) throws IllegalAccessException {
		BigDecimal a = new BigDecimal(Double.toString(value1));
		BigDecimal b = new BigDecimal(Double.toString(value2));
		// 如果精确范围小于0，抛出异常信息
		if(scale<0){
			throw new IllegalAccessException("精确度不能小于0");
		}
		a=a.setScale(scale);//必须重新赋值
		b=b.setScale(scale);//必须重新赋值
		return a.multiply(b).setScale(scale).doubleValue();
	}
	
	/**
	 * 提供精确的除法运算方法divide
	 * 
	 * @param value1
	 *            被除数
	 * @param value2
	 *            除数
	 * @param scale
	 *            精确范围
	 * @return 两个参数的商
	 * @throws IllegalAccessException
	 */
	public static double divide(double value1, double value2, int scale)
			throws IllegalAccessException {
		BigDecimal a = new BigDecimal(Double.toString(value1));
		BigDecimal b = new BigDecimal(Double.toString(value2));
		// 如果精确范围小于0，抛出异常信息
		if (scale < 0) {
			throw new IllegalAccessException("精确度不能小于0");
		}
		a=a.setScale(scale);//必须重新赋值
		b=b.setScale(scale);//必须重新赋值
		return a.divide(b,scale).doubleValue();
	}


}
```



# 4.总结



        (1)商业数据计算使用BigDecimal。

        (2)尽量使用参数类型为String的构造函数。

        (3) BigDecimal都是不可变的（immutable）的，在进行每一步运算时，都会产生一个新的对象，所以在做加减乘除运算时千万要保存操作后的值。

        (4)我们往往容易忽略JDK底层的一些实现细节，导致出现错误，需要多加注意。

参考博客地址：[http://blog.csdn.net/jackiehff/article/details/8582449](http://blog.csdn.net/jackiehff/article/details/8582449)




