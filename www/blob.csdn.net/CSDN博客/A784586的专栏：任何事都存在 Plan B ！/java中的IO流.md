# java中的IO流 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年04月08日 17:41:40[QuJack](https://me.csdn.net/A784586)阅读数：220标签：[java中的IO流																[IO流																[输入输出流](https://so.csdn.net/so/search/s.do?q=输入输出流&t=blog)
个人分类：[Java](https://blog.csdn.net/A784586/article/category/6556682)





1.分类

按流的方向分：输入流和输出流；

按处理数据单位分：字节流（抽象基类为InPutStream和OutPutStream）和字符流（抽象基类为Reader和Writer）；

按实现功能分：节点流（直接与数据源相连，读入或读出）和处理流（为了更快更方便的读写，套接在节点流上的就是处理流，对一个已存在的流的连接和封装）；




IO设计的设计模式：**装饰者Decorator模式**，好处：**运行时动态地给对象添加一些额外的职责；**




2.IO流的实现机制:输入输出都看做抽象的流，一组有序的字节集合；字节流以字节为单位；8bit,字符流以字符为单位，16bit。只要是处理纯文本数据，就要优先考虑使用字符流，除此之外都用字节流。（中文对应的字节数是两个，在UTF-8码表中是3个字节）。

3. 节点流：


|类型|字符流|字节流|
|----|----|----|
|File(文件)|FileReaderFileWriter|FileInputStreamFileOutputSream|
|Memory Array|CharArrayReaderCharArrayWriter|ByteArrayInputStreamByteArrayOutputSream|
|Memory String|StringReaderStringWriter|-|
|Pipe(管道)|PipedReaderPipedWriter|PipedInputSreamPipedOutputSream|

处理流要多，有如下几种：

1.缓冲流

BufferedWriter bufw=new BufferedWriter(new FileWriter("a.txt")); 

BufferedReader bufr=new BufferedReader(new FileReader("a.txt"));

bufr.readLine();


2.转换流（InputStreamReader/OutputStreamWriter）


从该流中读取到的字节数据进行指定编码的编码转换。

InputStreamReader(InputStream);
        


InputStreamWriter(InputStream,String charSet);
   





OutputStreamWriter(OutputStream);
    


OutputStreamwriter(OutputStream,String charSet);
   





3.数据流

DataInputStreamReader（InputStream）

DataInputStreamWriter（OutputStream）；  




4.打印流（PrintStream/PrintWriter）

System.out对应的类型就是PrintStream，字节打印流；


PrintStream是字节打印流：构造函数可以接受三种数据类型的值：1.字符串路径。2.File对象
 3.OutputStream


PrintWriter是字符打印流：构造函数可以接受四种数据类型的值：4.Writer





5.对象流

Object
 readObject();


void
 writeObject(Object)







流的操作总结：

数据源：读取：InputStream、Reader；

   数据目的：写入：OutputStream、Writer；

   纯文本数据就用字符流，否则就用字节流；

 数据源对应的设备：硬盘(File)，内存(数组)，键盘(System.in)

   数据目对应的设备：硬盘(File)，内存(数组)，控制台(System.out)。

   需要在基本操作上附加其他功能吗？比如缓冲，如果需要就进行装饰。](https://so.csdn.net/so/search/s.do?q=IO流&t=blog)](https://so.csdn.net/so/search/s.do?q=java中的IO流&t=blog)




