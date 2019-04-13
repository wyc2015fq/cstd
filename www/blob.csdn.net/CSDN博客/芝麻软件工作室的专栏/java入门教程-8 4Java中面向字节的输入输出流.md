
# java入门教程-8.4Java中面向字节的输入输出流 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月27日 09:34:48[seven-soft](https://me.csdn.net/softn)阅读数：192


字节流以字节为传输单位，用来读写8位的数据，除了能够处理纯文本文件之外，还能用来处理二进制文件的数据。InputStream类和OutputStream类是所有字节流的父类。
## InputStream类
面向字节的输入流都是InputStream类的子类，其类层次结构如图10-6所示。![](http://www.weixueyuan.net/uploads/allimg/130222/8-1302222326024a.png)
图10-6 InputStream的类层次结构图
表 10-5 列出了 InputStream 的主要子类及说明。
表 10-5 InputStream 的主要子类|类名|功能描述|
|FileInputStream|从文件中读取的输入流|
|PipedInputStream|输入管道|
|FilterInputStream|过滤输入流|
|ByteArrayInputStream|从字节数组读取的输入流|
|SequenceInputStream|两个或多个输入流的联合输入流，按顺序读取|
|ObjectInputStream|对象的输入流|
|LineNumberInputStream|为文本文件输入流附加行号|
|DataInputStream|包含读取 Java 标准数据类型方法的输入流|
|BufferedInputStream|缓冲输入流|
|PushbackInputStream|返回一个字节并把此字节放回输入流|
InputStream 流类中包含一套所有输入都需要的方法，可以完成最基本的从输入流读入数据的功能。表 10-6 列出了其中常用的方法及说明。
表 10-6 InputStream 的常用方法|方法|功能描述|
|void close()|关闭输入流|
|void mark()|标记输入流的当前位置|
|void reset()|将读取位置返回到标记处|
|int read()|从输入流中当前位置读入一个字节的二进制数据，以此数据为低位字节，补足16位的整型量（0~255）后返回，若输入流中当前位置没有数据，则返回-1|
|int read(byte b[])|从输入流中的当前位置连续读入多个字节保存在数组中，并返回所读取的字节数|
|int read(byte b[], int off, int len)|从输入流中当前位置连续读len长的字节，从数组第off+1个元素位置处开始存放，并返回所读取的字节数|
|int available()|返回输入流中可以读取的字节数|
|long skip(long n)|略过n个字节|
|long skip(long n)|跳过流内的n个字符|
|boolean markSupported()|测试输入数据流是否支持标记|
## OutputStream类
面向字节的输出流都是OutputStream类的子类，其类层次结构如图10-7所示。![图10-7 OutputStream的类层次结构图](http://www.weixueyuan.net/uploads/allimg/130222/8-1302222312562N.jpg)
图10-7 OutputStream的类层次结构图
10-7列出了OutputStream的主要子类及说明。
表10-7 OutputStream的主要子类|类名|功能描述|
|FileOutputStream|写入文件的输出流|
|PipedOutputStream|输出管道|
|FilterOutputStream|过滤输出流|
|ByteArrayOutputStream|写入字节数组的输出流|
|ObjectOutputStream|对象的输出流|
|DataOutputStream|包含写Java标准数据类型方法的输出流|
|BufferedOutputStream|缓冲输出流|
|PrintStream|包含print()和println()的输出流|
OutputStream流类中包含一套所有输出都需要的方法，可以完成最基本的向输出流写入数据的功能。表10-8列出了其中常用的方法及说明。
表10-8 OutputStream的常用方法|方法|功能描述|
|void close()|关闭输出流|
|void flush()|强制清空缓冲区并执行向外设输出数据|
|void write(int b)|将参数b的低位字节写入到输出流|
|void write(byte b[])|按顺序将数组b[]中的全部字节写入到输出流|
|void write(byte b[], int off, int len)|按顺序将数组b[]中第off+1个元素开始的len个数据写入到输出流|
由于InputStream和OutputStream都是抽象类，所以在程序中创建的输入流对象一般是它们某个子类的对象，通过调用对象继承的read()和write()方法就可实现对相应外设的输入输出操作。

