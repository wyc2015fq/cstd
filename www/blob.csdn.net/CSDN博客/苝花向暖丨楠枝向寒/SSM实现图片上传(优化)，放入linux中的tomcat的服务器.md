# SSM实现图片上传(优化)，放入linux中的tomcat的服务器 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年03月29日 23:28:21[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：1025


此篇文章适合知道如何用SSM框架 实现 服务器 和 本地资源 之间的图片上传的人。如不知道，请看我之前写的这篇文章

[SSM实现图片上传](https://blog.csdn.net/weixin_40247263/article/details/79718177)

进入正题

由于图片放在当前运行的项目下的服务器中，只要重启服务器，就会重新部署，曾经上传的图片就没了。所以我们需要在linux下搭建一个静态资源的服务，将图片以及js、vue等 文件全部放在 资源服务器中。 

那么就有 两个 问题。第一：放在哪？ 第二：怎么放。   

问题一：  

    首先我们要知道 http://ip地址:端口号/项目名   这个路径 对应的是 linux 系统下的    tomcat/webapp 这个文件夹。并且由于这是不同的服务器，且在不同的操作系统下，那么ftp传输肯定是无法实现的，肯定得用http传输。

也就是 我们放的路径应该是   http://ip地址:端口号/项目名 + 指定的文件夹。

问题二： 

    导jersey包    ----创建Client对象，创建网络资源，然后往指定位置放

![](https://img-blog.csdn.net/20180329230702127?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

    导common-io 包   ---获取文件后缀名

![](https://img-blog.csdn.net/20180329230752515?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

  controllor 中   --封装了将图片 放入数据库的方法 putStaticSource（MutipartFile file）

![](https://img-blog.csdn.net/20180329231406771?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通过上面的方法 拿到 图片名，然后将图片名 存入 数据库中

![](https://img-blog.csdn.net/20180329231749852?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

jsp 方面。 从linux 的服务器中拿图。也是往哪放的 就 从哪拿。

我在 head.jsp 中封装了  请求路径的 前半部分 即 http://ip地址:端口号/项目名 + 指定的文件夹，放入pageContext域中。

在需要加载图片的jsp页面中，先是引入head.jsp页面，再通过el表达式 和 数据库中 存的 图片名 进行 地址的拼接。

![](https://img-blog.csdn.net/20180329232614724?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/2018032923254783?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


