# ASP.NET 使用Swagger开发Web API接口项目 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年05月15日 13:08:49[boonya](https://me.csdn.net/boonya)阅读数：1672








ASP.NET 使用Swagger开发WebApi接口项目：项目使用Web API创建自动提供了API文档，采用mvc方式创建项目稍麻烦点需要手动添加WebApiConfig配置，而采用Web API项目这些都已经生成好了。

#### 创建Web API项目

![](https://img-blog.csdn.net/20180515125449165)

![](https://img-blog.csdn.net/20180515125533568)

#### 添加Swagger依赖库

![](https://img-blog.csdn.net/20180515125700726)

Swagger生成的文件


![](https://img-blog.csdn.net/20180515125730629)

项目右键属性>生成>添加XML生成配置：

![](https://img-blog.csdn.net/20180515130526611)


#### Web API提供的API列表

打开项目启动主页：[http://localhost:54790/](http://localhost:54790/)

Web API项目集成了API列表：

![](https://img-blog.csdn.net/20180515125855563)

![](https://img-blog.csdn.net/20180515125948248)






#### SwaggerAPI提供的API列表

浏览器输入：[http://localhost:54790/swagger/ui/index#/](http://localhost:54790/swagger/ui/index#/)

![](https://img-blog.csdn.net/20180515130200867)

具体接口测试：


![](https://img-blog.csdn.net/20180515130218389)

#### Swagger注意事项

1、启动如果报错，请注释掉如下内容

![](https://img-blog.csdn.net/20180515130638485)

2、如果在IE下访问[http://localhost:54790/swagger/ui/index#/](http://localhost:54790/swagger/ui/index#/)不能正确打开，请用Firefox或Chrome试试。


项目下载地址：[https://download.csdn.net/download/boonya/10415761](https://download.csdn.net/download/boonya/10415761)









