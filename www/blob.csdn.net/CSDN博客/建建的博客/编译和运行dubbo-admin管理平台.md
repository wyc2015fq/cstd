# 编译和运行dubbo-admin管理平台 - 建建的博客 - CSDN博客
2018年01月23日 09:47:49[纪建](https://me.csdn.net/u013898698)阅读数：561
**下载**
Github上下载最新的dubbo源码包并解压
**修改ZooKeeper相关的配置**
打开dubbo-admin/src/main/webapp/WEB-INF下的dubbo.properties
     dubbo.registry.address=zookeeper://192.168.200.1:2181
修改红色部分为你的ZooKeep服务器的IP和端口（如没有安装ZooKeeper,请先安装并启动ZooKeeper服务）
**编译打包**
进入解压后的dubbo项目根目录，执行：mvn install -DskipTests
复制target目录下的dubbo-admin-2.8.4.war（版本为你下载的版本）到tomcat容器，并启动tomcat
**导入到Eclipse中运行**
进入dubbo的根目录，运行：mvn eclipse:eclipse
然后在Eclipse中导入maven项目
运行dubbo-admin（在tomcat上跑）
**登录（参考**
dubbo.properties 中的用户名和密码**）**
访问http://localhost:8080/dubbo-admin/ ，输入用户名密码后即可进入管理界面
