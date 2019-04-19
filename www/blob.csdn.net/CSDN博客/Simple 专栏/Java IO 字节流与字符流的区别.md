# Java IO 字节流与字符流的区别 - Simple 专栏 - CSDN博客
2012年01月05日 14:53:42[Simple_Zz](https://me.csdn.net/love284969214)阅读数：278标签：[io																[java																[java面试](https://so.csdn.net/so/search/s.do?q=java面试&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=io&t=blog)
个人分类：[Java](https://blog.csdn.net/love284969214/article/category/1235868)
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
### **IO流主要用于硬盘、内存、键盘等处理设备上得数据操作。**
- 
**根据实现功能不同：节点流和处理流。**
- 
**根据数据流的方向：输入流和输出流。**
- 
**根据处理数据单位：字节流和字符流。**
### 主要结构图
![](https://img-blog.csdn.net/20160622190829830?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
### **字符流和字节流的主要区别：**
- 字节流读取的时候，读到一个字节就返回一个字节。
- 字符流使用了字节流，读到一个或多个字节时。先去查指定的编码表，将查到的字符返回。
- 字节流可以处理所有类型数据，如：图片，MP3，AVI视频文件。
- 字符流只能处理字符数据。只要是处理纯文本数据，就要优先考虑使用字符流，除此之外都用字节流。
### 节点流和处理流的概述：
按照流是否直接与特定的地方(如磁盘、内存、设备等)相连，分为节点流和处理流两类。
- 节点流：可以从或向一个特定的地点或者节点读写数据。
- 处理流：是对一个已存在的流的连接和封装，通过所封装的流的功能调用实现数据读写，处理流的构造方法总是要带一个其他流对象作为参数，一个流对象进过其他流的多次包装，叫做流的链接。
### IO流主要可以分为节点流和处理流两大类。
### **    一、节点流**
> 
该类型可以从或向一个特定的地点或者节点读写数据。主要类型如下：
|类型|字符流|字节流|
|----|----|----|
|基类|ReaderWriter|InputStreamOutPutStream|
|File(文件)|FileReaderFileWriter|FileInputStreamFileOutputSream|
|Memory Array(数组)|CharArrayReaderCharArrayWriter|ByteArrayInputStreamByteArrayOutputSream|
|Memory String(字符串)|StringReaderStringWriter|-|
|Pipe(管道)|PipedReaderPipedWriter|PipedInputSreamPipedOutputSream|
### **    二、处理流**
> 
    该类型是对一个已存在的流的连接和封装，通过所封装的流的功能调用实现数据读写，处理流的构造方法总是要带一个其他流对象作为参数，一个流对象进过其他流的多次包装，叫做流的链接。主要可以分为以下几种：
**1.缓冲流  BufferedInPutStream  BufferedOutPutStream  BufferedWriter  BufferedReader**
该类型的流有一个特有的方法：readLine()；一次读一行，到行标记时，将行标记之前的字符数据作为字符串返回，当读到末尾时，返回null，其原理还是与缓冲区关联的流对象的read方法，只不过每一次读取到一个字符，先不进行具体操作，先进行临时储存，当读取到回车标记时，将临时容器中储存的数据一次性返回。    
```java
// 写入缓冲区对象 
BufferedWriter bufw=new BufferedWriter(new FileWriter("buf.txt"));
// 读取缓冲区对象
BufferedReader bufr=new BufferedReader(new FileReader("buf.txt"));
```
**2.转换流  InputStreamReader  OutputStreamWriter**
      该类型时字节流和字符流之间的桥梁，该流对象中可以对读取到的字节数据进行指定编码的编码转换。
      构造函数主要有：    
`InputStreamReader(InputStream); //通过构造函数初始化，使用的是本系统默认的编码表GBK。`
`InputStreamWriter(InputStream,String charSet); //通过该构造函数初始化，可以指定编码表。`
`OutputStreamWriter(OutputStream); //通过该构造函数初始化，使用的是本系统默认的编码表GBK。`
`OutputStreamwriter(OutputStream,String charSet); //通过该构造函数初始化，可以指定编码表。`
**注意：**在使用FileReader操作文本数据时，该对象使用的时默认的编码表，即FileReader fr=new FileReader(“a.txt”);      与InputStreamReader isr=new InputStreamReader(new FileInputStream("a.txt"));  的意义相同。如果要使用指定表编码表时，必须使用转换流，即如果a.txt中的文件中的字符数据是通过utf-8的形式编码，那么在读取时，就必须指定编码表，那么转换流时必须的。即：InputStreamReader isr=new InputStreamReader(new FileInputStream("a.txt"),utf-8);
**3.数据流  DataInputStream  DataOutputStream**
          该数据流可以方便地对一些基本类型数据进行直接的存储和读取，不需要再进一步进行转换，通常只要操作基本数据类型的数据，就需要通过DataStream进行包装。
          构造方法：        
`DataInputStreamReader（InputStream）；`
`DataInputStreamWriter（OutputStream）；`
          方法举例：          
`int readInt()；//一次读取四个字节，并将其转成int值`
`writeInt(int)；//一次写入四个字节，注意和write(int)不同，`
`          //write(int)只将该整数的最低一个8位写入，剩余三个8为丢失`
`hort readShort();`
`writeShort(short);`
`String readUTF();//按照utf-8修改版读取字符，注意，它只能读writeUTF()写入的字符数据。`
`writeUTF(String);//按照utf-8修改版将字符数据进行存储，只能通过readUTF读取。`
**注意：**在使用数据流读/存数据的时候，需要有一定的顺序，即某个类型的数据先写入就必须先读出，服从先进先出的原则。
**4.打印流  PrintStream  PrintWriter**
       PrintStream是一个字节打印流，System.out对应的类型就是PrintStream，它的构造函数可以接受三种数据类型的值  字符串路径、File对象、OutputStream。 
       PrintStream是一个字符打印流，构造函数可以接受四种类型的值 字符串路径、File对象、OutputStream、Writer，  对于  字符串路径、File对象，可以指定编码表，也就是字符集，对于OutputStream、Writer类型的数据，可以指定自动刷新，当该自动刷新为True时，只有3个方法可以用：println,printf,format。
**5.对象流  ObjectInputStream  ObjectOutputStream**
        该类型的流可以把类作为一个整体进行存取，主要方法有：
        Object readObject();该方法抛出异常：ClassNotFountException。
        void writeObject(Object)：被写入的对象必须实现一个接口：Serializable，否则就会抛出：NotSerializableException
