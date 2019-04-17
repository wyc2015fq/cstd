# Java —— 对象流 - Alex_McAvoy的博客 - CSDN博客





2018年10月31日 20:27:32[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：48
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【概述】

程序运行时，有时需要保存数据，对于基本数据类型，可以直接保存到文件中，程序下次启动时，直接读取数据初始化程序即可。

但对于复杂的对象类型，若需要永久保存，采用上述的方法较难以实现，需要将对象中的不同属性分解成基本数据类型，分别保存在文件中，再次运行时建立新的对象，读取有关数据，分别进行初始化。

使用对象流可以实现对象序列化，直接存取对象，其中，将对象存入一个流称为序列化，从一个流中读取一个对象称为反序列化。

因此，使用对象序列化功能可以十分方便地从输入流中读取对象、将对象写入输出流。

# 【ObjectInput 接口与 ObjectOutput 接口】

ObjectInput 接口与 ObjectOutput 接口分别继承了 DataInput 接口与 DataOutput 接口，提供了对基本数据类型和对象序列化的方法，其方法的实现在 ObjectInputStream 类与 ObjectOutputStream 类中。

如果想要某个对象支持序列化机制，则必须让其类是可序列化的，因此必须实现 Serializable 接口或 Externalizable 接口之一。

对象序列化方法：writeObject(obj)：将对象写入输出流，可以是文件输出流、网络输出流、其他数据输出流等

对象反序列化方法：Object obj=readObject()：该方法获取的序列化对象是Object类型的，必须通过强行类型转换后才能使用

# 【ObjectInputStream 类与 ObjectOutputStream 类】

ObjectInputStream 类与 ObjectOutputStream 类是存储和读取对象的数据流，其强大之处是可以将 Java 对象写入数据源中，也能将对象从数据源中还原出来。

ObjectInputStream 类构造方法：ObjectInputStream(InputStream in)

ObjectOutputStream 类构造方法：ObjectOutputStream(OutputStream out)

```java
//读取
FileInputString fis=new FileInputStream("Example.txt");
ObjectInputStream ois=new ObjectInputStream(fis);
ois.readObject();
ois.close();

//写入
FileOutputStream fos=new FileOutputStream("Example.txt");
ObjectOutputStream obs=new ObjectOutputStream(fos);
obs.writeObject("Example");
obs.close();
```





