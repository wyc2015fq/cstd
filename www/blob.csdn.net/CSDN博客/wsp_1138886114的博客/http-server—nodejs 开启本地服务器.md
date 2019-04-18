# http-server—nodejs 开启本地服务器 - wsp_1138886114的博客 - CSDN博客





2018年11月15日 20:00:25[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：65








### 简单搭建本地服务器（分享文件：同一网络下）

##### 一、下载 node.js

下载地址：[https://nodejs.org/en/download/](https://nodejs.org/en/download/)

##### 二、安装配置环境变量

下载完成，点击安装，一路next，默认添加环境变量了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181115193819976.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

安装完成执行`cmd` 打开终端
```python
node -v  #显示Node.js的版本说明已经安装成功
npm -v   #自带的npm已经安装成功
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181115193725551.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

##### 三、部署项目启动访问

执行 `npm install http-server -g`   其中 ：-g全局安装

执行 `http-server` 启动服务（在你想分享的文件目录下启动）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181115195652507.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)

你可以使用手机和其他设备（同一网络下）输入`http://192.168.1.101:8080`或`http://127.0.0.1:8080`下载所需文件。
鸣谢
[https://blog.csdn.net/zjh_746140129/article/details/80460965](https://blog.csdn.net/zjh_746140129/article/details/80460965)
[https://www.jianshu.com/p/7ddcc6f4ff71](https://www.jianshu.com/p/7ddcc6f4ff71)





