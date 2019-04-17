# Java —— 打印流与过滤器流（数据流） - Alex_McAvoy的博客 - CSDN博客





2018年10月31日 20:24:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：169
所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【打印流】

打印流是输出信息最方便的类，包含字节打印流 PrintStream 和字符打印流 PrintWriter

打印流提供了非常方便的打印功能，可以打印任何类型的数据信息，例如：小数，整数，字符串。

其提供了两种输出方法：
- print(Object obj)：不换行打印 Object 类型数据
- println(Object obj)：换行打印 Object 类型数据

## 1.PrintStream 类

PrintStream 类是字节打印输出流，它可以直接输出各种类型的数据，其构造方法为：
- PrintStream(OutPutStream out)：该方法使用 OutputStream 类对象创建实例对象

## 2.PrintWriter 类

PrintWriter 类是字符打印输出流，它可以自己把内构类型以字符形式传送到相应的输出流中，可以以文本的形式浏览，其构造方法为：
- PrintWriter(Writer out)：该方法使用 Writer 类对象创建实例对象
- PrintWriter(OutPutStream out)：该方法使用 OutputStream 类对象创建实例对象

## 3.实例

```java
File file=new File("D:\\Test\\example");
FileOutputStream fos=new FileOutputStream(file);
PrintStream ps=new PrintStream(fos);
System.SetOut(ps);//将标准输出(控制台输出)改为文件输出
System.out.println("Hello World!");
ps.close();
```

# 【过滤器流】

基本输入流提供读取方法，只能读取字节或字符，有时为了某种目的需要过滤字节或字符的数据流，而过滤器流能够读取整数、浮点数等基本数据类型。

常用的过滤器流分为 DataInputStream 类与 DataOutputStream 类，他们分别实现了 DataInput 接口和 DataOutput 接口，接口中定义了独立于具体机器的带有格式的读写操作，从而实现对Java不同基本类型的读写。

## 1.DataInputStream 类

DataInputStream 类是套接在 InputStream 类上的，用于读取数据。

常用方法如下：
- readByte()：从输入流中读取1个字节，转为 Byte 型
- readShort()：从输入流中读取2个字节，转为 Short 型
- readInt()：从输入流中读取4个字节，转为 Int 型
- readLong()：从输入流中读取8个字节，转为 Long 型
- readFloat()：从输入流中读取4个字节，转为 Float 型
- readDouble()：从输入流中读取8个字节，转为 Double 型
- readBoolean()：从输入流中读取4个字节，转为 Boolean 型
- readChar()：从输入流中读取2个字节，转为 Char 型
- readUTF()：从输入流中读取数据，以 UTF 编码解码，转为 String 类型

## 2.DataOutputStream 类

DataOutputStream 类是套接在 OutputStream 类上的，用于写入数据。

常用方法如下：
- writeByte()：读取1个字节，转为 Byte 型，写入输出流
- writeShort()：读取2个字节，转为 Short 型，写入输出流
- writeInt()：读取4个字节，转为 Int 型，写入输出流
- writeLong()：读取8个字节，转为 Long 型，写入输出流
- writeFloat()：读取4个字节，转为 Float 型，写入输出流
- writeDouble()：读取8个字节，转为 Double 型，写入输出流
- writeBoolean()：读取4个字节，转为 Boolean 型，写入输出流
- writeChar()：读取2个字节，转为 Char 型，写入输出流
- writeUTF()：读取数据，以 UTF 编码解码，转为 String 类型，写入输出流

## 3.实例

```java
FileOutputStream fos=new FileOutputStream("test.txt");
DataOutputStream dos=new DataOutputStream(fos);
dos.writeUTF("Hello World!");
dos.writeBoolean(true);
dos.writeLong(111111111111);

DataInputStream dis=new DataInputStream("text.txt");
String str=dis.readUTF();
System.out.println(str);
boolean b=dis.readBoolean();
System.out.println(b);
long l=dis.readLong();
System.out.println(l);
```





