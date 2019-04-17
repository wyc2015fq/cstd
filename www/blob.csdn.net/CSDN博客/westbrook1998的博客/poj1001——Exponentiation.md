# poj1001——Exponentiation - westbrook1998的博客 - CSDN博客





2018年07月12日 21:54:44[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：51








> 
Problems involving the computation of exact values of very large magnitude and precision are common. For example, the computation of the national debt is a taxing experience for many computer systems.  

  This problem requires that you write a program to compute the exact value of R n where R is a real number ( 0.0 < R < 99.999 ) and n is an integer such that 0 < n <= 25. 

  Input 

  The input will consist of a set of pairs of values for R and n. The R value will occupy columns 1 through 6, and the n value will be in columns 8 and 9. 

  Output 

  The output will consist of one line for each line of input giving the exact value of R^n. Leading zeros should be suppressed in the output. Insignificant trailing zeros must not be printed. Don’t print the decimal point if the result is an integer. 

  Sample Input 

  95.123 12 

  0.4321 20 

  5.1234 15 

  6.7592  9 

  98.999 10 

  1.0100 12 

  Sample Output 

  548815620517731830194541.899025343415715973535967221869852721 

  .00000005148554641076956121994511276767154838481760200726351203835429763013462401 

  43992025569.928573701266488041146654993318703707511666295476720493953024 

  29448126.764121021618164430206909037173276672 

  90429072743629540498.107596019456651774561044010001 

  1.126825030131969720661201
直接用java大数类乱搞，注意`.stripTrailingZeros().toPlainString()` 这两个方法 

还要题目要求 要去掉小于1的小数的前导0，字符串再处理一下 

代码：
```
import java.math.BigDecimal;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        BigDecimal a;
        int b;
        Scanner scanner=new Scanner(System.in);
        while(scanner.hasNext()){
            a=scanner.nextBigDecimal();
            b=scanner.nextInt();
            String res=a.pow(b).stripTrailingZeros().toPlainString();
            if(res.charAt(0)=='0'){
                res=res.substring(1,res.length());
            }
            System.out.println(res);
        }
    }
}
```






