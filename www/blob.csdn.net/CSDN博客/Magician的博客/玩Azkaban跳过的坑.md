
# 玩Azkaban跳过的坑 - Magician的博客 - CSDN博客


2018年11月13日 15:38:54[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：800



### 文章目录
[一号坑：启动Azkaban报错：User xml file conf/azkaban-users.xml doesn't exist.](#AzkabanUser_xml_file_confazkabanusersxml_doesnt_exist_4)
[二号坑：报错：failed SslSocketConnector@0.0.0.0:8443: java.io.FileNotFoundException: /home/hadoop/app/azkaban/azkaban-web-2.5.0/bin/keystore (No such file or directory)](#failed_SslSocketConnector00008443_javaioFileNotFoundException_homehadoopappazkabanazkabanweb250binkeystore_No_such_file_or_directory_11)
[三号坑：启动Azkaban报错：conf/global.properties (No such file or directory)](#Azkabanconfglobalproperties_No_such_file_or_directory_16)
[四号坑：azkaban 提交 job报错：Error submitting flow command. org.apache.http.conn.HttpHostConnectException: Connection to http://localhost:12321 refused](#azkaban__jobError_submitting_flow_command_orgapachehttpconnHttpHostConnectException_Connection_to_httplocalhost12321_refused_25)
[五号坑：azkaban Web UI变“丑”。](#azkaban_Web_UI_31)

Azkaban装过之后放了很久都没碰过，最近由于工作需求想拿出来练下手，但是发现了一会坑。。。在这里记录下这些坑。
## 一号坑：启动Azkaban报错：User xml file conf/azkaban-users.xml doesn’t exist.
解决办法：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113152310737.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113152353464.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)
`即：user.manager.xml.file=/home/hadoop/app/azkaban/azkaban-web-2.5.0/conf/azkaban-users.xml`
## 二号坑：报错：failed SslSocketConnector@0.0.0.0:8443: java.io.FileNotFoundException: /home/hadoop/app/azkaban/azkaban-web-2.5.0/bin/keystore (No such file or directory)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113152719277.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113152750652.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111315280988.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)
## 三号坑：启动Azkaban报错：conf/global.properties (No such file or directory)
解决方法：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113151747887.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113151808471.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)
`即：executor.global.properties=/home/hadoop/app/azkaban/azkaban-executor-2.5.0/conf/global.properties`
## 四号坑：azkaban 提交 job报错：Error submitting flow command. org.apache.http.conn.HttpHostConnectException: Connection to
## [http://localhost:12321](http://localhost:12321)
## refused
解决办法：
排查 azkaban 执行服务器有没有启动。使用 jps 命令查看：
AzkabanExecutorServer
AzkabanWebServer
## 五号坑：azkaban Web UI变“丑”。
解决办法：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111315324170.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113153253350.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113153352494.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113153409650.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113153526366.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111315354937.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)
参考：
[https://hacpai.com/article/1477458544881](https://hacpai.com/article/1477458544881)
[https://www.cnblogs.com/zlslch/p/7124205.html](https://www.cnblogs.com/zlslch/p/7124205.html)

[
  ](https://img-blog.csdnimg.cn/20181113153526366.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)