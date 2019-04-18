# Pet store的安装---http://blog.csdn.net/kevinluck/archive/2007/04/14/1564715.aspx - weixin_33985507的博客 - CSDN博客
2007年06月21日 17:07:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
 jdk1.6.0_01 
windows xp盗版(咱以前可都是用2003的强人呀 装64位vista把系统装挂后 2003盘找不到只好用xp) 比2003好用
netbeans5.5中文版(郁闷E文版的丢了 浪费了一个学E文的机会)从[http://gceclub.sun.com.cn/download.html](http://gceclub.sun.com.cn/download.html)下载的下载的是 NetBeans 5.5 多国语言版安装程序 （Windows Platform） 
sun application server 也是从上方的地址中下载的 NetBeans 5.5 企业开发包多国语言版安装程序 （Windows Platform）中带的
下载的版本是javapetstore-2.0-ea4.5
从[https://blueprints.dev.java.net/servlets/ProjectDocumentList?folderID=5315&expandFolder=5315&folderID=0](https://blueprints.dev.java.net/servlets/ProjectDocumentList?folderID=5315&expandFolder=5315&folderID=0)下载的
介绍
AJAX-enabled Web 2.0 Java Pet Store Reference Application for Java EE 5, 2.0 Early Access distribution.
 E文不好意思可能是说这个版本是基于JAVA EE5平台下使用了AJAX技术的宠物店程序示例
下载完后7.3 mB 
把下完后的JAR包双击 把滚动条拖到下方（代表我们看完介绍了哈）然后选择accept会自动解压到当前目录
然后运行 netBeans选择打开项目 打开刚才那个解开的文件夹会提示你 找不到三个JAR文件
这三个jar的下载地址
[http://jaist.dl.sourceforge.net/sourceforge/httpunit/httpunit-1.6.2.zip](http://jaist.dl.sourceforge.net/sourceforge/httpunit/httpunit-1.6.2.zip)
[http://www.apache.org/~andyc/neko/nekohtml-latest.zip](http://www.apache.org/~andyc/neko/nekohtml-latest.zip)
[http://archive.apache.org/dist/xml/xerces-j/Xerces-J-bin.2.9.0.zip](http://archive.apache.org/dist/xml/xerces-j/Xerces-J-bin.2.9.0.zip)
在项目上点击右键选择解决问题 然后去确认这三个包的位置
然后点击运行环境
点击数据库(安装sun appserver的时候自带的derby) 在  jdbc:derby://localhost:1527/sample上点右键 选择连接密码没改过的话就是默认的app
然后启动sun java system application server 9服务器(**一定要先安装java ee**)
然后 [http://localhost:4848](http://localhost:4848/)进入netBeans后台默认的用户 名密码admin/adminadmin
点击资源，点击JDBC点击连接池，点击新建，名称：PetstorePool 资源类型javax.sql.DataSource数据库选择Derby,点击下一步，然后最下方其它属性 DatabaseName:petstore  Password:APP ，点击完成
再点击JDBC资源，点击新建，JNDI名称：jdbc/PetstoreDB 池名称：PetstorePool点击确定
然后点击项目中的 服务器资源 右键点击setup.xml选择运行项目delete-db然后点击create-db
然后右键点击项目 选择部署项目，部署完毕后 进入后台 [http://localhost:4848](http://localhost:4848/)点击WEB应用程序
然后在javapetstore-2.0-ea4后面点击启动
弹出新页面 Java Pet Store Reference Application, 2.0 Early Access
点击Enter th Store 便可体验传说中的Pet Store喽
