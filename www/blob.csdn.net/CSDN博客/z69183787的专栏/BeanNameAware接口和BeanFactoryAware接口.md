# BeanNameAware接口和BeanFactoryAware接口 - z69183787的专栏 - CSDN博客
2016年06月13日 14:38:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1065
迄今为止，所接触到的Bean都是“无知觉”的，就像黑客帝国中机械工厂里面“养殖”的人类，他们虽然能完成一定的功能，但是根本不知道自己在工厂（BeanFactory）中的代号（id），或者自己是在哪个工厂（BeanFactory的引用）中沉睡。所以，本节的目的就是要创造出一个尼奥一样的Bean，让他知道自己在工厂中的id和自己原来躺在哪个工厂中。这里，称之为，Bean对Spring有知觉。
但是有言在先，如果要在Spring容器中做到这两点，当然，可以自己通过某些方法实现，代价是大量冗余代码，好处是跟Spring解耦。如果使用Spring提供的接口，好处当然减少代码的规模，“缺点”（如果算的话）是与 Spring耦合。总之，无论采取那种办法还是要看实际需要。
**/////////////////////让Bean对Name有知觉/////////////////////**
作用：让Bean获取自己在BeanFactory配置中的名字（根据情况是id或者name）。
实现：通过实现BeanNameAware接口，接口中就一个方法setBeanName()
例程如下（P100，代码清单5-11，部分）
public class LogginBean implements BeanNameAware {
private String beanName = null;
public void setBeanName(String beanName) {
this.beanName = beanName;
}
}
额外说明：
Spring 自动调用。并且会在Spring自身完成Bean配置之后，且在调用任何Bean生命周期回调（初始化或者销毁）方法之前就调用这个方法。换言之，在程序中使用BeanFactory.getBean(String beanName)之前，Bean的名字就已经设定好了。所以，程序中可以尽情的使用BeanName而不用担心它没有被初始化。
当然，Bean之中一定要有个String类型变量来保存BeanName的值，这个是在编写Bean代码时有程序员手工完成的，而不是通过什么特殊的饿配置。
通常都是为了更好的饿配置Log，但是请注意，不要只是因为配合 Log就把bean那么中添加任何业务含义，否则会让你自己的类关联到某个Spring特性上去，而它带来的好处却是微不足道的。如果确实需要Bean带有某种内部名称，那么最好让Bean自己实现某个方法，例如setName()，然后使用DI给每个Bean注入一个名字，这样技能保持配置Bean的名字仍然能够简洁，且你也无需修改自己的配置文件，好让Bean的名字带有业务含义。
**////////////////////////////////让Bean对工厂有知觉////////////////////////////////**
作用：让Bean获取配置他们的BeanFactory的引用。
实现：实现BeanFactoryAware接口，其中只有一个方法即setFactory(BeanFactory factory)。使用上与BeanNameAware接口无异，只不过BeanFactoryAware注入的是个工厂，BeanNameAware注入的是个Bean的名字。
额外说明：
虽然原书中并未提及setFactory()方法的调用时机，但是可以猜想到，这个方法可能是在根据某个配置文件创建了一个新工厂之后，Spring才调用这个方法，并把BeanFactory注入到Bean中。
让bean获取配置自己的工厂之后，当然可以在Bean中使用这个工厂的getBean()方法，但是，实际上非常不推荐这样做，因为结果是进一步加大Bean与Spring的耦合，而且，能通过DI注入进来的尽量通过DI来注入。
当然，除了查找bean，BeanFactory可以提供大量其他的功能，例如销毁singleton模式的Bean。
补充说明：
factory.preInstantiateSingletons();方法。preInstantiateSingletons()方法立即实例化所有的Bean实例，有必要对这个方法和Spring加载bean的机制做个简单说明。
方法本身的目的是让Spring立即处理工厂中所有Bean的定义，并且将这些Bean全部实例化。因为Spring默认实例化Bean的情况下，采用的是lazy机制，换言之，如果不通过getBean()方法（BeanFactory或者ApplicationContext的方法）获取Bean的话，那么为了节省内存将不实例话Bean，只有在Bean被调用的时候才实例化他们。
