# java的基础学习之（toString()、String.valueOf()、String强转的区别） - wjheha的博客 - CSDN博客
2017年04月10日 15:44:00[wjheha](https://me.csdn.net/wjheha)阅读数：215
在java项目的实际开发和应用中，常常需要用到将对象转为String这一基本功能。本文将对常用的转换方法进行一个总结。常用的方法有Object#toString()，（String）要转换的对象，String.valueOf(Object)等。下面对这些方法一一进行分析。
**方法1**：采用
 Object#toString()方法请看下面的例子：
Object object = getObject();
System.out.println(object.toString());
在这种使用方法中，因为java.lang.Object类里已有public方法.toString()，所以对任何严格意义上的java对象都可以调用此方法。但在使用时要注意，必须保证object不是null值，否则将抛出NullPointerException异常。采用这种方法时，通常派生类会覆盖Object里的toString（）方法。
**方法2：**采用类型转换（String）object方法这是标准的类型转换，将object转成String类型的值。使用这种方法时，需要注意的是类型必须能转成String类型。因此最好用instanceof做个类型检查，以判断是否可以转换。否则容易抛出CalssCastException异常。此外，需特别小心的是因定义为Object
 类型的对象在转成String时语法检查并不会报错，这将可能导致潜在的错误存在。这时要格外小心。如：
Object obj = new Integer(100);
String strVal = (String)obj;
在运行时将会出错，因为将Integer类型强制转换为String类型，无法通过。但是，
Integer obj = new Integer(100);
String strVal = (String)obj;
如是格式代码，将会报语法错误。
此外，因null值可以强制转换为任何java类类型，(String)null也是合法的。
**方法3：**采用String.valueOf(Object)
 String.valueOf(Object)的基础是Object#toString()。但它与Object#toString()又有所不同。在前面方法1的分析中提到，使用后者时需保证不为null。但采用第三种方法时，将不用担心object是否为null值这一问题。为了便于说明问题，我们来分析一下相关的源代码。Jdk里String# valueOf(Object)源码如下：
public static String valueOf(Object obj) {
return (obj == null) ? "null" : obj.toString(); }
从上面的源码可以很清晰的看出null值不用担心的理由。但是，这也恰恰给了我们隐患。我们应当注意到，当object为null时，String.valueOf（object）的值是字符串”null”，而不是null！！！在使用过程中切记要注意。试想一下，如果我们用 if(String.valueOf（object）==null){System.out.println(“传入的值是null！”);}这样的语句将可能会发生什么问题。再想一下，向控制台输出时，在视觉上如下语句在执行的结果上有什么不同：
System.out.println(String.valueOf(null));
System.out.println(null);
我们看到的输出将是一模一样的东西：null，但它们意义相同吗？
以上是对object对象转换为String的一些总结。
