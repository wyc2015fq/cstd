# JAVA 枚举单例模式及源码分析 - z69183787的专栏 - CSDN博客
2017年01月06日 17:01:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：289
个人分类：[Java-枚举类型](https://blog.csdn.net/z69183787/article/category/6668403)

关于单例模式的实现有很多种，网上也分析了如今实现单利模式最好用枚举，好处不外乎三点：1.线程安全 2.不会因为序列化而产生新实例 3.防止反射攻击
但是貌似没有一篇文章解释ENUM单例如何实现了上述三点，请高手解释一下这三点~
关于第二点序列化问题，有一篇文章说枚举类自己实现了readResolve（）方法，所以抗序列化，这个方法是当前类自己实现的（解决）
关于第一点线程安全，从反编译后的类源码中可以看出也是通过类加载机制保证的，应该是这样吧（解决）
关于第三点反射攻击，我有自己试着反射攻击了以下，不过报错了...看了下方的反编译类源码，明白了，因为单例类的修饰是abstract的，所以没法实例化。（解决）
以下是我写的一个枚举单例，以及其class文件反编译过后的类
枚举单例
```
public enum Singleton {
    INSTANCE {
        @Override
        protected void read() {
            System.out.println("read");
        }
        @Override
        protected void write() {
            System.out.println("write");
        }
    };
    protected abstract void read();
    protected abstract void write();
}
```
反编译过后还原的类
```java
```
public abstract class Singleton extends Enum
{
    private Singleton(String s, int i)
    {
        super(s, i);
    }
    protected abstract void read();
    protected abstract void write();
    public static Singleton[] values()
    {
        Singleton asingleton[];
        int i;
        Singleton asingleton1[];
        System.arraycopy(asingleton = ENUM$VALUES, 0, asingleton1 = new Singleton[i = asingleton.length], 0, i);
        return asingleton1;
    }
    public static Singleton valueOf(String s)
    {
        return (Singleton)Enum.valueOf(singleton/Singleton, s);
    }
    Singleton(String s, int i, Singleton singleton)
    {
        this(s, i);
    }
    public static final Singleton INSTANCE;
    private static final Singleton ENUM$VALUES[];
    static 
    {
        INSTANCE = new Singleton("INSTANCE", 0) {
            protected void read()
            {
                System.out.println("read");
            }
            protected void write()
            {
                System.out.println("write");
            }
        };
        ENUM$VALUES = (new Singleton[] {
            INSTANCE
        });
    }
}
```
```
```
