# Java中有atoi函数吗？ - bigfacesafdasgfewgf - CSDN博客





2015年03月17日 17:33:09[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：2875










    我们知道C++中有atoi函数，可以把一个表示数字的字符串转为int类型的数值。




    那么我们就要问：Java中有atoi函数吗？

    答案是没有，不过有类似与atoi功能的函数，那就是Integer.parseInt()。

    这个函数的具体调用方法如下：






```java
String val = "1980000000"; 
System.out.println(Integer.parseInt(val));
```


它就是输出1980000000。也是一样的功能。




    关于Integer.parseInt的详细说明文档如下：




1、方法原型：public static int parseInt(String s) throws NumberFormatException


方法说明：将字符串参数作为有符号的十进制整数进行解析。除了第一个字符可以是用来表示负值的 ASCII 减号 '-' ('\u002D') 外，字符串中的字符都必须是十进制数字。返回得到的整数值，就好像将该参数和基数 10 作为参数赋予 parseInt(java.lang.String, int) 方法一样。



参数：

s - 包含要解析的 int 表示形式的 String。 

返回：

用十进制参数表示的整数值。 

抛出： 

NumberFormatException - 如果字符串不包含可解析的整数。


2、还有另外一种形式：

public static int parseInt(String s,int radix)throws NumberFormatException

方法说明：使用第二个参数指定的基数，将字符串参数解析为有符号的整数。除了第一个字符可以是用来表示负值的 ASCII 减号 '-' ('\u002D’)外，字符串中的字符必须都是指定基数的数字（通过 Character.digit(char, int) 是否返回一个负值确定）。返回得到的整数值。


如果发生以下任意一种情况，则抛出一个 NumberFormatException 类型的异常： 


第一个参数为 null 或一个长度为零的字符串。 

基数小于 Character.MIN_RADIX 或者大于 Character.MAX_RADIX。 

假如字符串的长度超过 1，那么除了第一个字符可以是减号 '-' ('u002D’) 外，字符串中存在任意不是由指定基数的数字表示的字符。 

字符串表示的值不是 int 类型的值。 


示例： 


parseInt("0", 10) 返回 0

parseInt("473", 10) 返回 473

parseInt("-0", 10) 返回 0

parseInt("-FF", 16) 返回 -255

parseInt("1100110", 2) 返回 102

parseInt("2147483647", 10) 返回 2147483647

parseInt("-2147483648", 10) 返回 -2147483648

parseInt("2147483648", 10) 抛出 NumberFormatException

parseInt("99", 8) 抛出 NumberFormatException

parseInt("Kona", 10) 抛出 NumberFormatException

parseInt("Kona", 27) 返回 411787


参数：

s - 包含要解析的整数表示形式的 String

radix - 解析 s 时使用的基数。 

返回：

使用指定基数的字符串参数表示的整数。 

抛出： 

NumberFormatException - 如果 String 不包含可解析的 int。






















