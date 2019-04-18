# 【java】java高级开发之泛型 - csdn_baotai的博客 - CSDN博客

2018年07月27日 20:10:11[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：29


**Java高级开发之泛型**

**目录**

[Java高级开发之泛型](#Java%E9%AB%98%E7%BA%A7%E5%BC%80%E5%8F%91%E4%B9%8B%E6%B3%9B%E5%9E%8B)

泛型问题引出

泛型类的基本使用

泛型方法

通配符(重点)

泛型接口

类型擦除

泛型问题引出

假设需要你定义一个描述坐标的程序类Point，需要提供两个属性x、y。对于这两个属性的内容可能有如下选择：

1. x = 10、y = 20 ；

2. x = 10.1、y = 20.1 ；

3. x = 东经80度、y = 北纬20度

那么现在首先要解决的问题就是Point类中的x、y的属性类型问题，此时需要保存的有int、double、String，所以

在java中只有一种类型可以保存所有类型：Object型

范例：定义Point类

```java
class Point {
private Object x ;
private Object y ;
public Object getX() {
return x;
}
public void setX(Object x) {
this.x = x;
}
public Object getY() {
return y;
}
public void setY(Object y) {
this.y = y;
}
}
```

 范例：设置整型坐标

```
// 设置数据
Point p = new Point() ;
p.setX(10); // 自动装箱并且向上转型为Object
p.setY(20);
// 取出数据
int x = (Integer) p.getX() ; // 强制向下转型为Integer并且自动拆箱
int y = (Integer) p.getY() ;
System.out.println("x = " +x+",y = "+y);
```

范例：设置字符串

```java
// 设置数据
Point p = new Point() ;
p.setX("东经80度");
p.setY("北纬20度");
// 取出数据
String x = (String) p.getX() ;
String y = (String) p.getY() ;
System.out.println("x = " +x+",y = "+y);
```

观察问题：

```java
// 设置数据
Point p = new Point() ;
p.setX(10.2);
p.setY("北纬20度");
// 取出数据
String x = (String) p.getX() ;
String y = (String) p.getY() ;
System.out.println("x = " +x+",y = "+y);
```

这个时候由于设置方的错误，将坐标内容设置成了double与String，但是接收方不知道，于是在执行时就会出现

ClassCastException。

ClassCastException指的是两个没有关系的对象进行强转出现的异常。

这个时候语法不会对其做任何限制，但执行的时候出现了程序错误，所以得出结论：向下转型是不安全的操作，会

带来隐患。

泛型类的基本使用

范例：泛型类引入多个类型参数以及使用

```java
class MyClass<T,E> {
  T value1;
  E value2;
}
public class Test {
  public static void main(String[] args) {
    MyClass<String,Integer> myClass1 = new MyClass<String,Integer>();
  }
}
```

注意：泛型只能接受类，所有的基本数据类型必须使用包装类！

范例：使用泛型定义Point类

```java
class Point <T> { // T表示参数，是一个占位的标记；如果有多个泛型就继续在后面追加
private T x ;
private T y ;
public T getX() {
return x;
}
public void setX(T x) {
this.x = x;
}
public T getY() {
return y;
}
public void setY(T y) {
this.y = y;
}
}
public class TestDemo {
public static void main(String[] args) {
// 设置数据
Point<String> p = new Point<String>() ; // JDK1.5的语法
p.setX("东经80度");
p.setY("北纬20度");
// 取出数据
String x = p.getX() ; // 避免了向下转型
String y = p.getY() ;
System.out.println("x = " +x+",y = "+y);
}
}
```

泛型方法

泛型方法定义

```java
class MyClass{
public <T> void testMethod(T t) {
System.out.println(t);
}
}
```

使用类型参数做返回值的泛型方法

```java
class MyClass{
public <T> T testMethod(T t) {
return t;
}
}
```

泛型方法与泛型类共存

```java
class MyClass<T>{
public void testMethod1(T t) {
System.out.println(t);
}
public <T> T testMethod2(T t) {
return t;
}
}
public class Test {
public static void main(String[] args) {
class MyClass<T>{
public void testMethod1(T t) {
System.out.println(t);
}
public <T> T testMethod2(T t) {
return t;
}
}
public class Test {
public static void main(String[] args) {
```

上面代码中，MyClass <T> 是泛型类，testMethod1 是泛型类中的普通方法，而 testMethod2 是一个泛型方法。

而泛型类中的类型参数与泛型方法中的类型参数是没有相应的联系的，泛型方法始终以自己定义的类型参数为准。

泛型类的实际类型参数是 String，而传递给泛型方法的类型参数是 Integer，两者不相干。

但是，为了避免混淆，如果在一个泛型类中存在泛型方法，那么两者的类型参数最好不要同名。

比如，MyClass <T> 代码可以更改为这样：

```java
class MyClass<T>{
public void testMethod1(T t) {
System.out.println(t);
}
public <E> E testMethod2(E e) {
return e;
}
}
```

通配符(重点)

在程序类中追加了泛型的定义后，避免了ClassCastException的问题，但是又会产生新的情况：参数的统一问题。

使用通配符

```java
class Message<T> {
private T message ;
public T getMessage() {
return message;
}
public void setMessage(T message) {
this.message = message;
}
}
public class TestDemo {
public static void main(String[] args) {
Message<String> message = new Message() ;
message.setMessage("比特科技欢迎您");
fun(message);
}
public static void fun(Message<String> temp){
System.out.println(temp.getMessage());
}
}
public class TestDemo {
public static void main(String[] args) {
Message<Integer> message = new Message() ;
message.setMessage(99);
fun(message); // 出现错误，只能接收String
}
public static void fun(Message<String> temp){
System.out.println(temp.getMessage());
}
}
```

使用通配符

```java
public class TestDemo {
public static void main(String[] args) {
Message<Integer> message = new Message() ;
message.setMessage(55);
fun(message);
}
// 此时使用通配符"?"描述的是它可以接收任意类型，但是由于不确定类型，所以无法修改
public static void fun(Message<?> temp){
//temp.setMessage(100); 无法修改！
System.out.println(temp.getMessage());
}
}
```

在"?"的基础上又产生了两个子通配符：
? extends 类：设置泛型上限：

例如：? extends Number，表示只能够设置Number或其子类，例如：Integer、Double等；
? super 类：设置泛型下限：

例如：? super String，表示只能够设置String及其父类Object。

观察泛型上限

```java
class Message<T extends Number> { // 设置泛型上限
private T message ;
public T getMessage() {
return message;
}
public void setMessage(T message) {
this.message = message;
}
}
public class TestDemo {
public static void main(String[] args) {
Message<Integer> message = new Message() ;
message.setMessage(55);
fun(message);
}
// 此时使用通配符"?"描述的是它可以接收任意类型，但是由于不确定类型，所以无法修改
public static void fun(Message<? extends Number> temp){
//temp.setMessage(100); 仍然无法修改！
System.out.println(temp.getMessage());
}
}
```

设置泛型下限

```java
class Message<T> {
private T message ;
public T getMessage() {
return message;
}
public void setMessage(T message) {
this.message = message;
}
}
public class TestDemo {
public static void main(String[] args) {
Message<String> message = new Message() ;
message.setMessage("Hello World");
fun(message);
}
public static void fun(Message<? super String> temp){
// 此时可以修改！！
temp.setMessage("bit!");
System.out.println(temp.getMessage());
}
}
```

泛型接口

在子类定义时继续使用泛型

```java
interface IMessage<T> { // 在接口上定义了泛型
public void print(T t) ;
}
class MessageImpl<T> implements IMessage<T> {
@Override
public void print(T t) {
System.out.println(t);
}
}
public class TestDemo {
public static void main(String[] args) {
IMessage<String> msg = new MessageImpl() ;
msg.print("Hello World");
}
}
```

在子类实现接口的时候明确给出具体类型

```java
interface IMessage<T> { // 在接口上定义了泛型
public void print(T t) ;
}
class MessageImpl implements IMessage<String> {
@Override
public void print(String t) {
System.out.println(t);
}
}
public class TestDemo {
public static void main(String[] args) {
IMessage<String> msg = new MessageImpl() ;
msg.print("Hello World");
}
}
```

类型擦除

观察类型擦除

```java
class MyClass<T,E>{
private T message;
private E text;
public E getText() {
return text;
}
public void setText(E text) {
this.text = text;
}
public T getMessage() {
return message;
}
public void setMessage(T message) {
this.message = message;
}
public void testMethod1(T t) {
System.out.println(t);
}
}
public class Test {
public static void main(String[] args) {
MyClass<String,Integer> myClass1 = new MyClass<>();
Class cls = myClass1.getClass();
Field[] fields = cls.getDeclaredFields();
for (Field field : fields) {
System.out.println(field.getType());
}
}
}
```

在泛型类被类型擦除的时候，之前泛型类中的类型参数部分如果没有指定上限，如 <T> 则会被转译成普通的

Object 类型，如果指定了上限如 <T extends String> 则类型参数就被替换成类型上限。






