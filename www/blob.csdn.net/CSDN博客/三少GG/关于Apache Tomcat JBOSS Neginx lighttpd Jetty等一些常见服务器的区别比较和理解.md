# 关于Apache/Tomcat/JBOSS/Neginx/lighttpd/Jetty等一些常见服务器的区别比较和理解 - 三少GG - CSDN博客
2014年11月30日 22:16:53[三少GG](https://me.csdn.net/scut1135)阅读数：739
个人分类：[Web网站设计](https://blog.csdn.net/scut1135/article/category/762263)
# [关于Apache/Tomcat/JBOSS/Neginx/lighttpd/Jetty等一些常见服务器的区别比较和理解](http://blog.csdn.net/allenlinrui/article/details/6675998)
       今天是个很丰富的日子，早上一上班，第一个听到的惊爆消息就是楷子得了肠胃炎，一大早去医院挂水了…… 随后风胜和笑虎也没来，后来得知他们俩去去华星现代产业园参加培训，内容是关于Apache与Nginx的。于是乎，我非常感兴趣地查了一下培训用的PPT，并跟旁边的俊牧了解了一下关于服务器的一些东西……
       整个交流过程中，我发现好多概念已经被我遗忘了，有的也很模糊，于是乎，我还是决定到网上查一下，并记录下来！ 
      下面是令人纠结的正文……
**先说Apache和Tomcat的区别：**
         Apache是世界使用排名第一的Web服务器软件。它可以运行在几乎所有广泛使用的计算机平台上，由于其跨平台和安全性被广泛使用，是最流行的Web服务器端软件之一。 
　　在Apache基金会里面ApacheServer永远会被赋予最大的支持，毕竟大儿子最亲嘛，而Apache的开源服务器软件Tomcat同样值得关注，毕竟Tomcat是开源免费的产品，用户会给予最大的支持。但是经常在用Apache和Tomcat等这些服务器时，你总感觉还是不清楚他们之间有什么关系，在用Tomcat的时候总出现Apache，总感到迷惑，到底谁是主谁是次，因此特意在网上查询了一些这方面的资料，总结了一下。 
**解析一：**
　　Apache支持静态页，Tomcat支持动态的，比如Servlet等， 
　　一般使用Apache+Tomcat的话，Apache只是作为一个转发，对JSP的处理是由Tomcat来处理的。 
　　Apche可以支持PHPcgiperl,但是要使用Java的话，你需要Tomcat在Apache后台支撑，将Java请求由Apache转发给Tomcat处理。 
　　Apache是Web服务器，Tomcat是应用（Java）服务器，它只是一个Servlet(JSP也翻译成Servlet)容器，可以认为是Apache的扩展，但是可以独立于Apache运行。 
        这两个有以下几点可以比较的： 
　　◆两者都是Apache组织开发的 
　　◆两者都有HTTP服务的功能 
　　◆两者都是免费的 
　　不同点： 
　　Apache是专门用了提供HTTP服务的，以及相关配置的（例如虚拟主机、URL转发等等） 
　　Tomcat是Apache组织在符合Java EE的JSP、Servlet标准下开发的一个JSP服务器. 
         Runtime r=Runtime.getRuntime();   Process p=null;   try   {   p=r.exec("notepad");   }   catch(Exception ex)   {   System.out.println("fffff");   }    
**         解析二： **
　　Apache是一个Web服务器环境程序,启用他可以作为Web服务器使用,不过只支持静态网页 如(ASP,PHP,CGI,JSP)等动态网页的就不行。 
　　如果要在Apache环境下运行JSP的话就需要一个解释器来执行JSP网页,而这个JSP解释器就是Tomcat,为什么还要JDK呢？因为JSP需要连接数据库的话就要jdk来提供连接数据库的驱程，所以要运行JSP的Web服务器平台就需要Apache+Tomcat+JDK。 
　　整合的好处是： 
　　◆如果客户端请求的是静态页面，则只需要Apache服务器响应请求。 
　　◆如果客户端请求动态页面，则是Tomcat服务器响应请求。 
　　◆因为JSP是服务器端解释代码的，这样整合就可以减少Tomcat的服务开销。 
　　C是一个结构化语言，如谭老爷子所说：它的重点在于算法和数据结构。C程序的设计首要考虑的是如何通过一个过程，对输入（或环境条件）进行运算处理得到输出（或实现过程（事务）控制），而对于C++，首要考虑的是如何构造一个对象模型，让这个模型能够契合与之对应的问题域，这样就可以通过获取对象的状态信息得到输出或实现过程（事务）控制。 
**解析三：**
　　Apache:侧重于HTTP Server 
　　Tomcat:侧重于Servlet引擎，如果以Standalone方式运行，功能上与Apache等效 ， 支持JSP，但对静态网页不太理想； 
　　Apache是Web服务器，Tomcat是应用（Java）服务器，它只是一个Servlet(JSP也翻译成Servlet)容器，可以认为是Apache的扩展，但是可以独立于Apache运行。
**PS：至于为什么要集成Tomcat和Apache,原因是**
> 
    Tomcat的最主要的功能是提供Servlet/JSP容器，尽管它也可以作为独立的Java Web 服
务器，但在对静态资源（如HTML 文件或图像文件）的处理速度，以及提供的Web 服务器
管理功能方面Tomcat都不如其他专业的HTTP服务器，如IIS和Apache服务器。
因此在实际应用中，常常把Tomcat 与其他HTTP 服务器集成。对于不支持Servlet/JSP
的HTTP服务器，可以通过Tomcat服务器来运行Servlet/JSP组件。
当Tomcat与其他HTTP服务器集成时，Tomcat服务器的工作模式通常为进程外的Servlet
容器，Tomcat 服务器与其他HTTP 服务器之间通过专门的插件来通信。
- **1)独立的Servlet容器**
        在这种模式下，Tomcat可以作为独立的Java Web服务器，Servlet容器作为构成Web服务器的一部分而存在。独立的Servlet容器是Tomcat的默认模式
- 
- **2)进程内的Servlet容器**
        Servlet容器分为Web服务器插件和Java容器两部分。Web服务器插件在其他Web服务器内部地址空间打开一个Java虚拟机，Java容器在此JVM中运行Servlet。如有客户端发出调用Servlet的请求，插件获得对此请求的控制并将它传递给Java容器。进程内Servlet容器对于单进程、多进程的服务器非常适合，可以提供较高的运行速度，单缺乏伸缩性。  
