# Java学习（一） Java预备知识 - HJ - CSDN博客
2018年02月07日 14:15:15[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：192
所属专栏：[Java学习](https://blog.csdn.net/column/details/19570.html)
 一、Windows中常见的dos命令
dir：列出当前目录下的文件及文件夹
md：创建目录
rd：删除目录 （注意：rd不能删除非空的文件夹，而且只能用于删除文件夹）
cd：进入指定目录
cd ..：退回到上一级目录
cd  \：退回到根目录
Javac 文件名.java：编译‘文件名.java’的java文件，创建出一个‘文件名.class’文件
java 文件名：运行‘文件名’的java代码，并显示结果
二、JDK目录介绍和JRE介绍
1.JDK目录介绍
bin目录：存放Java的编译器、解释器等工具(可执行文件)。
db目录：JDK附带的一个轻量级的数据库，名字叫做Derby。
include目录：存放的调用系统资源的接口文件。
jre目录：存放Java运行环境文件。
lib目录：存放Java的类库文件。
src.zip文件：JDK提供的类的源代码。
**安装JDK需要注意的事项：**
①安装路径中不要包含中文**；**
②安装路径中不要包含空格；
2.JRE介绍
JRE（ Java Runtime Environment ），Java运行环境。包括Java虚拟机和Java程序所需的核心类库等，如果想要运行一个开发好的Java程序，计算机中只需要安装JRE即可。
三、配置Java环境变量
1.配置path环境变量
我们将jdk下bin的路径进行复制
在我们电脑-----属性-----高级-----环境变量---------path这项
将bin的路径放置到path的最前边，别忘记加分号.
2.配置JAVA_HOME环境
1、 创建一个JAVA_HOME的变量。
2、 JAVA_HOME的值为JDK的安装目录。
3、 配置path路径：”**%JAVA_HOME%/bin”**。
以后需要更换JDK的目录时，只需要更换JAVA_HOME的目录即可。
3.配置classpath变量
1、创建一个Classpath的变量。
3、变量值键入”**.;%JAVA_HOME%\lib\dt.jar;%JAVA_HOME%\lib\tools.jar;**”
注意不要漏掉变量值中的符号，最后点击确定。
如果在命令行中输入’java’或者’javac’，出现相关详细信息，则表示配置成功。
