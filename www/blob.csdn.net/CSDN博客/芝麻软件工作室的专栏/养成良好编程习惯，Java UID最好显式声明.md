
# 养成良好编程习惯，Java UID最好显式声明 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月31日 06:39:04[seven-soft](https://me.csdn.net/softn)阅读数：1468


我们编写一个实现了Serializable 接口（序列化标志接口）的类， Eclipse 马上就会给一个黄色警告：需要增加一个Serial Version ID。为什么要增加？它是怎么计算出来的？有什么用？本章就来解释该问题。
类实现Serializable 接口的目的是为了可持久化，比如网络传输或本地存储，为系统的分布和异构部署提供先决支持条件。若没有序列化，现在我们熟悉的远程调用、对象数据库都不可能存在，我们来看一个简单的序列化类：
public class Person implements Serializable{
private String name;
/*name 属性的getter/setter 方法省略*/
}
这是一个简单JavaBean，实现了Serializable 接口，可以在网络上传输，也可以本地存储然后读取。这里我们以Java 消息服务（Java Message Service）方式传递该对象（即通过网络传递一个对象），定义在消息队列中的数据类型为ObjectMessage，首先定义一个消息的生产者（Producer），代码如下：
public class Producer {
public static void main(String[] args) throws Exception {
Person person = new Person();
person.setName(" 混世魔王");
// 序列化，保存到磁盘上
SerializationUtils.writeObject(person);
}
}
这里引入了一个工具类SerializationUtils，其作用是对一个类进行序列化和反序列化，并存储到硬盘上（模拟网络传输），其代码如下：
public class SerializationUtils {
private static String FILE_NAME = "c:/obj.bin";
// 序列化
public static void writeObject(Serializable s) {
try {
Obje ctOutputStream oos = new ObjectOutputStream(new FileOutputStream(FILE_NAME));
oos.writeObject(s);
oos.close();
} catch (Exception e) {
e.printStackTrace();
}
}
public static Object readObject(){
Object obj=null;
// 反序列化
try {
ObjectInput  input = new  ObjectInputStream(new FileInputStream(FILE_NAME));
obj = input.readObject();
input.close();
} catch (Exception e) {
e.printStackTrace();
}
return obj;
}
}
通过对象序列化过程，把一个对象从内存块转化为可传输的数据流，然后通过网络发送到消息消费者（Consumer）那里，并进行反序列化，生成实例对象，代码如下：
public class Consumer {
public static void main(String[] args) throws Exception {
// 反序列化
Person p = (Person) SerializationUtils.readObject();
System.out.println("name="+p.getName());
}
}
这是一个反序列化过程，也就是对象数据流转换为一个实例对象的过程，其运行后的输出结果为：混世魔王。这太easy 了，是的，这就是序列化和反序列化典型的demo。但此处隐藏着一个问题：如果消息的生产者和消息的消费者所参考的类（Person
 类）有差异，会出现何种神奇事件？比如：消息生产者中的Person 类增加了一个年龄属性，而消费者没有增加该属性。为啥没有增加？！因为这是个分布式部署的应用，你甚至都不知道这个应用部署在何处，特别是通过广播（broadcast）方式发送消息的情况，漏掉一两个订阅者也是很正常的。
在这种序列化和反序列化的类不一致的情形下，反序列化时会报一个InvalidClassException异常，原因是序列化和反序列化所对应的类版本发生了变化，JVM 不能把数据流转换为实例对象。接着刨根问底：JVM 是根据什么来判断一个类版本的呢？
好问题，通过SerialVersionUID，也叫做流标识符（Stream Unique Identifier），即类的版本定义的，它可以显式声明也可以隐式声明。显式声明格式如下：
private static final long serialVersionUID = XXXXXL;
而隐式声明则是我不声明，你编译器在编译的时候帮我生成。生成的依据是通过包名、类名、继承关系、非私有的方法和属性，以及参数、返回值等诸多因子计算得出的，极度复杂，基本上计算出来的这个值是唯一的。
serialVersionUID 如何生成已经说明了，我们再来看看serialVersionUID 的作用。JVM在反序列化时，会比较数据流中的serialVersionUID 与类的serialVersionUID 是否相同，如果相同，则认为类没有发生改变，可以把数据流load
 为实例对象；如果不相同，对不起，我JVM 不干了，抛个异常InvalidClassException 给你瞧瞧。这是一个非常好的校验机制，可以保证一个对象即使在网络或磁盘中“滚过”一次，仍能做到“出淤泥而不染”，完美地实现类的一致性。
但是，有时候我们需要一点特例场景，例如：我的类改变不大，JVM 是否可以把我以前的对象反序列化过来？就是依靠显式声明serialVersionUID，向JVM 撒谎说“我的类版本没有变更”，如此，我们编写的类就实现了向上兼容。我们修改一下上面的Person
 类，代码如下：
public class Person implements Serializable{
private static final long serialVersionUID = 55799L;
/* 其他保持不变*/
}
刚开始生产者和消费者持有的Person 类版本一致，都是V1.0，某天生产者的Person 类版本变更了，增加了一个“年龄”属性，升级为V2.0，而由于种种原因（比如程序员疏忽、升级时间窗口不同等）消费端的Person 还保持为V1.0
 版本，代码如下：
public class Person implements Serializable{
private static final long serialVersionUID = 5799L;
private int age;
/*age、name 的getter/setter 方法省略*/
}
此时虽然生产者和消费者对应的类版本不同，但是显式声明的serialVersionUID 相同，反序列化也是可以运行的，所带来的业务问题就是消费端不能读取到新增的业务属性（age属性）而已。
通过此例，我们的反序列化实现了版本向上兼容的功能，使用V1.0 版本的应用访问了一个V2.0 版本的对象，这无疑提高了代码的健壮性。我们在编写序列化类代码时，随手加上serialVersionUID 字段，也不会给我们带来太多的工作量，但它却可以在关键时候发挥异乎寻常的作用。
注意　显式声明serialVersionUID 可以避免对象不一致，但尽量不要以这种方式向JVM“撒谎”。

