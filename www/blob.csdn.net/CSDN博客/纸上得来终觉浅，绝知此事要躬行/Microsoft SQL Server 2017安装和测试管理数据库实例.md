# Microsoft SQL Server 2017安装和测试管理数据库实例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年05月24日 16:26:27[boonya](https://me.csdn.net/boonya)阅读数：6115








                参考文章地址（部分截图来自该文）：[https://jingyan.baidu.com/article/4853e1e545b2581909f726b9.html](https://jingyan.baidu.com/article/4853e1e545b2581909f726b9.html)

#### 目标安装对象

主要安装以下截图内容：
![](https://img-blog.csdn.net/20180524180126801)
有些我们用不到的功能就不需要安装了，以安装Developer版本为例：

#### 开始安装
从快速安装工具中下载安装包：[Developer Edition](https://go.microsoft.com/fwlink/?linkid=853016)，下载完成后会弹出安装，选择“自定义”安装，选择“developer”开发版。

![](https://img-blog.csdn.net/20180527204641739?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



#### 不要勾选更新



![](https://img-blog.csdn.net/2018052720472854?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


#### 关闭系统防火墙

请先关闭防火墙，否则Windows防火墙会提示警告。

![](https://img-blog.csdn.net/20180527204815421?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




选择具体需要的功能，并且您还可以改变安装位置，设置完成后，下一步。

#### 选择安装内容

推荐选择默认实例安装。

![](https://img-blog.csdn.net/2018052720484664?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




现在设置管理员账号密码，并且添加当前用户。

大概等20分钟就会安装成功。

![](https://img-blog.csdn.net/20180527204910195?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





#### 安装数据库管理工具
[](http://jingyan.baidu.com/album/4853e1e545b2581909f726b9.html?picindex=5)



下载[SQLserver Mamngement Studio](https://docs.microsoft.com/zh-cn/sql/ssms/download-sql-server-management-studio-ssms?view=sql-server-2017)的安装包，进行连接数据库

![](https://img-blog.csdn.net/20180527204944610?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



#### 启动SQL Server实例

![](https://img-blog.csdn.net/20180524181326682?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 连接SQL Server实例
选择启动的示例，输入用户名和密码进行连接测试：

![](https://img-blog.csdn.net/20180524181419231?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




[](http://jingyan.baidu.com/album/4853e1e545b2581909f726b9.html?picindex=7)



连接成功，如下图：





![](https://img-blog.csdn.net/20180524180825315?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 数据库建表
建表后刷新才能显示出新建的表：

![](https://img-blog.csdn.net/20180524183316825?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 测试数据插入和查询
![](https://img-blog.csdn.net/20180524183815446?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[](http://jingyan.baidu.com/album/4853e1e545b2581909f726b9.html?picindex=8)












