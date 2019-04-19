# 【Android Debug系列】keytool---JDK1.6所含工具 - 三少GG - CSDN博客
2011年09月04日 22:34:00[三少GG](https://me.csdn.net/scut1135)阅读数：2951标签：[android																[jdk																[工具																[glassfish																[tomcat](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)](https://so.csdn.net/so/search/s.do?q=glassfish&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=jdk&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
1. **""默认 .deb文件或者keytool这类可执行程序的  属性-->Permissions-->Allow executing file as program" 没有打上勾！！！要去提示的目录去打开这个选项！！！**坑爹啊 两天时间。。。
Error generating final archive: Failed to create key: Cannot run program "/home/grant/MyEclipse-8.6/Common/binary/com.sun.java.jdk.linux.x86_1.6.0.013/jre/bin/keytool": java.io.IOException: error=13, Permission denied
JAVA_HOME is set to: /home/grant/MyEclipse-8.6/Common/binary/com.sun.java.jdk.linux.x86_1.6.0.013/jre
Update it if necessary, or manually execute the following command:
/home/grant/MyEclipse-8.6/Common/binary/com.sun.java.jdk.linux.x86_1.6.0.013/jre/bin/keytool -genkey -alias AndroidDebugKey -keyalg RSA -dname "CN=Android Debug,O=Android,C=US" -validity 365 -keypass android -keystore /home/grant/.android/debug.keystore -storepass android
提示 修复指令（Grant 20110904）：
/usr/lib/jvm/java-6-sun-1.6.0.26/bin$ （其实应该是目录 /home/grant/MyEclipse-8.6/Common/binary/com.sun.java.jdk.linux.x86_1.6.0.013/jre下的）
keytool  -genkey -alias AndroidDebugKey -keyalg RSA -dname "CN=Android Debug,O=Android,C=US" -validity 10950 -keypass android -keystore /home/grant/.android/debug.keystore -storepass androidffmpeg
问题1的解决方法一开始不对导致问题2，3. **java.io.IOException: error=13, 权限不够**
2. java.lang.Exception: Keystore 文件不存在 由于keytool默认执行权限问题导致没能够自动生成这个debug阶段的debug.keystore.
3. java.io.IOException: Keystore was tampered with, or password was incorrect。自己按照上述1的following command执行后 会提示的密码不对等问题，这个估计是需要android debug默认的keystore，及 debugkey。
【参考】
1.   
使用keytool工具删除证书时出现java.io.IOException: Keystore was tampered with, or password was incorrect 异常的解决办法：
对于很多服务器比如glassfish或者tomcat之类的，在证书过期，我们需要删除时，需要输入保护密码，默认的就是changeit，输入这个密码就可以了。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
【解决问题必备知识】
1.[http://hi.baidu.com/gehaixia2008/blog/item/8572c91e13cfa345f3de32df.html](http://hi.baidu.com/gehaixia2008/blog/item/8572c91e13cfa345f3de32df.html)
2. http://www.blogjava.net/crespochen/archive/2011/04/22/348819.html
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
JDK中keytool常用命令
-genkey 在用户主目录中创建一个默认文件".keystore",还会产生一个mykey的别名，mykey中包含用户的公钥、私钥和证书
-alias 产生别名
-keystore 指定密钥库的名称(产生的各类信息将不在.keystore文件中
-keyalg 指定密钥的算法 
-validity 指定创建的证书有效期多少天
-keysize 指定密钥长度
-storepass 指定密钥库的密码
-keypass 指定别名条目的密码
-dname 指定证书拥有者信息 例如： "CN=sagely,OU=atr,O=szu,L=sz,ST=gd,C=cn"
-list 显示密钥库中的证书信息 keytool -list -v -keystore sage -storepass ....
-v 显示密钥库中的证书详细信息
-export 将别名指定的证书导出到文件 keytool -export -alias caroot -file caroot.crt
-file 参数指定导出到文件的文件名
-delete 删除密钥库中某条目 keytool -delete -alias sage -keystore sage
-keypasswd 修改密钥库中指定条目口令 keytool -keypasswd -alias sage -keypass .... -new .... -storepass ... -keystore sage
-import 将已签名数字证书导入密钥库 keytool -import -alias sage -keystore sagely -file sagely.crt
导入已签名数字证书用keytool -list -v 以后可以明显发现多了认证链长度，并且把整个CA链全部打印出来。
1.证书的显示
-list
[-v | -rfc] [-alias <alias>]
[-keystore <keystore>] [-storepass <storepass>]
[-storetype <storetype>] [-provider <provider_class_name>]
例如：keytool -list -v -alias RapaServer -keystore cacerts -storepass 12345678
keytool -list -v -keystore d2aapplet.keystore -storepass 12345678 -storetype IAIKKeystore
2.将证书导出到证书文件
例如：keytool -export -keystore monitor.keystore -alias monitor -file monitor.cer
将把证书库 monitor.keystore 中的别名为 monitor 的证书导出到 monitor.cer 证书文件中，它包含证书主体的信息及证书的公钥，不包括私钥，可以公开。
keytool -export -keystore d2aApplet.keystore -alias RapaServer -file Rapa.cert -storetype IAIKKeystore
3.将keystore导入证书中
这里向Java默认的证书 cacerts导入Rapa.cert
keytool -import -alias RapaServer -keystore cacerts -file Rapa.cert -keystore cacerts
4.证书条目的删除
keytool的命令行参数 -delete 可以删除密钥库中的条目，如： keytool -delete -alias RapaServer -keystore d2aApplet.keystore ，这条命令将 d2aApplet.keystore 中的 RapaServer 这一条证书删除了。
5.证书条目口令的修改
使用 -keypasswd 参数，如：keytool -keypasswd -alias RapaServer -keystore d2aApplet.keystore，可以以交互的方式修改 d2aApplet.keystore证书库中的条目为 RapaServer 的证书。
Keytool -keypasswd -alias RapaServer -keypass 654321 -new 123456 -storepass 888888 -keystore d2aApplet.keystore这一行命令以非交互式的方式修改库中别名为 RapaServer 的证书的密码为新密码 654321，行中的 123456 是指该条证书的原密码， 888888 是指证书库的密码
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
keytool生成私钥文件(.key)和签名请求文件(.csr)，openssl签发数字证书
      J2SDK在目录%JAVA_HOME%/bin提供了密钥库管理工具Keytool，用于管理密钥、证书和证书链。Keytool工具的命令在JavaSE6中已经改变，不过以前的命令仍然支持。Keytool也可以用来管理对称加密算法中的密钥。
最简单的命令是生成一个自签名的证书，并把它放到指定的keystore文件中：
keytool -genkey -alias tomcat -keyalg RSA -keystore c:/mykey
如果c:/mykey文件不存在，keytool会生成这个文件。按照命令的提示，回答一系列问题，就生成了数字证书。注意，公共名称(cn)应该是服务器的域名。这样keystore中就存在一个别名为tomcat的实体，它包括公钥、私钥和证书。这个证书是自签名的。<o:p></o:p>
Keytool工具可以从keystore中导出证书，但是不能导出私钥。对于配置apache这样的服务器，就不太方便。这种情况下就完全用OpenSSL吧，下面将会介绍。
# [来自：用Keytool和OpenSSL生成和签发数字证书](http://www.blogjava.net/duanzhimin528/archive/2010/06/22/324182.html)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
【知识扩展】
**VisualVM简介**
      简单说来，VisualVM是一种集成了多个JDK命令行工具的可视化工具，它能为您提供强大的分析能力。所有这些都是免费的！它囊括的命令行工具包括jstat, JConsole, jstack, jmap 和 jinfo，这些工具与JDK的标准版本是一致的。   
      你可以使用VisualVM生成和分析海量数据、跟踪内存泄漏、监控垃圾回收器、执行内存和CPU分析，同时它还支持在MBeans上进行浏览和操作。尽管VisualVM自身要在JDK6这个版本上运行，但是JDK1.4以上版本的程序它都能监控。   
      对于我来说，这些功能都是至关重要的。以前我一直在使用如YourKit这样的一些付费软件来检测我的应用软件，但现在我可以用VisualVM这样一个开源工具来实现同样的功能。更何况，有谁比能Sun公司更了解JVM呢？  
**如何获取VisualVM**
      VisualVM的一个最大好处就是，它已经在你的JDK bin目录里了，只要你使用的是JDK1.6 Update7之后的版本。点击一下jvisualvm.exe图标它就可以运行了。   
      你所需要做的只是点一下jvisualvm.exe，就能启动它。运行中的所有Java应用程序将显示在右边的导航栏。你会发现根本不需要在VisualVM 里亲自为你的应用程序注册，因为它们全都会自动显示出来。你甚至还可以在导航栏里查看到远程的应用程序。 
# [JDK中还藏着一个宝贝 它的名字叫做VisualVM](http://www.ixpub.net/thread-2666746-1-1.html)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
