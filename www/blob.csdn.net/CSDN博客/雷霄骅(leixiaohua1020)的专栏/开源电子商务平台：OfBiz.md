# 开源电子商务平台：OfBiz - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年12月28日 00:20:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：44
个人分类：[开源项目](https://blog.csdn.net/leixiaohua1020/article/category/1652801)










OFBiz是一个电子商务平台，是一个非常著名的开源项目，提供了创建基于最新J2EE/XML规范和技术标准，构建大中型企业级、跨平台、跨数据库、跨应用服务器的多层、分布式电子商务类WEB应用系统的框架。 OFBiz最主要的特点是OFBiz提供了一整套的开发基于Java的web应用程序的组件和工具。包括实体引擎, 服务引擎, 消息引擎, 工作流引擎, 规则引擎等。

![](https://img-blog.csdn.net/20131126145737437)



**安装步骤**

l Java 1.6（至少）SDK

l 解压缩 release文件，打开新生成的目录

l 使用命令行（CMD），使用如下命令编译系统（包含demo数据）：


`ant load-demo start`


（在 Linux 上使用 "./ant" 而不是"ant"）

l 使用浏览器访问 [http://localhost:8080/catalog](http://localhost:8080/catalog)

l 使用用户名"admin" ，密码"ofbiz"登录系统



**系统体验：**

注：这个版本是相对早一点的版本，新版本界面不一定是这个样子的。

打开http://localhost:8080/ecommerce访问前台页面。可以看出和淘宝是比较类似的。

注：ofbiz是支持简体中文的。

![](https://img-blog.csdn.net/20131224002212093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

点击打开一个物品的详细信息页面，如下图所示：

![](https://img-blog.csdn.net/20131224002221093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

注册新用户页面如下图所示。

![](https://img-blog.csdn.net/20131224002226281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

查看购物车页面如下图所示。

![](https://img-blog.csdn.net/20131224002234640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

打开https://localhost:8443/webtools访问管理员页面。各种管理工具。

PS：页面还挺酷的。

![](https://img-blog.csdn.net/20131224002239250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下图所示是打开了一个财务管理的功能。

![](https://img-blog.csdn.net/20131224002250046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



ofbiz项目首页：[http://ofbiz.apache.org/](http://ofbiz.apache.org/)






