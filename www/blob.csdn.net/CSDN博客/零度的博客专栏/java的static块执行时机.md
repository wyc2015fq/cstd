# java的static块执行时机 - 零度的博客专栏 - CSDN博客
2016年06月03日 17:06:48[零度anngle](https://me.csdn.net/zmx729618)阅读数：1254
一、误区：简单认为JAVA静态代码块在类被加载时就会自动执行。证错如下：  
**[java]**[view plain](http://blog.csdn.net/berber78/article/details/46472789#)[copy](http://blog.csdn.net/berber78/article/details/46472789#)[print](http://blog.csdn.net/berber78/article/details/46472789#)[?](http://blog.csdn.net/berber78/article/details/46472789#)
- class MyClass1 {  
- static {//静态块
-         System.out.println("static block ");  
-     }  
- }  
- publicclass Main {  
- 
-     Class[] classArray = {  
-             MyClass1.class//这样引用该类，必然需要将该类加载到虚拟机中
-     };  
- publicstaticvoid main(String[] args){  
-         System.out.println("hello word");  
-     }  
- 
- }  
```java
<span style="font-family:Arial;font-size:12px;">class MyClass1 {
    static {//静态块
        System.out.println("static block ");
    }
}
public class Main {
    Class[] classArray = {
            MyClass1.class//这样引用该类，必然需要将该类加载到虚拟机中
    };
    public static void main(String[] args){
        System.out.println("hello word");
    }
}
</span>
```
执行结果：并没有输出" static bolck" 
二、正解：static块真正的执行时机。如果了解JVM原理，我们知道，一个类的运行分为以下步骤：
> - **装载**
- **连接**
- **初始化**
其中, 装载阶段又三个基本动作组成：
-     通过类型的完全限定名，产生一个代表该类型的二进制数据流
-     解析这个二进制数据流为方法区内的内部数据结
-     构创建一个表示该类型的java.lang.Class类的实例
    另外如果一个类装载器在预先装载的时遇到缺失或错误的class文件，它需要等到程序首次主动使用该类时才报告错误。
连接阶段又分为三部分：
- 验证，确认类型符合Java语言的语义，检查各个类之间的二进制兼容性(比如final的类不用拥有子类等)，另外还需要进行符号引用的验证。
- 准备，Java虚拟机为类变量分配内存，设置默认初始值。
- 解析(可选的) ，在类型的常量池中寻找类，接口，字段和方法的符号引用，把这些符号引用替换成直接引用的过程。
当一个类被主动使用时，Java虚拟就会对其初始化，如下六种情况为主动使用：
- 当创建某个类的新实例时（如通过new或者反射，克隆，反序列化等）
- 当调用某个类的静态方法时
- 当使用某个类或接口的静态字段时
- 当调用Java API中的某些反射方法时，比如类Class中的方法，或者java.lang.reflect中的类的方法时
- 当初始化某个子类时
- 当虚拟机启动某个被标明为启动类的类（即包含main方法的那个类）
    Java编译器会收集所有的类变量初始化语句和类型的静态初始化器，将这些放到一个特殊的方法中：clinit。 
      实际上，static块的执行发生在“初始化”的阶段。初始化阶段，jvm主要完成对静态变量的初始化，静态块执行等工作。
     下面我们看看执行static块的几种情况：
     1、第一次new A()的过程会打印""；因为这个过程包括了初始化
     2、第一次Class.forName("A")的过程会打印""；因为这个过程相当于Class.forName("A",true,this.getClass().getClassLoader());
     3、第一次Class.forName("A",false,this.getClass().getClassLoader())的过程则不会打印""。因为false指明了装载类的过程中，不进行初始化。不初始化则不会执行static块。
