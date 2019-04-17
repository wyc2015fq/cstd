# Java —— 流的基本概念 - Alex_McAvoy的博客 - CSDN博客





2018年10月23日 14:29:32[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：58
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【概述】

流（stream）是一组有序的数据序列，IO 流用于处理设备之间的数据传输，Java 中对于数据的输入输出均是以流的方式进行，在 java.io 包下提供了各种流类与接口，用于获取不同种类的数据，并通过标准的方法输入输出数据 。

# 【输入与输出】

输入（input）：读取磁盘、光盘等外部存储设备的数据到程序(内存)中

输出（output）：将程序数据输出到磁盘、光盘等外部存储设备中

![](https://img-blog.csdnimg.cn/20181028223410612.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_27,color_FFFFFF,t_70)

# 【流的分类】

根据操作数据单位的不同，分为：字节流(8 bits)、字符流(16 bits)

根据数据流的流向，分为：输入流、输出流

根据流的角色的不同，分为：节点流(直接作用于文件上)、处理流(作用在已有的节点流之上)

![](https://img-blog.csdnimg.cn/20181028225342629.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_27,color_FFFFFF,t_70)

Java 的流类共涉及 40 多个类，实际都是通过以下 4 个抽象基类派生的，由以下 4 个抽象基类派生出的子类名称都是以其父类名作为子类名后缀。
|抽象基类|字节流|字符流|
|----|----|----|
|输入流|InputStream|Reader|
|输出流|OutputStream|Writer|

## 1.字节流

字节流分为输入字节流、输出字节流。

InputStream 类是字节输入流的抽象类，它是所有字节输入流的父类，InputStream 中存在多个子类，它们实现了不同的数据输入流。

OutputStream 类是字节输出流的抽象类，它是所有字节输出流的父类，OutputStream 中存在多个子类，它们实现了不同的数据输出流。

![](https://img-blog.csdnimg.cn/20181029192710566.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

## 2.字符流

字符流分为输入字符流、输出字符流。

Reader 类是字符输入流的抽象类，所有字符输入流的实现都是它的子类。

Writer 类是字符输出流的抽象类，所有字符输出流的实现都是它的子类。

![](https://img-blog.csdnimg.cn/20181029192721426.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

## 3.划分
||抽象基类|节点流|缓冲流|转换流|数据流|对象流|打印流|
|----|----|----|----|----|----|----|----|
|字节流|InputStream|FileInputStream|BufferedInputStream||DataInputStream|ObjectInputStream|PrintStream|
|OutputStream|FileOutputStream|BufferedOutputStream||DataOutputStream|ObjectOutputStream| | |
|字符流|Reader|FileReader|BufferedReader|InputStreamReader|||PrintWriter|
|Writer|FileWriter|BufferedWr|OutputStreamWriter||| | |



