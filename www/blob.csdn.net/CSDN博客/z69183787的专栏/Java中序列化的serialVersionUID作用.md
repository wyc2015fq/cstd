# Java中序列化的serialVersionUID作用 - z69183787的专栏 - CSDN博客
2015年07月10日 19:44:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4633
Java序列化是将一个对象编码成一个字节流，反序列化将字节流编码转换成一个对象。 序列化是Java中实现持久化存储的一种方法；为数据传输提供了线路级对象表示法。
Java的序列化机制是通过在运行时判断类的serialVersionUID来验证版本一致性的。在进行反序列化时，JVM会把传来的字节流中的serialVersionUID与本地相应实体（类）的serialVersionUID进行比较，如果相同就认为是一致的，可以进行反序列化，否则就会出现序列化版本不一致的异常。 
Eclipse中The serializable class XXXXXX  does not declare a static final serialVersionUID field
 of type long出现这样的警告处理办法。 
当采用程序的Add default Serial version ID修复时，Eclipse会加上：private static final long serialVersionUID
 = 1L; 
当采用程序的Add generated Serial version ID修复时，Eclipse会加上：private static final long serialVersionUID
 = xxxxL; 
其实这个问题出现的具体原因是和序列化中的这个serialVersionUID有关。 serialVersionUID
 用来表明类的不同版本间的兼容性。有两种生成方式： 一个是默认的1L；另一种是根据类名、接口名、成员方法及属性等来生成一个64位的哈希字段
 。
在JDK中，可以利用JDK的bin目录下的serialver.exe工具产生这个serialVersionUID 的值，对于Test.class，执行命令： 
serialver Test   这时JVM（java虚拟机）会生成一个哈希字段。 
对比一下这个哈希字段的值与方法2中生成的字段值是一样的，可见，在CMD中使用serialver指令就是根据类名、接口名、成员方法及属性等来生成哈希字段的。 
java类中为什么需要重载 serialVersionUID 属性。 
当两个进程在进行远程通信时，彼此可以发送各种类型的数据。无论是何种类型的数据，都会以二进制序列的形式在网络上传送。发送方需要把这个Java对象转换为字节序列，才能在网络上传送；接收方则需要把字节序列再恢复为Java对象。 把Java对象转换为字节序列的过程称为对象的序列化，把字节序列恢复为Java对象的过程称为对象的反序列化。 
　　对象的序列化主要有两种用途： 
　　1） 把对象的字节序列永久地保存到硬盘上，通常存放在一个文件中； 
　　2） 在网络上传送对象的字节序列。 
java.io.ObjectOutputStream代表对象输出流，它的writeObject(Object obj)方法可对参数指定的obj对象进行序列化，把得到的字节序列写到一个目标输出流中。 java.io.ObjectInputStream代表对象输入流，它的readObject()方法从一个源输入流中读取字节序列，再把它们反序列化为一个对象，并将其返回。 只有实现了Serializable或Externalizable接口的类的对象才能被序列化。Externalizable接口继承自Serializable接口，实现Externalizable接口的类完全由自身来控制序列化的行为，而仅实现Serializable接口的类可以采用默认的序列化方式
 。 凡是实现Serializable接口的类都有一个表示序列化版本标识符的静态变量：private static final long serialVersionUID; 类的serialVersionUID的默认值完全依赖于Java编译器的实现，对于同一个类，用不同的Java编译器编译，有可能会导致不同的serialVersionUID。显式地定义serialVersionUID有两种用途： 
　　1）在某些场合，希望类的不同版本对序列化兼容，因此需要确保类的不同版本具有相同的serialVersionUID；在某些场合，不希望类的不同版本对序列化兼容，因此需要确保类的不同版本具有不同的serialVersionUID。 
　　2）当你序列化了一个类实例后，希望更改一个字段或添加一个字段，不设置serialVersionUID，所做的任何更改都将导致无法反序化旧有实例，并在反序列化时抛出一个异常。如果你添加了serialVersionUID，在反序列旧有实例时，新添加或更改的字段值将设为初始化值（对象为null，基本类型为相应的初始默认值），字段被删除将不设置。 
序列化算法一般会按步骤：
- 将对象实例相关的类元数据输出。
- 递归地输出类的超类描述直到不再有超类。
- 类元数据完了以后，开始从最顶层的超类开始输出对象实例的实际数据值。
- 从上至下递归输出实例的数据
