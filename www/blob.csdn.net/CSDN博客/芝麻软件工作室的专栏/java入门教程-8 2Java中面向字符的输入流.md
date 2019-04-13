
# java入门教程-8.2Java中面向字符的输入流 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月27日 09:32:46[seven-soft](https://me.csdn.net/softn)阅读数：219


字符流是针对字符数据的特点进行过优化的，因而提供一些面向字符的有用特性，字符流的源或目标通常是文本文件。 Reader和Writer是java.io包中所有字符流的父类。由于它们都是抽象类，所以应使用它们的子类来创建实体对象，利用对象来处理相关的读写操作。Reader和Writer的子类又可以分为两大类：一类用来从数据源读入数据或往目的地写出数据（称为节点流），另一类对数据执行某种处理（称为处理流）。
面向字符的输入流类都是Reader的子类，其类层次结构如图10-2所示。![图10-2 Reader的类层次结构图](http://www.weixueyuan.net/uploads/allimg/130222/8-130222205I61F.jpg)
图10-2 Reader的类层次结构图
表 10-1 列出了 Reader 的主要子类及说明。
表 10-1 Reader 的主要子类|类名|功能描述|
|CharArrayReader|从字符数组读取的输入流|
|BufferedReader|缓冲输入字符流|
|PipedReader|输入管道|
|InputStreamReader|将字节转换到字符的输入流|
|FilterReader|过滤输入流|
|StringReader|从字符串读取的输入流|
|LineNumberReader|为输入数据附加行号|
|PushbackReader|返回一个字符并把此字节放回输入流|
|FileReader|从文件读取的输入流|
Reader 所提供的方法如表 10-2 所示，可以利用这些方法来获得流内的位数据。
表 10-2 Reader 的常用方法|方法|功能描述|
|void close()|关闭输入流|
|void mark()|标记输入流的当前位置|
|boolean markSupported()|测试输入流是否支持 mark|
|int read()|从输入流中读取一个字符|
|int read(char[] ch)|从输入流中读取字符数组|
|int read(char[] ch, int off, int len)|从输入流中读 len 长的字符到 ch 内|
|boolean ready()|测试流是否可以读取|
|void reset()|重定位输入流|
|long skip(long n)|跳过流内的 n 个字符|
## 使用 FileReader 类读取文件
FileReader 类是 Reader 子类 InputStreamReader 类的子类，因此 FileReader 类既可以使用Reader 类的方法也可以使用 InputStreamReader 类的方法来创建对象。
在使用 FileReader 类读取文件时，必须先调用 FileReader()构造方法创建 FileReader 类的对象，再调用 read()方法。FileReader 构造方法的格式为：
public
 FileReader(String name);  //根据文件名创建一个可读取的输入流对象
【例 10-1】利用 FileReader 类读取纯文本文件的内容（[查看源代码](http://www.weixueyuan.net/uploads/code/java/rumen/10-1.txt)）。
运行结果如图 10-3 所示：
![](http://www.weixueyuan.net/uploads/allimg/130222/8-130222212005X5.jpg)
图 10-3  例 10_1 运行结果（输出内容为文件ep10_1.txt的内容）
需要注意的是，Java 把一个汉字或英文字母作为一个字符对待，回车或换行作为两个字符对待。
## 使用 BufferedReader 类读取文件
BufferedReader 类是用来读取缓冲区中的数据。使用时必须创建 FileReader 类对象，再以该对象为参数创建 BufferedReader 类的对象。BufferedReader 类有两个构造方法，其格式为：
public
 BufferedReader(Reader in);  //创建缓冲区字符输入流
public BufferedReader(Reader in,int size);  //创建输入流并设置缓冲区大小
【例 10-2】利用 BufferedReader 类读取纯文本文件的内容（[查看源代码](http://www.weixueyuan.net/uploads/code/java/rumen/10-2.txt)）。
运行结果如图 10-4 所示：
![](http://www.weixueyuan.net/uploads/allimg/130222/8-130222212425P5.jpg)
图 10-4  例 10_2 运行结果
需要注意的是，执行 read()或 write()方法时，可能由于 IO 错误，系统抛出 IOException 异常，需要将执行读写操作的语句包括在 try 块中，并通过相应的 catch 块来处理可能产生的异常。

