# JAVA中的选择语句 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 15:40:36[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：2450








- Java中的比较字符，既可以对数值进行比较，也可以对字符进行比较。比较的结果是一个boolean数据类型：true or false;
- Java和C语言一样，对于选择语句有如下几种类型：if语句和switch语句。
- if语句，即判断括号里面的布尔表达式计算的结果为true，则执行if块内的语句；否则，不执行；注意，if判断的条件一定要用括号框住，这一点和MATLAB语言不同。
- Java中的if-else语句和C语言中使用模式一样；
- 注意在if语句中经常会出现的错误：布尔值的冗余判断。例如：if(even==true)这样的判断语句就是冗余的，因为even可以自动计算出结果，实现判断，无需再判断是否为true，这些过程都是可以自动判断完成的；
- 产生随机数的方法：使用Math类中random方法。random是一个方法（函数），可以返回一个双精度的随机值，该值的范围在 [0.0, 1.0) 之间，注意不包含1.0。故语句(int)(Math.random()*10)会返回一个随机的0~9之间的整数；而且，random方法是没有参数的。另外一种产生随机数的方法用Random类来做，代码如下：Random random1 = new Random(3); //以种子3产生一个Random类的对象random1;
  random1.nextInt(10); 产生了一个0~n（不包括0和n）的随机整型值，Random类中的方法还有nextDouble, 返回一个(0.0, 1.0)之间的随机double型值。如果两个Random类的对象有相同的种子，那么它们生成的随机值也是相同的；
- System.exit(0);  实在System类中定义的，调用这个方法可以终止程序，参数0表示程序正常结束；
- 一个布尔类型的值不能转换为其他类型的值，其它类型的值也不能转换为布尔类型值；
- Java中的switch语句。当switch表达式的值与case语句的值相匹配的时候，就从该case开始的语句执行，直到遇到一个break语句，或者达到switch语句的末端。所以，一个break可以终止整个switch块，一般情况每种case之后都会有一个break。而default是可有可无的语句，其作用就是用来处理计算结果不能和任何一个case匹配的情况，这个时候就执行default语句，所以default一般情况下都放置在所有case语句的后面，也就是把默认情况放在最后。
- 控制输出精度。前面介绍过一种方法：(int) (result*100)/100.0的形式； 还有一种方式就是使用System类中的printf方法。而printf方法的使用模式和C语言中是一致的；




