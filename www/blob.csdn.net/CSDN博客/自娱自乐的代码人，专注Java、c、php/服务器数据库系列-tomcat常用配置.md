# 服务器数据库系列 - tomcat常用配置 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年02月27日 16:51:10[initphp](https://me.csdn.net/initphp)阅读数：1029







**1启动内存参数的配置**

　　tomcat/bin/catalina.bat 如果是linux 就是 catalina.sh

　　在rem 的后面增加如下参数

　　set JAVA_OPTS= -Xms256m -Xmx256m -XX:MaxPermSize=64m
**2 修改Tomcat的JDK目录**

　　打开tomcat/bin/catalina.bat

　　在最后一个rem后面增加

　　set JAVA_HOME=C:\Program Files\Java\jdk1.6.0
**3 增加虚拟目录**

　　/tomcat/conf/server.xml

　　第一行是以前默认存在的，第二行是新增的

　　<Context path="" docBase="ROOT" debug="0"reloadable="true"></Context>

　　<Context path="/jsp/a" reloadable="true"docBase="E:\workplace\www.java2000.net\WebContent"/>
**4 GET方式URL乱码问题解决**

　　打开 tomcat/conf/server.xml

　　查找下面这部分，在最后增加一段代码就可以了。

　　<Connector port="80"maxHttpHeaderSize="8192"

　　.................

　　URIEncoding="UTF-8" useBodyEncodingForURI="true"

　　...............

　　/>

　　其中的UTF-8 请根据你的需要自己修改，比如GBK
**5 虚拟主机配置文件**

　　tomcat/conf/server.xml

　　<!-- 默认的主机 -->

　　<Host name="localhost" appBase="webapps"

　　unpackWARs="true" 

　　xmlValidation="false"xmlNamespaceAware="false">

　　<Context path="" docBase="ROOT" debug="0"reloadable="true"></Context>

　　...

　　</host>

　　<!-- 以下是新增的虚拟主机 -->

　　<Host name="www.java2000.net"appBase="webapps"

　　unpackWARs="true" 

　　xmlValidation="false"xmlNamespaceAware="false">

　　<Context path="" docBase="d:/www.java2000.net"debug="0"reloadable="true"></Context>

　　<!-- 虚拟目录 -->

　　<Context path="/count"docBase="d:/counter.java2000.net" debug="0"reloadable="true"></Context>

　　</Host>

　　<Host name="java2000.net" appBase="webapps"

　　unpackWARs="true" 

　　xmlValidation="false"xmlNamespaceAware="false">

　　<Context path="" docBase="d:/www.java2000.net"debug="0"reloadable="true"></Context>

　　<Context path="/count"docBase="d:/counter.java2000.net" debug="0"reloadable="true"></Context>

　　</Host>
**6 数据源配置**

　　比较复杂，各个版本都有所不同，请直接查看http://java2000.net/p1906，包括tomcat5.0,tomcat5.5x,tomcat6.0的各个版本的配置方法。

　　更多关于Tomcat的使用，请看参考资料


## [[编辑本段](http://blog.sina.com.cn/s/blog_4ab26bdd0100gwpk.html#)]Tomcat配置的10个技巧

**1. 配置系统管理（Admin Web Application）**

　　大多数商业化的J2EE服务器都提供一个功能强大的管理界面，且大都采用易于理解的Web应用界面。Tomcat按照自己的方式，同样提供一个成熟的管理工具，并且丝毫不逊于那些商业化的竞争对手。Tomcat的Admin WebApplication最初在4.1版本时出现，当时的功能包括管理context、datasource、user和group等。当然也可以管理像初始化参数，user、group、role的多种数据库管理等。在后续的版本中，这些功能将得到很大的扩展，但现有的功能已经非常实用了。

　　Admin Web Application被定义在自动部署文件：CATALINA_BASE/webapps/admin.xml。

　　必须编辑这个文件，以确定Context中的docBase参数是绝对路径。也就是说，CATALINA_BASE/webapps/admin.xml的路径是绝对路径。作为另外一种选择，也可以删除这个自动部署文件，而在server.xml文件中建立一个Admin WebApplication的context，效果是一样的。不能管理Admin WebApplication这个应用，换而言之，除了删除CATALINA_BASE/webapps/admin.xml，可能什么都做不了。

　　如果使用UserDatabaseRealm（默认），将需要添加一个user以及一个role到CATALINA_BASE/conf/tomcat-users.xml文件中。你编辑这个文件，添加一个名叫“admin”的role 到该文件中，如下：

