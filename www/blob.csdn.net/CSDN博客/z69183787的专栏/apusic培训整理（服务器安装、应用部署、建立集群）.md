# apusic培训整理（服务器安装、应用部署、建立集群） - z69183787的专栏 - CSDN博客
2013年11月12日 10:34:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2284
一、Apusic服务器安装
1、将Apusic安装包解压，将JDK路径替换apusic目录下的bin文件中setenv.cmd文件的
rem set JAVA_HOME=your_java_home_dir下边加入JDK路径
set java_home=C:/Program Files/Java/jdk1.5.0_04
2、启动apusic-5.1/bin的startsample.cmd 双击即可
3、访问http://localhost6888/admin  默认用户名和密码为：admin  admin
二、创建一个域
1、cmd
2、运行config.cmd文件 cd d: D:/ApplicationSoftware/apsuic5/apusic-5.1/bin
3、按照命令提示选择相关选项
4、创建域的默认路径为：D:/ApplicationSoftware/apsuic5/apusic-5.1/domains
5、按照命令提示继续执行即可。通常选择默认，注意：服务器安装时默认端口为6888，创建域时需要建立监听端口，可以自行设定，但不能与服务器默认端口冲突。
6、如果创建域的时候端口冲突，需要更改端口，更改地址为：域目录中的 config/apusic.conf文件中更改。
三、启动与停止一个域
1、启动：CMD 下  D:/ApplicationSoftware/apsuic5/apusic-5.1/domains/liudemo(域名称)/bin/startapusic.cmd
2、停止：CMD 下  D:/ApplicationSoftware/apsuic5/apusic-5.1/domains/liudemo(域名称)/bin/stopapusic.cmd admin admin iiop://localhost:7889(域的端口号)
或在已经启动的服务的CMD窗口中按快捷键ctrl+c即可
四、部署一个应用
1、启动一个所建立的域的服务
2、通过WEB访问服务器，点击“J2EE应用”
3、在右侧“应用列表”，点击“部署”
4、配置数据源
5、填写应用名称，并选择应用地址，apusic部署包为“war”和"ear"
6、部署好后，点击确定，并点击“JDX属性”中的“通过浏览器访问此Web应用”
五、创建一个集群
1、建立三个apusic 服务  也就是三个域，如：分别为：server1 端口为6888 、server2 端口为7888、loadbalancer 端口为80
2、将server1 和 server2 中config文件中的apusic.conf文件备份或删除，并把cluster.conf文件改名为apusic.conf
3、将server1中更改名称后的apusic.conf文件中的
<SERVICE  CLASS="com.apusic.cluster.ClusterService"中的
<ATTRIBUTE NAME="ServerName" VALUE="server2"/>
4、将server2中更改名称后的apusic.conf文件中的
<SERVICE  CLASS="com.apusic.cluster.ClusterService"中的
<ATTRIBUTE NAME="ServerName" VALUE="server1"/>
5、将server1和server2中更改名称后的apusic.conf文件中
<SERVICE CLASS="com.apusic.web.session.SessionService">中的
 <ATTRIBUTE NAME="DestorySessionOnApplicationStop" VALUE="false"/>中的VALUE更改为false。
5、将loadbalancer中config文件中的apusic.conf文件备份或删除，并把loadbalancer.conf文件改名为apusic.conf
6、将loadbalancer中更改名称后的apusic.conf文件中的
<SERVICE CLASS="com.apusic.web.loadbalancer.LoadBalancer">中的
 <ATTRIBUTE NAME="BackendServers" VALUE="localhost:6888,localhost:7888"/>
 指定server1和server2的端口。
7、启动三个服务，并将应用部署在loadbalancer 服务中，访问应用。
http://localhost:80/admin
8、WEB访问应用时  地址应为loadbalancer的端口   http://localhost:80/estore/
六、在电脑的服务里安装Apusic服务与卸载
1、安装: 打开cmd 进入所创建的域的目录，如：D:/ApplicationSoftware/apsuic5/apusic-5.1/domains/icssliu(创建的域)/bin>apusicsvc -install
此时，右键我的电脑->管理->服务与管理->服务  可以见到apusic的服务，为手动。
2、卸载：打开cmd 进入所创建的域的目录，如：
D:/ApplicationSoftware/apsuic5/apusic-5.1/domains/icssliu(创建的域)/bin>apusicsvc -uninstall
此时，右键我的电脑->管理->服务与管理->服务 apusic的服务已经被卸载。