- 
- **3)进程外的Servlet容器**
        Servlet容器分为Web服务器插件和java容器两部分。Web服务器插件在其他Web服务器的外部地址空间打开一个JVM。java容器在此JVM中运行Servlet。如有客户端发出调用Servlet的请求，插件获得对此请求的控制并将它传递给java容器。进程外Servlet容器对客户请求的响应速度不如进程内容器，但进程外容器具有更好的伸缩性和稳定性。
**         Tomcat 既可作为独立的Servlet容器，也可和其他的Web服务器集成，作为进程内的Servlet容器或者进程外的Servlet容器。**
**         在说Tomcat和Jetty的区别：**
          Tomcat 服务器是一个免费的开放源代码的Web 应用服务器（主要用于解析servlet/JSP,同时具备http服务），技术先进、性能稳定，而且免费，因而深受Java 爱好者的喜爱并得到了部分软件开发商的认可。其运行时占用的系统资源小，扩展性好，且支持负载平衡与邮件服务等开发应用系统常用的功能。作为一个小型的轻量级应用服务器，Tomcat在中小型系统和并发访问用户不是很多的场合下被普遍使用，成为目前比较流行的Web 应用服务器。
　　而Jetty采用业界最优的开源Java Web引擎， 将Java社区中下载量最大，用户数最多，标准支持最完备的Tomcat内核作为其Servlet容器引擎，并加以审核和调优。单纯的Tomcat性能有限，在很多地方表现有欠缺，如活动连接支持、静态内容、大文件和HTTPS等。除了性能问题，Tomcat的另一大缺点是它是一个受限的集成平台，仅能运行Java应用程序。企业在使用时Tomcat，往往还需同时部署Apache Web Server以与之整合。此配置较为繁琐，且不能保证性能的优越性。
　　Jetty通过使用APR和Tomcat本地技术的混合模型来解决Tomcat的诸多不足。混合技术模型从最新的操作系统技术里提供了最好的线程和事件处理。结果，Jetty达到了可扩展性，性能参数匹配甚至超越了本地Apache HTTP服务器或者IIS。譬如Jetty能够提供数据库连接池服务，不仅支持 JSP 等 Java 技术，同时还支持其他 Web 技术的集成，譬如 PHP、.NET 两大阵营。
　　标准化是减小技术依赖风险，保护投资最好的方式。Jerry率先支持全系列JEE Web标准，从根本上保证了应用“一次开发，到处运行”的特点，使应用成品能方便地在Jetty和其他Java Web服务器之间轻易迁移。
**相同点：**
       都是web容器，作用基本一致，都作为jboss的集成的web容器（有tomcat版的jboss、也有jetty版的jboss，即4.0以后的版本）
