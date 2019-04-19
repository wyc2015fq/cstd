# Java学习（十二） 异常 - HJ - CSDN博客
2018年03月08日 13:08:53[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：125
所属专栏：[Java学习](https://blog.csdn.net/column/details/19570.html)
一、异常概述
1. Java异常体系图
			|——Throwable  （实现类描述java的错误和异常）
    |——Error （错误）一般不通过代码去处理。
				    |——Exceprion （异常）
					            |——RuntimeException （运行时异常）
					            |——非运行时异常
2. 常见的Error
比如说，Java虚拟机默认管理了64M的内存，一下数组需要1G的内存，这样子会造成内存溢出；
3. Throwable类
（1）toString()        输出该异常的类名；
（2）getMessage()     输出异常的信息，需要通过构造方法传入异常信息；
（3）printStackTrace()   打印栈信息；
二、程序中的异常处理
1. 自行处理
（1）try{//可能发生异常的代码 }catch(异常类 变量名){//处理}，如果没有进行try catch处理，出现异常程序就停止。进行处理后，程序会继续执行;
（2）如果出现多个异常，使用将可能发生异常的代码放入try语句中，添加多个catch语句即可；
（3）注意，Java可以处理多种异常，但是同时只能处理一种异常；
（4）如果异常被catch匹配上，接着执行try{} catch(){}后的语句。否则程序停止；
（5）try中多个异常同时出现，只会处理第一条出现异常的一句，剩余的异常不再处理；
（6）多个catch语句之间的执行顺序：
①进行顺序执行，从上到下；
②如果多个catch内的异常有子父类关系，那么对于子类异常在上，父类在下，编译通过运行没有问题；对于父类异常在上，子类在下，编译不通过，因为父类可以将子类的异常处理，子类的catch处理不到。因此，多个异常要按照子类和父类进行catch;
2. 抛出处理
（1）try语句不能单独存在，可以和catch、finally组成try...catch...finally、try...catch、try...finally三种结构；
（2）catch语句可以有一个或多个，finally语句最多一个，try、catch、finally这三个关键字均不能单独使用；
（3）try、catch、finally三个代码块中变量的作用域分别独立而不能相互访问。如果要在三个块中都可以访问，则需要将变量定义到这些块的外面；
（4）多个catch块的时候，Java虚拟机会匹配其中一个异常类或其子类，就执行这个catch块，而不会执行别的catch块。记住，子类在上，父类在下；
（5）Throw语句后不允许有紧跟其他语句，因为这些没有机会执行；
（6）如果一个方法调用了另外一个声明抛出异常的方法，那么这个方法要么处理异常，要么声明抛出；
3. 自定义异常
Java的面向对象思想将程序中出现的特有问题进行封装，比如说可以定义没有IP地址抛出异常等；
三、运行时异常和非运行时异常
1. RuntimeException（运行时异常）
RuntimeException的子类：
（1）ClassCastException：多态中可以使用instanceof判断，进行规避；
（2）ArithmeticException：进行if判断，如果除数为0，进行return；
（3）NullPointerException：进行if判断，是否为null；
（4）ArrayIndexOutOfBoundsException：使用数组length属性，避免越界；
2. 非运行时异常
（1）子类覆盖父类方法时，父类方法抛出异常，子类的覆盖方法可以不抛出异常，或者抛出父类方法的异常，或者该父类方法异常的子类；
（2）父类方法抛出了多个异常，子类覆盖方法时，只能抛出父类异常的子集；
（3）父类没有抛出异常子类不可以抛出异常；
（4）子类不能比父类抛出更多的异常；
四、finally总结
（1）比较适合用于专门的处理异常的diamante，不适合释放资源的代码：
    try{ // 可能发生异常的代码 } catch( 异常类的类型 e ){ // 当发生指定异常的时候的处理代码 }catch...		
（2）比较适合用于既要处理异常又有资源释放的代码：
    try{  } catch(){} finally{ // 释放资源的代码 }
（3）比较适合处理的都是运行时异常且有资源释放的代码：
       try{  }finally{ // 释放资源 }
（4）finally关键字主要用于释放系统资源，在处理异常的时候该语句只能有一个，且无论程序正常还是异常，都执行finally；
（5）只有JVM退出了System.exit(0)，finally才会不执行；
