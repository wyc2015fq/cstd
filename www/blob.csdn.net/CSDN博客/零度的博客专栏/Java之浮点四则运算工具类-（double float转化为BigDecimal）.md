# Java之浮点四则运算工具类-（double/float转化为BigDecimal） - 零度的博客专栏 - CSDN博客
2016年04月11日 14:05:50[零度anngle](https://me.csdn.net/zmx729618)阅读数：7724
package zmx.util;
import java.math.BigDecimal;
public class ArithUtil {
 // 源文件Arith.java：
 /**
  * 由于Java的简单类型不能够精确的对浮点数进行运算，这个工具类提供精 确的浮点数运算，包括加减乘除和四舍五入。
  */
 // 默认除法运算精度
 private static final int DEF_DIV_SCALE = 10;
 // 这个类不能实例化
 private ArithUtil() {}
 /**
  * 提供精确的加法运算。
  * 
  * @param v1
  *            被加数
  * @param v2
  *            加数
  * @return 两个参数的和
  */
 public static double add(double v1, double v2) {
  BigDecimal b1 = new BigDecimal(Double.toString(v1));
  BigDecimal b2 = new BigDecimal(Double.toString(v2));
  return b1.add(b2).doubleValue();
 }
 /**
  * 提供精确的减法运算。
  * 
  * @param v1
  *            被减数
  * @param v2
  *            减数
  * @return 两个参数的差
  */
 public static double sub(double v1, double v2) {
  BigDecimal b1 = new BigDecimal(Double.toString(v1));
  BigDecimal b2 = new BigDecimal(Double.toString(v2));
  return b1.subtract(b2).doubleValue();
 }
 /**
  * 提供精确的乘法运算。
  * 
  * @param v1
  *            被乘数
  * @param v2
  *            乘数
  * @return 两个参数的积
  */
 public static double mul(double v1, double v2) {
  BigDecimal b1 = new BigDecimal(Double.toString(v1));
  BigDecimal b2 = new BigDecimal(Double.toString(v2));
  return b1.multiply(b2).doubleValue();
 }
 /**
  * 提供（相对）精确的除法运算，当发生除不尽的情况时，精确到 小数点以后10位，以后的数字四舍五入。
  * 
  * @param v1
  *            被除数
  * @param v2
  *            除数
  * @return 两个参数的商
  */
 public static double div(double v1, double v2) {
  return div(v1, v2, DEF_DIV_SCALE);
 }
 /**
  * 提供（相对）精确的除法运算。当发生除不尽的情况时，由scale参数指 定精度，以后的数字四舍五入。
  * 
  * @param v1
  *            被除数
  * @param v2
  *            除数
  * @param scale
  *            表示表示需要精确到小数点以后几位。
  * @return 两个参数的商
  */
 public static double div(double v1, double v2, int scale) {
  if (scale < 0) {
   throw new IllegalArgumentException(
     "The   scale   must   be   a   positive   integer   or   zero");
  }
  BigDecimal b1 = new BigDecimal(Double.toString(v1));
  BigDecimal b2 = new BigDecimal(Double.toString(v2));
  return b1.divide(b2, scale, BigDecimal.ROUND_HALF_UP).doubleValue();
 }
 /**
  * 提供精确的小数位四舍五入处理。
  * 
  * @param v
  *            需要四舍五入的数字
  * @param scale
  *            小数点后保留几位
  * @return 四舍五入后的结果
  */
 public static double round(double v, int scale) {
  if (scale < 0) {
   throw new IllegalArgumentException(
     "The   scale   must   be   a   positive   integer   or   zero");
  }
  BigDecimal b = new BigDecimal(Double.toString(v));
  BigDecimal one = new BigDecimal("1");
  return b.divide(one, scale, BigDecimal.ROUND_HALF_UP).doubleValue();
 }
 public static void main(String[] args) {
  /**
   * 浮点数类型float和double进行运算，由于精度问题不能得到想要的结果
   */
        System.out.println(0.05 + 0.01);  // 0.060000000000000005
        System.out.println(1.0 - 0.42);   // 0.5800000000000001
        System.out.println(4.015 * 100);  // 401.49999999999994
        System.out.println(123.3 / 100);  // 1.2329999999999999
  /**
   * java中可以转话为BigDecimal进行计算
   */
        System.out.println(ArithUtil.add(0.05, 0.01)); //0.06
        System.out.println(ArithUtil.sub(1.0, 0.42));  //0.58
        System.out.println(ArithUtil.mul(4.015, 100)); //401.5
        System.out.println(ArithUtil.div(123.3, 100, 5)); //1.233
        System.out.println(ArithUtil.round(0.0425, 3));   //0.043
 }
}
