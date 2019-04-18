# Tomcat设置虚拟目录 - z69183787的专栏 - CSDN博客
2013年12月21日 16:14:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1436
今天在组织学习Tomcat的时候提出了一个问题--如何配置虚拟目录，当时没有得到答案，回家之后我找到了本介绍JSP的书，在里面找到了配置的方法。所以在这里写出来，一是加深我的认识，二也是为了让同学们多了解下Tomcat的用法。
当不想把JSP程序、网页等编写的程序文件部署在Tomcat的根目录下时，虚拟目录是最好的选择。虚拟目录实际上是在服务器上做一个映射，把某个名称命名的目录指向另外一个事实上存在的目录，这样可以增强安全性，访问者并不清楚服务器是否确实有无目录，当服务器某个盘中的空间不够时，可以把程序部署到另一个盘中做虚拟目录。
比如，在服务器的E盘中有个目录到JSP，要将她配置成一个虚拟目录的方法为：打开Tomcat 服务器上的conf子目录中的server.xml文件，在其中找到如下一段文字：
<!--Tomcat Root Context -->
   <!--
<Context path=”” docBase=”ROOT” debug=”0”/>
-->
<--Tomcat Manager Context -->
 <Context path=”/manager” docBase=”manager” debug=”0” privileged=”true”/>
……    ……
……    ……
</Context>
</Host>
然后在</Context>和</Host>之间插入配置文字如下：
<Context path=”/jsp” docBase=”e:/jsp” debug=”0” reloadable=”true” crossContext=”true”></Context>
其中path=”/jsp”表示配置的虚拟路径名称，docBase=”e:/jsp”是虚拟目录指向的事实目录。
保存server.xml后，重新启动Tomcat,设置就成功了，就可以把所编写的程序文件放在e:/jsp目录下。访问的地址为：[](http://localhost:8080/jsp/*.jsp)[http://localhost:8080/jsp/](http://localhost:8080/jsp/)*.jsp
以上就是我从书中学到的配置虚拟目录的方法，我已经实际使用过所以拿出来和大家分享呵呵。
