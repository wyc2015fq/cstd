# hdu1753——大明A+B - westbrook1998的博客 - CSDN博客





2018年06月17日 15:36:45[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：46








> 
话说，经过了漫长的一个多月，小明已经成长了许多，所以他改了一个名字叫“大明”。  

  这时他已经不是那个只会做100以内加法的那个“小明”了，现在他甚至会任意长度的正小数的加法。  

  现在，给你两个正的小数A和B，你的任务是代表大明计算出A+B的值。  

  Input 

  本题目包含多组测试数据，请处理到文件结束。  

  每一组测试数据在一行里面包含两个长度不大于400的正小数A和B。 

  Output 

  请在一行里面输出输出A+B的值，请输出最简形式。详细要求请见Sample Output。  

  Sample Input 

  1.1 2.9 

  1.1111111111 2.3444323343 

  1 1.1 

  Sample Output 

  4 

  3.4555434454 

  2.1
java水过…打死也不学c++高精度 

这题还学到了这两个函数`.stripTrailingZeros().toPlainString()` 去掉多余的0还是转成朴素表示法 

代码:
```
import java.math.BigDecimal;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner s=new Scanner(System.in);
        while(s.hasNext()){
            BigDecimal a=s.nextBigDecimal();
            BigDecimal b=s.nextBigDecimal();
            System.out.println(a.add(b).stripTrailingZeros().toPlainString());
        }
    }
}
```






