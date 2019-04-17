# Java-唠下《生命周期》 - DEVELOPER - CSDN博客





2018年03月14日 17:23:38[学术袁](https://me.csdn.net/u012827205)阅读数：125
所属专栏：[JAVA-语言](https://blog.csdn.net/column/details/20397.html)









> 
Java 虚拟机为 Java 程序提供运行时环境，其中一项重要的任务就是管理类和对象的生命周期。 

  类的生命周期从类被加载、连接和初始化开始，到类被卸载时结束；而类对象的生命周期进入开始阶段，Java虚拟机要为对象分配内存，并初始化她的实例变量。当程序不在使用该对象时，她的内存可以被Java虚拟机的垃圾回收器回收，此时对象就会结束自己的生命周期。


```java
public class JavaTest{
    private static final Map<String, JavaTest> javas = new HashMap<>();
    private static int test = "666";//测试变量，未用
    private final String name;

    public JavaTest(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }


    protected void lifeMethod() {
        String lifeName = "局部变量的生命周期......很短暂";
        System.out.println(lifeName);
    }

    public static JavaTest getInstance(String name){
        JavaTest javaTest = javas.get(name);
        if(javaTest ==null) {
            javaTest = new JavaTest(name);
            javas.put(name,  javaTest);
        }
        return javaTest;
    }

    public static void removeInstance(String name) {
        javas.remove(name);
    }

    protected void finalize(){
        javas.put(name, this);
        System.out.println("execute finalize......");
        System.out.println("execute finalize--->>>> this="+this);
        System.out.println("execute finalize--->>>> javas.get="+javas.get(name));
    }

    public static void main(String[] args) throws InterruptedException {
        JavaTest javaTest = JavaTest.getInstance("I-am-back");
        javaTest.lifeMethod();
        System.out.println("-----------------main>>>>"+javaTest);
        String name= javaTest.getName();
        javaTest = null;
        JavaTest.removeInstance(name);
        System.gc();
        Thread.sleep(3500);
        javaTest = JavaTest.getInstance("I-am-back");
        System.out.println("-----------------main>>>>"+javaTest);
    }
}
```

执行结果

```java
局部变量的生命周期......很短暂
-----------------main>>>>org.robby.test.JavaTest@7852e922
execute finalize......
execute finalize--->>>> this=org.robby.test.JavaTest@7852e922
execute finalize--->>>> javas.get=org.robby.test.JavaTest@7852e922
-----------------main>>>>org.robby.test.JavaTest@7852e922
```

通过对生命周期的理解，就从这些代码开始。 
**针对这个类 JavaTest**
**首先**是通过类的加载，当执行到 main方法的第一行代码时，会把JavaTest的.class 文件中的二进制数据读入到内存。并存放到运行时数据区的方法内，然后在堆区中创建一个 java.lang.Class 对象，用来封装类在方法区内的数据结构。 
**其次**是对类 JavaTest进行验证，就是把已经读入到内存的类的二进制数据合并到虚拟机的运行时环境中去。 
***然后***是进入它的准备阶段，Java 虚拟机要为类JavaTest的静态变量分配内存，并设置默认的初始值。比如上面的第四行代码，默认的静态变量初始值**test=0**。***然后***通过类的解析，Java 虚拟机将类JavaTest的二进制数据中的符号引用替换为直接引用。 
**最后** Java虚拟机就要对类 JavaTest执行初始化语句。就是这样，比如”test=666”，真正的初始化赋值。
> 
直到这里，类 JavaTest的生命周期就开始了。那直到什么时候它的生命周期才会结束呢？ 

  答案是，当代表 JavaTest类的Class对象不再被引用，即**不可触及状态[1](#fn:footnote)**时，那么Class对象就会结束生命周期。类在方法区内的数据被卸载，类的生命周期结束。


此时，类 JavaTest 处于生命周期中， 

执行完42行代码时，程序使用该类创建实例。 

执行完43行代码时，方法lifeMethod()中的局部变量lifeName的生命周期随即就结束了。因为对于局部变量，它的生命周期很短。 

执行完46行代码时，即使javaTest变量被设置为null，JavaTest对象仍然被其属性String name 间接引用，因此依然处于可触及状态。 

执行完47行代码时，JavaTest对象的引用从HashMap容器中删除，JavaTest对象不再被程序引用，此时进入了可复活状态。变成了无用的对象。虽然JavaTest对象没有被任何属性引用，但是在方法区中依然存在自己的数据，只是这个变量javaTest被置为null，即切断了它对JavaTest对象方法区数据位置的指向。所以你能看到37、38行代码打印出来的JavaTest对象的内存地址存在并且还是相同的。 

执行完48行代码时，假如最后调用了JavaTest对象的finalize()方法。该方法又把JavaTest对象放入到了HashMap对象中，此时JavaTest对象又回到了可触及状态。垃圾回收器将放弃对它内存的回收。 

执行完50行代码时，javaTest 变量又引用JavaTest对象。 javaTest 表示的是 指向类JavaTest 的实例对象的变量。
> 
如何在执行完main方法中的代码之后，能够让垃圾回收器回收javaTest对象并释放它所占的内存呢(结束javaTest对象的生命周期)？注释掉35行代码。


注释之后的运行结果

```java
局部变量的生命周期......很短暂
-----------------main>>>>org.robby.test.JavaTest@7852e922
execute finalize......
execute finalize--->>>> this=org.robby.test.JavaTest@7852e922
execute finalize--->>>> javas.get=null
-----------------main>>>>org.robby.test.JavaTest@4e25154f
```

- 不可触及状态，它是垃圾回收的概念。在Java虚拟机的垃圾回收器看来，堆区中的每个对象都可能处于三种状态：可触及状态、可复活状态和不可触及状态三种。**可触及状态：**当一个对象被创建后，只要程序中还有引用变量引用它，那么它就始终处于可触及状态。 **可复活状态：**当程序不再有任何引用变量引用类JavaTest对象时，那么它就进入了可复活状态。在这个状态中，垃圾回收器就会准备释放她所占用的内存。并在释放之前调用它以及其他处于可复活状态的对象的方法finalize()。但是这些方法finalize()有可能使JavaTest的对象重新转入到可触及状态。**不可触及状态：**当Java虚拟机执行完所有客服或对象的finalize()方法后，加入这些方法都没有使JavaTest的对象转入可触及状态，那么avaTest的对象就会进入不可触及状态。并会真正的被垃圾回收器回收它所占用的内存。 [↩](#fnref:footnote)






