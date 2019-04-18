# Java Double相加出现的怪事 - z69183787的专栏 - CSDN博客
2014年12月25日 16:54:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9338
问题的提出：   
  编译运行下面这个程序会看到什么
**[java]**[view
 plain](http://blog.csdn.net/benyuxy/article/details/7719208#)[copy](http://blog.csdn.net/benyuxy/article/details/7719208#)
- publicclass test {  
- publicstaticvoid main(String args[]) {  
-         System.out.println(0.05 + 0.01);  
-         System.out.println(1.0 - 0.42);  
-         System.out.println(4.015 * 100);  
-         System.out.println(123.3 / 100);  
-     }  
- };  
你没有看错！结果确实是
**[java]**[view
 plain](http://blog.csdn.net/benyuxy/article/details/7719208#)[copy](http://blog.csdn.net/benyuxy/article/details/7719208#)
- 0.060000000000000005
- 0.5800000000000001
- 401.49999999999994
- 1.2329999999999999
Java中的简单浮点数类型float和double不能够进行运算。不光是Java，在其它很多编程语言中也有这样的问题。在大多数情况下，计算的结果是准确的，但是多试几次（可以做一个循环）就可以试出类似上面的错误。现在终于理解为什么要有BCD码了。   
  这个问题相当严重，如果你有9.999999999999元，你的计算机是不会认为你可以购买10元的商品的。   
  在有的编程语言中提供了专门的货币类型来处理这种情况，但是Java没有。现在让我们看看如何解决这个问题。    
**解决方案**
  现在我们已经可以解决这个问题了，原则是使用BigDecimal并且一定要用String来够造。   
  但是想像一下吧，如果我们要做一个加法运算，需要先将两个浮点数转为String，然后够造成BigDecimal，在其中一个上调用add方法，传入另一个作为参数，然后把运算的结果（BigDecimal）再转换为浮点数。你能够忍受这么烦琐的过程吗？下面我们提供一个工具类Arith来简化操作。它提供以下静态方法，包括加减乘除和四舍五入：
**[java]**[view
 plain](http://blog.csdn.net/benyuxy/article/details/7719208#)[copy](http://blog.csdn.net/benyuxy/article/details/7719208#)
- publicstaticdouble add(double v1, double v2);  
- 
- publicstaticdouble sub(double v1, double v2);  
- 
- publicstaticdouble mul(double v1, double v2);  
- 
- publicstaticdouble div(double v1, double v2);  
- 
- publicstaticdouble div(double v1, double v2, int scale);  
- 
- publicstaticdouble round(double v, int scale);  
**[java]**[view
 plain](http://blog.csdn.net/benyuxy/article/details/7719208#)[copy](http://blog.csdn.net/benyuxy/article/details/7719208#)
- package org.nutz.mvc.core;  
- 
- import java.math.BigDecimal;  
- 
- publicclass Arith {  
- // 源文件Arith.java：
- 
- /**
-      * 由于Java的简单类型不能够精确的对浮点数进行运算，这个工具类提供精 确的浮点数运算，包括加减乘除和四舍五入。
-      */
- 
- // 默认除法运算精度
- privatestaticfinalint DEF_DIV_SCALE = 10;  
- 
- // 这个类不能实例化
- private Arith() {  
-     }  
- 
- /**
-      * 提供精确的加法运算。
-      * 
-      * @param v1
-      *            被加数
-      * @param v2
-      *            加数
-      * @return 两个参数的和
-      */
- 
- publicstaticdouble add(double v1, double v2) {  
-         BigDecimal b1 = new BigDecimal(Double.toString(v1));  
-         BigDecimal b2 = new BigDecimal(Double.toString(v2));  
- return b1.add(b2).doubleValue();  
-     }  
- 
- /**
-      * 提供精确的减法运算。
-      * 
-      * @param v1
-      *            被减数
-      * @param v2
-      *            减数
-      * @return 两个参数的差
-      */
- 
- publicstaticdouble sub(double v1, double v2) {  
-         BigDecimal b1 = new BigDecimal(Double.toString(v1));  
-         BigDecimal b2 = new BigDecimal(Double.toString(v2));  
- return b1.subtract(b2).doubleValue();  
-     }  
- 
- /**
-      * 提供精确的乘法运算。
-      * 
-      * @param v1
-      *            被乘数
-      * @param v2
-      *            乘数
-      * @return 两个参数的积
-      */
- 
- publicstaticdouble mul(double v1, double v2) {  
-         BigDecimal b1 = new BigDecimal(Double.toString(v1));  
-         BigDecimal b2 = new BigDecimal(Double.toString(v2));  
- return b1.multiply(b2).doubleValue();  
-     }  
- 
- /**
-      * 提供（相对）精确的除法运算，当发生除不尽的情况时，精确到 小数点以后10位，以后的数字四舍五入。
-      * 
-      * @param v1
-      *            被除数
-      * @param v2
-      *            除数
-      * @return 两个参数的商
-      */
- 
- publicstaticdouble div(double v1, double v2) {  
- return div(v1, v2, DEF_DIV_SCALE);  
-     }  
- 
- /**
-      * 提供（相对）精确的除法运算。当发生除不尽的情况时，由scale参数指 定精度，以后的数字四舍五入。
-      * 
-      * @param v1
-      *            被除数
-      * @param v2
-      *            除数
-      * @param scale
-      *            表示表示需要精确到小数点以后几位。
-      * @return 两个参数的商
-      */
- 
- publicstaticdouble div(double v1, double v2, int scale) {  
- if (scale < 0) {  
- thrownew IllegalArgumentException(  
- "The   scale   must   be   a   positive   integer   or   zero");  
-         }  
-         BigDecimal b1 = new BigDecimal(Double.toString(v1));  
-         BigDecimal b2 = new BigDecimal(Double.toString(v2));  
- return b1.divide(b2, scale, BigDecimal.ROUND_HALF_UP).doubleValue();  
-     }  
- 
- /**
-      * 提供精确的小数位四舍五入处理。
-      * 
-      * @param v
-      *            需要四舍五入的数字
-      * @param scale
-      *            小数点后保留几位
-      * @return 四舍五入后的结果
-      */
- 
- publicstaticdouble round(double v, int scale) {  
- if (scale < 0) {  
- thrownew IllegalArgumentException(  
- "The   scale   must   be   a   positive   integer   or   zero");  
-         }  
-         BigDecimal b = new BigDecimal(Double.toString(v));  
-         BigDecimal one = new BigDecimal("1");  
- return b.divide(one, scale, BigDecimal.ROUND_HALF_UP).doubleValue();  
-     }  
- };  
- 
