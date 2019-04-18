# JAVA：内层方法抛出异常 外层方法捕捉并处理异常 - z69183787的专栏 - CSDN博客
2014年01月27日 11:28:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：15853
这是一篇有关[JAVA](http://www.edu84.com/dn/JAVA/)：内层方法抛出异常，外层方法捕捉并处理异常的文章信息。
我发布到[报名在线](http://www.edu84.com/)EDU84.COM上，和大家分享一下[JAVA](http://www.edu84.com/dn/JAVA/)：内层方法抛出异常，外层方法捕捉并处理异常
public void methodA (){ 
try{ 
// 调用methodB 
methodB(); 
} 
catch(ExceptionType et){ 
// 相应处理措施 
} 
} 
public void methodB throws ExceptionType{ 
if (condition is true) 
{ 
// 相应处理措施 
} 
else 
{ 
throw new ExceptionType(argument); 
} 
} 
在这个例子中，方法B的头部中声明了该方法会抛出一个类型为ExceptionType的异常，在方法体中使用throw子句抛出了一个异常，那么该异常被谁捕捉到呢，答案是方法A。因为异常抛出后，JVM会顺着该方法的调用栈一层一层的往上找。因为方法A中有一个catch(ExceptionType et),所以被抛出的异常会被捕捉到并处理。
