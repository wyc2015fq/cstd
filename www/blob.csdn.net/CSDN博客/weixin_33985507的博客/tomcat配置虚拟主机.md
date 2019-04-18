# tomcat配置虚拟主机 - weixin_33985507的博客 - CSDN博客
2009年08月12日 07:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
**一、网站系统的组成**只要在一台计算机上安装了WEB服务器软件，从功能上讲，这台计算机就可以称为WEB服务器。一个网站的规模可大可小，功能可多可少，最简单的网站只需要一台Web服务器即可对外提供网页浏览服务。复杂的网站包括多台WEB服务器组成的群集系统、负载均衡设备、具有缓存功能的代理服务器（可以有多级，甚至包括放置在服务器端的缓存系统）、数据库系统等，如图2.2所示。
![](https://images.cnblogs.com/cnblogs_com/xieduo/i8.jpg)
 图2.2
www.sina.com网站系统采用的基本上就是图2.2所示的架构，不同地区的人们在访问www.sina.com站点时，浏览器实际上所访问的服务器是不一样的，例如，吉林省的用户访问的服务器实际是sina放在吉林地区的代理服务器，湖北省的用户访问的服务器实际是sina放在湖北地区的代理服务器。各地区的浏览器访问[www.sina.com](http://www.sina.com/)站点的过程如图2.3所示。
![](https://images.cnblogs.com/cnblogs_com/xieduo/i9.jpg)
 图2.3
为了能够让浏览器透明地访问到WEB站点，让用户感觉不到是在访问区域代理服务器，在DNS系统中需要将www.sina.com主机名指向所有的区域代理服务器的IP地址。在浏览器访问www.sina.com站点中的页面而向DNS服务器请求解析www.sina.com主机名时，DNS服务器根据访问者的地理位置信息返回他附近的区域代理服务器的IP地址，这样，浏览器的访问请求将发送给该区域代理服务器。只有当区域代理服务器中没有浏览器要访问的页面时，区域代理服务器才去从真正的www.sina.com站点服务器上获取该页面并进行缓存，以后该区域的其他浏览器就都可以就近从区域代理服务器中访问到该页面了，从而大大提高了访问效率和减少了网络流量。
WEB浏览器与WEB服务器建立连接后，除了将请求URL中的资源路径发送给WEB服务器外，还会将URL中的主机名部分作为HTTP请求消息的Host头发送给WEB服务器。例如，在浏览器地址栏中输入http://www.it315.org，浏览器发送给www.it315.org主机上的WEB服务器的请求消息内容如下：
 GET / HTTP/1.1<回车>
 Host: [www.it315.org](http://www.it315.org/)<回车>
 <回车>
WEB服务器接收到浏览器的访问请求消息后，根据Host头字段中所设置的主机名，就知道该选择哪个WEB站点来进行响应，因此，可以使用不同的主机名来作为区分同一个WEB服务器上的不同站点的标识信息。
Tomcat的Server.xml配置文件中有一个<Host>元素，一个<Host>元素用于建立一个WEB站点，使用多个<Host>元素则可以建立多个WEB站点。<Host>元素的父级元素为<Engine>元素，嵌套在同一个<Engine>元素中的多个<Host>元素的name属性不能相同，<Host>元素的name属性指定WEB站点所对应的主机名称。Tomcat的Server.xml配置文件中初始设置的<Host>元素内容如下：
<Host appBase="webapps" …>
…
</Host>
<Host>元素中的appBase属性指定了一个路径，该路径将作为嵌套在它里面的<Context>元素的docBase属性中设置的相对路径的基准路径。
当Tomcat接收到访问请求时，将比较请求消息中的Host头字段的值与<Host>元素的name属性值，并以匹配的<Host>元素所创建的WEB站点来响应。如果Server.xml文件中没有与请求消息的Host头字段匹配的<Host>元素，Tomcat将以默认的WEB站点来响应。只要<Engine>元素的defaultHost属性设置为嵌套在它里面的某个<Host>元素的name属性值，该<Host>元素所创建的WEB站点就成了该引擎的默认WEB站点。例如，Tomcat的Server.xml文件中的<Engine>元素的默认设置如下：
<Engine defaultHost="localhost" debug="0">
…
 <Host name="localhost" appBase="webapps" …>
 …
 </Host>
…
</Engine>
上面的这段配置信息说明，该引擎的默认WEB站点为嵌套在<Engine>元素中的name属性为“localhost”的<Host>元素所创建的WEB站点。
在同一台计算机上建立了多个基于主机名的虚拟主机后，WEB浏览器要访问其中的某个虚拟主机的资源时，在访问URL中必须采用主机名，而不能采用IP地址。这是因为WEB浏览器要将URL中的主机名部分作为HTTP请求消息的Host头发送给WEB服务器，如果URL中的主机名部分使用的是IP地址，那么，浏览器发出的请求消息中的Host头字段的值就是这个IP地址，而在同一台计算机上建立的多个基于主机名的虚拟主机共享同一个IP地址，在Host头字段使用IP地址根本就无法区分不同的站点。
即使在URL中指定的是主机名时，WEB浏览器还是要先获得该主机名所对应的IP地址，然后再使用这个IP去连接WEB服务器。所以，在建立基于主机名的虚拟主机时，除了要在Tomcat的server.xml文件中进行设置外，还需要在整个网络系统中建立主机名与IP地址的映射关系，即必须将主机名添加到名称解析系统，以便WEB浏览器能够从名称解析系统中查询出主机名所对应的IP地址。建立主机名与IP地址的映射关系的惯用方式有两种：一是使用客户机本地的Hosts 文件，二是使用DNS(Domain Name System，域名系统)服务器。Hosts 文件和DNS的作用都是允许用户使用“友好”的、文本格式的主机名称，而不是数字格式的IP地址来访问网络中的计算机。Hosts文件可用于小型的Intranet（企业内部网），网络中的所有计算机上都需要使用Hosts文件。DNS通常用于大型的网络，特别是Internet上对外提供服务的计算机都是通过DNS来建立其主机名与IP地址的映射关系。客户机首先在本地的Hosts文件中查找主机名称所映射的IP地址，如果没有找到，再去查询DNS服务器。为了简单起见，这里仅介绍一下Hosts文件。对于Windows 2000系统，Hosts 文件位于操作系统根目录（取决操作系统所在的分区，通常是c:\winnt）下的System32\Drivers\Etc子目录中，默认情况下，该文件中有如下一行内容：
 127.0.0.1
 localhost
这行文本的作用就是将IP地址（127.0.0.1）映射成主机名（localhost），这也就是在IE浏览器地址栏中可以使用localhost访问本地WEB服务器的原因。如果要增加更多的主机名与IP地址的映射，可以在Hosts文件中增加更多的行，然后参照上面这行内容的格式在每行中填写IP地址和相应的主机名。
 :动手体验：使用Tomcat建立基于主机名的虚拟主机
（1）用UltraEdit打开<Tomcat主目录>/conf目录下的Server.xml文件，使用“查找”菜单查找内容为“</Host>”的行，紧接该行下面增加一对<Host></Host>标签。参照前面的<Host>标签的属性设置情况，设置新增的<Host>标签的属性，并在它里面嵌套一个设置该WEB站点根目录的<Context>元素，最终的内容如下：
 <Host name="site1" debug="0" appBase="d:\VirtualHost1">
 <Context path="" docBase="." debug="0"/>
 </Host>
这样，将创建一个新的WEB站点。上面的<Context>元素的docBase属性值被设置为一个点（.），即表示使用<Host>元素的appBase属性中所设置的路径作为这个<Context>所映射的目录。
（2）在上面新增的<Host></Host>标签对下面再增加一对<Host></Host>标签，并将它设置为如下形式：
 <Host name="site2" debug="0" appBase="d:\VirtualHost2">
 <Context path="" docBase="." debug="0"/>
 </Host>
这又创建了一个新的WEB站点，该站点的主机名称为site2，根目录对应的本地文件系统目录为d:\VirtualHost2。
（3）在d:盘下创建名称为VirtualHost1和VirtualHost2两个目录，并在这两个目录中分别创建一个名为test.html的文件，在d:\VirtualHost1\test.html文件中写入如下一行内容：
***这是*d:\VirtualHost1*目录中的test.html文件***
在d:\VirtualHost2\test.html文件中写入如下一行内容：
***这是*d:\VirtualHost2*目录中的test.html文件***
（4）保存修改后的Server.xml文件，重新启动Tomcat WEB服务器程序。打开一个新的命令行窗口中，并在这个命令行窗口中执行如下命令：
telnet 127.0.0.1 8080
接着在连接成功的telnet程序命令窗口中，输入如下内容：
 GET /test.html HTTP/1.1<回车>
 Host:<空格>site1<回车>
 <回车>
这时在telnet程序窗口中可以看到，WEB服务器返回内容的正文部分为d:\VirtualHost1\test.html文件中的内容。接着WEB服务器返回内容的下边，输入如下内容：
 GET /test.html HTTP/1.1<回车>
 Host:<空格>site2<回车>
 <回车>
在telnet程序窗口中又可以看到，WEB服务器这次返回内容的正文部分为d:\VirtualHost2\test.html文件中的内容。接着WEB服务器返回内容的下边，输入如下内容：
 GET /test.html HTTP/1.1<回车>
 Host:<空格><回车>
 <回车>
在telnet程序窗口中又可以看到，WEB服务器这次返回内容的正文部分为d:\test\test.html文件中的内容。接着WEB服务器返回内容的下边，再次输入如下内容：
 GET /test.html HTTP/1.1<回车>
 Host:<空格>xxx<回车>
 <回车>
在telnet程序窗口中又可以看到，WEB服务器这次返回内容的正文部分仍然为d:\test\test.html文件中的内容。
上面的整个交互过程如图2.34所示，Tomcat根据第1次和第2次请求中所指定Host头的值，查找Server.xml文件中与之相匹配的<Host>元素的name属性值，并以匹配的<Host>元素所设置的WEB站点来响应；在第3次和第4次请求中所指定Host头的值，在Server.xml文件中没有与之对应的<Host>元素的name属性值，Tomcat将以默认的WEB站点来响应。
![](https://images.cnblogs.com/cnblogs_com/xieduo/i10.jpg)
 图2.34
（5）在Windows 2000系统中，用记事本程序打开c:\winnt\System32\Drivers\Etc\Hosts文件，可以看到有如下一行内容：
 127.0.0.1
 localhost
紧接这行文本的下面，用它复制出两行文本，并将这两行文本中的localhost分别修改成site1和site2，这样，就使用Hosts文件为当前计算机设置了多个主机名。如果要用site1和site2这个两主机名访问其他计算机上的WEB站点，则应将127.0.0.1修改成其他计算机的实际IP地址。保存Hosts文件后，在命令行窗口中执行“ping site1”和“ping site2”命令，查看该主机名是否被正确解析到了相应的IP地址上。建立site1和site2与计算机IP地址的映射后，在IE浏览器地址栏中输入http://localhost:8080/test.html、http://site1:8080/test.html和http://site2:8080/test.html，可以看到浏览器将显示出各自站点中的test.html网页文件的内容。
可见，使用主机名的方式在同一台WEB服务器上创建多个虚拟主机后，在WEB浏览器中使用主机名访问Web服务器时，Web服务器将选择与该主机名关联的WEB站点进行响应。通过这种方式，多个WEB站点可以共享同一个IP地址和相同的端口号，唯一不足的就是WEB浏览不能通过IP地址去访问这些WEB站点。基于主机名的虚拟主机是目前Internet上的大多数虚拟主机业务提供商所通常采用的方式。只要找出几个小型公司或个人的网站，如果在WEB浏览器地址栏中输入“http://主机名”可以访问到该WEB站点，接着在命令行窗口中执行“ping 主机名”，查看到该域名对应的IP地址，然后在WEB浏览器地址栏中输入“http://IP地址”时，却无法访问这个WEB站点了，那么，这个WEB站点就是一个基于主机名的虚拟主机，它与其他一些WEB站点共享一台WEB服务器，而不是自己独享一台WEB服务器。
注意：当使用安全套接字层 (SSL) 时，不能使用主机头字段来实现虚拟主机，这是因为使用SSL的HTTP请求有加密保护。主机头字段是加密内容的一部分，不能被解释和路由到正确的站点。
