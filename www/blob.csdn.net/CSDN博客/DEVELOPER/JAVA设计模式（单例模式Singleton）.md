# JAVA设计模式（单例模式Singleton） - DEVELOPER - CSDN博客





2017年04月12日 13:51:06[学术袁](https://me.csdn.net/u012827205)阅读数：222
个人分类：[java设计模式](https://blog.csdn.net/u012827205/article/category/2345605)

所属专栏：[JAVA设计模式](https://blog.csdn.net/column/details/15234.html)









单例模式很简单，我们经常使用到的也就饿汉和懒汉模式，我咋就是不喜欢这名字呢！！

让我分类，就是 是否在类加载的时候就已经创建了该类的对象：创建了（饿汉），牟（懒汉）

首先要了解为什么使用单例？？

单例模式应用的场景一般是这样：


　　（1）资源共享的情况下，避免由于资源操作时导致程序性能降低或损耗等。


　　（2）控制资源的情况下，方便资源之间的互相通信。如线程池等。

懒汉模式（类加载时，未创建实例）



```java
//懒汉模式
    private static Singleton instance;
    private Singleton(){}
    public static synchronized Singleton getInstance(){
        if(null == instance){
            instance =  new Singleton();
        }
        return instance;
    }
```




饿汉模式（类加载时，创建了实例）



```java
//饿汉模式
    //所谓的饿汉模式就是指 对象直接被new出来，并可以直接获取
    private static Singleton instance = new Singleton();
    private Singleton(){}
    public static  Singleton getInstance(){
        return instance;
    }
```