　　<role name=“admin”/>

　　同样需要有一个用户，并且这个用户的角色是“admin”。象存在的用户那样，添加一个用户（改变密码使其更加安全）：

　　<user name=“admin” password=“deep_dark_secret”roles=“admin”/>

　　当完成这些步骤后，请重新启动Tomcat，访问http://localhost:8080/admin，将看到一个登录界面。AdminWeb Application采用基于容器管理的安全机制，并采用了JakartaStruts框架。一旦作为“admin”角色的用户登录管理界面，将能够使用这个管理界面配置Tomcat。
**2.配置应用管理（Manager Web Application）**

　　Manager Web Application让你通过一个比Admin WebApplication更为简单的用户界面，执行一些简单的Web应用任务。

　　Manager Web Application被被定义在一个自动部署文件中：

　　CATALINA_BASE/webapps/manager.xml 。

　　必须编辑这个文件，以确保context的docBase参数是绝对路径，也就是说CATALINA_HOME/server/webapps/manager的绝对路径。

　　如果使用的是UserDatabaseRealm，那么需要添加一个角色和一个用户到CATALINA_BASE/conf/tomcat-users.xml文件中。接下来，编辑这个文件，添加一个名为“manager”的角色到该文件中：

　　<role name=“manager”>

　　同样需要有一个角色为“manager”的用户。像已经存在的用户那样，添加一个新用户（改变密码使其更加安全）：

　　<user name=“manager” password=“deep_dark_secret”roles=“manager”/>

　　然后重新启动Tomcat，访问http://localhost/manager/list，将看到一个很朴素的文本型管理界面，或者访问http://localhost/manager/html/list，将看到一个HMTL的管理界面。不管是哪种方式都说明你的ManagerWeb Application现在已经启动了。

　　Managerapplication可以在没有系统管理特权的基础上，安装新的Web应用，以用于测试。如果我们有一个新的web应用位于/home/user/hello下在，并且想把它安装到/hello下，为了测试这个应用，可以这么做，在第一个文件框中输入“/hello”（作为访问时的path），在第二个文本框中输入“file:/home/user/hello”（作为Config URL）。

　　Managerapplication还允许停止、重新启动、移除以及重新部署一个web应用。停止一个应用使其无法被访问，当有用户尝试访问这个被停止的应用时，将看到一个503的错误——“503 - This application is not currentlyavailable”。

　　移除一个web应用，只是指从Tomcat的运行拷贝中删除了该应用，如果重新启动Tomcat，被删除的应用将再次出现（也就是说，移除并不是指从硬盘上删除）。
**3.部署一个web应用**

　　有两个办法可以在系统中部署web服务。

　　1>拷贝WAR文件或者web应用文件夹（包括该web的所有内容）到$CATALINA_BASE/webapps目录下。

　　2>为web服务建立一个只包括context内容的XML片断文件，并把该文件放到$CATALINA_BASE/webapps目录下。这个web应用本身可以存储在硬盘上的任何地方。

　　如果有一个WAR文件，想部署它，则只需要把该文件简单的拷贝到CATALINA_BASE/webapps目录下即可，文件必须以“。war”作为扩展名。一旦Tomcat监听到这个文件，它将（缺省的）解开该文件包作为一个子目录，并以WAR文件的文件名作为子目录的名字。接下来，Tomcat将在内存中建立一个context，就好象在server.xml文件里建立一样。当然，其他必需的内容，将从server.xml中的DefaultContext获得。

　　部署web应用的另一种方式是写一个ContextXML片断文件，然后把该文件拷贝到CATALINA_BASE/webapps目录下。一个Context片断并非一个完整的XML文件，而只是一个context元素，以及对该应用的相应描述。这种片断文件就像是从server.xml中切取出来的context元素一样，所以这种片断被命名为“context片断”。

　　举个例子，如果我们想部署一个名叫MyWebApp.war的应用，该应用使用realm作为访问控制方式，我们可以使用下面这个片断：

　　<!--

　　Context fragment for deploying MyWebApp.war

　　-->

　　<Context path=“/demo”docBase=“webapps/MyWebApp.war”

　　debug=“0” privileged=“true”>

　　<RealmclassName=“org.apache.catalina.realm.UserDatabaseRealm”

　　resourceName=“UserDatabase”/>

　　</Context>

　　把该片断命名为“MyWebApp.xml”，然后拷贝到CATALINA_BASE/webapps目录下。

