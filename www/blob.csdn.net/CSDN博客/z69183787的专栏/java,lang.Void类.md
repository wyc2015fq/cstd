# java,lang.Void类 - z69183787的专栏 - CSDN博客
2016年10月27日 15:55:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：490
在一次源码查看ThreadGroup的时候，看到一段代码，为以下：
**[java]**[view
 plain](http://blog.csdn.net/u012734441/article/details/51614214#)[copy](http://blog.csdn.net/u012734441/article/details/51614214#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1711835/fork)
- /*
-      * @throws  NullPointerException  if the parent argument is {@code null}
-      * @throws  SecurityException     if the current thread cannot create a
-      *                                thread in the specified thread group.
-      */
- privatestatic Void checkParentAccess(ThreadGroup parent) {  
-         parent.checkAccess();  
- returnnull;  
-     }  
            这个方法用于检查parent访问权限，然后直接返回null，方法的返回类型为Void原以为Void类为void类的包装类，但是查看Void类的
源码后发现并不是如此，Void类的源码如下：
**[java]**[view
 plain](http://blog.csdn.net/u012734441/article/details/51614214#)[copy](http://blog.csdn.net/u012734441/article/details/51614214#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1711835/fork)
- /**
-  * The {@code Void} class is an uninstantiable placeholder class to hold a
-  * reference to the {@code Class} object representing the Java keyword
-  * void.
-  *
-  * @author  unascribed
-  * @since   JDK1.1
-  */
- publicfinal
- class Void {  
- 
- /**
-      * The {@code Class} object representing the pseudo-type corresponding to
-      * the keyword {@code void}.
-      */
- @SuppressWarnings("unchecked")  
- publicstaticfinal Class<Void> TYPE = (Class<Void>) Class.getPrimitiveClass("void");  
- 
- /*
-      * The Void class cannot be instantiated.
-      */
- private Void() {}  
- }  
            在最上面的注释中，描述的是
*The {****@code ****Void} class is an uninstantiable placeholder class to hold a*** reference to the {****@code ****Class} object representing the Java keyword*
            这段话的意思就是Void类是一个不可实例化的占位符类，它持有对标识[Java](http://lib.csdn.net/base/javaee)关键字void的Class对象的引用。
         并且本身的构造函数为private，并且注明：
/*
 * The Void class cannot be instantiated.
 */
即该类是不可以实例化的。
        Void类可能本身作用就只是不起任何作用，但是本身只是一个占位符类。
