# BigDecimal的使用 - 菜鸟路上的小白 - CSDN博客





2016年09月06日 14:45:34[somnus_小凯](https://me.csdn.net/u012486840)阅读数：1709







BigDecimal类型(+ - * /)所用的属性

**11.10 BigDecimal类**

对于不需要任何准确计算精度的数字可以直接使用float或double，但是如果需要精确计算的结果，则必须使用BigDecimal类，而且使用BigDecimal类也可以进行大数的操作。BigDecimal类的常用方法如表11-15所示。

表11-15 BigDecimal类的常用方法

|序号|方 法|类型|描 述|
|----|----|----|----|
|1|public BigDecimal(double val)|构造|将double表示形式转换为BigDecimal|
|2|public BigDecimal(int val)|构造|将int表示形式转换为BigDecimal|
|3|public BigDecimal(String val)|构造|将字符串表示形式转换为BigDecimal|
|4|public BigDecimal add(BigDecimal augend)|普通|加法|
|5|public BigDecimal subtract(BigDecimalsubtrahend)|普通|减法|
|6|public BigDecimal multiply(BigDecimalmultiplicand)|普通|乘法|
|7|public BigDecimal divide(BigDecimaldivisor)|普通|除法|


范例：进行四舍五入的四则运算
- **package** org.lxh.demo11.numberdemo;
- **import** java.math.BigDecimal;
- **class** MyMath {
- **publicstaticdouble** add(**double** d1, **double** d2)

{        // 进行加法运算
-          BigDecimal b1 = **new** BigDecimal(d1);
-          BigDecimal b2 = **new** BigDecimal(d2);
- **return** b1.add(b2).doubleValue();
-      }
- **publicstaticdouble** sub(**double** d1, **double** d2)

{        // 进行减法运算
-          BigDecimal b1 = **new** BigDecimal(d1);
-          BigDecimal b2 = **new** BigDecimal(d2);
- **return** b1.subtract(b2).doubleValue();
-      }
- **publicstaticdouble** mul(**double** d1, **double** d2)

{        // 进行乘法运算
-          BigDecimal b1 = **new** BigDecimal(d1);
-          BigDecimal b2 = **new** BigDecimal(d2);
- **return** b1.multiply(b2).doubleValue();
-      }
- **publicstaticdouble** div(**double** d1,
**double** d2,**int** len) {//
 进行除法运算
-          BigDecimal b1 = **new** BigDecimal(d1);
-          BigDecimal b2 = **new** BigDecimal(d2);
- **return** b1.divide(b2,len,BigDecimal.

ROUND_HALF_UP).doubleValue();
-      }
- **publicstaticdouble** round(**double** d,
**int** len) {     // 进行四舍五入
- 操作
-          BigDecimal b1 = **new** BigDecimal(d);
-          BigDecimal b2 = **new** BigDecimal(1);
- // 任何一个数字除以1都是原数字
- // ROUND_HALF_UP是BigDecimal的一个常量，

表示进行四舍五入的操作
- **return** b1.divide(b2, len,BigDecimal.

ROUND_HALF_UP).doubleValue();
-      }
- }
- **publicclass** BigDecimalDemo01
 {
- **publicstaticvoid** main(String[]
 args) {
-          System.out.println("加法运算：" +

MyMath.round(MyMath.add(10.345,
- 3.333), 1));
-          System.out.println("乘法运算：" +

MyMath.round(MyMath.mul(10.345,
- 3.333), 3));
-          System.out.println("除法运算：" +

MyMath.div(10.345, 3.333, 3));
-          System.out.println("减法运算：" +

MyMath.round(MyMath.sub(10.345,
- 3.333), 3));
-      }
- }








**BigDecimal比较**

BigDecimal是通过使用compareTo(BigDecimal)来比较的，具体比较情况如下：


publicstaticvoid main(String[] args) {
    BigDecimal a = new BigDecimal("1");
    BigDecimal b = new BigDecimal("2");
    BigDecimal c = new BigDecimal("1");
    int result1 = a.compareTo(b);
    int result2 = a.compareTo(c);
    int result3 = b.compareTo(a);
    System.out.println(result1);
    System.out.println(result2);
    System.out.println(result3);
    
}



```
打印结果是：-1、0、1，即左边比右边数大，返回1，相等返回0，比右边小返回-1。
```






