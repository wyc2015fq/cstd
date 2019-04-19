# Java的单例及多例设计模式 - wjheha的博客 - CSDN博客
2017年10月16日 23:01:37[wjheha](https://me.csdn.net/wjheha)阅读数：244
### 单例
先从谈构造方法私有化开始: 
在正常情况下，给了你一个类，那么只有通过产生对象之后才可以操作这个类。
```
class Singleton {
    public void print() {
        System.out.println("Hello World");
    }
}
public class TestDemo {
    public static void main(String args[]) {
        Singleton s = null;   //声明对象
        s = new Singleton();  //实例化对象
        s.print();
    }
}
```
现在Singleton类里面存在有构造方法，如果没有明确定义一个构造的话，会自动调用什么都不做的构造方法，即：一个类至少会保留有一个构造方法。 
范例：修改Singleton类的定义
```
class Singleton {
    private Singleton() {}   //构造方法私有化
    public void print() {
        System.out.println("Hello World");
    }
}
```
一旦构造方法私有化了，那么在外部将无法直接通过关键字new来进行对象的实例化调用。 
范例：错误的代码
```
public class TestDemo {
    public static void main(String args[]) {
        Singleton s = null;   //声明对象
        s = new Singleton();  //实例化对象
        s.print();
    }
}
```
对于整个程序到底该如何修改，才可以让TestDemo类里面得到Singleton类的实例化对象，并且调用实例化方法呢？ 
分析步骤： 
1、构造方法上使用了private()声明，那么就表明这个构造方法只能被类的内部所使用。既然如此，可以直接在类的内部实例化一个对象
```
class Singleton {
    Singleton instance = new Singleton();
    private Singleton() {}   //构造方法私有化
    public void print() {
        System.out.println("Hello World");
    }
}
```
2、现在的instance在Singleton里面只是一个普通的类属性，而所有的普通类属性必须在类产生实例化对象之后才可以使用，是否存在一个方式让这个类属性不受Singleton类实例化对象的控制呢？ 
如果使用了static声明instance属性，那么就表示可以在一个类没有产生实例化对象的时候直接使用该属性。
```
class Singleton {
    static Singleton instance = new Singleton();
    private Singleton() {}   //构造方法私有化
    public void print() {
        System.out.println("Hello World");
    }
}
public class TestDemo {
    public static void main(String args[]) {
        Singleton s = null;   //声明对象
        s = Singleton.instance;  //实例化对象
        s.print();
    }
}
```
3、在一个类定义的时候应该首先想到的就是类中的属性需要进行封装
`private static Singleton instance = new Singleton();`
4、而一旦封装之后如果想要访问此属性只能够通过getter方法，那么那就需要提供有一个getter方法，完成对Singleton实例化对象的控制，继续使用static属性。
```
class Singleton {
    private static Singleton instance = new Singleton();
    private Singleton() {}   //构造方法私有化
    public static Singleton getInstance() {
        return instance;
    }
    public void print() {
        System.out.println("Hello World");
    }
}
public class TestDemo {
    public static void main(String args[]) {
        Singleton s = null;   //声明对象
        s = Singleton.getInstance();  //实例化对象
        s.print();
    }
}
```
代码意义： 
如果说现在要控制一个类中实例化对象的产生个数，那么首先要锁定的就是类中的构造方法，因为在实例化任何新对象时都要使用构造方法，如果构造方法被锁了，那么自然就无法产生新的实例化对象。 
可是既然需要是一个实例化对象，那么就需要在类的内部使用static方式来定义一个公共的对象，外部调用static方法返回唯一的一个对象，这样外部不管有多少次调用，那么最终一个类只能够产生一个对象，这样的设计就属于***单例***设计模式（Singleton）。
```
class Singleton {
    private static final Singleton INSTANCE = new Singleton();
    private Singleton() {}   //构造方法私有化
    public static Singleton getInstance() {
        return INSTANCE;
    }
    public void print() {
        System.out.println("Hello World");
    }
}
public class TestDemo {
    public static void main(String args[]) {
        Singleton s = null;   //声明对象
        s = Singleton.getInstance();  //实例化对象
        s.print();
    }
}
```
***单例程序的特点***： 
构造方法私有化，在类的内部定义static属性和方法，利用static方法取得本类的实例化对象，这样不管外部会产生多少个Singleton类的对象，但是本质上永远只有唯一的一个实例化对象。 
  可是对于单例设计模式，有两种方式：饿汉式、懒汉式 
  在之前所编写的单例，就属于饿汉式，在Singleton定义的时候就已经实例化了一个对象INSTANCE，而并没有用关心这个对象是否使用。
```
//饿汉式实现
class Singleton {
    private static final Singleton INSTANCE = new Singleton();
    private Singleton() {}   //构造方法私有化
    public static Singleton getInstance() {
        return INSTANCE;
    }
    public void print() {
        System.out.println("Hello World");
    }
}
public class TestDemo {
    public static void main(String args[]) {
        Singleton s = null;   //声明对象
        s = Singleton.getInstance();  //实例化对象
        s.print();
    }
}
```
而懒汉式的最大特点就在于它是在第一次使用的时候才进行实例化操作。
```
//懒汉式实现
class Singleton {
    private static final Singleton instance;
    private Singleton() {}   //构造方法私有化
    public static Singleton getInstance() {
        if(instance == null) {   //此时还没有实例化
            instance = new SingletonI();   //实例化instance对象
        }
        return instance;
    }
    public void print() {
        System.out.println("Hello World");
    }
}
public class TestDemo {
    public static void main(String args[]) {
        Singleton s = null;   //声明对象
        s = Singleton.getInstance();  //实例化对象
        s.print();
    }
}
```
单例是一个理解过程。核心的目的：让一个类在整个系统里面只允许存在一个实例化对象。
### 多例
使用多例设计模式，可以让一个类产生指定多个实例化对对象 
例如：现在要定义一个表示一周时间数的类，这个类只能够取7个对象； 
或者定义一个表示性别的类，这个类只能有两个对象
```
calss Sex {
    private String title;
    private static final Sex MALE = new Sex("男");
    private static final Sex FEMALE = new Sex("女");
    private Sex(String title) {  //构造私有化了
        this.title = title;
    }
    public String toString() {
        return this.title;
    }
    public static Sex getInstance(int ch) {
        switch(ch) {
            case 1:
                    return MALE;
            case 2:
                    return FEMALE;
            default:
                    return null;
        }
    }
}
public class TestDemo {
    public static void main(String args[]) {
        Sex sex = Sex.getInstance("2");
        System.out.println(sex);
    }
}
```
在JDK1.7之前，switch只能够利用int或char进行判断，但是正因为如果纯粹是数字或者字符，意义不明确，所以1.7之后就支持String类型了。
