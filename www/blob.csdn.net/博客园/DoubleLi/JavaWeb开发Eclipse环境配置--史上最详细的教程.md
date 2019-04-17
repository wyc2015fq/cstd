# JavaWeb开发Eclipse环境配置--史上最详细的教程 - DoubleLi - 博客园






【前言】

JSP本身是JavaWeb中的知识，但是在学习Android网络时，必然要涉及到与服务器之间的交互，所以学一下JSP以及其他JavaWeb的内容还是很有必要的，至少能明白程序在访问服务器时，整个过程的原理。

其实，在学习Android之前，Java和JavaWeb的知识都是要先学习的。本人是在2014年7月正式开始Android方向的研究学习，在这之前没有接触任何和计算机软件相关的知识（唯一相关的是，本科学过一门C语言课程，不过现在已经忘光了）。

我们来看下面的这张图就知道了：

![](https://images0.cnblogs.com/blog/641601/201410/232052552158804.png)

![](https://images0.cnblogs.com/blog/641601/201410/232056133557672.png)

两张图拼起来看就对了，借鉴的是李刚的“疯狂Java学习路线图”，手机像素渣，图片不是很清晰，将就一下吧。说白了，在学习Android之前要具备一定的Java基础（Java SE 、JavaWeb、数据库等）。而如果要学的扎实并且有个好的就业岗位，以下基础知识需要全部具备：**计算机基础知识（操作系统、计算机网络、数据结构、数据库、设计模式等）、C/C++、Java、Android、Linux**。

所以，考虑到时间的因素，像我这种初学者，只能在边学Android的时候边学其他的知识。**我学到了哪里，我的博客就会写哪里**，也希望能和其他初学者一起共勉，一起见证！



【正文】

**一、JSP简介**

JSP：Java Server Pages。在传统的HTML文件(*htm,*.html)中加入Java程序片段(Scriptlet)和JSP标记(tag)，就构成了JSP网页。



**二、学习重点：**
- JSP语法（脚本、指令、动作）
- JSP的内置对象
- 创建动态的内容
- 用户会话跟踪



**三、****使用tomcat软件在本地搭建服务器：**

Tomcat是Apache组织的Jakarta项目中的一个重要子项目，是Sun公司推荐的运行Servlet和JSP的容器(引擎)，其源代码完全公开。

有了这个服务器，就相当于在本地的计算机有了一个网站，然后我们可以通过浏览器来访问这个网站。

tomcat软甲是apache旗下的一个开源项目。软件下载链接：[http://tomcat.apache.org/](http://tomcat.apache.org/)

![](https://images0.cnblogs.com/blog/641601/201410/232104286524890.png)

下载之后，将压缩包解压：

![](https://images0.cnblogs.com/blog/641601/201410/232104410586765.png)

注意目录名不能有中文和空格。目录介绍如下：
- bin：二进制执行文件。里面最常用的文件是**startup.bat**
- conf:配置目录。里面最核心的文件是**server.xml**。可以在里面改端口号等。默认端口号是8080，也就是说，此端口号不能被其他应用程序占用。
- lib：库文件。tomcat运行时需要的jar包所在的目录
- logs：日志
- temp：临时产生的文件，即缓存
- webapps：web的应用程序。web应用放置到此目录下浏览器可以直接访问
- work：编译以后的class文件。

软件运行之前要保证Java环境变量已经配置：

![](https://images0.cnblogs.com/blog/641601/201410/232105017932166.jpg)

上图中，变量名为JAVA_HOME，变量值为：JDK安装的绝对路径。

注：Catalina_Home环境变量:指定tomcat在启动时启动哪个tomcat,一般不推荐配置。

回到tomcat的bin目录中, 双击startup.bat：

![](https://images0.cnblogs.com/blog/641601/201410/232105246057262.png)

之后弹出如下界面：

![](https://images0.cnblogs.com/blog/641601/201410/232105392158264.png)

这个时候，本地的服务器就已经搭建起来了。**如果想关闭服务器**，可以直接关闭上面的窗口，或者在里面输入Ctrl+C禁止服务。

首先查看自己电脑的ip地址，我的计算机的ip地址为：192.168.1.112。

在浏览器中输入http://192.168.1.112:8080/（或者输入http://localhost:8080/也是可以的）。如果弹出如下界面，进入本地服务器的首页，表示tomcat安装成功并且启动起来了：

![](https://images0.cnblogs.com/blog/641601/201410/232106014657934.jpg)

上方图片中，显示我的Tomcat的版本为：8.0.14。它的版本号是跟着JDK的版本走的，所以，建议JDK的版本为1.8。

我们现在在浏览器上测试一下它吧：

首先在D:\apache-tomcat-8.0.14\webapps\ROOT目录中新建一个jsp文件：

![](https://images0.cnblogs.com/blog/641601/201410/232106198557249.jpg)

jsp文件中填入如下内容：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)


![复制代码](https://common.cnblogs.com/images/copycode.gif)

<%

String name = request.getParameter("name");
String pwd = request.getParameter("password"); 

out.print("name:" + name + ",password:" + pwd); //在浏览器上显示输入地址中的用户名与密码

%> 

![复制代码](https://common.cnblogs.com/images/copycode.gif)


![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

现在我们随便起一个用户名和密码的账号，比如用户名smyhvae，密码为smyh，然后在浏览器中输入如下内容：

http://192.168.1.112:8080/test.jsp?name=smyhvae&password=smyh

输入这个链接之后，回车，出现如下界面：

![](https://images0.cnblogs.com/blog/641601/201410/232110160433561.jpg)

上图中表示，我们向服务器发送这样一个请求，链接中，问号前面的内容代表请求的路径，问号后面是我们要传送的参数（键是固定不变的，值是用户自己填写的），然后服务器返还给我们这样的数据。



**三、将Tomcat和eclipse相关联：**

打开eclipse for Java EE ，选择菜单栏Windows-->preferences，弹出如下界面：

![](https://images0.cnblogs.com/blog/641601/201410/232111301681549.png)

上图中，点击“add”的添加按钮，弹出如下界面：

![](https://images0.cnblogs.com/blog/641601/201410/232111442933866.png)

![](https://images0.cnblogs.com/blog/641601/201410/232112054656263.png)

上图中，选择对应的Tomcat版本，继续：

![](https://images0.cnblogs.com/blog/641601/201410/232112245587963.png)

上图中，选择Tomcat的路径，以及JRE，点击“完成”，配置完毕。

新建java工程，建一个动态的工程：

![](https://images0.cnblogs.com/blog/641601/201410/232112541213100.png)

注：对“动态”的理解：html是静态的，写成什么，就是什么。动态指的是根据服务器端返回的数据动态地生成页面。比如张三登陆可以看到张三的信息；换成李四登陆，可以看到李四的信息。

![](https://images0.cnblogs.com/blog/641601/201411/302044303245040.png)

点开上图中的红框部分，弹出如下界面：

![](https://images0.cnblogs.com/blog/641601/201411/302045039346772.png)

按照上图进行配置，其中，第三个红框中，是加载自己的jdk的安装路径即可:

![](https://images0.cnblogs.com/blog/641601/201411/302045252932113.png)

然后，单击finish。继续：



![](https://images0.cnblogs.com/blog/641601/201410/232113121219000.png)

![](https://images0.cnblogs.com/blog/641601/201410/232113256216676.png)

工程文件结构：

![](https://images0.cnblogs.com/blog/641601/201410/232113367466511.png)

上图中，deployment descriptor：部署的描述。Web App Libraries：自己加的包可以放在里面。build：放入编译之后的文件。WebContent:放进写入的页面。

在WebContent文件夹下新建一个jsp文件。在下图中可以看到它的默认代码：

![](https://images0.cnblogs.com/blog/641601/201410/232115106372126.png)

上图中，这种编码方式不支持中文。我们来修改一下JSP文件的编码方式，按上图所示，鼠标右击，选择"Preferences"，弹出如下对话框：

![](https://images0.cnblogs.com/blog/641601/201410/232116185271919.png)

上图中，将编码方式改为UTF-8。

同样，我们还需要将文本内容的编码方式改为UTF-8（这个编码和程序无关），选择菜单栏Windows--preferences，打开如下界面，将编码方式改为UTF-8，并点击update：

![](https://images0.cnblogs.com/blog/641601/201410/311039297063444.png)

以后每次新建一个jsp文件，默认的编码方式就是UTF-8了。默认代码如下：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)


![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 <%@ page language="java" contentType="text/html; charset=UTF-8"
 2     pageEncoding="UTF-8"%>
 3 <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
 4 <html>
 5 <head>
 6 <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
 7 <title>Insert title here</title>
 8 </head>
 9 <body>
10 
11 </body>
12 </html>

![复制代码](https://common.cnblogs.com/images/copycode.gif)


![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

我们在hello.jsp中修改一下，将上方的第7行的标题改一下，并在第10行添加输出语句。最终代码如下：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)


![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 <%@ page language="java" contentType="text/html; charset=UTF-8"
 2     pageEncoding="UTF-8"%>
 3 <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
 4 <html>
 5 <head>
 6 <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
** 7 <title>hello JSP</title>**
 8 </head>
 9 <body>
**10     <%11         out.println("hello,JSP");12     %>**
13 </body>
14 </html>

![复制代码](https://common.cnblogs.com/images/copycode.gif)


![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

上方的第11行，out指的是输出流。通过页面获取这个输出流，并打印在页面中。以前学的System.out.println()指的是向控制台输出。

程序运行之前，我们先修改一下浏览器选项:

![](https://images0.cnblogs.com/blog/641601/201410/232119394023584.png)

现在我们开始运行程序：

![](https://images0.cnblogs.com/blog/641601/201410/232119524331103.png)

运行时，弹出如下错误：(如果没有此错误，请忽略)

![](https://images0.cnblogs.com/blog/641601/201410/232120047932694.png)

原因是，我们之前点击了Tomcat安装包中的​startup.bat，这样一来就手动打开了Tomcat服务器，这明显是多余的，因为程序运行时，eclipse会自动开启Tomcat服务器。所以我们先**手动关掉tomcat软件**，再次运行程序，就行了。控制台信息如下：

![](https://images0.cnblogs.com/blog/641601/201410/232120441212179.png)

![](https://images0.cnblogs.com/blog/641601/201410/232120199803353.png)

浏览器会自动打开，网页信息如下：

![](https://images0.cnblogs.com/blog/641601/201410/232120597936224.png)

现在来解释一下上方网址的名称为什么显示的是 http://localhost:8080/TomcatTest/

我们选中项目，右键选择“properties”，弹出如下对话框：

![](https://images0.cnblogs.com/blog/641601/201410/232121476835759.png)

上图显示，我们所部署的路径是根目录，根目录的名称默认为我们新建的项目名，所以网址才会显示为：主机名+端口号+项目名。

网页显示的错误为404，即找不到网页，可见网页中并没有看到我们新建的jsp文件，我们再来找一下原因。打开工程文件中，WEB-INF目录下的web.xml文件：

![](https://images0.cnblogs.com/blog/641601/201410/232122567934080.png)

上图解释：当程序运行时，Tomcat会首先读取工程的配置文件，且名字必须为web.xml。当系统默认进入的链接为：主机名+端口+工程名时，服务器就会找上图中<welcome-file-list>标签里的页面（有好几个页面的话，就依次往下找）；而标签<welcome-file-list>中并没有hello.jsp文件。所以，我们需要在浏览器地址栏输入：http://localhost:8080/TomcatTest/hello.jsp，才会将hello.jsp页面显示出来。效果如下：

![](https://images0.cnblogs.com/blog/641601/201410/232124339491865.png)

**四、程序运行的原理：**

我们现在来分析一下上面的程序运行的原理。

当在服务器上运行后，会生成与工程文件并列的一个文件夹：Servers。如下：（如果删掉了Servers文件夹，当重新运行时，文件夹又会自动生成）

![](https://images0.cnblogs.com/blog/641601/201410/232124450741700.png)

这个文件夹是Tomcat服务器的一个基本的配置。

![](https://images0.cnblogs.com/blog/641601/201410/232124553407962.png)

上图中表明，我们新建的项目已经部署到Tomcat服务器上去了，也就是看到了TomcatTest这个工程被发布出去了（发布的过程即：将写的工程打包以后放到Tomcat里）。

其实eclipse for EE已经包含了Tomcat服务的插件，但其也必须依赖Tomcat来启动。我们双击上图的红框部分，显示如下信息：

![](https://images0.cnblogs.com/blog/641601/201410/232125204187102.png)

上图的红框部分表明，服务的部署是在eclipse里面（默认是放在工作空间里的.metadata文件夹下），而不是在Tomcat里面。我们来改一下，前提是工程并没有发布到Tomcat中去，那我们先把之前发布的版本删了吧（稍后重新发布）：

![](https://images0.cnblogs.com/blog/641601/201410/232125525273092.png)

然后就可以修改部署的路径了：

![](https://images0.cnblogs.com/blog/641601/201410/232126056684811.png)

上图中，使用Tomcat的安装目录作为部署的位置，并修改部署的路径Deploy path（建议改成Tomcat的webapps目录下），然后保存即可。这时，**重新运行程序**。现在来到Tomcat的webapps目录下，发现多了一个TomcatTest文件夹（即工程文件名），并且文件夹下包含了工程文件中WebContent的内容：

![](https://images0.cnblogs.com/blog/641601/201410/232126293873764.png)

上图说明，说明这才是真正将程序发布到服务器上。

我们再来到Tomcat的work目录中，看一下编译之后的文件：

![](https://images0.cnblogs.com/blog/641601/201410/232126434804325.png)​

上图表明，**Tomcat会先将jsp文件转为java文件，然后再把java文件编译为class文件，最终执行的是class文件**。现在来看一下JSP运行的原理。



**四、JSP的运行原理：**
- 只有当客户端第一次请求JSP时，才需要将其转换、编译（所以第二次浏览同样的网页时，速度会更快）
- Web服务器在遇到访问JSP网页的请求时，首先执行其中的程序片段，然后将执行结果以HTML格式返回给客户。
- 程序片段可以操作数据库、重新定向网页以及发送email等等，这就是建立动态网站所需要的功能。
- 所有程序操作都在服务器端执行，网络上传送给客户端的仅是得到的结果，对客户浏览器的要求最低。

总结：本文的整个过程，让我们学会了如何配置Tomcat和部署工程文件，明白了jsp文件是怎样发布到服务器上并最终显示出来。关于JSP的进一步学习，将在后面呈现。







**五、Tomcat的其他问题：**

**1、端口占用问题：**

在cmd中输入**netstat -ano**命令，查看占用端口的进程pid，再用任务管理器关闭相应进程即可。

我们在浏览器中输入"www.baidu.com"，但是并没有输入端口号依然能进入网页，这是因为浏览器默认的端口号为80，如果对方服务器是监听在80端口上，则在浏览器中输入网址时，可以不用输入端口号。

Tomcat默认监听的端口号是8080（server.xml文件的第63行），可以在配置文**件conf/server.xml**中修改**。**

【备注】Tomcat关联帮助文档Javadoc

我们以后如果要使用到Servlet类，但是想查看里面的源码和帮助文档，发现看不到：

![](https://images0.cnblogs.com/blog/641601/201411/211920478439210.png)

按住ctrl点进去之后，是下面的界面：

![](https://images0.cnblogs.com/blog/641601/201411/211922135627189.png)

我们下载的tomcat中自带了源码，但是帮助文档需要另外下载：









