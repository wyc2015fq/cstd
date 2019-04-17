# Java实现算法导论中Pollard的rho启发式方法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月01日 18:20:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2033
所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)









Pollard的rho启发式方法用于启发式求解大整数n分解因子，具体要结合导论中来理解，参考代码如下：



```java
package cn.ansj;

import java.math.BigInteger;
import java.security.SecureRandom;
    
class PollardRho{
    private final static BigInteger ZERO = new BigInteger("0");
    private final static BigInteger ONE  = new BigInteger("1");
    private final static BigInteger TWO  = new BigInteger("2");
    private final static SecureRandom random = new SecureRandom();

    public static BigInteger rho(BigInteger N) {
        BigInteger divisor;
        BigInteger c  = new BigInteger(N.bitLength(), random);
        BigInteger x  = new BigInteger(N.bitLength(), random);
        BigInteger xx = x;

        if (N.mod(TWO).compareTo(ZERO) == 0) return TWO;

        do {
            x  =  x.multiply(x).mod(N).add(c).mod(N);
            xx = xx.multiply(xx).mod(N).add(c).mod(N);
            xx = xx.multiply(xx).mod(N).add(c).mod(N);
            divisor = x.subtract(xx).gcd(N);
        } while((divisor.compareTo(ONE)) == 0);

        return divisor;
    }

    public static void factor(BigInteger N) {
        if (N.compareTo(ONE) == 0) return;
        if (N.isProbablePrime(20)) { 
        	System.out.println(N); 
        	return; 
        }
        BigInteger divisor = rho(N);
        factor(divisor);
        factor(N.divide(divisor));
    }
 
    public static void main(String[] args)  {
        BigInteger N = BigInteger.valueOf(96879);
        System.out.println(N);
        factor(N);
    }
}
```
执行结果：



```
96879
3
43
751
```






