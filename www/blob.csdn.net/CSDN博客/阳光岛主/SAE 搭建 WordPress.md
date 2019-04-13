
# SAE 搭建 WordPress - 阳光岛主 - CSDN博客

2013年10月06日 04:24:00[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：8751个人分类：[Cloud																](https://blog.csdn.net/sunboy_2050/article/category/1398664)



**[WordPress](http://wordpress.org/)****，**是国外一款使用PHP语言开发的开源博客平台，用户可以在支持PHP和MySQL 数据库的服务器上架设自己的博客站点，也可以把 WordPress 当作一个内容管理系统（CMS）来使用。WordPress 是一个免费的开源项目，在GNU通用公共许可证下授权发布，可以用于商业目的，前提是也开放源代码，其中文网站见[ cn.wordpress](http://cn.wordpress.org/)

WordPress started in 2003 with a single bit of code to enhance the typography of everyday writing and with fewer users than you can count on your fingers and toes. Since then it has grown to be the largest self-hosted blogging tool in the world, used on millions of sites and seen by tens of millions of people every day.

Everything you see here, from the documentation to the code itself, was createdby and for the community. WordPress is anOpen Sourceproject（[GPLv2](http://wordpress.org/about/license/)）, which means there are hundreds of people all over the world working on it. (More than most commercial platforms.) It also means you are free to use it for anything from yourcat’s home pageto aFortune 500 web sitewithout paying anyone a license feeand a number of other important freedoms.


上一篇博客[SAE 搭建 Discuz](http://blog.csdn.net/ithomer/article/details/12277065)，详细讲解了在SAE上搭建自己的discuz论坛，本文将详细讲解在SAE上搭建自己的WordPress博客，请见：[http://blog.ithomer.net](http://blog.ithomer.net/)

在SAE上，基于WordPress，搭建自己的个人博客，有两个方法：
**1、直接在线安装**
在SAE首页最下方——》[应用仓库](http://sae.sina.com.cn/?m=appstore)——》Web应用——》Blog——》WordPress for SAE ——》点击右侧“安装应用”，如下图：
![](https://img-blog.csdn.net/20131006042050000)
上图，二级域名是指自己创建的应用ID（一级域名为sinaapp.com），创建的ID不能被占用，也不能是自己先前申请的应用ID（申请后即占用）
在线安装很简单，网上也有很多教程，顺着提示一步步做即可，本文重点将讲解下面的自定义安装


**2、下载安装包，自定义安装**

1） 下载 WordPress for SAE
进入SAE应用仓库——》[WordPress for SAE](http://sae.sina.com.cn/?m=apps&a=detail&aid=1)——》点击右侧“下载应用安装包”，修改名字为“wordpress4sae.zip”

2）上传 wordpress2sae
进入SAE——》我的应用——》ithomer（我的应用ID，改成你自己的应用名）——》代码管理——》上传代码包，如下图：
![](https://img-blog.csdn.net/20131006044736875)

3）安装 wordpress2sae
代码管理——》点击访问[http://1.ithomer.sinaapp.com](http://1.ithomer.sinaapp.com/)，如下图：
![](https://img-blog.csdn.net/20131006050519187)

4）按照上图提示，分别初始化MySQL、Memcache、Storage
（a）MySQL初始化： 左侧栏 “服务管理” ——》 MySQL ——》点击“初始化MySQL”
（b）Memcache初始化： 左侧栏 “服务管理” ——》 Memcache ——》创建10M大小
（c）Storage 创建Domain： 左侧栏 “服务管理” ——》 Storage ——》新建一个domain，名称按上图提示输入“wordpress”
上面三个初始化，在上一篇博客中，有详细介绍，详见[SAE 搭建 Discuz](http://blog.csdn.net/ithomer/article/details/12277065)——》 4、 初始化准备工作 的1）、2）、3）步骤

初始化完后，依次输入自己的“站点标题”、管理员密码、电子邮件，如上图所标记。

5）完成安装 WordPress
点击“安装WordPress”，完成安装，出现如下图：
![](https://img-blog.csdn.net/20131006052005546)

6）直接点击“登录”，出现登陆页面，如下图：
![](https://img-blog.csdn.net/20131006052315781)

7）在上图中，输入步骤3）设定的用户名和密码，点击登录，如下图：
![](https://img-blog.csdn.net/20131006052653609)

8）开启自己的个人博客之旅，在浏览器输入域名：[http://ithomer.sinaapp.com](http://ithomer.sinaapp.com/)（更改为[http://blog.ithomer.net](http://blog.ithomer.net)）
![](https://img-blog.csdn.net/20131006055749109)

博客地址更新为：[http://blog.ithomer.net](http://blog.ithomer.net/)


**wordpress 迁移**
想把博客从SAE迁到Aliyun阿里云服务上，使用phpadmin导出数据（约64M）不完整，因此改用大数据操作工具 DeferredJob 进行导出，步骤如下：
1） 参考SAE官方论坛帖子：[SAE的mysql导出数据不完整呢](http://cloudbbs.org/forum.php?mod=viewthread&tid=13302&extra=page%3D1)
2） 进入到SAE官方管理台：[DeferredJob](http://sae.sina.com.cn/?m=notify&app_id=ithomer&ver=0)
![](https://img-blog.csdn.net/20140224184738406)
3） 填写要导出的数据库名，如 app_wordpress.zip 会自动帮助压缩，文件保存路径在：
http://sae.sina.com.cn/?m=storage&a=managestor&app_id=xxx&domName=wordpress
![](https://img-blog.csdn.net/20140224185257968)
4） 导出完成后，在上图的“结果查询”，可以查到结果，如下图：
![](https://img-blog.csdn.net/20140224185158078)


**参考推荐：**
[SAE搭建WordPress博客教程](http://jingyan.baidu.com/article/8cdccae9b89d69315413cde7.html)
[SAE一分钟打造你的WORDPRESS独立博客](http://blog.sina.com.cn/s/blog_73b89cd301010i06.html)
[WordPress 迁移SAE 操作攻略](http://www.liton.me/archives/590.html)
[SAE的mysql导出数据不完整呢](http://cloudbbs.org/forum.php?mod=viewthread&tid=13302&extra=page%3D1)
[BAE与SAE数据库互访](http://ju.outofmemory.cn/entry/1001)



