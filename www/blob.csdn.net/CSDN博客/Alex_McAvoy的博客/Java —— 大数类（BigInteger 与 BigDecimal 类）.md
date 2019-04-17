# Java —— 大数类（BigInteger 与 BigDecimal 类） - Alex_McAvoy的博客 - CSDN博客





2018年09月10日 11:35:38[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：245
所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【概述】

在 C++ 中数据类型的长度最多能到 64 位，一旦超出这个位数，就要用数组进行模拟计算，即高精度算法

而在 Java 中有两个类：BigInteger、BigDecimal 分别表示大整数类和大浮点数类，其理论上能表示无限大的数，只要计算机内存足够大。 

这两个类都是 Number 类的子类，但存放在 java.math.* 包中，因此每次使用必须在开头处引用该包。

# 【常量】

BigInteger 的 ONE、ZERO、TEN 分别代表 1、0、10

```java
BigInteger.ZERO //大整数0 
BigInteger.ONE //大整数1 
BigInteger.TEN //大整数10
```

而在 BigDecimal 中除了以上三个常量外还有8个关于舍入的常量：ROUND_UP、ROUND_DOWN、ROUND_CEILING、ROUND_FLOOR、ROUND_HALF_UP、ROUND_HALF_DOWN、ROUND_HALF_EVEN、ROUND_UNNECESSARY。

# 【声明赋值】

基本类型的定义：

```java
BigInteger name = new BigInteger(String) ;//参数是字符串 
BigInteger name = BigInteger.valueOf(123); //参数是 int、long
BigDecimal name = new BigDecimal(String) ;//参数是字符串 
BigDecimal name = BigDecimal.valueOf(123.45); //参数是 float、double
```

# 【输入框架】

```java
Scanner input = new Scanner(System.in);
while(input.hasNext()){  //等同 !=EOF
    BigInteger a;
    a = input.BigInteger();
    System.out.println(a);
}
```

# 【基本用法】

## 1.四则运算

```java
BigInteger a = new BigInteger.valueOf(10);
BigInteger b = new BigInteger.valueOf(10);
BigInteger res new BigInteger();
res=a.add(b);         //加法 
res=a.subtract(b);    //减法 
res=a.divide(b);      //除法 
res=a.multiply(b);    //乘法
```

## 2.比较大小

```java
BigInteger a = new BigInteger.valueOf(10);
BigInteger b = new BigInteger.valueOf(10);
a.equals(b);      //如果a、b相等返回true否则返回false 
a.comareTo(b);    //a小于b返回-1，等于返回0，大于返回1
```

## 3.常用方法

```java
BigInteger a = new BigInteger.valueOf(10);
BigInteger b = new BigInteger.valueOf(15);
BigInteger mod = new BigInteger.valueOf(10007);
BigInteger res=new BigInteger();
res=a.mod(mod);       //求余 
res=a.gcd(b);         //求最大公约数 
res=a.max(b);         //求最大值 
res=a.min(b);         //求最小值
res=a.modPow(b,mod);  //求(a^b)%mod
```





