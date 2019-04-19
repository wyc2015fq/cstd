# drupal6安装简明图文教程 - 三少GG - CSDN博客
2011年02月07日 23:33:00[三少GG](https://me.csdn.net/scut1135)阅读数：1427
# Drupal6入门系列教程—1. drupal6安装简明图文教程
- [新手专区](http://writeblog.csdn.net/forum/5)
- [Drupal6.x](http://writeblog.csdn.net/taxonomy/term/36)
由 niuch 于 星期六, 2009-03-28 03:25 发表
原文地址：[http://drupalgarden.cn/node/34](http://drupalgarden.cn/node/34)
最近发现有很多新朋友加入到drupal的阵营中来，经常在群里有人问drupal安装的一些小问题，刚开始会有些朋友回答一下，次数多了，也很少有人解答了。虽然安装的教程很多人写过，但是比较零散，所以我想写一个简单点的drupal6安装使用教程。能力比较有限，如有问题，希望大家多多指教。
这里以Drupal6.10为例，简单、快速搞定drupal的安装，那咱们就开始把！
1.先到drupal.org上下载drupal6.10 （http://ftp.drupal.org/files/projects/drupal-6.10.tar.gz），解压缩到本地的环境下，将其改为你站的目录名，我这里改为“610”。
下面是drupal6.10解压后的文件列表：
![](http://www.drupalchina.org/files/install-1.jpg)
图1-1
2.解压以后务必要将sites/default/下的default.settings.php复制一份，并重命名为settings.php，下面，需要建立mysql数据库，给数据库起个名字吧，我就叫它“610”吧，我本地环境是XAMPP搭建的环境，自带phpmyadmin，我这里的目录是http://localhost/phpMyAdmin/ ：
![](http://www.drupalchina.org/files/install-2.jpg)
图1-2
3.建立好数据库以后，就开始安装drupal了，打开http://localhost/610 ，点击“Install Drupal In English”：
![](http://www.drupalchina.org/files/install-3.jpg)
图1-3
4.接下来填入选择数据库类型，填入数据库名字、数据库用户名和密码，如下：
![](http://www.drupalchina.org/files/install-4.jpg)
图1-4
安装界面的进度条：
![](http://www.drupalchina.org/files/install-5.jpg)
图1-5
5.接下来会提示你录入相关信息，填写Site Name，E-mail，第一个使用者（权限最高，一般设为admin）的用户名、密码，其他的现在可以忽略不考虑。
![](http://www.drupalchina.org/files/install-6.jpg)
图1-6
![](http://www.drupalchina.org/files/install-7.jpg)
图1-7
6.点保存按钮，安装基本就完成了，这时会有邮件配置的错误提示，如果你的站点暂时不需要发送邮件的，暂时忽略。
![](http://www.drupalchina.org/files/install-8.jpg)
图1-8
7.点击“your new site”，查看安装好的drupal：
![](http://www.drupalchina.org/files/install-9.jpg)
图1-9
点击“Ccreated content”建立一篇文章，文章类型为Story：
![](http://www.drupalchina.org/files/install-10.jpg)
图1-10
添加文章后的效果：
![](http://www.drupalchina.org/files/install-11.jpg)
图1-11
这样我们的drupal就宣告安装完成了，太简单了吧。
下一篇将介绍所见即所得的内容编辑器模块FCKeditor进行配置，敬请关注，更多教程请登陆[drupal花园](http://www.drupalgarden.cn/)学习，网址为：[http://drupalgarden.cn](http://drupalgarden.cn)。感谢您的关注！
==================================================================
============================================================
=======================================================
==================================================
==========================================
## [Xampp和drupal6的安装，配置](http://www.cnblogs.com/popstar/archive/2010/04/26/1721310.html)
XAMPP 是一个易于安装且包含 MySQL、PHP 和 Perl 的 Apache的集成，用于搭建简单web开发环境十分方便。drupal是一个免费开源的cms系统，开发语言为php。
1） 下载xmapp（[点击下载](http://www.apachefriends.org/zh_cn/xampp.html))，得到xampp-win32-1.7.3.zip；
　　下载drupal6 ([点击下载](http://drupal.org/))，得到drupal-6.16.tar.gz；
2)  加压xampp-win32-1.7.3.zip得到"xampp"文件夹，所有mysql，php，apache等都在该文件夹中；
     在xampp文件中，运行setup_xampp.bat 安装xampp；（如果以后要重新安装xampp，应先运行uninstall_xampp.bat文件卸载xampp，再重新安装）
　  安装完毕，点击xampp-control.exe启动xampp控制台，测试各项服务能否顺利启动
![xampp control panel](http://pic002.cnblogs.com/img/Popstar/201004/2010042615383616.gif)
 这里常见问题是apache启动不了，一般是默认端口80被占用，可用netstat -a -n命令查看当前占用的端口号，然后关闭占用端口号的程序；
3）启动xampp，在浏览器中输入http://localhost/， 一切正常则xampp安装成功
     接着，一般先修改MySQL (phpmyadmin)的密码，默认用户名为root，默认密码为空，可在http://localhost/中MySQL 安全控制台修改，例如将密码改为：rootroot；
     输入http://localhost/phpmyadmin （不行的话试试http://localhost/phpMyAdmin）进入mysql，输入用户名和密码，成功进入后，新建一个数据库，随便取名为webcom，显然，这个数据库里面是空的。
4）安装drupal
    a. 首先drupal-6.16.tar.gz解压，改名为drupal6（主要是为以后输入路径方便）
    b. 将drupal6文件夹拷贝到xampp/htdocs文件夹下，然后进入xampp/htdocs/drupal6/sites/default中，有个default.settings.php文件，在当前文件夹下copy一份然后改名为settings.php；
   c. 在settings.php中找到 $db_url = 'mysql://username:password@localhost/databasename';
   在这里username和password是你的mysql的账户名和密码，前面已设为root和rootroot，dabasename则是你新建的web项目数据库，就用前面新建的“webcom”好了
   所以将这句话改为 $db_url = 'mysql://root:rootroot@localhost/webcom'; （当然这里名字密码都是根据我举例得来的）
　如果遇到由于文件权限不能修改，可将该文件权限改为777；
   d. 浏览器中输入http://localhost/drupal6/ 进入drupal安装，里面会要求你输入mysql的帐号密码等，一切应该OK！
　ALL SET!
