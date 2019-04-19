# 序列化与readResolve()方法 - 零度的博客专栏 - CSDN博客
2016年10月14日 11:30:44[零度anngle](https://me.csdn.net/zmx729618)阅读数：903
 readResolve方法是作用是什么？这个方法跟对象的序列化相关(这样倒是解释了为什么 readResolve方法是private修饰的)。 怎么跟对象的序列化相关了?
       下面我们先简要地回顾下对象的序列化。一般来说，一个类实现了 Serializable接口，我们就可以把它往内存地写再从内存里读出而"组装"成一个跟原来一模一样的对象。不过当序列化遇到单例时，里边就有了个问题：从内存读出而组装的对象破坏了单例的规则。单例是要求一个JVM中只有一个类对象的，而现在通过反序列，一个新的对象克隆了出来。如下例所示：
　　Java代码：
```java
public final class MySingleton implements Serializable {
     private MySingleton() { }
     private static final MySingleton INSTANCE = new MySingleton();
     public static MySingleton getInstance() { return INSTANCE; }
}
```
       当把 MySingleton对象(通过getInstance方法获得的那个单例对象)序列化后再从内存中读出时，就有一个全新但跟原来一样的MySingleton对象存在了。那怎么来维护单例模式呢?这就要用到readResolve方法。 如下所示：
          Java代码：
```java
public final class MySingleton implements Serializable{
    private MySingleton() { }
    private static final MySingleton INSTANCE = new MySingleton();
    public static MySingleton getInstance() { return INSTANCE; }
    private Object readResolve() throws ObjectStreamException {
       // instead of the object we're on,
       // return the class variable INSTANCE
      return INSTANCE;
   }
}
```
          这样，当JVM从内存中反序列化地"组装"一个新对象时，就会自动调用这个 readResolve方法来返回我们指定好的对象了，单例规则也就得到了保证。
