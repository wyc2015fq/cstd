# 【java】为什么要使用BigDecimal类型进行高精度运算 - Big Smile - CSDN博客
2018年03月25日 21:16:54[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1862
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        在公司，每次进行记账，或者算钱的时候，都会调用一个统一处理的类，名字叫ArithUntil，查了一下Arith就是数学、运算的意思，那么，为什么不直接用+ - * / 来进行对于数字类型数据的运算呢？（Ps.存储数据的时候有的用的int类型比如打折，但是存储金额的时候用的Decimal类型）。
# 一、探索
        探究其原因，从网上查了查，查到了一个有意思的东西，就是直接输出java运算某些数字的结果，却发现会在千万甚至亿万的小数级上出现错误，并且都是在浮点运算的时候才会出现的一些小误差的结果，这里有两组，大家可以跑一下试试：
```java
public static void main(String[] args) {
		System.out.println(1.0-0.8);
		System.out.println(1.1+0.8);
		System.out.println(1.1*0.9);
		System.out.println(606.3/3000);
	}
```
```java
System.out.println(0.05 + 0.01);
    System.out.println(1.0 - 0.42);
    System.out.println(4.015 * 100);
    System.out.println(123.3 / 100);
```
![](https://img-blog.csdn.net/20180325202648249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)第一个的结果
![](https://img-blog.csdn.net/20180325202735351?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)第二个的结果
        这个是什么原因呢？原因在于我们的计算机是二进制的。浮点数没有办法是用二进制进行精确表示。我们的CPU表示浮点数由两个部分组成：指数和尾数，这样的表示方法一般都会失去一定的精确度，有些浮点数运算也会产生一定的误差。
比如我在网上查了一下十进制小数的二进制表示方法：
[https://jingyan.baidu.com/article/425e69e6e93ca9be15fc1626.html](https://jingyan.baidu.com/article/425e69e6e93ca9be15fc1626.html)
由此可以得到，带小数的数字，如4.015其实是分成了两个部分表示，分别是整数部分4，和0.015部分，因为运算后的小数部分按照文中所述方法不能完全的变为由0,1表示的二进制数，所以cpu对于数值进行了舍弃，得到 了如上所示的值。
然后找到了一句特别官方的话：
` 其实java的float只能用来进行科学计算或工程计算，在大多数的商业计算中，一般采用java.math.BigDecimal类来进行精确计算。`
# 二、实际用法
        下面奉上通用的操作类（网上的好像都是这样写的）：
```java
import java.math.BigDecimal;   
/**  
* 由于Java的简单类型不能够精确的对浮点数进行运算，这个工具类提供精  
* 确的浮点数运算，包括加减乘除和四舍五入。  
*/  
public class ArithUntil{ //默认除法运算精度   
	private static final int DEF_DIV_SCALE = 10; //这个类不能实例化   
		private ArithUntil(){   
	}   
	/**  
	* 提供精确的加法运算。  
	* @param v1 被加数  
	* @param v2 加数  
	* @return 两个参数的和  
	*/  
	public static double add(double v1,double v2){   
		BigDecimal b1 = new BigDecimal(Double.toString(v1));   
		BigDecimal b2 = new BigDecimal(Double.toString(v2));   
		return b1.add(b2).doubleValue();   
	}   
	/**  
	* 提供精确的减法运算。  
	* @param v1 被减数  
	* @param v2 减数  
	* @return 两个参数的差  
	*/  
	public static double sub(double v1,double v2){   
		BigDecimal b1 = new BigDecimal(Double.toString(v1));   
		BigDecimal b2 = new BigDecimal(Double.toString(v2));   
		return b1.subtract(b2).doubleValue();   
	}   
	/**  
	* 提供精确的乘法运算。  
	* @param v1 被乘数  
	* @param v2 乘数  
	* @return 两个参数的积  
	*/  
	public static double mul(double v1,double v2){   
		BigDecimal b1 = new BigDecimal(Double.toString(v1));   
		BigDecimal b2 = new BigDecimal(Double.toString(v2));   
		return b1.multiply(b2).doubleValue();   
	}   
	/**  
	* 提供（相对）精确的除法运算，当发生除不尽的情况时，精确到  
	* 小数点以后10位，以后的数字四舍五入。  
	* @param v1 被除数  
	* @param v2 除数  
	* @return 两个参数的商  
	*/  
	public static double div(double v1,double v2){   
		return div(v1,v2,DEF_DIV_SCALE);   
	}   
	/**  
	* 提供（相对）精确的除法运算。当发生除不尽的情况时，由scale参数指  
	* 定精度，以后的数字四舍五入。  
	* @param v1 被除数  
	* @param v2 除数  
	* @param scale 表示表示需要精确到小数点以后几位。  
	* @return 两个参数的商  
	*/  
	public static double div(double v1,double v2,int scale){   
		if(scale<0){   
			throw new IllegalArgumentException(   
			"The scale must be a positive integer or zero");   
		}   
		BigDecimal b1 = new BigDecimal(Double.toString(v1));   
		BigDecimal b2 = new BigDecimal(Double.toString(v2));   
		return b1.divide(b2,scale,BigDecimal.ROUND_HALF_UP).doubleValue();   
	}   
	/**  
	* 提供精确的小数位四舍五入处理。  
	* @param v 需要四舍五入的数字  
	* @param scale 小数点后保留几位  
	* @return 四舍五入后的结果  
	*/  
	public static double round(double v,int scale){   
		if(scale<0){   
			throw new IllegalArgumentException("The scale must be a positive integer or zero");   
		}   
		BigDecimal b = new BigDecimal(Double.toString(v));   
		BigDecimal one = new BigDecimal("1");   
		return b.divide(one,scale,BigDecimal.ROUND_HALF_UP).doubleValue();   
	}   
};
```
总结一下，一般步骤为：
1）用float或者double变量构建BigDecimal对象。
2）通过调用BigDecimal的加，减，乘，除等相应的方法进行算术运算。
3）把BigDecimal对象转换成float，double，int等类型。
 一般来说，可以使用BigDecimal的构造方法或者静态方法的valueOf()方法把基本类型的变量构建成BigDecimal对象。
# 三、BigDecimal简介
         BigDecimal 由任意精度的整数非标度值 和32 位的整数标度 (scale) 组成。如果为零或正数，则标度是小数点后的位数。如果为负数，则将该数的非标度值乘以 10 的负scale 次幂。因此，BigDecimal表示的数值是(unscaledValue × 10-scale)。
部分源码：
1、valueOf方法
```java
public   static BigDecimal valueOf(double val) {
       // Reminder: a zero double returns '0.0', so we cannotfastpath
       // to use the constant ZERO. This might be important enough to
       // justify a factory approach, a cache, or a few private
       // constants, later.
       return new BigDecimal(Double.toString(val));
    }
```
2、add(BigDecimal augend)方法
```java
public BigDecimal   add(BigDecimal augend) {
          long xs =this.intCompact; //整型数字表示的BigDecimal,例a的intCompact值为122
          long ys = augend.intCompact;//同上
          BigInteger fst = (this.intCompact !=INFLATED) ?null :this.intVal;//初始化BigInteger的值，intVal为BigDecimal的一个BigInteger类型的属性
          BigInteger snd =(augend.intCompact !=INFLATED) ?null : augend.intVal;
          int rscale =this.scale;//小数位数
 
          long sdiff = (long)rscale - augend.scale;//小数位数之差
          if (sdiff != 0) {//取小数位数多的为结果的小数位数
              if (sdiff < 0) {
                 int raise =checkScale(-sdiff);
                 rscale =augend.scale;
                 if (xs ==INFLATED ||
                     (xs = longMultiplyPowerTen(xs,raise)) ==INFLATED)
                     fst =bigMultiplyPowerTen(raise);
                }else {
                   int raise =augend.checkScale(sdiff);
                   if (ys ==INFLATED ||(ys =longMultiplyPowerTen(ys,raise)) ==INFLATED)
                       snd = augend.bigMultiplyPowerTen(raise);
               }
          }
          if (xs !=INFLATED && ys !=INFLATED) {
              long sum = xs + ys;
              if ( (((sum ^ xs) &(sum ^ ys))) >= 0L)//判断有无溢出
                 return BigDecimal.valueOf(sum,rscale);//返回使用BigDecimal的静态工厂方法得到的BigDecimal实例
           }
           if (fst ==null)
               fst =BigInteger.valueOf(xs);//BigInteger的静态工厂方法
           if (snd ==null)
               snd =BigInteger.valueOf(ys);
           BigInteger sum =fst.add(snd);
           return (fst.signum == snd.signum) ?new BigDecimal(sum,INFLATED, rscale, 0) :
              new BigDecimal(sum,compactValFor(sum),rscale, 0);//返回通过其他构造方法得到的BigDecimal对象
 }
```
      我们可以看到，BigDecimal操作类专门有一个工厂方法来处理二进制位数溢出（操作系统原理有讲过）的状况。这样我们就可以很好的处理溢出的状况来得到想要的值了。
     因为BigInteger与BigDecimal都是不可变的（immutable）的，在进行每一步运算时，都会产生一个新的对象，所以a.add(b);虽然做了加法操作，但是a并没有保存加操作后的值，正确的用法应该是a=a.add(b);
