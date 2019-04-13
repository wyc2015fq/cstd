
# 程序猿浅谈JAVA的序列化 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月13日 21:57:54[Jlins](https://me.csdn.net/dyllove98)阅读数：1087


序列化，是我们在开发过程中经常会碰到的问题。经常会看到一个类(Class)实现Serializable接口。这里就是实现序列化。
今天小编就来聊聊什么是序列化。
Java是面向对象编程，对象保存了一个类的完整的数据。但是Java有自己专门的垃圾回收机制，当一个对象不再被使用，就会被回收，释放空间。如果我们想长久地保存一个对象的数据该怎么办呢？这个时候就用到序列化了。
序列化就是将对象的状态信息转换为可以存储或者传输的过程。在序列化期间，对象将其当前的状态写入到临时或者持久性存储区。以后可以通过从存储区读取或者反序列化对象的状态，重写创建该对象。通过序列化，不仅可以持久化存储一个对象，也方便不同的程序之间共享对象的数据，同时程序维护起来也更方便。
如何实现序列化呢？
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|import|java.io.Serializable;
|public|class|Person|implements|Serializable{
|private|static|final|long|serialVersionUID = 1L;
|public|String name;
|public|int|age;
|public|double|money;
|private|static|final|long|serialVersionUID = 1L;
|public|Person(String name,|int|age,|double|money) {
|this|.name = name;
|this|.age = age;
|this|.money = money;
|}
|@Override
|public|String toString() {
|return|"name is："|+name+|" , age is："|+age+|" , money is："|+money;
|}
|}
|

这里我们发现一个类实现一个接口却没有要实现的方法。这是因为，类通过实现Serializable接口是来启动序列化功能。序列化接口没有方法和字段，仅仅用于标识可序列化。这里需要强调的是，可序列化的子类都是可序列化的。序列化是保存对象的数据，如果要获取对象的数据就要反序列化。
序列化和反序列化是通过writeObejct()和readObject()两个方法实现的。
writeObject()方法负责写入特点类的对象状态。通过调用 out.defaultWriteObject 可以调用保存 Object
 的字段的默认机制，该方法本身不需要涉及属于其超类或子类的状态。
readObject()方法负责从流中读取并恢复类字段。它可以调用 in.defaultReadObject 来调用默认机制，以恢复对象的非静态和非瞬态字段。
一个java文件序列化一个对象
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|import|java.io.FileNotFoundException;
|import|java.io.FileOutputStream;
|import|java.io.IOException;
|import|java.io.ObjectOutputStream;
|public|class|DemoText01 {
|public|static|void|main(String[] args) {
|Person person =|new|Person(|"joinName"|,|20|,|100.89|);
|//序列化对象 -->只有可序列化的类，对象才能序列化
|try|{
|ObjectOutputStream oos =|new|ObjectOutputStream(|new|FileOutputStream(|"d:/save.txt"|));
|oos.writeObject(person);
|System.out.println(|"保存对象"|);
|oos.close();
|}|catch|(FileNotFoundException e) {
|e.printStackTrace();
|}|catch|(IOException e) {
|e.printStackTrace();
|}
|}
|}
|

另一个java文件反序列化一个对象
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|import|java.io.FileInputStream;
|import|java.io.FileNotFoundException;
|import|java.io.IOException;
|import|java.io.ObjectInputStream;
|public|class|DemoText02 {
|public|static|void|main(String[] args) {
|try|{
|ObjectInputStream ois =|new|ObjectInputStream(|new|FileInputStream(|"d:/save.txt"|));
|Person person = (Person) ois.readObject();
|System.out.println(person);
|ois.close();
|}|catch|(FileNotFoundException e) {
|e.printStackTrace();
|}|catch|(IOException e) {
|e.printStackTrace();
|}|catch|(ClassNotFoundException e) {
|// TODO Auto-generated catch block
|e.printStackTrace();
|}
|}
|}
|

需要强调生产的save文件类型可以是任意类型。
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

