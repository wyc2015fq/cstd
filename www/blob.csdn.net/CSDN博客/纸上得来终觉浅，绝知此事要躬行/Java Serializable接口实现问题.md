# Java Serializable接口实现问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年11月25日 14:23:10[boonya](https://me.csdn.net/boonya)阅读数：4102








#### **JavaBeans**

      要了解Serializable接口之前先来了解最基本的JavaBeans。JavaBeans为我们提供了最简单的Java类实体，这些实体伴随着业务在应用中来回穿梭。JavaBeans是Java中一种特殊的类，可以将多个对象封装到一个对象（bean）中。特点是可序列化，提供无参构造器，提供getter方法和setter方法访问对象的属性。名称中的“Bean”是用于Java的可重用软件组件的惯用叫法。

** JavaBeans优点：**

   （1） Bean可以控制它的属性、事件和方法是否暴露给其他程序。

   （2） Bean可以接收来自其他对象的事件，也可以产生事件给其他对象。

   （3） 有软件可用来配置Bean。

   （4） Bean的属性可以被序列化，以供日后重用。

**   JavaBeans规范：**

    要成为JavaBean类，则必须遵循关于命名、构造器、方法的特定规范。有了这些规范，才能有可以使用、复用、替代和连接JavaBeans的工具。规范如下：


    （1）有一个公有的无参构造器。

    （2） 属性可以通过get、set、is（可以替代get，用在布尔型属性上）方法或遵循特定命名规范的其他方法访问。

    （3）可序列化。


  JavaBean例子：

![](https://img-blog.csdn.net/20141125134522233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

注：以上使用的是默认的serialVersionUID，会被警告，因为这样不符合Java的性能要求。


#### Serializable

        Java类通过实现 java.io.Serializable 接口以启用其序列化功能。未实现此接口的类将无法使其任何状态序列化或反序列化。可序列化类的所有子类型本身都是可序列化的。序列化接口没有方法或字段，仅用于标识可序列化的语义。要允许不可序列化类的子类型序列化，可以假定该子类型负责保存和还原超类型的公用 (public)、受保护的 (protected) 和（如果可访问）包 (package) 字段的状态。仅在子类型扩展的类（父类）有一个可访问的无参数构造方法来初始化该类的状态时，才可以假定子类型有此责任。如果不是这种情况，则声明一个类为可序列化类是错误的。该错误将在运行时检测到。在反序列化过程中，将使用该类的公用或受保护的无参数构造方法初始化不可序列化类的字段。可序列化的子类必须能够访问无参数的构造方法。可序列化子类的字段将从该流中还原。

**serialVersionUID**

      序列化运行时使用一个称为 serialVersionUID 的版本号与每个可序列化类相关联，该序列号在反序列化过程中用于验证序列化对象的发送者和接收者是否为该对象加载了与序列化兼容的类。如果接收者加载的该对象的类的 serialVersionUID 与对应的发送者的类的版本号不同，则反序列化将会导致 InvalidClassException。可序列化类可以通过声明名为 "serialVersionUID"
 的字段（该字段必须是静态 (static)、最终 (final) 的 long 型字段）显式声明其自己的 serialVersionUID：
![](https://img-blog.csdn.net/20141125140040062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       如果可序列化类未显式声明 serialVersionUID，则序列化运行时将基于该类的各个方面计算该类的默认 serialVersionUID 值，如“Java(TM) 对象序列化规范”中所述。不过，强烈建议 所有可序列化类都显式声明 serialVersionUID 值，原因计算默认的 serialVersionUID 对类的详细信息具有较高的敏感性，根据编译器实现的不同可能千差万别，这样在反序列化过程中可能会导致意外的 InvalidClassException。因此，为保证 serialVersionUID
 值跨不同 java 编译器实现的一致性，序列化类必须声明一个明确的 serialVersionUID 值。还强烈建议使用 private 修改器显示声明 serialVersionUID（如果可能），原因是这种声明仅应用于立即声明类 -- serialVersionUID 字段作为继承成员没有用处。

        实现java.io.Serializable 接口的类是可序列化的。没有实现此接口的类将不能使它们的任一状态被序列化或逆序列化。

        序列化类的所有子类本身都是可序列化的。这个序列化接口没有任何方法和域，仅用于标识序列化的语意。允许非序列化类的子类型序列化，子类型可以假定负责保存和恢复父类型的公有的、保护的和(如果可访问)包的域的状态。只要该类(即父类)有一个无参构造子，可初始化它的状态，那么子类型就可承担上述职责；如果该类没有无参构造函数，在这种情况下申明一个可序列化的类是一个错误。此错误将在运行时被检测。


      任何类型只要实现了Serializable接口，就可以被保存到文件中，或者作为数据流通过网络发送到别的地方。也可以用管道来传输到系统的其他程序中。

#### 提供案例

      如下所示的两个方法，都没有问题，实质的问题在他们传递的SortingQualityCollection对象（JavaBean）：


![](https://img-blog.csdn.net/20141125140547203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20141125140621717?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        在程序运行时，save和update方法都可以执行成功，但是永远返回的状态都是false，这一点很让人费解，在debug后发现使用Spring AOP的使用是不能够成功的，另外最直接的错误是报出cglib的资源不存在。很疑惑的是，程序执行成功了update和insert的语句在后台也打印出来了，数据库中记录正确，由于返回的是false导致程序中业务处理始终不能往下进行。

       对比过程：查看数据对象和数据库字段是否一致、另外JavaBean是否规范。但做完这些事情还是无济于事，后来问题只能锁定在JavaBean上。JavaBean是通过工具生成的，serialVersionUID如下：

![](https://img-blog.csdn.net/20141125140040062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      于是重新生成serialVersionUID，新的serialVersionUID如下：

![](https://img-blog.csdn.net/20141125141714422?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

    重新运行程序，程序执行成功，由此可见serialVersionUID的声明很重要，最好在写完JavaBean之后手动生成之，并且显示声明。