　　这种context片断提供了一种便利的方法来部署web应用，不需要编辑server.xml，除非想改变缺省的部署特性，安装一个新的web应用时不需要重启动Tomcat。
**4.配置虚拟主机（Virtual Hosts）**

　　关于server.xml中“Host”这个元素，只有在设置虚拟主机的才需要修改。虚拟主机是一种在一个web服务器上服务多个域名的机制，对每个域名而言，都好象独享了整个主机。实际上，大多数的小型商务网站都是采用虚拟主机实现的，这主要是因为虚拟主机能直接连接到Internet并提供相应的带宽，以保障合理的访问响应速度，另外虚拟主机还能提供一个稳定的固定IP。

　　基于名字的虚拟主机可以被建立在任何web服务器上，建立的方法就是通过在域名服务器（DNS）上建立IP地址的别名，并且告诉web服务器把去往不同域名的请求分发到相应的网页目录。

　　在Tomcat中使用虚拟主机，需要设置DNS或主机数据。为了测试，为本地IP设置一个IP别名就足够了，接下来，你需要在server.xml中添加几行内容，如下：

　　<Server port=“8005” shutdown=“SHUTDOWN”debug=“0”>

　　<Servicename=“Tomcat-Standalone”>

　　<ConnectorclassName=“org.apache.coyote.tomcat4.CoyoteConnector”

　　port=“8080” minProcessors=“5” maxProcessors=“75”

　　enableLookups=“true” redirectPort=“8443”/>

　　<ConnectorclassName=“org.apache.coyote.tomcat4.CoyoteConnector”

　　port=“8443” minProcessors=“5” maxProcessors=“75”

　　acceptCount=“10” debug=“0” scheme=“https”secure=“true”/>

　　<FactoryclassName=“org.apache.coyote.tomcat4.CoyoteServerSocketFactor

y”

　　clientAuth=“false” protocol=“TLS” />

　　</Connector>

　　<Engine name=“Standalone” defaultHost=“localhost”debug=“0”>

　　<!-- This Host is the default Host-->

　　<Host name=“localhost” debug=“0”appBase=“webapps”

　　unpackWARs=“true” autoDeploy=“true”>

　　<Context path=“” docBase=“ROOT”debug=“0”/>

　　<Context path=“/orders” docBase=“/home/ian/orders”debug=“0”

　　reloadable=“true” crossContext=“true”>

　　</Context>

　　</Host>

　　<!-- This Host is the first “Virtual Host”:www.example.com -->

　　<Host name=“www.example.com”appBase=“/home/example/webapp”>

　　<Context path=“”docBase=“.”/>

　　</Host>

　　</Engine>

　　</Service>

　　</Server>

　　Tomcat的server.xml文件，在初始状态下，只包括一个虚拟主机，但是它容易被扩充到支持多个虚拟主机。在前面的例子中展示的是一个简单的server.xml版本，其中粗体部分就是用于添加一个虚拟主机。每一个Host元素必须包括一个或多个context元素，所包含的context元素中必须有一个是默认的context，这个默认的context的显示路径应该为空（例如，path=“”）。
**5.配置基础验证（Basic Authentication）**

　　容器管理验证方法控制着当用户访问受保护的web应用资源时，如何进行用户的身份鉴别。当一个web应用使用了BasicAuthentication（BASIC参数在web.xml文件中auto-method元素中设置），而有用户访问受保护的web应用时，Tomcat将通过HTTP BasicAuthentication方式，弹出一个对话框，要求用户输入用户名和密码。在这种验证方法中，所有密码将被以64位的编码方式在网络上传输。

　　注意：使用BasicAuthentication通过被认为是不安全的，因为它没有强健的加密方法，除非在客户端和服务器端都使用HTTPS或者其他密码加密码方式（比如，在一个虚拟私人网络中）。若没有额外的加密方法，网络管理员将能够截获（或滥用）用户的密码。但是，如果是刚开始使用Tomcat，或者你想在你的web应用中测试一下基于容器的安全管理，BasicAuthentication还是非常易于设置和使用的。只需要添加<security-constraint>和<login-config>两个元素到web应用的web.xml文件中，并且在CATALINA_BASE/conf/tomcat-users.xml文件中添加适当的<role>和<user>即可，然后重新启动Tomcat。
**6.配置单点登录（Single Sign-On）**

