# Ubuntu11.04上LAMP的安装 - Orisun - 博客园







# [Ubuntu11.04上LAMP的安装](https://www.cnblogs.com/zhangchaoyang/articles/2044128.html)





只需2步：

sudo apt-get install tasksel

sudo tasksel install lamp-server

安装途中MySQL要求设定账户有密码，这是与操作系统没关系的账户与密码。

装好后打开http://127.0.0.1/，如果显示 It works!说明安装成功。

接着安装phpmyadmin：

sudo apt-get install phpmyadmin

安装好后http://127.0.0.1/phpmyadmin

单独Apache的安装配置说明详见： [http://man.ddvip.com/linux/debian/apache2/index.html](http://man.ddvip.com/linux/debian/apache2/index.html)

Apache安装后是跟随系统自动启动的，关于服务启动的相关问题详见 [http://www.freebsd.org/doc/zh_CN.GB2312/books/handbook/configtuning-rcd.html](http://www.freebsd.org/doc/zh_CN.GB2312/books/handbook/configtuning-rcd.html)












