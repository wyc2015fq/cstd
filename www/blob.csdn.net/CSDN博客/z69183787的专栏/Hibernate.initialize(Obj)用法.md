# Hibernate.initialize(Obj)用法 - z69183787的专栏 - CSDN博客
2013年12月09日 14:01:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1372
导读： 
　　在使用hibernate进行持久化时，有时需要动态的改变对象的加载，比如在编辑页面里面lazy=true,而在浏览页面lazy＝false,这样可以在需要lazy的地方才进行控制。而配置文件中Lazy属性是全局控制的，如何处理呢？ 
　　当元素或者元素的lazy属性为true时，load() or get() or find()加载这些对象时，Hibernate不会马上产生任何select语句，只是产生一个Obj代理类实例，只有在session没有关闭的情况下运行Obj.getXxx()时才会执行select语句从数据库加载对象，如果没有运行任何Obj.getXxx()方法，而session已经关闭，Obj已成游离状态，此时再运行Obj.getXxx()方法，Hibernate就会抛出"Could
 not initialize proxy - the owning Session was closeed"的异常，是说Obj代理类实例无法被初始化。然而想在Session关闭之前不调用Obj.getXxx()方法而关闭Session之后又要用，此时只要在Session关闭之前调用Hibernate.initialize(Obj)或者Hibernate.initialize(Obj.getXxx())即可，net.sf.hibernate.Hibernate类的initialize()静态方法用于在Session范围内显示初始化代理类实例。 
　　在配置文件里面可以用lazy=true，在程序里面可以用强制加载的方法Hibernate.initialize(Object proxy) 方法强制加载这样就相当于动态改变为lazy=false。 
　　但在使用时需要注意的一点是：其中的proxy是持久对象的关联对象属性，比如A实体，你要把A的关联实体B也检出，则要写Hibernate.initialize(a.b)。 
本文转自 
[http://hi.baidu.com/beforedead/blog/item/c626b60fbbffa5efab6457f4.html](http://hi.baidu.com/beforedead/blog/item/c626b60fbbffa5efab6457f4.html)
