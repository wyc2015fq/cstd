# geoserver发布地图服务 - Ms.Fox的专栏 - CSDN博客
2019年02月28日 16:09:22[lulu-lu](https://me.csdn.net/smbluesky)阅读数：43
1、下载geoserver.war 
2、在Tomcat中启动该war包
3、将要发布的数据在……\apache-tomcat-geoserver-8080\webapps\geoserver\data\data中新建一个数据文件夹，把数据放进去
4、登录打开[http://localhost:8080/geoserver/web/](http://localhost:8080/geoserver/web/) 进入geoserver服务器  账号：admin 密码：geoserver
5、新建一个工作空间，workspace
6、新建一个存储空间，stores，选择shapefile，选择上面创建的空间，将3新建的文件夹关联。
7、新建图层，选择上面的存储空间
8、选择新建文件夹下的某一个shape文件，填入一下参数，发布即可。
