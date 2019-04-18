# Java 数字千位分隔符 - z69183787的专栏 - CSDN博客
2018年02月09日 10:43:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：561
```java
package com.zing;
import java.math.BigDecimal;
import java.text.DecimalFormat;
public class Test {
public static void main(String[] arg) {
BigDecimal bd = new BigDecimal(123456789.3);
System.out.println(parseNumber(",###,###", bd)); // out: 123,456,789
System.out.println(parseNumber(",###,###.00", bd)); // out: 123,456,789.30
}
public static String parseNumber(String pattern, BigDecimal bd) {
DecimalFormat df = new DecimalFormat(pattern);
return df.format(bd);
}
}
```
