# 【java web】java执行预编译Groovy脚本 - 江南烟雨 - CSDN博客
2014年05月25日 14:31:35[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：9786

在JVM中运行Groovy类有两种方式：
使用Groovy编译所有的*.groovy为java的*.class文件，把这些*.class文件放在java类路径中，通过java类加载器来加载这些类。
通过groovy类加载器在运行时直接加载*.groovy文件并生成对象。在这种方式下，没有生成任何*.class，但是生成了一个java.lang.Class对象的实例。
![](https://img-blog.csdn.net/20140525164631078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下面介绍前一种使用Groovy的方法：编译成java字节码并且作为正常java应用程序运行在java虚拟机上，即预编译模式。
1、安装Groovy
下载Groovy binary：
[http://groovy.codehaus.org/Download](http://groovy.codehaus.org/Download)
将其解压。然后设置环境变量GROOVY_HOME为解压后文件的目录。
然后在path环境变量中添加：%GROOVY_HOME%/bin。
测试是否安装成功，打开cmd输入groovy即可：
![](https://img-blog.csdn.net/20140525142905484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2、编写Groovy脚本文件SimpleScript.groovy：
此脚本实现的功能是输出Fibonacci数列：
```java
//println"Welcome to $language"
 
//打印Fibonacci数列
current = 1
next = 1
10.times{
printcurrent + ' '
newCurrent= next
next= next + current
current= newCurrent
}
 
return "TheEnd"
```
然后使用内置编译器groovyc编译此脚本文件：
![](https://img-blog.csdn.net/20140525143020406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
编译将会生成一个classes文件夹，其中包含两个class文件：
![](https://img-blog.csdn.net/20140525143041234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3、使用java运行编译好的groovy脚本
![](https://img-blog.csdn.net/20140525143102328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
需要注意的是，我们不用指定包含主类class文件的扩展名.class。使用-cp选项指定jar包位置。
