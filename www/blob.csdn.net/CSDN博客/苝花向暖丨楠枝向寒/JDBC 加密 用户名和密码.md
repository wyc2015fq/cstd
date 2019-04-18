# JDBC 加密 用户名和密码 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月04日 20:21:32[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：872


由于我用的是SSM+druid，所以我在 配置 JDBC的文件中 配置了如下内容

第一个bean节点 是 db.properties(驱动、url、用户名、密码 写这面) 的位置

第二个bean节点 是配置 druid 连接池、由于驱动、url、用户名、密码写在了db.properties中，所以可以通过el表达式获取

![](https://img-blog.csdn.net/20180404200457850?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

db.propertis； 这里需要注意一点，写在property标签中 & -->&    在db.propertis中 直接写 &就可以

![](https://img-blog.csdn.net/20180404200842767?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


下面是 加密 和 解密的 方法

![](https://img-blog.csdn.net/20180404201134799?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

----------------------------------------------------------------------------------------------------------------------------------

![](https://img-blog.csdn.net/2018040420120737?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

----------------------------------------------------------------------------------------------------------------------------------

![](https://img-blog.csdn.net/2018040420130792?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

----------------------------------------------------------------------------------------------------------------------------------

解释： 

加密： 先拿到 图纸、工厂生产、 加密

解密： 先拿到 图纸、解密 、工厂拆解

拿图纸：图纸一样、所以写在static 代码块中

通过加密方法先拿到 加密后的用户名和密码、然后在db.properties对应位置做 替换。

然后就是需要写一个类 继承 PropertyPlaceholderConfigurer  ，因为我们驱动和url不需要加密、也就是不需要解密，而用户名和密码，需要加密、也需要解密。

代码如下：

![](https://img-blog.csdn.net/20180404201950634?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

