# ThreadLocal 内部实现和应用场景 - z69183787的专栏 - CSDN博客
2016年05月24日 14:41:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8812
个人分类：[多线程](https://blog.csdn.net/z69183787/article/category/2176989)

很多人都知道java中有ThreadLocal这个类，但是知道ThreadLocal这个类具体有什么作用，然后适用什么样的业务场景还是很少的。今天我就尝试以自己的理解，来讲解下ThreadLocal类的内部实现和应用场景，如果有什么不对之处，还望大家指正。
首先明确一个概念，那就是ThreadLocal并不是用来并发控制访问一个共同对象，而是为了给每个线程分配一个只属于该线程的对象（这么粗暴的解释可能还不太准确），更准确的说是为了实现线程间的数据隔离。而ThreadLocal应用场景更多是想共享一个变量，但是该变量又不是线程安全的，那么可以用ThreadLocal维护一个线程一个实例。有时候ThreadLocal也可以用来避免一些参数传递，通过ThreadLocal来访问对象。
首先我们先看下ThreadLocal（jdk1.7）内部的实现： 
ThreadLocal get方法
```
public T get() {
  Thread t = Thread.currentThread();
  ThreadLocalMap map = getMap(t);
  if (map != null) {
    ThreadLocalMap.Entry e = map.getEntry(this);
    if (e != null)
      return (T)e.value;
  }
  return setInitialValue();
}
```
ThreadLocal set方法
```
public void set(T value) {
  Thread t = Thread.currentThread();
  ThreadLocalMap map = getMap(t);
  if (map != null)
    map.set(this, value);
  else
    createMap(t, value);
}
```
get和set方法是ThreadLocal类中最常用的两个方法。
get方法
代码很容易理解，首先我们通过Thread.currentThread得到当前线程，然后获取当前线程的threadLocals变量，这个变量就是ThreadLocalMap类型的。然后根据当前的ThreadLocal实例作为key，获取到Entry对象。
set方法
代码同样很容易理解。同样根据Thread.currentThread得到当前线程，如果当前线程存在threadLocals这个变量不为空，那么根据当前的ThreadLocal实例作为key寻找在map中位置，然后用新的value值来替换旧值。
在ThreadLocal这个类中比较引人注目的应该是ThreadLocal->ThreadLocalMap->Entry这个类。这个类继承自WeakReference。关于弱引用的知识，以后我会抽时间写篇文章来介绍下。
最近在我们的web项目中servlet需要频繁创建SimpleDateFormat这个对象，进行日期格式化。因为创建这个对象本身很费时的，而且我们也知道SimpleDateFormat本身不是线程安全的，也不能缓存一个共享的SimpleDateFormat实例，为此我们想到使用ThreadLocal来给每个线程缓存一个SimpleDateFormat实例，提高性能。同时因为每个Servlet会用到不同pattern的时间格式化类，所以我们对应每一种pattern生成了一个ThreadLocal实例。
DateFormatFactory
```java
```
public class DateFormatFactory {
private static final Map<DatePattern, ThreadLocal<DateFormat>> pattern2ThreadLocal;
static {
  DatePattern[] patterns = DatePattern.values();
  int len = patterns.length;
  pattern2ThreadLocal = new HashMap<DatePattern, ThreadLocal<DateFormat>>(len);
  for (int i = 0; i < len; i++) {
    DatePattern datePattern = patterns[i];
    final String pattern = datePattern.pattern;
    pattern2ThreadLocal.put(datePattern, new ThreadLocal<DateFormat>() {
      @Override
      protected DateFormat initialValue() {
        return new SimpleDateFormat(pattern);
      }
    });
  }
}
//获取DateFormat
public static DateFormat getDateFormat(DatePattern pattern) {
  ThreadLocal<DateFormat> threadDateFormat = pattern2ThreadLocal.get(pattern);
  //不需要判断threadDateFormat是否为空
  return threadDateFormat.get(); 
}
```
```
}
DatePattern 枚举类
```
public enum DatePattern {
TimePattern("yyyy-MM-dd HH:mm:ss"),
DatePattern("yyyy-MM-dd"),
public String pattern;
private DatePattern(String pattern) {
    this.pattern = pattern;
}
```
}
这样我们就可以每次调用DateFormatFactory.getDateFormat获取到对应的时间格式化类了。之前我们提到使用ThreadLocal同时可以避免参数传递。假如我们这个Servlet要调用到其他类的方法，而且方法内需要使用时间格式化类。按照正常情况下我们把该时间格式化类作为参数进行传递，但如果有了ThreadLocal这个类，我们可以不需要作为参数传递了，可以在方法类通过ThreadLocal得到时间格式化类。当然代码的通用性就差了。
