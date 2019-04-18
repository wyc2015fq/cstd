# 在Eclipse中运行Tomcat6源代码，保证正确的classpath - z69183787的专栏 - CSDN博客
2014年02月25日 16:25:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1879
　网上有一些文章介绍将Tomcat源代码导入Eclipse并运行。然而，这些文章几乎都没有提及classpath的设置。
　　使用默认的classpath会使Eclipse中运行的Tomcat与发行版Tomcat（通过startup.bat等方式运行）的system class loader所能加载到的类不同，这样可能导致一些问题。例如部署shopizer 1.1.5（[http://www.shopizer.com/](http://www.shopizer.com/)）应用时，在Eclipse中运行的Tomcat会有如下异常信息：
　　java.lang.VerifyError: (class: org/springframework/mail/javamail/JavaMailSenderImpl, method: doSend signature: ([Ljavax/mail/internet/MimeMessage;[Ljava/lang/Object;)V) Incompatible argument to function
　　而发行版Tomcat没有问题。
　　我们试图通过Eclipse的run configuration设置使Tomcat在运行时的classpath等各种配置信息与发行版Tomcat尽量保持一致。
　　我们使用Eclipse 3.6.2，将Tomcat 6.0.32源代码导入（导入方法不再赘述，请参考网上其他文章）。假设Tomcat源代码所在目录为“E:\tomcat6”，Ant builder（需要自己工程的“Builders”里添加一个“Ant Builder”，Buildfile选工程根目录下的build.xml）的输出目录为“E:\tomcat6\output\build”（这是Tomcat的build.xml的默认设置）。
　　新建一个run configuration，Main class选择org.apache.catalina.startup.Bootstrap，如下图所示：
![](http://hi.csdn.net/attachment/201109/9/0_1315536868BWCZ.gif)
　　Program arguments填“start”，VM arguments填如下内容：
　　-Djava.util.logging.config.file="E:\tomcat6\output\build\conf\logging.properties" -Djava.util.logging.manager=org.apache.juli.ClassLoaderLogManager -Djava.endorsed.dirs="E:\tomcat6\output\build\endorsed" -Dcatalina.base="E:\tomcat6\output\build" -Dcatalina.home="E:\tomcat6\output\build"
 -Djava.io.tmpdir="E:\tomcat6\output\build\temp"
　　如下图所示：
![](http://hi.csdn.net/attachment/201109/9/0_13155369104Y46.gif)
　　在Classpath选项卡中，删除User Entries下的所有内容，然后点“Add Jars”，将工程中的output\build\bin\bootstrap.jar加入，如下图所示：
![](http://hi.csdn.net/attachment/201109/9/0_1315536964uudd.gif)
　　这样就可以运行了。部署shopizer 1.1.5应用也没有问题。
