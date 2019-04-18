# readResolve()方法与序列化 - z69183787的专栏 - CSDN博客
2015年11月16日 14:52:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：519
在CJC(一) 中提到一个问题,即 readResolve方法是干啥的? 当时也没多想, 只是列在那里, 今天忙里偷闲地把搜点材料整理下这个问题.
　　原来这个方法跟对象的序列化相关(这样倒是解释了为什么 readResolve方法是private修饰的). ??? 怎么跟对象的序列化相关了?
　　下面我们先简要地回顾下对象的序列化. 一般来说, 一个类实现了 Serializable接口, 我们就可以把它往内存地写再从内存里读出而"组装"成一个跟原来一模一样的对象. 不过当序列化遇到单例时,这里边就有了个问题: 从内存读出而组装的对象破坏了单例的规则. 单例是要求一个JVM中只有一个类对象的, 而现在通过反序列化,一个新的对象克隆了出来.
　　如下例所示:
　　Java代码
**[java]**[view
 plain](http://blog.csdn.net/huangbiao86/article/details/6896565#)[copy](http://blog.csdn.net/huangbiao86/article/details/6896565#)[print](http://blog.csdn.net/huangbiao86/article/details/6896565#)[?](http://blog.csdn.net/huangbiao86/article/details/6896565#)
- publicfinalclass MySingleton implements Serializable {  
- private MySingleton() { }  
- privatestaticfinal MySingleton INSTANCE = new MySingleton();  
- publicstatic MySingleton getInstance() { return INSTANCE; }  
- }  
　当把 MySingleton对象(通过getInstance方法获得的那个单例对象)序列化后再从内存中读出时, 就有一个全新但跟原来一样的MySingleton对象存在了. 那怎么来维护单例模式呢?这就要用到readResolve方法了. 如下所示:
　　Java代码　
**[java]**[view
 plain](http://blog.csdn.net/huangbiao86/article/details/6896565#)[copy](http://blog.csdn.net/huangbiao86/article/details/6896565#)[print](http://blog.csdn.net/huangbiao86/article/details/6896565#)[?](http://blog.csdn.net/huangbiao86/article/details/6896565#)
- publicfinalclass MySingleton implements Serializable{  
- private MySingleton() { }  
- privatestaticfinal MySingleton INSTANCE = new MySingleton();  
- publicstatic MySingleton getInstance() { return INSTANCE; }  
- private Object readResolve() throws ObjectStreamException {  
- // instead of the object we're on,
- // return the class variable INSTANCE
- return INSTANCE;  
-    }  
- }  
这样当JVM从内存中反序列化地"组装"一个新对象时,就会自动调用这个 readResolve方法来返回我们指定好的对象了, 单例规则也就得到了保证.
　　上面用的例子来源于这个链接:http://www.javalobby.org/java/forums/t17491.html, 另这个链接中还有一个更为高级的例子, 如有兴趣可去一看.
