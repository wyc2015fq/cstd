# 知识图谱（二）：Linux下部署Fuseki，数据无法上传，无法创建dataset - ChihkAnchor的博客 - CSDN博客





2019年04月15日 17:35:48[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：8








之前部署Fuseki是在本机的Windows环境下，今天在Linux下部署了Fuseki，但是用 http://ip:3030 访问之后，无法添加数据

![](https://img-blog.csdnimg.cn/20190415171229349.png)

 在服务器上启动了fuseki：bash fuseki-server

显示在3030端口启动了服务，但是在Windows下使用IP访问，点击 add new dataset 没有反应

![](https://img-blog.csdnimg.cn/2019041517155769.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

正常应该是这样的：

![](https://img-blog.csdnimg.cn/20190415171727433.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

这个可能是请求被拒绝了，浏览器打开调试界面，我用的谷歌浏览器，直接按F12，刷新页面

![](https://img-blog.csdnimg.cn/20190415172146251.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

果然403，报的错报错 Access denied : only localhost access allowed

only localhost，很显然是服务的配置问题，应该是修改某个配置文件让其获取权限应该就可以了，去官网溜达一圈，官网介绍：It provides security (using  Apache Shiro) and has a user interface for server monitoring and administration.



在执行启动服务同级目录下 有个run文件夹，vim  shiro.ini 

![](https://img-blog.csdnimg.cn/20190415172833292.png)

显然，应该将 /$/** 的值 更改为anon 

![](https://img-blog.csdnimg.cn/20190415173114321.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

 保存退出后，再次启动Fuseki，使用Windows访问，就可以创建dataset了

![](https://img-blog.csdnimg.cn/20190415173427653.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)





