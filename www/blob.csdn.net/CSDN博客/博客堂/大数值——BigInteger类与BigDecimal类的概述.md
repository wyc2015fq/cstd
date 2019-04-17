# 大数值——BigInteger类与BigDecimal类的概述 - 博客堂 - CSDN博客





2017年03月01日 18:58:27[最小森林](https://me.csdn.net/u012052268)阅读数：321








# 大数值——BigInteger类与BigDecimal类的概述


- [大数值BigInteger类与BigDecimal类的概述](#大数值biginteger类与bigdecimal类的概述)- [BigInteger类](#biginteger类)- [概述](#概述)
- [测试主要方法](#测试主要方法)

- [BigDecimal类](#bigdecimal类)- [概述](#概述-1)
- [主要用法](#主要用法)





## BigInteger类

### 概述

可以让超过Integer（主要用来与String进行转换）范围内的数据进行加减乘除的运算。

测试构造方法： BigInteger(String val) 

```
public class BigIntegerDemo {
    public static void main(String[] args) {
        // 这几个测试，是为了简单超过int范围内，Integer就不能再表示，所以就更谈不上计算了。
        // Integer i = new Integer(100);
        // System.out.println(i);
        // // System.out.println(Integer.MAX_VALUE);
        // Integer ii = new Integer("2147483647");
        // System.out.println(ii);
        // // NumberFormatException
        // Integer iii = new Integer("2147483648");
        // System.out.println(iii);

        // 通过大整数来创建对象
        BigInteger bi = new BigInteger("2147483648");
        System.out.println("bi:" + bi);
    }
}
```

### 测试主要方法：

public BigInteger add(BigInteger val):加

public BigInteger subtract(BigInteger val):减

public BigInteger multiply(BigInteger val):乘

public BigInteger divide(BigInteger val):除

public BigInteger[] divideAndRemainder(BigInteger val):返回商和余数的数组

```
public class BigIntegerDemo {
    public static void main(String[] args) {
        BigInteger bi1 = new BigInteger("100");
        BigInteger bi2 = new BigInteger("50");

        // public BigInteger add(BigInteger val):加
        System.out.println("add:" + bi1.add(bi2));
        // public BigInteger subtract(BigInteger val):加
        System.out.println("subtract:" + bi1.subtract(bi2));
        // public BigInteger multiply(BigInteger val):加
        System.out.println("multiply:" + bi1.multiply(bi2));
        // public BigInteger divide(BigInteger val):加
        System.out.println("divide:" + bi1.divide(bi2));

        // public BigInteger[] divideAndRemainder(BigInteger val):返回商和余数的数组
        BigInteger[] bis = bi1.divideAndRemainder(bi2);
        System.out.println("商：" + bis[0]);
        System.out.println("余数：" + bis[1]);
    }
}
```

## BigDecimal类

### 概述

float类型的数据存储和整数不一样导致的。它们大部分的时候，都是带有有效数字位。

由于在运算的时候，float类型和double很容易丢失精度。所以，为了能精确的表示、计算浮点数，Java提供了BigDecimal类

BigDecimal类：不可变的、任意精度的有符号十进制数,可以解决数据丢失问题。

### 主要用法：

构造方法：

public BigDecimal(String val)

主要方法： 

public BigDecimal add(BigDecimal augend)

public BigDecimal subtract(BigDecimal subtrahend)

public BigDecimal multiply(BigDecimal multiplicand)

public BigDecimal divide(BigDecimal divisor)

public BigDecimal divide(BigDecimal divisor,int scale,int roundingMode):商，几位小数，如何舍取

```
public class BigDecimalDemo {
    public static void main(String[] args) {
        // System.out.println(0.09 + 0.01);
        // System.out.println(1.0 - 0.32);
        // System.out.println(1.015 * 100);
        // System.out.println(1.301 / 100);

        BigDecimal bd1 = new BigDecimal("0.09");
        BigDecimal bd2 = new BigDecimal("0.01");
        System.out.println("add:" + bd1.add(bd2));
        System.out.println("-------------------");

        BigDecimal bd3 = new BigDecimal("1.0");
        BigDecimal bd4 = new BigDecimal("0.32");
        System.out.println("subtract:" + bd3.subtract(bd4));
        System.out.println("-------------------");

        BigDecimal bd5 = new BigDecimal("1.015");
        BigDecimal bd6 = new BigDecimal("100");
        System.out.println("multiply:" + bd5.multiply(bd6));
        System.out.println("-------------------");

        BigDecimal bd7 = new BigDecimal("1.301");
        BigDecimal bd8 = new BigDecimal("100");
        System.out.println("divide:" + bd7.divide(bd8));
        System.out.println("divide:"
                + bd7.divide(bd8, 3, BigDecimal.ROUND_HALF_UP));
        System.out.println("divide:"
                + bd7.divide(bd8, 8, BigDecimal.ROUND_HALF_UP));
    }
}
```



