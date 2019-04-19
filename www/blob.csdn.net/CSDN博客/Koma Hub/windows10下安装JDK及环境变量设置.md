# windows10下安装JDK及环境变量设置 - Koma Hub - CSDN博客
2018年01月08日 15:35:02[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：863
Java是一门面向对象编程语言，不仅吸收了C++语言的各种优点，还摒弃了C++里难以理解的多继承、指针等概念，因此Java语言具有功能强大和简单易用两个特征。Java语言作为静态面向对象编程语言的代表，极好地实现了面向对象理论，允许程序员以优雅的思维方式进行复杂的编程。
Java版本可以分为：
Java SE：标准版java standard edition；
Java EE：企业版java Enterprise Editon（例如电子商务、淘宝、京东都用的是EE；其包括了Java SE的全部功能）；
Java ME：嵌入式java Micro edition（手机游戏等。。逐渐被淘汰）；
我们这里讲解Java SE在windows10下的安装过程与环境变量和修改。
首先下载（建议使用JDK8，甲骨文声称JDK8将成为永久支持版本，9就不一定了，尽管9有新特性）：
[JDK9下载地址](http://www.oracle.com/technetwork/java/javase/downloads/jdk9-downloads-3848520.html)
[JDK8下载地址](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)
在JDK下载链接下面还有很多Demos，也可以下载，可以作为学习资料，都是一些html，可以直接用浏览器打开。
然后进行无脑式安装JDK和JRE。
双击下载好的jdk（或以管理员身份运行）（可以更改路径）：
![](https://img-blog.csdn.net/20180108152117534?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20180108152155629?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击下一步开始安装，安装过程会出现提示安装jre，我把它安装在jdk的上级目录中：
![](https://img-blog.csdn.net/20180108152247168?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
jre安装完成后窗口自动消失：
![](https://img-blog.csdn.net/20180108152308973?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后修改环境变量：
右击此电脑》属性》高级系统选项》环境变量
新建JAVA_HOME，内容D:\Java\jdk1.8.0_152；
在Path中添加%JAVA_HOME%\bin和%JAVA_HOME%\jre\bin；
新建 CLASSPATH 变量
变量值填写   .;%JAVA_HOME%\lib\dt.jar;%JAVA_HOME%\lib\tools.jar;（注意最前面有一点.）
环境变量修改结束：
win+r或win+x再点击运行，输入cmd，再终端输入java -version，结果如下，则证明安装成功：
C:\Users\Toa>java -version
java version "1.8.0_152"
Java(TM) SE Runtime Environment (build 1.8.0_152-b16)
Java HotSpot(TM) 64-Bit Server VM (build 25.152-b16, mixed mode)

