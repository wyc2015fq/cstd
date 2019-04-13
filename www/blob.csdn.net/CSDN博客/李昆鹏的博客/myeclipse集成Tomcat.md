
# myeclipse集成Tomcat - 李昆鹏的博客 - CSDN博客


2018年07月12日 10:45:27[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：888


--------------------------- myeclipse集成Tomcat-------------------------------
# 1. MyEclipse集合Tomcat

## 1　MyEclipse配置服务器
使用MyEclipse配置服务器后，就可以使用MyEclipse来启动和停止服务器了。当然，你需要先安装好服务器（Tomcat），才能配置。
MyEclipse自带了一个Tomcat，但我们不会使用它。所以，我们需要先把MyEclipse自带的Tomcat关闭，然后再来配置我们自己的Tomcat。
1.    关闭MyEclipse自带Tomcat。
在工具栏中找到，点击下箭头，点击ConfigureServer Connector。
![](https://img-blog.csdn.net/20180712104213144?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
弹出对话框
![](https://img-blog.csdn.net/20180712104230172?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击MyEclipseTomcat6，选中Disable，然后OK！
2.    配置我们安装的Tomcat。
点击
![](https://img-blog.csdn.net/20180712104247657?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180712104253744?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
使用MyEclipse启动Tomcat
![](https://img-blog.csdn.net/20180712104304649?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2　MyEclipse中的Web项目
先配置JDK！
![](https://img-blog.csdn.net/20180712104309163?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在Package空白处点右键，选择new，再选择Web Project。
![](https://img-blog.csdn.net/20180712104319741?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180712104330544?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180712104336438?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**以下是Myeclipse2017版本配置Tomcat**
![](https://img-blog.csdn.net/20180712104347646?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180712104355132?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180712104402146?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 3　MyEclipse部署Web项目
创建web项目
![](https://img-blog.csdn.net/20180712104412241?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
项目创建完之后，还不能运行，因为还需要把项部署到Tomcat上，即把项目发布到Tomcat的webapps目录下。
注意，不是把整个项目都发布过去，只是把项目中WebRoot中的内容，用项目名命名的文件夹装起来，copy到webapps目录下。
![](https://img-blog.csdn.net/20180712104431107?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180712104440516?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180712104447326?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其实项目部署就是把我们使用MyEclipse编写的东西copy到webapps目录下，这与我们自己在webapps下自己写没什么区别。但你也知道，使用MyEcplise写代码要方便很多，但使用MyEclipse写的项目不在Tomcat的webapps下，所以还需要部署一下。部署的内容不包含Java源代码！只包含项目中WebRoot目录下的内容！
项目部署后，启动Tomcat，然后就可以在浏览器中访问项目了：[http://localhost:8080/hello/index.jsp](http://localhost:8080/hello/index.jsp)
**项目部署：**
![](https://img-blog.csdn.net/20180712104505383?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


