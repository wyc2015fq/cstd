# 说说 Java 的枚举类型 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [占小狼](http://www.jobbole.com/members/wx4097456919) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
### enum实现
JDK5中提供了Java枚举类型的实现，与其说是一种新类型，倒不如说是一种语法糖。


```
public enum Season {
    SPRING,
    SUMMER,
    AUTUMN,
    WINTER
}
```
通过[反编译工具 jad](http://varaneckas.com/jad/)来看看这段代码是如何实现的，反编译后的代码如下：


```
public final class Season extends Enum {
    public static Season[] values() {
        return (Season[])$VALUES.clone();
    }
    public static Season valueOf(String s) {
        return (Season)Enum.valueOf(Season, s);
    }
    private Season(String s, int i) {
        super(s, i);
    }
    public static final Season SPRING;
    public static final Season SUMMER;
    public static final Season AUTUMN;
    public static final Season WINTER;
    private static final Season $VALUES[];
    static {
        SPRING = new Season("SPRING", 0);
        SUMMER = new Season("SUMMER", 1);
        AUTUMN = new Season("AUTUMN", 2);
        WINTER = new Season("WINTER", 3);
        $VALUES = (new Season[] {
            SPRING, SUMMER, AUTUMN, WINTER
        });
    }
}
```
通过反编译的代码可以发现：
1、Season是一个普通的类，继承自Enum，并通过final关键字修饰，避免被继承，
2、枚举中的SPRING、SUMMER、AUTUMN和WINTER是Season类的静态实例，并在类构造器方法中进行初始化。
3、values()方法返回私有变量$VALUES[]的副本，$VALUES[]也是在方法中进行初始化。
### 如何使用enum
**1、单例模式**
在[JVM类加载的那些事](http://www.jianshu.com/p/2133558b4735)中，我们已经知道类构造器只能被一个线程在类加载的初始化阶段进行执行，所以枚举的每个实例在Java堆中有且只有一个副本，这种特性让枚举很容易就实现了单例模式，这也正是Effective Java作者 Josh Bloch 提倡使用实现单利模式的方式。


```
public enum Singleton { INSTANCE;}
```
**2、在switch中使用**
**3、自定义字段和方法**
enum中除了默认字段和方法之外，可以针对业务逻辑进行自定义。


```
public enum EnumTest {
  PLUS("+") {
      @Override
      public int bind(int arg1, int arg2) {
          return arg1 + arg2;
      }
  },
  SUB("-") {
      @Override
      public int bind(int arg1, int arg2) {
          return arg1 - arg2;
      }
  };
  final String operation;
  EnumTest(String operation) {
      this.operation = operation;
  }
  abstract int bind(int arg1, int arg2);
}
```
**4、实现接口**


```
interface Operation{
    int operate(int arg1, int arg2);
}
public enum EnumTest implements Operation {
    PLUS("+") {
        @Override
        public int operate(int arg1, int arg2) {
            return arg1 + arg2;
        }
    },
    SUB("-") {
        @Override
        public int operate(int arg1, int arg2) {
            return arg1 - arg2;
        }
    };
    final String operation;
    EnumTest(String operation) {
        this.operation = operation;
    }
}
```
在实际应用中，可以把 “+”、”-” 作为key，PLUS和SUB作为value，预先保存在hashMap中，具体使用方式如下：


```
Operation operation = hashMap.get("+");
int result = operation.bind(1, 2);
```
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/08/c3fbbefbe3e302d0f6944c94db3cb1c4.jpg)
