# 如何用IDEA一步一步开发WebService服务器端 - z69183787的专栏 - CSDN博客
2016年12月06日 16:39:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4832
最近在搞一些东西，例如WebService，参考了很多教程，但是毕竟每一个环境都有不一样的问题，在此记录下我在开发这个过程当中遇到的一些问题。
工具：IntelliJ IDEA 15.0.4 
IDEA这款IDE还是非常强大的，对WebService也有很好的支持。下面我们来一步一步的实现WebService服务器端： 
第一步，新建一个工程：File->new->project。需要注意的看下面的图片： 
![这里写图片描述](https://img-blog.csdn.net/20161025191232026)
点击next: 
![这里写图片描述](https://img-blog.csdn.net/20161025191430794)
点击Finish,我们得到的工程目录大概是下面这样子的： 
![这里写图片描述](https://img-blog.csdn.net/20161025191714440)
然后我们看到有一个Java类。右键HelloWorld.java: 
![这里写图片描述](https://img-blog.csdn.net/20161025192040318)
在这里生成wsdl文件进行配置： 
![这里写图片描述](https://img-blog.csdn.net/20161025192139380)
还有一个地方需要注意： 
![这里写图片描述](https://img-blog.csdn.net/20161025193416508)
这里是有可能报错的，如果报错，点击下面的Fix->Add ‘JAX-WS-Apache’ to the…嗯修复错误。
需要注意的地方是Web Service URL的地址的配置，我之前就是用的默认的localhost:8080//services/……估计是没有配置Tomcat，所以这里是两个/，所以我调了很久都没有搞出来，路径问题；配置好之后，发现Java文件下面多了一个wsdl文件。
下面配置Tomcat，tomcat配置比较简单，所以在此只贴几张图片出来： 
![这里写图片描述](https://img-blog.csdn.net/20161025192850884)
![这里写图片描述](https://img-blog.csdn.net/20161025192905009)
![这里写图片描述](https://img-blog.csdn.net/20161025192926415)
![这里写图片描述](https://img-blog.csdn.net/20161025192945658)
至此，Tomcat配置完成，启动Tomcat，访问[http://localhost:8080/flight](http://localhost:8080/flight) 显示index.jsp页面，说明Tomcat配置成功。当我们访问WebService的时候；路径为： 
[http://localhost:8080/flight/services/HelloWorld](http://localhost:8080/flight/services/HelloWorld)
结果显示![这里写图片描述](https://img-blog.csdn.net/20161025193758356)
找了很久也没有找到原因，然后发现当我输入下图的路径的时候出现的结果： 
![这里写图片描述](https://img-blog.csdn.net/20161025193927093)
仔细看这个图发现是缺少了我们自己配置的HelloWorld，然后想一下应该是有个地方配置： 
![这里写图片描述](https://img-blog.csdn.net/20161025194146170)
找到这里我仿照配置文件写了HelloWorld的配置： 
重启Tomcat之后再次访问发现上面的图片多了我们刚才配置的项HelloWorld： 
![这里写图片描述](https://img-blog.csdn.net/20161025194452234)
点进去看看： 
![这里写图片描述](https://img-blog.csdn.net/20161025194526132)
嗯就是我们要的结果了，至此，在用Idea搭建WebService服务器端的过程中遇到的全部问题我在这里做了一个总结： 
下面一篇文章我会讲一下怎么搭建WebService客户端。
