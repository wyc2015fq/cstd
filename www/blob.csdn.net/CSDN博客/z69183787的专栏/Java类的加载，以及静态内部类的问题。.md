# Java类的加载，以及静态内部类的问题。 - z69183787的专栏 - CSDN博客
2017年03月18日 21:59:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1312
个人分类：[面试																[Java-内部类](https://blog.csdn.net/z69183787/article/category/6830252)](https://blog.csdn.net/z69183787/article/category/2185959)
http://blog.csdn.net/lengyueaiq/article/details/51143064
**1、属性的加载以及初始化的问题**
当要实例化一个类时，JVM会首先加载该类，并且在加载过程中检查这个类是否有静态属性以及静态代码块，如果有，就按顺序分配内存并初始化他们，并且只在类加载的过程中初始化一次。
对于构造代码块，以及普通属性，是在类实例化时进行的，并且每次实例化都会调用，并且构造代码块先于构造方法执行（无论代码块出现在构造方法前后，总是优先于构造方法）。
下面看这段代码：
![](https://img-blog.csdn.net/20160413144604993?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160413144802030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
首先JVM会加载这个类，通过断点调试发现当执行到静态代码块时，staticValue已经有值，这说明静态属性和静态代码块按顺序执行；
这段代码的结果请看下图：
![](https://img-blog.csdn.net/20160413144903746?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果把主方法的内容全部注释掉，只留一个System.out.println("***********");可以发现结果为
![](https://img-blog.csdn.net/20160413145216173?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
通过以上结果可以很清晰的验证属性的加载顺序。
**2、static与final的区别**
static属性在加载时就已经分配内存，并且只分配一次，可以用于对象间共享属性。
final定义的变量为常量，不能被改变，方法不能被覆盖，类不能被继承。
**3、对于父类以及子类中的情况**
会按照如下顺序加载，1、父类中的静态块、静态方法；2、子类中的静态块、静态方法；3、父类的构造块；4、父类的构造方法；5、子类的构造块；6、子类的构造方法。
我们看如下代码：
![](https://img-blog.csdn.net/20160413150029411?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对于在StaticIniBlockOrderTest类中实例化子类时，JVM首先会加载Parent类，并且同时给Parent类的静态属性分配内存和初始化、执行静态代码块；
其次，JVM会加载Child类，并对其进行与父类中相同的过程；
然后JVM会初始化父类，并按顺序执行父类的构造代码块与构造方法；
最后，JVM初始化子类，并执行构造代码块，执行构造方法，完成实例化。
那么输出结果为：
![](https://img-blog.csdn.net/20160413150550004?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
[](http://blog.csdn.net/lengyueaiq/article/details/51143064#)[](http://blog.csdn.net/lengyueaiq/article/details/51143064#)[](http://blog.csdn.net/lengyueaiq/article/details/51143064#)[](http://blog.csdn.net/lengyueaiq/article/details/51143064#)[](http://blog.csdn.net/lengyueaiq/article/details/51143064#)[](http://blog.csdn.net/lengyueaiq/article/details/51143064#)
