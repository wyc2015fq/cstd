# spring注入时bean的set方法为什么不能是static类型的？ - z69183787的专栏 - CSDN博客
2014年08月06日 15:35:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8333
**静态方法是属于类（class）的，普通方法才是属于实体对象（也就是New出来的对象）的，spring注入是在容器中实例化对象，所以不能使用静态方法****在springframework里,我们不能@Autowired一个静态变量,使之成为一个spring bean,例如下面这样:**
**[java]**[view plain](http://blog.csdn.net/gaolu/article/details/38317445#)[copy](http://blog.csdn.net/gaolu/article/details/38317445#)
- @Autowired
- privatestatic YourClass yourClass;  
可以试一下,yourClass在这种状态下不能够被依赖注入,会抛出运行时异常java.lang.NullPointerException,为什么呢?静态变量/类变量不是对象的属性,而是一个类的属性,spring则是基于对象层面上的依赖注入.
而使用静态变量/类变量扩大了静态方法的使用范围.静态方法在spring是不推荐使用的.依赖注入的主要目的,是让容器去产生一个对象的实例,然后在整个生命周期中使用他们,同时也让testing工作更加容易.
一旦你使用静态方法,就不再需要去产生这个类的实例,这会让testing变得更加困难,同时你也不能为一个给定的类,依靠注入方式去产生多个具有不同的依赖环境的实例.这种static field是隐含共享的,并且是一种global全局状态,spring同样不推荐这样去做.
