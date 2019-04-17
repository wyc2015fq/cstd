# JavaStep-小数末尾除零以及in.hasNext() - CD's Coding - CSDN博客





2013年09月23日 20:24:30[糖果天王](https://me.csdn.net/okcd00)阅读数：919








```java
//Java POJ1001 Exponentiation
```

```java
import java.math.*;
import java.util.*;
public class Main{
    public static void main(String[] args){
        Scanner in=new Scanner(System.in);
        while(in.hasNext()){
            BigDecimal val=in.nextBigDecimal();
            int n=in.nextInt();
            BigDecimal ret=val.pow(n).stripTrailingZeros();
            System.out.println( ret.toPlainString().replaceAll("^0", "") );
        }
    }
```






