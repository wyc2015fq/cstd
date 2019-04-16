# IO流第一课File类 - PeterBishop - CSDN博客





2018年11月02日 21:27:22[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：30








  这节课开始我们开始学文件操作或者叫IO操作(IO就是输入(Input)输出(Output)的意思)



  既然是文件操作，先来学学一个关于文件的类——File类


public class File implements Serializable, Comparable<File>


File类实现了Serializable(序列化)和Comparable(可比较)接口





* 1. File类实现了Serializable(序列化)和Comparable(可比较)接口

* 2. File类对象是文件和目录路径名的抽象表示形式。

*      2.1 什么叫目录？就是文件夹

*      2.2 所以File类对象不仅可以表示文件，也可以表示文件夹

* 3. 用户界面和操作系统使用与系统相关的路径名字符串 来命名文件和目录。

*      3.1 就是说文件和目录的名字其实就是路径

*      3.2 我们说的文件名其实是值文件所在路径的全称在省略了该文件的父文件的路径后剩下的字符串

*          3.2.1 举例:

*              第30课——IO流第一课File类.docx 的完整路径是C:\Users\xiaoye\Desktop\java教程\java se基础教程\常用类\IO流\第30课——IO流第一课File类.docx

*              第30课——IO流第一课File类.docx 的父文件夹的完整路径是C:\Users\xiaoye\Desktop\java教程\java se基础教程\常用类\IO流\

*              所以省略了父路径后的文件名就是第30课——IO流第一课File类.docx





File类的构造器:



![](https://img-blog.csdnimg.cn/20181102212444722.png)

常用方法:


public boolean **exists**()
测试此抽象路径名表示的文件或目录是否存在。


public boolean **createNewFile**()                      throws IOException
当且仅当不存在具有此抽象路径名指定名称的文件时，不可分地创建一个新的空文件。检查文件是否存在，若不存在则创建该文件，这是单个操作，对于其他所有可能影响该文件的文件系统活动来说，该操作是不可分的

**返回：**

如果指定的文件不存在并成功地创建，则返回 `true`；如果指定的文件已经存在，则返回 `false`






//String getName() 获取文件名

//String getAbsolutePath() 获取绝对路径

//boolean isFile() 是否是文件

//boolean isDirectory() 是否是目录(文件夹)

//boolean delete() 删除文件

//long length() 获取文件内容的长度(其实就是文件大小)








