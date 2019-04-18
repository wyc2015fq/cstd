# java 判断两个数是否异号 - z69183787的专栏 - CSDN博客
2017年07月11日 17:12:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1095
java 整型int占4个字节32位，两个数异或后移动31位判断结果，如果是1则异号，如果是0则同号
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 public class ShowEnviromentViarible {
 2 
 3     public static void main(String[] args) {
 4         int num1 = 1;
 5         int num2 = -1;
 6         System.out.println("num1 = " + num1);
 7         System.out.println("num2 = " + num2);
 8         System.out.println((num1 ^ num2) >>> 31);    //无符号移动31位，最左边补0
 9         num1 = 1;
10         num2 = 1;
11         System.out.println("num1 = " + num1);
12         System.out.println("num2 = " + num2);
13         System.out.println((num1 ^ num2) >>> 31);
14         num1 = -1;
15         num2 = -1;
16         System.out.println("num1 = " + num1);
17         System.out.println("num2 = " + num2);
18         System.out.println((num1 ^ num2) >>> 31);
19     }
20 
21 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
output:
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
num1 = 1
num2 = -1
1
num1 = 1
num2 = 1
0
num1 = -1
num2 = -1
0
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