**区别：**
       实现机制稍有不同，Jetty性能更优
**Nginx / Apache / lighttpd 的区别：**
> 
**1. lighttpd**
       Lighttpd是一个具有非常低的内存开销，cpu占用率低，效能好，以及丰富的模块等特点。lighttpd是众多OpenSource轻量级的web server中较为优秀的一个。支持FastCGI, CGI, Auth, 输出压缩(output compress), URL重写, Alias等重要功能。
Lighttpd使用fastcgi方式运行php,它会使用很少的PHP进程响应很大的并发量。
Fastcgi的优点在于：
· 从稳定性上看, fastcgi是以独立的进程池运行来cgi,单独一个进程死掉,系统可以很轻易的丢弃,然后重新分配新的进程来运行逻辑.
· 从安全性上看, fastcgi和宿主的server完全独立, fastcgi怎么down也不会把server搞垮,
· 从性能上看, fastcgi把动态逻辑的处理从server中分离出来, 大负荷的IO处理还是留给宿主server, 这样宿主server可以一心一意作IO,对于一个普通的动态网页来说, 逻辑处理可能只有一小部分, 大量的图片等静态IO处理完全不需要逻辑程序的参与(注1)
· 从扩展性上讲, fastcgi是一个中立的技术标准, 完全可以支持任何语言写的处理程序(php,java,python...)
> 
**2.apache**
         apache是世界排名第一的web服务器, 根据netcraft([www.netsraft.co.uk](http://wenwen.soso.com/z/UrlAlertPage.e?sp=Swww.netsraft.co.uk))所作的调查,世界上百分之五十以上的web服务器在使用apache.
1995年4月, 最早的apache(0.6.2版)由apache group公布发行. apache group 是一个完全通过internet进行运作的非盈利机构, 由它来决定apache web服务器的标准发行版中应该包含哪些内容. 准许任何人修改隐错, 提供新的特征和将它移植到新的平台上, 以及其它的工作. 当新的代码被提交给apache group时, 该团体审核它的具体内容, 进行测试, 如果认为满意, 该代码就会被集成到apache的主要发行版中.
apache 的特性:
1) 几乎可以运行在所有的计算机平台上.
2) 支持最新的http/1.1协议
3) 简单而且强有力的基于文件的配置(httpd.conf).
4) 支持通用网关接口(cgi)
5) 支持虚拟主机.
6) 支持http认证.
7) 集成perl.
8) 集成的代理服务器
9) 可以通过web浏览器监视服务器的状态, 可以自定义日志.
10) 支持服务器端包含命令(ssi).
11) 支持安全socket层(ssl).
12) 具有用户会话过程的跟踪能力.
13) 支持fastcgi
14) 支持java servlets
> 
**3.nginx**
Nginx是俄罗斯人编写的十分轻量级的HTTP服务器,Nginx，它的发音为“engine X”， 是一个高性能的HTTP和反向代理服务器，同时也是一个IMAP/POP3/SMTP 代理服务器．Nginx是由俄罗斯人 Igor Sysoev为俄罗斯访问量第二的 Rambler.ru站点开发.
Nginx以事件驱动的方式编写，所以有非常好的性能，同时也是一个非常高效的反向代理、负载平衡。其拥有匹配 Lighttpd的性能，同时还没有Lighttpd的内存泄漏问题，而且Lighttpd的mod_proxy也有一些问题并且很久没有更新。但是Nginx并不支持cgi方式运行，原因是可以减少因此带来的一些程序上的漏洞。所以必须使用FastCGI方式来执行PHP程序。
nginx做为HTTP服务器，有以下几项基本特性：
处理静态文件，索引文件以及自动索引；打开文件描述符缓冲．
无缓存的反向代理加速，简单的负载均衡和容错．
FastCGI，简单的负载均衡和容错．
模块化的结构。包括gzipping, byte ranges, chunked responses,以及 SSI-filter等filter。如果由FastCGI或其它代理服务器处理单页中存在的多个SSI，则这项处理可以并行运行，而不需要相互等待。
Nginx专为性能优化而开发，性能是其最重要的考量,实现上非常注重效率。它支持内核Poll模型，能经受高负载的考验,有报告表明能支持高达 50,000个并发连接数。
Nginx具有很高的稳定性。其它HTTP服务器，当遇到访问的峰值，或者有人恶意发起慢速连接时，也很可能会导致服务器物理内存耗尽频繁交换，失去响应，只能重启服务器。例如当前apache一旦上到200个以上进程，web响应速度就明显非常缓慢了。而Nginx采取了分阶段资源分配技术，使得它的CPU与内存占用率非常低。nginx官方表示保持10,000个没有活动的连接，它只占2.5M内存，所以类似DOS这样的攻击对nginx来说基本上是毫无用处的。就稳定性而言,nginx比lighthttpd更胜一筹。
Nginx支持热部署。它的启动特别容易, 并且几乎可以做到7*24不间断运行，即使运行数个月也不需要重新启动。你还能够在不间断服务的情况下，对软件版本进行进行升级。
二.3种WEB服务器的比较：
|server|Apache|Nginx|Lighttpd|
|----|----|----|----|
|Proxy代理|非常好|非常好|一般|
|Rewriter|好|非常好|一般|
|Fcgi|不好|好|非常好|
|热部署|不支持|支持|不支持|
|系统压力比较|很大|很小|比较小|
|稳定性|好|非常好|不好|
|安全性|好|一般|一般|
|技术支持|非常好|很少|一般|
|静态文件处理|一般|非常好|好|
|Vhosts虚拟主机|支持|不支持|支持|
|反向代理|一般|非常好|一般|
|Session sticky|支持|不支持|不支持|
注：在相对比较大的网站，节约下来的服务器成本无疑是客观的。而有些小型网站往往服务器不多，如果采用 Apache 这类传统 Web 服务器，似乎也还能撑过去。但有其很明显的弊端： Apache 在处理流量爆发的时候(比如爬虫或者是 Digg 效应) 很容易过载，这样的情况下采用 Nginx 最为合适。
建议方案：
Apache 后台服务器（主要处理php及一些功能请求 如：中文url）
Nginx 前端服务器（利用它占用系统资源少得优势来处理静态页面大量请求）
Lighttpd 图片服务器
总体来说，随着nginx功能得完善将使他成为今后web server得主流。
三.性能测试：
将分别测试3种软件在对动态页面和静态页面请求及并发时的响应时间
l 静态页面 搜狐首页
LIGHTTPD
|n/-c(ab参数)|cpu%|Mem|RequestsperSecond|Time taken for tests|
|----|----|----|----|----|
|100000/100|64|60|462.75|21.6|
|100000/200|67|60|312.07|32.4|
|100000/500|83|60|137.24|72.8|
|100000/1000出现错误丢包|94|60|126.6|78.9|
NGINX
|n/-c(ab参数)|cpu%|Mem|RequestsperSecond|Time taken for tests|
|----|----|----|----|----|
|100000/100|34.6|140|943.66|10.597|
|100000/200|35.6|110|924.32|10.818|
|100000/500|34.3|110|912.68|10.956|
|100000/1000|37|160|832.59|12.106|
APACHE
|n/-c(ab参数)|cpu%|Mem|RequestsperSecond|Time taken for tests|
|----|----|----|----|----|
|100000/100|40.6|170|690.72|14.47|
|100000/200|41.1|180|685.39|14.59|
|100000/500|42.3|190|633.64|15.78|
|100000/1000|43.1|200|547.53|18.26|
l 动态页面 内部社区首页
LIGHTTPD
|n/-c(ab参数)|cpu%|Mem|RequestsperSecond|Time taken for tests|
|----|----|----|----|----|
|1000/100|50|200|33.54|29.816|
|1000/200|52|210|30.43|32.858|
|1000/500|54|230|25.79|38.76|
|1000/1000|62|250|24.83|40.28|
NGINX
|n/-c(ab参数)|cpu%|Mem|RequestsperSecond|Time taken for tests|
|----|----|----|----|----|
|1000/100|53.8|250|83.12|12.305|
|1000/200|55.8|250|74.05|13.504|
|1000/500|56|260|58.99|16.951|
|1000/1000|58|260|43.41|23.347|
APACHE
|n/-c(ab参数)|cpu%|Mem|RequestsperSecond|Time taken for tests|
|----|----|----|----|----|
|100000/100|60|200|27.37|36.541|
|100000/200|61|220|23.82|41.981|
|100000/500|73|150|20.59|48.562|
|100000/1000|53|200|27.18|36.796|
l PHPINFO函数页
LIGHTTPD
|n/-c(ab参数)|cpu%|Mem|RequestsperSecond|Time taken for tests|
|----|----|----|----|----|
|100000/100|45|20|168.06|59.504|
|100000/200|47|22|140.64|71.103|
|100000/500|49|24|52.80|189.386|
|100000/1000|在请求到4840时测试测试程序死掉| | | |
NGINX
|n/-c(ab参数)|cpu%|Mem|RequestsperSecond|Time taken for tests|
|----|----|----|----|----|
|100000/100|70|120|143.46|69.706|
|100000/200|72|130|140.57|71.140|
|100000/500|73|150|135.87|73.601|
|100000/1000|77|160|132.18|75.657|
APACHE 出现丢包
|n/-c(ab参数)|cpu%|Mem|RequestsperSecond|Time taken for tests|
|----|----|----|----|----|
|100000/100|70|180|245.73|40.694|
|100000/200|72|190|245.79|40.684|
|100000/500|75|200|241.29|41.443|
|100000/1000|77|220|236.74|42.239|
四．各大网站WEB服务器资源列表
网站名 操作系统 web服务器
1.门户网站类：
搜狐 LINUX apache 1.3.37
新浪 LINUX apache 2.0.54
迅雷 LINUX nginx 0.6.31
163 LINUX apache 2.2.6
2.搜索类
百度 unknown BWS 1.0
Google linux gws
Sougou FreeBSD apache 2.2.4
Hao123 linux apache 2.2.4
4. 电子邮箱类
126 linux apache
Hotmail win2003 microsoft-IIS 6.0
新浪邮箱 F5 Big-IP apache 2.2.8
263 linux apache 2.2.6
5. 博客类
新浪博客 linux nginx 0.5.35
搜狐博客 linux nginx
迅雷博客 linux nginx 0.6.32
天涯博客 F5 Big-IP Microsoft-IIS/5.0
6．视频类
优酷 linux apache
土豆 linux apache
Ku6 linux apache
六间房 linux nginx 0.6.14
        上面说了那么多，可以看出，其实
Apache / Nginx / lighttpd 是一类，都是专用的WEB服务器，Tomcat / Jetty 是一类，是servlet/jsp应用服务器+WEB服务器 ，Jboss则是一个大框架，也是应用服务器 + WEB服务器，但是Tomcat 或则Jetty 都是它的一部分 。 
