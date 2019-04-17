# Java —— 流的抽象基类 - Alex_McAvoy的博客 - CSDN博客





2018年10月28日 20:50:04[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：52
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【字节流】

InputStream 类与 OutputStream 类是字节流的抽象类，它们定义了字节流读取和写入的基本方法，各个子类会依其特点实现或覆盖其方法。

## 1.InputStream 类

InputStream 类是所有字节输入流的父类，它定义了操作输入流的方法，常见方法如下：
|available()|返回当前输入流的数据读取方法可以读取的有效字节数量|
|----|----|
|read(byte[] bytes)|从输入数据流中读取字节并存入 bytes 数组中|
|read(byte[] bytes,int off,int len)|从输入数据流读取 len 个字节，并存入 bytes 数组下标 off 开始的位置|
|mark(int readlimit)|在输入数据流中加入标记|
|markSupported()|测试输入流中是否支持标记|
|reset()|将当前输入流重新定位到最后一次调用 mark() 方法的位置|
|close()|关闭当前输入流，并释放任何与之关联的系统资源|
|read()|为让继承该类的子类可以针对不同的外部设备可以实现，因此被定义为抽象方法，从当前数据流中读取一个字节，若已到达流结尾，则返回 -1|

## 2.OutputStream 类

OutputStream 类是所有字节输出流的父类，它定义了操作输出流的方法，常见方法如下：
|write(byte[] bytes)|将 bytes 中的数据写入当前输出流|
|----|----|
|write(byte[] bytes,int off,int len)|将 bytes 下标 off 开始的 len 长度的数据写入当前输出流|
|flush()|刷新当前输出流，并强制写入所有缓冲的字节数据|
|close()|关闭当前输出流，并释放任何与之关联的系统资源|
|write()|为让继承该类的子类可以针对不同的外部设备可以实现，因此被定义为抽象方法，写入一个 byte 数据到当前输出流|

# 【字符流】

Reader 类与 Writer 类是字符流的抽象类，它们定义了字符流读取和写入的基本方法，各个子类会依其特点实现或覆盖其方法。

## 1.Reader 类

Reader 类是所有字符输入类的父类，它定义了操作字符输入流的方法，常用方法如下：
|read()|读入一个字符，若已读到流结尾，则返回 -1|
|----|----|
|read(char[] cbuf)|读入一些字符到 cbuf 数组内，并返回读入字符的数量，若已到达流结尾，则返回-1|
|mark(int readlimit)|在输入数据流中加入标记|
|reset()|将当前输入流重新定位到最后一次调用 mark() 方法的位置|
|skip(long n)|跳过参数 n 指定的字符数量，并返回所跳过字符的数量|
|getEncoding()|返回此流使用的编码名称|
|ready()|报告此流是否准备已读|
|close()|关闭当前输入流，并释放任何与之关联的系统资源|

## 2.Writer 类

Writer 类是所有字符输出类的父类，它定义了操作字符输出流的方法，常用方法如下：
|write(char c)|将字符 c 写入当前输出流|
|----|----|
|write(String str)|将字符串 str 写入当前输出流|
|write(char[] cbuf)|将字符数组 cbuf 写入当前输出流|
|flush()|刷新当前输出流，并强制写入所有缓冲的字节数据|
|close()|关闭当前输出流，并释放任何与之关联的系统资源|



