# 关于maven项目手动导入的jar包出现 ClassNotFound - 苝花向暖丨楠枝向寒 - CSDN博客

2018年07月08日 11:37:12[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：135


fastFDS的客户端jar包在maven中心仓库下载的都不好使，所以我自己在网上找了一个.

通过右键项目Build Path --->Configure Build Path.. --->Add External JARS  这样导入的jar包 只存在工作环境当中。

当项目部署到tomcat以后，我们会发现 webapp文件夹下的项目文件夹中并不会存在我们的jar包。

解决方法 ： 在WEB-INF下建一个文件夹 lib，把jar包 copy 进去，这样在部署项目的时候就会将jar也部署进去了。



