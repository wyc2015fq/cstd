# JDK1.5.0+TomCat5.5.20+Eclipse3.2.1+MyEclipse5.1.1GA - 战斗蜗牛的专栏 - CSDN博客





2010年01月13日 16:27:00[vbskj](https://me.csdn.net/vbskj)阅读数：1339








JDK1.5.0+TomCat5.5.20+Eclipse3.2.1+MyEclipse5.1.1GA
无聊在家里，弄个环境作为玩的基础，或许对有些网友有用就发上来了。

下载地址
1）[eclipse-SDK-3.2.1-win32.zip](http://www.datawiz.co.th/mirror/eclipse/downloads/drops/R-3.2.1-200609210945/eclipse-SDK-3.2.1-win32.zip)
2）[NLpack1-eclipse-SDK-3.2.1-win32.zip（多语言支持包）](http://anak.kambing.vlsm.org/eclipse/eclipse/downloads/drops/L-3.2.1_Language_Packs-200609210945/NLpack1-eclipse-SDK-3.2.1-win32.zip)
3）[jdk-1_5_0_10-windows-i586-p.exe](http://192.18.108.136/ECom/EComTicketServlet/BEGINB1EEF109B558F2D9C0D3B56CEE613335/-2147483648/2081191371/1/783938/783674/2081191371/2ts%20/westCoastFSEND/jdk-1.5.0_10-oth-JPR/jdk-1.5.0_10-oth-JPR:2/jdk-1_5_0_10-windows-i586-p.exe)
4）[MyEclipse_5.1.1GA_Installer.exe](http://www.myeclipseide.com/Downloads+index-req-getit-lid-59.html)
5）[apache-tomcat-5.5.20.zip](http://apache.justdn.org/tomcat/tomcat-5/v5.5.20/bin/apache-tomcat-5.5.20.zip)

安装j2sdk1.5，设定环境：
1.(新建)JAVA_HOME:C:/ProgramFiles/Java/jdk1.5.0_07
2.(新建)CLASSPATH:.;.;%JAVA_HOME%/lib/dt.jar;.;%JAVA_HOME%/lib/tools.jar
3.编辑PATH的变量值，加上;%JAVA_HOME%/bin
4.用命令简单测试一下:java

安装Tomcat5
下载Tomcat的zip版，不用安装版的目的是可以使用多个Tomcat。
解压放在任意目录即可。

安装Eclipse3.2
解压
eclipse-SDK-3.2.1-win32.zip
NLpack1-eclipse-SDK-3.2.1-win32.zip（多语言支持包）
放在同一个目录里即可。

安装MyEclipse
选择eclipse所在目录，其它都用默认下一步安装。安装完毕之后，找到myeclipse的安装目录，
将myeclipse安装目录复制一份，将刚才安装的myeclipse删除（反安装）。
这样做的目的是为了在一台电脑上可以使用多个独立的eclipse开发平台。
解释上面这样做还可以使用的原因：因为myeclipse是eclipse的插件，所以按照插件的安装方法安装，eclipse会自动认出这些插件。


破解myeclipse5.0
启动eclipse
点击菜单Window／Preferences／Subscription／EnterSubscription,输入注册码。
Subscriber:[www.1cn.biz](http://www.1cn.biz/)
SubscriberCode:jLR8ZC-444-55-4467865481680090
注册成功后显示：
Subscriber:[www.1cn.biz](http://www.1cn.biz/)
ProductID:E3MP(MyEclipseProfessionalSubscription)
Licenseversion:9.99
FullMaintenanceIncluded
Subscriptionexpirationdate(YYYYMMDD):20991231
Numberoflicenses:Unlimited


设定运行环境
在eclipse中设定使用的jdk和myEclipse使用的tomcat服务器。

1.指定使用的jre
启动eclipse，在主窗口中window/preferences/java/installed JREs，点击Add
JRE name：jre1.5.0_10
JRE home directory：C:/Program Files/Java/jre1.5.0_10
(注意：这里NAME一定要是JRE开头，路径指向JRE的路径);

2.指定Tomcat服务器
Window/Preferences/MyEclipse/Application Servers/Tomcat5
tomcat home directory:D:/apache-tomcat-5.5.20

设定该tomcat所用的jdk，点add
jre名称：jdk1.5.0_10
jre主目录：C:/Program Files/Java/jdk1.5.0_10
（注意这里的路径一定要指定jdk目录，jre是不被接受的）

到这里开发环境搭建完毕

新建一个项目来试一试？
1.Eclipse中File/New/Project,选择MyEclips/J2EE Projects/WebProject，再点下一步。
2.在ProjectName中输入test，点finish。生成test项目。
3.点WebRoot/(右键)New/JSP,新建一个JSP页面（akane.jsp）。
5.把生成的jsp文件简单修改一下。
7.deploy（部署）这个test项目，这里要指定使用的服务器。
8.启动Tomcat服务器。在控制台可以看到Tomcat启动输出信息。
9.浏览器浏览[http://localhost:8080/test/akane.jsp](http://localhost:8080/test/akane.jsp)可以看到运行结果。
10.可以去eclipse的workspace目录下看看test项目的代码，也可以去Tomcat的webapps目录下，看看部署之后的test项目的结构。
11.没有自动认出插件
删除C:/eclipse/configuration下的文件夹org.eclipse.update，然后重起。