　　一旦设置了realm和验证的方法，就需要进行实际的用户登录处理。一般说来，对用户而言登录系统是一件很麻烦的事情，必须尽量减少用户登录验证的次数。作为缺省的情况，当用户第一次请求受保护的资源时，每一个web应用都会要求用户登录。如果运行了多个web应用，并且每个应用都需要进行单独的用户验证，那这看起来就有点像在用户搏斗。用户们不知道怎样才能把多个分离的应用整合成一个单独的系统，所有用户也就不知道他们需要访问多少个不同的应用，只是很迷惑，为什么总要不停的登录。

　　Tomcat 4的“singlesign-on”特性允许用户在访问同一虚拟主机下所有web应用时，只需登录一次。为了使用这个功能，只需要在Host上添加一个SingleSignOnValve元素即可，如下所示：

　　<ValveclassName=“org.apache.catalina.authenticator.SingleSignOn”

　　debug=“0”/>

　　在Tomcat初始安装后，server.xml的注释里面包括SingleSignOnValve配置的例子，只需要去掉注释，即可使用。那么，任何用户只要登录过一个应用，则对于同一虚拟主机下的所有应用同样有效。

　　使用single sign-on valve有一些重要的限制：

　　1>value必须被配置和嵌套在相同的Host元素里，并且所有需要进行单点验证的web应用（必须通过context元素定义）都位于该Host下。

　　2> 包括共享用户信息的realm必须被设置在同一级Host中或者嵌套之外。

　　3> 不能被context中的realm覆盖。

　　4>使用单点登录的web应用最好使用一个Tomcat的内置的验证方式（被定义在web.xml中的<auth-method>中），这比自定义的验证方式强，Tomcat内置的的验证方式包括basic、digest、form和client-cert。

　　5>如果你使用单点登录，还希望集成一个第三方的web应用到你的网站中来，并且这个新的web应用使用它自己的验证方式，而不使用容器管理安全，那你基本上就没招了。用户每次登录原来所有应用时需要登录一次，并且在请求新的第三方应用时还得再登录一次。

　　6> 单点登录需要使用cookies。
**7.配置用户定制目录（Customized User Directores）**

　　一些站点允许个别用户在服务器上发布网页。例如，一所大学的学院可能想给每一位学生一个公共区域，或者是一个ISP希望给一些web空间给他的客户，但这又不是虚拟主机。在这种情况下，一个典型的方法就是在用户名前面加一个特殊字符（~），作为每位用户的网站，比如：

　　http://www.cs.myuniversity.edu/~username提供两种方法在主机上映射这些个人网站，主要使用一对特殊的Listener元素。Listener的className属性应该是org.apache.catalina.startup.UserConfig，userClass属性应该是几个映射类之一。如果电脑系统是Unix，它将有一个标准的/etc/passwd文件，该文件中的帐号能够被运行中的Tomcat很容易的读取，该文件指定了用户的主目录，使用PasswdUserDatabase
 映射类。

　　http://members.mybigisp.com/~username

　　Tomcat

　　<ListenerclassName=“org.apache.catalina.startup.UserConfig”

　　directoryName=“public_html”

　　userClass=“org.apache.catalina.startup.PasswdUserDatabase”/>

　　web文件需要放置在像/home/users/ian/public_html 或者/users/jbrittain/public_html一样的目录下面。当然你也可以改变public_html到其他任何子目录下。

　　实际上，这个用户目录根本不一定需要位于用户主目录下里面。如果你没有一个密码文件，但你又想把一个用户名映射到公共的像/home一样目录的子目录里面，则可以使用HomesUserDatabase类。

　　<ListenerclassName=“org.apache.catalina.startup.UserConfig”

　　directoryName=“public_html” homeBase=“/home”

　　userClass=“org.apache.catalina.startup.HomesUserDatabase”/>

　　这样一来，web文件就可以位于像/home/ian/public_html 或者/home/jasonb/public_html一样的目录下。这种形式对Windows而言更加有利，你可以使用一个像c:\home这样的目录。

　　这些Listener元素，如果出现，则必须在Host元素里面，而不能在context元素里面，因为它们都用应用于Host本身。
**8.在Tomcat中使用CGI脚本**

　　Tomcat主要是作为Servlet/JSP容器，但它也有许多传统web服务器的性能。支持通用网关接口（CommonGatewayInterface，即CGI）就是其中之一，CGI提供一组方法在响应浏览器请求时运行一些扩展程序。CGI之所以被称为通用，是因为它能在大多数程序或脚本中被调用，包括：Perl，Python，awk，Unix shellscripting等，甚至包括Java。不会把一个Java应用程序当作CGI来运行，毕竟这样太过原始。一般而言，开发Servlet总要比CGI具有更好的效率，因为当用户点击一个链接或一个按钮时，不需要从操作系统层开始进行处理。

