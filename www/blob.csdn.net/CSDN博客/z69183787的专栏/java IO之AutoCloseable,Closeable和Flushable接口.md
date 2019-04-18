# java IO之AutoCloseable,Closeable和Flushable接口 - z69183787的专栏 - CSDN博客
2017年10月17日 11:11:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2685
有3个接口对于流类相当重要。其中两个接口是**Closeable**和**Flushable**，它们是在java.io包中定义的，并且是由JDK5添加的。第3个接口是**AutoColseable**，它是由JDK7添加的新接口，被打包到java.lang包中。
AutoCloseable接口对JDK7新添加的带资源的try语句提供了支持，这种try语句可以自动执行资源关闭过程。只有实现了AutoCloseable接口的类的对象才可以由带资源的try语句进行管理。AutoCloseable接口只定义了close()方法：
```
```java
void
```
```java
close()
```
```java
throws
```
```java
Exception
```
```
这个方法关闭调用对象，释放可能占用的所有资源。在带资源的try语句的末尾，会自动调用该方法，因此消除了显式调用close()方法的需要。
Closeable接口也定义了close()方法。实现了Closeable接口的类的对象可以被关闭。从JDK7开始，**Closeable扩展了AutoCloseable**。因此，在JDK7中，所有实现了Closeable接口的类也都实现了AutoCloseable接口。
实现了**Flushable**接口的类的对象，可以强制将缓存的输出写入到与对象关联的流中。该接口定义了flush()方法，如下所示：
```
```java
void
```
```java
flush()
```
```java
throws
```
```java
IOException
```
```
刷新流通常会导致缓存的输出被物理地写入到底层设备中。写入流的所有I/O类都实现了Flushable接口。
注：关于带资源的try语句的3个关键点：
- 
由带资源的try语句管理的资源必须是实现了AutoCloseable接口的类的对象。
- 
在try代码中声明的资源被隐式声明为fianl。
- 
通过使用分号分隔每个声明可以管理多个资源。
此外请记住，所声明资源的作用域被限制在带资源的try语句中。带资源的try语句的主要优点是：当try代码块结束时，资源（在此时流）会被自动关闭。因此，不太可能会忘记关闭流。使有带资源的try语句，通常可以使源代码更短，更清晰，更容易维护。如例：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package demo;
import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
/**
 * AutoCloseable接口，表示一种不再使用时需要关闭的资源。这个接口下只有一个方法，close()。这个方法在try-with-
 * resource语法下会被自动调用，支持抛出Exception，当然它也鼓励抛出更详细的异常。close()建议不要抛出线程中断的
 * InterruptedException。对这个接口的实现，规范强烈建议close()是幂等的，也就是说多次调用close()方法和一次调用的结
 * 果是一样的。
 * JDK1.7 新特性
 * 
 */
public class InputStreamReaderTest {
    public static void main(String[] args) {
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(
                new FileInputStream("src\\fuhd.txt"), "UTF8"), 1024)) {
            System.out.println(reader.readLine());
        } catch (Exception e) {
            e.printStackTrace();
        }
        //示例，声明自己的两个资源类，实现AutoCloseable接口。
        try (MyResource myResource = new MyResource();
                MyResource2 myResource2 = new MyResource2()) {
            myResource.readResource();
            myResource2.readResource();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
class MyResource implements AutoCloseable {
    @Override
    public void close() throws Exception {
        System.out.println("close resource");
    }
    public void readResource() {
        System.out.println("read resource");
    }
}
class MyResource2 implements AutoCloseable {
    @Override
    public void close() throws Exception {
        System.out.println("close resource2");
    }
    public void readResource() {
        System.out.println("read resource2");
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
 输出
```
热度...........................。
read resource
read resource2
close resource2
close resource
```
