
# 自定义 WordPress 样式 - 阳光岛主 - CSDN博客

2013年10月06日 07:12:29[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：9559


修改页面头部、脚部的文件路径：wp-content ——》themes ——》twentyten ——》footer.php、header.php

1） 删除页脚下方的 “自豪地采用 WordPress” 字样
在SAE上安装WordPress后，默认页脚下方会出现版权字样，如下图：
![](https://img-blog.csdn.net/20131006070659296)

在 管理页面——》外观——》编辑——》修改主题，没法保存，是不能修改生效的，正确的做法类似[自定义discuz样式](http://blog.csdn.net/ithomer/article/details/12298333)，打开./wp-content/themes/twentyten/footer.php(twentyten是我选择的样式，选择你自己的样式），查找 “<div id="site-generator">”，注释或删去掉，如下图：
![](https://img-blog.csdn.net/20131006071211328)

2） 将上图的“IT-Homer”字样，移到页脚右下方
通过chrome调试工具，查看“IT-Homer”样式，找出修改的css文件，调试方法如下：
![](https://img-blog.csdn.net/20131006071803046)

找到了“IT-Homer"字样居左显示的原因，修改就很简单了，打开wp-content/themes/twentyten/style.css，搜索”site-info", 修改成如下：
![](https://img-blog.csdn.net/20131006072650343)
修改了3处，分别是：
1）float左对齐（left）改成了右对齐（right）
2）删除了宽度限制（删除 width:700px;)
3）添加了一行“margin-right：10px）
修改后的效果，如下图：
![](https://img-blog.csdn.net/20131006073111140)

3）登录时，修改界面WordPress的logo和链接
登陆和注册时，点击上方“WordPress”的logo，莫名链接到了“[http://cn.wordpress.org/](http://cn.wordpress.org/)”，分析原因如下图：
![](https://img-blog.csdn.net/20131006092040093)

找到了的原因，修改就很简单了，顺藤摸瓜，打开./wp-login.php文件，搜索“wordpress.org”（不要搜[http://cn.wordpress.org/](http://cn.wordpress.org/)，他可能是拼接字符串出来的），如下图：
![](https://img-blog.csdn.net/20131006092808890)

把上图链接“http://wordpress.org”修改成自己的链接，如“[http://iforum.sinaapp.com/](http://iforum.sinaapp.com/)”，修改后的效果，可以访问我的[http://ithomer.sinaapp.com/wp-login.php](http://ithomer.sinaapp.com/wp-login.php)，点击看一下效果

4、修改logo
步骤3修改了链接，还没修改logo，这里在分析修改logo，分析图如下：
![](https://img-blog.csdn.net/20131006093845031)
上图有步骤3修改后的链接，这里我们重点看login引出的图片地址，打开摸出的图片所在路径：./wp-admin/images/wordpress-logo.png，修改替换成自己的logo
wordpress-logo.png图片默认宽和高为274px 63px
.login h1 a{background-image:url('../images/wordpress-logo.png?ver=20120216');background-size:274px 63px;background-position:top center;background-repeat:no-repeat;width:326px;height:67px;
根据这个宽高可以调整自己图片大小，也可以修改其默认宽高，打开css文件：./wp-admin/css/wp-admin.css，搜索“.login h1 a”，找到上面浅蓝色位置，修改成自己图片宽高，如下图修改成我自己iforum的logo大小113px 70px
![](https://img-blog.csdn.net/20131006100715343)
./wp-admin/css/wp-admin.css文本太多有点大，在SAE上直接打开较慢，建议在本地修改好后，上传上去就行（线上、线下源文件，都要做好备份）
替换logo后的效果如下图：
![](https://img-blog.csdn.net/20131006101140281)
可以访问我的WordPress站点，看实际效果：[http://ithomer.sinaapp.com/wp-login.php](http://ithomer.sinaapp.com/wp-login.php)

**5、给导航栏添加页面**
WordPress安装完毕后，导航栏默认只有“首页” 和 “示例页面”两项，需要自己添加导航页面，添加方法如下：
![](https://img-blog.csdn.net/20131006101952078)

记得在“新建页面”后，一定要发布，才能看得见效果，如下图：
![](https://img-blog.csdn.net/20131006102212453)

博客详见：[ http://blog.ithomer.net](http://blog.ithomer.net)


个人认为WordPress搭建的博客网站很丑，在网上找了一下，发现还是自己水平不行，下面汇总了一些搭建比较优秀的站点：
苹果联盟：[http://www.appleunion.com/](http://www.appleunion.com/)
青松博客：[http://www.song100e.com/](http://www.song100e.com/)

TechCrunch：[http://techcrunch.com/](http://techcrunch.com/)
Flickr Code：[http://code.flickr.net/](http://code.flickr.net/)
SAP Info：[http://en.sap.info/](http://en.sap.info/)
Quartz：[http://qz.com/](http://qz.com/)

更多WordPress搭建的案例，请见官方的：
[http://wordpress.org/showcase/](http://wordpress.org/showcase/)


**知名博客推荐**
月光博客：[http://www.williamlong.info/](http://www.williamlong.info/)
望月博客：[http://wangyueblog.com/](http://wangyueblog.com/)
卢松松：[http://lusongsong.com/](http://lusongsong.com/)
青松博客：[http://www.song100e.com/](http://www.song100e.com/)
夏日博客：[http://www.04ie.com/](http://www.04ie.com/)
张存超博客：[http://www.521php.com/](http://www.521php.com/)


**WordPress模板主题：**
WordPress啦：[http://www.wordpress.la/theme.html](http://www.wordpress.la/theme.html)
WOPUS博客：[http://themes.wopus.org/](http://themes.wopus.org/)


**博客之星评选，请投我一票：**
**http://vote.blog.csdn.net/blogstaritem/blogstar2013/sunboy_2050**


