# Java范型 - weixin_33985507的博客 - CSDN博客
2017年04月20日 11:15:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
定义类的时候，不为类的属性或方法的参数设置具体的类型，只使用一个标记表示，类使用的时候才动态地绑定一种数据类型，这就是**范型**。
如果应用使用了范型的类时没有指定类型，则会使用默认的类型Object。
##### 1.范型类
在类上使用范型就是范型类。一般用ClassName<T>，这里的T只是一种习惯，并不是非得T。
用代码演示下范型类的效果，创建一个范型类Message<T>，分别在两个测试方法test1、test2中指定范型类型Integer、String，都能正常实例化对象并设置指定的范型类型的属性值，这就是范型的作用。
```java
package demo.pattern;
public class Test {
    @org.junit.Test
    public void test1(){
        Message<Integer> message = new Message<Integer>();
        message.setInfo(123);
        System.out.println(message.getInfo());
    }
    @org.junit.Test
    public void test2(){
        Message<String> message = new Message<String>();
        message.setInfo("you have a message");
        System.out.println(message.getInfo());
    }
}
class Message<T>{
    private T info;
    public T getInfo() {
        return info;
    }
    public void setInfo(T info) {
        this.info = info;
    }
}
```
##### 2.通配符 ?
接下来对上面的例子稍作改动，在test1方法中调用run1方法（参数Message不指定任何类型），执行后发现原来指定的Integer类型的属性被重新赋予了String类型的值。为了避免这种不受控制的情况，范型提供了通配符？。接着看run2方法，指定的参数类型Message<?>，表示接收任意类型的范型，但此时参数引用（也就是message）不能被修改（无法调用setInfo方法），只能获取它的信息（getInfo方法）。这就是通配符的作用。
```java
package demo.pattern;
public class Test {
    public void run1(Message message){
        message.setInfo("Integer被替换成了String");
    }
    public void run2(Message<?> message){
        System.out.println("使用通配符?接收范型类："+message.getInfo());
    }
    @org.junit.Test
    public void test1(){
        Message<Integer> message = new Message<Integer>();
        message.setInfo(123);
        run1(message);
        System.out.println(message.getInfo());
    }
    @org.junit.Test
    public void test2(){
        Message<String> message = new Message<String>();
        message.setInfo("you have a message");
        run2(message);
        System.out.println(message.getInfo());
    }
}
class Message<T>{
    private T info;
    public T getInfo() {
        return info;
    }
    public void setInfo(T info) {
        this.info = info;
    }
}
```
此外，通配符?有2种扩展形式如下
**? extends** ：范型可以使用的只能是此类型及其子类，用于方法返回值
**? super** ：范型可以使用的只能是此类型及其父类，用于方法的参数
- **通配符总结**
- 无限定通配符：只读
- 子类型通配符：set方法受限，只读
- 超类型通配符：get方法受限，不可读(Object除外)，只可写
##### 3.范型接口
在接口上使用范型就是范型接口。
使用范型接口时，其子类可以指定范型的具体类型，也可以不指定范型类型（子类也是范型类）。
```
package demo.pattern;
public class TestInterface {
    @org.junit.Test
    public void test(){
        try {
            MyInterfaceImpl1.class.newInstance().myMethod("1");
            MyInterfaceImpl2.class.newInstance().myMethod("2");
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
    }
}
interface MyInterface<T>{
    void myMethod(T t);
}
class MyInterfaceImpl1<T> implements MyInterface<T>{
    public void myMethod(T t) {
        System.out.println("1.实现范型接口不指定范型类型");
    }
}
class MyInterfaceImpl2<String> implements MyInterface<String>{
    public void myMethod(String s) {
        System.out.println("2.实现范型接口指定范型类型");
    }
}
```
##### 4.范型方法
在方法上使用范型，这个方法就是范型方法。范型方法不一定需要定义在范型类中。
其用法为在返回类型前加<T>，当然返回类型也可以是范型（T）.
```
package demo.pattern;
public class TestMethod {
    @org.junit.Test
    public void test(){
        run("String类型");
        run(12345);
        run(true);
    }
    public <T> void run(T t){
        System.out.println(t.toString());
    }
}
```
##### 5.总结
范型的本质思想：类的属性、方法的参数，都可以由用户在使用时动态设置。
