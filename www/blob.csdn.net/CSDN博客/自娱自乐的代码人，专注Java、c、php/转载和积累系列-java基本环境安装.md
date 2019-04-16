# 转载和积累系列 - java基本环境安装 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年09月04日 14:19:45[initphp](https://me.csdn.net/initphp)阅读数：774








## 一、开发工具获取

**http://www.oracle.com/technetwork/java/javasebusiness/downloads/java-archive-downloads-javase6-419409.html?ssSourceSiteId=otncn**

JDK6地址


### 1、开发工具包JDK

#### ● 下载地址：

到ORACLE公司官方网站([http://www.oracle.com/](http://www.oracle.com/))下载。

####  ● 下载文件的选择：

对于windows操作系统，最后下载的文件的文件名格式为：jdk-6uX-windows-i586.exe，其中“6uX”代表版本号，其中包含了JDK和JRE，文件大小大概70M； 例如笔者下载最新的JDK为Java SE Development Kit 6 Update 25，相应的文件名为jdk-6u25-windows-i586.exe，文件大小为76.66M。相关网页如图所示：



![ORACLE公司官方网站http://www.oracle.com/](http://hi.csdn.net/attachment/201105/7/0_13047556527Ydc.gif)

![java download选择按钮](http://hi.csdn.net/attachment/201105/7/0_1304755954McH0.gif)![JDK文件的选择](http://hi.csdn.net/attachment/201105/7/0_1304756117eisS.gif)



### 2、Java Web应用的Web服务器——Tomcat



#### ● 下载地址：

下载地址：Tomcat官网([http://tomcat.apache.org](http://tomcat.apache.org/))

#### ● 下载文件的选择：

官网左侧Download给出了Tomcat的各种版本，选择一种版本转入下载页面；在Quick Navigation区域中点击最新的版本超级链接定位到最下端的针对各操作系统和平台的Tomcat下载列表，其中ZIP对应的是Windows系统下的版本。Tar.gz为Linux平台下的开发包。相关的页面参考指引图如下：



![tomcat官网01](http://hi.csdn.net/attachment/201105/7/0_13047565675imj.gif)![tomcat官网02](http://hi.csdn.net/attachment/201105/7/0_1304756610zN5x.gif)![tomcat官网03](http://hi.csdn.net/attachment/201105/7/0_1304756713ZaTV.gif)





### 3、Java开发IDE——Eclipse



#### ● 下载地址：

Eclipse官方网站([http://www.eclipse.org](http://www.eclipse.org/))，选择Download进入下载列表页面；

#### ● 下载文件的选择：

下载选择：找到Eclipse IDE for Java EE Developers(206M)，进入下载页面，下载；笔者下载的最终文件是eclipse-jee-helios-SR2-win32.zip，相关页面截图如下：



![eclipse官网下载01](http://hi.csdn.net/attachment/201105/7/0_1304757941WzZk.gif)![eclipse官网下载01](http://hi.csdn.net/attachment/201105/7/0_13047580946yo6.gif)



#### ● Eclipse中文包下载：

       下载地址[http://www.eclipse.org/babel](http://www.eclipse.org/babel) ， 在Babel项目首页的左侧，点击“Download”进入下载列表页面；在此根据下载的Eclipse选择对应的版本的语言包，如helios版本的，点击进入多国语言包下载页面；找到Language: Chinese
 (Simplified)子标题，它下面包含了简体中文包，这些包适用于不同的Eclipse插件，其中BabelLanguagePack-eclipse-zh_3.6.0.v***.zip 文件是Eclipse的核心语言包，点击进入具体的下载页面进行下载。下载完成后覆盖Eclipse相应的文件夹即可成功添加中文语言包。相关指引图如下：



![Eclipse中文包下载01](http://hi.csdn.net/attachment/201105/7/0_1304762878647O.gif)

![Eclipse中文包下载02](http://hi.csdn.net/attachment/201105/7/0_1304763010bXv0.gif)

![Eclipse中文包下载03](http://hi.csdn.net/attachment/201105/7/0_1304763129khaK.gif)

![Eclipse中文包下载04](http://hi.csdn.net/attachment/201105/7/0_13047632056QHh.gif)



## 二、开发工具的安装与环境配置

### 1、JDK安装与配置



#### ● 安装：



直接运行下载的JDK安装文件jdk-6u25-windows-i586.exe，按照提示指引进行安装。其中中途有选择JRE的安装，也一并安装了。

**补充说明：JDK与JRE的关系**

JDK是Java的开发平台，在编写Java程序时，需要JDK进行编译处理；JRE是Java程序的运行环境，包含了JVM的实现 及Java核心类库，编译后的Java程序必须使用JRE执行。在下载的JDK安装包中集成了JDK与JRE，所以在安装JDK过程中会提示安装JRE。





####  ● 环境配置：

在”我的电脑”的环境变量中，添加JAVA系统变量；具体操作是在“系统变量”区域新建变量名为“JAVA_HOME”的变量，变量值为JDK的安装路径，如“C：/Java/jdk1.6.0_25”；接着将JAVA_HOME添加到系统变量Path之中，方法是编辑打开系统变量Path，在原有的变量值前添加双引号内的代码“%JAVA_HOME %/bin;”，其中的分号起分隔作用，不可缺少。



####  ● 测试：

安装配置好JDK后，在CMD命令行窗口中运行JDK命令：javac。其成功的效果如图：





![JDK测试结果](http://hi.csdn.net/attachment/201105/7/0_1304764100cJkg.gif)





### 2、Tomcat安装与目录说明



#### ● 安装：



下载Tomcat后，直接解压缩就可以使用了；





















































#### ● Tomcat关键目录及文件作用：



![Tomcat关键目录](http://hi.csdn.net/attachment/201105/7/0_1304764340k1D1.gif)





 bin：

用于存放各种平台下启动和关闭Tomcat的脚本文件。在该目录中有两个非常关键的文件——startup.bat、shutdown.bat，前者是Windows下启动Tomcat的文件，后者是对应的关闭文件；

 conf：

Tomcat的各种配置文件，其中server.xml为服务器的主配置文件，web.xml为所有Web应用的配置文件，tomcat-users.xml用于定义Tomcat的用户信息、配置用户的权限与安全。

 lib：

此目录存放Tomcat服务器和所有Web应用都能访问的JAR。

 logs：

用于存放Tomcat的日志文件，Tomcat的所有日志都存放在此目录中。

 temp：

临时文件夹，Tomcat运行时候如果有临时文件将保存于此目录。

 webapps目录：

Web应用的发布目录，把Java Web站点或war文件放入这个目录下，就可以通过Tomcat服务器访问了。

 work：

Tomcat解析JSP生成的Servlet文件放在这个目录中。





#### ● 测试：



在bin目录下双击starup.bat文件运行Tomcat，在浏览器输入：http://localhost:8080 ，访问Tomcat。显示如下图所示页面，证明Tomcat运行成功。



![Tomcat启动信息01](http://hi.csdn.net/attachment/201105/7/0_13047645600Z0d.gif)![Tomcat启动成功页面](http://hi.csdn.net/attachment/201105/7/0_1304764617h557.gif)





### 3、****



### ****

Eclipse安装与配置





#### ● 安装：



下载Eclipse后，将得到的eclipse-jee-helios-SR2-win32.zip文件直接解压缩就完成了Eclipse的安装；



####  ● 汉化：

将之前下载的中文包BabelLanguagePack-eclipse-zh_3.6.0.v***.zip直接解压缩，得到一个名称为eclipse的目录，里面包含两个文件夹：features和plugins，将它们选中并复制到Eclipse安装目录中覆盖其中的features和plugins就完成汉化；



####  ●集成Eclipse和Tomcat：

在开发过程中，手动的部署和运行Web项目，过程繁琐，效率低下。所以需要将Tomcat服务器配置到Eclipse中，为Web项目指定一个Web应用服务器。这样就可以在Eclipse中操作Tomcat，并自动部署和运行Web项目。配置步骤如下：



在Eclipse中，窗口——首选项——服务器(Server)——运行时环境(Runtime Environments) ——添加(Add)，添加Tomcat服务器。对应安装的Tomcat版本选择Apache Tomcat v6.0。下一步通过“浏览(Brower)”按钮选择之前Tomcat的安装目录，指定后点击“完成”完成配置。指引参考图如下：






![集成eclipse与tomcat01](http://hi.csdn.net/attachment/201105/7/0_13047650418gJ3.gif)![集成eclipse与tomcat02](http://hi.csdn.net/attachment/201105/7/0_1304765111aZU8.gif)

![集成eclipse与tomcat03](http://hi.csdn.net/attachment/201105/7/0_1304765253Ee5l.gif)



****

****

####  ●为Eclipse指定Web浏览器********









打开Eclipse，窗口——首选项——常规——Web浏览器——选中使用外部Web浏览器——勾选Internet Explorer——确定。


********
####  ●指定Eclipse中JSP页面的编码格式********







默认情况下，在Eclipse中创建JSP页面是“ISO-8859-1”编码格式。该格式不支持中文字符集，编写中会出现乱码，所以需要为其指定一个支持中文的字符集。指定方法如下：

打开Eclipse，窗口——首选项——Web——JSP文件——编码(Encoding)——下拉选择ISO 10646/Unicode(UTF-8)



![eclipse JSP页面编码格式](http://hi.csdn.net/attachment/201105/7/0_1304765685ngiG.gif)



************
****************
####  ●测试Java Web项目Hello World

1）、文件——新建——动态Web项目——输入项目名(如1.1)——完成，项目创建成功；

2）、项目资源管理器——选中项目(如1.1)——WebContent节点——右键——新建——JSP文件——输入文件名(如index.jsp)——完成，成功新建JSP文件；

3）、打开index.jsp页面的代码窗口，编写输出“Hello World !”的代码，如下所示：








**[xhtml]**[view plain](http://blog.csdn.net/cxwen78/article/details/6400798#)[copy](http://blog.csdn.net/cxwen78/article/details/6400798#)[print](http://blog.csdn.net/cxwen78/article/details/6400798#)[?](http://blog.csdn.net/cxwen78/article/details/6400798#)

- **<**%@ page language="java"contentType="text/html; charset=UTF-8"
- pageEncoding="UTF-8"%**>**
- <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd"**>**
- **<html>**
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=UTF-8">
- <title>Hello World</title>
- </head>
- <body>
- <center>Hello World !</center>
- </body>
- </html>

<%@ page language="java" contentType="text/html; charset=UTF-8"    pageEncoding="UTF-8"%><!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd"><html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8"><title>Hello World</title></head><body><center>Hello World !</center></body></html>







4）、编写完成后，保存，在工具栏单击“![](http://hi.csdn.net/attachment/201105/7/0_13047664616CuX.gif)”的黑三角，在弹出的快捷菜单中选择“运行方式在服务器上运行”，在打开的对话框中，勾选“将服务器设置为项目默认值”复选框，单击“完成”按钮，即可通过Tomcat运行该项目，弹出的浏览器显示“Hello World !”字样，well done。相关步骤操作截图如下：



![新建Java Web项目01](http://hi.csdn.net/attachment/201105/7/0_13047665560Be1.gif)



![添加JSP文件](http://hi.csdn.net/attachment/201105/7/0_1304766621lD3d.gif)



![JSP文件命名](http://hi.csdn.net/attachment/201105/7/0_1304766710jkxA.gif)



![hello world](http://hi.csdn.net/attachment/201105/7/0_1304766772F7T7.gif)























## 三、小结



### 呼呼...总算完成了！虽然很基础，但是磨刀不误砍柴工，行动GO GO GO...