　　Tomcat包括一个可选的CGI Servlet，允许你运行遗留下来的CGI脚本。
**为了使Tomcat能够运行CGI，必须做的几件事：**

　　1. 把servlets-cgi.renametojar（在CATALINA_HOME/server/lib/目录下）改名为servlets-cgi.jar。处理CGI的servlet应该位于Tomcat的CLASSPATH下。

　　2. 在Tomcat的CATALINA_BASE/conf/web.xml文件中，把关于<servlet-name>CGI的那段的注释去掉（默认情况下，该段位于第241行）。

　　3.同样，在Tomcat的CATALINA_BASE/conf/web.xml文件中，把关于对CGI进行映射的那段的注释去掉（默认情况下，该段位于第299行）。注意，这段内容指定了HTML链接到CGI脚本的访问方式。

　　4. 可以把CGI脚本放置在WEB-INF/cgi目录下（注意，WEB-INF是一个安全的地方，你可以把一些不想被用户看见或基于安全考虑不想暴露的文件放在此处），或者也可以把CGI脚本放置在context下的其他目录下，并为CGI Servlet调整cgiPathPrefix初始化参数。这就指定的CGIServlet的实际位置，且不能与上一步指定的URL重名。

　　5. 重新启动Tomcat，你的CGI就可以运行了。

　　在Tomcat中，CGI程序缺省放置在WEB-INF/cgi目录下，正如前面所提示的那样，WEB-INF目录受保护的，通过客户端的浏览器无法窥探到其中内容，所以对于放置含有密码或其他敏感信息的CGI脚本而言，这是一个非常好的地方。为了兼容其他服务器，尽管你也可以把CGI脚本保存在传统的/cgi-bin目录，但要知道，在这些目录中的文件有可能被网上好奇的冲浪者看到。另外，在Unix中，请确定运行Tomcat的用户有执行CGI脚本的权限。
**　9.改变Tomcat中的JSP编译器（JSP Compiler）**

　　在Tomcat4.1（或更高版本，大概），JSP的编译由包含在Tomcat里面的Ant程序控制器直接执行。这听起来有一点点奇怪，但这正是Ant有意为之的一部分，有一个API文档指导开发者在没有启动一个新的JVM的情况下，使用Ant。这是使用Ant进行Java开发的一大优势。另外，这也意味着你现在能够在Ant中使用任何javac支持的编译方式，这里有一个关于Apache Ant使用手册的javacpage列表。使用起来是容易的，因为你只需要在<init-param>元素中定义一个名字叫“compiler”，并且在value中有一个支持编译的编译器名字，示例如下：

　　<servlet>

　　<servlet-name>jsp</servlet-name>

　　<servlet-class>

　　org.apache.jasper.servlet.JspServlet

　　</servlet-class>

　　<init-param>

　　<param-name>logVerbosityLevel</param-name>

　　<param-value>WARNING</param-value>

　　</init-param>

　　<init-param>

　　<param-name>compiler</param-name>

　　<param-value>jikes</param-value>

　　</init-param>

　　<load-on-startup>3</load-on-startup>

　　</servlet>

　　当然，给出的编译器必须已经安装在你的系统中，并且CLASSPATH可能需要设置，那处决于你选择的是何种编译器。
**10.限制特定主机访问（Restricting Access to Specific Hosts）**

　　有时，可能想限制对Tomcatweb应用的访问，比如，希望只有指定的主机或IP地址可以访问应用。这样一来，就只有那些指定的的客户端可以访问服务的内容了。为了实现这种效果，Tomcat提供了两个参数供你配置：RemoteHostValve 和RemoteAddrValve。

　　通过配置这两个参数，可以让你过滤来自请求的主机或IP地址，并允许或拒绝哪些主机/IP。与之类似的，在Apache的httpd文件里有对每个目录的允许/拒绝指定。

　　可以把Admin Web application设置成只允许本地访问，设置如下：

　　<Context path=“/path/to/secret_files”…>

　　<ValveclassName=“org.apache.catalina.valves.RemoteAddrValve”

　　allow=“127.0.0.1” deny=“”/>

　　</Context>

　　如果没有给出允许主机的指定，那么与拒绝主机匹配的主机就会被拒绝，除此之外的都是允许的。与之类似，如果没有给出拒绝主机的指定，那么与允许主机匹配的主机就会被允许，除此之外的都是拒绝的。




