# JavaStep-进制切换BigInteger - CD's Coding - CSDN博客





2013年09月23日 20:17:56[糖果天王](https://me.csdn.net/okcd00)阅读数：709








```java
//java POJ2305 Basic remains
```

```java
import java.math.*;
import java.io.*;
import java.util.*;
public class Main{
       public static void main(String[] args){
              Scanner in = new Scanner(new BufferedInputStream(System.in));
              BigInteger a,b;
              int n;
              String str;
              while (true){
                  n = in.nextInt();
                  if (n==0) break;
                  str = in.next();
                  a = new BigInteger(str,n);
                  str = in.next();
                  b = new BigInteger(str,n);
                  a = a.mod(b);
                  System.out.println(a.toString(n));
              }
       }
}
```




