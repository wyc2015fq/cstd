
# Apache设置禁止访问网站目录 - 阳光岛主 - CSDN博客

2016年01月09日 11:23:27[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：23684


Apache默认在当前目录下没有index.html入口就会显示网站根目录，让网站目录文件都暴露在外面，是一件非常危险的事，例如：数据库密码泄露，隐藏页面暴露等严重安全问题！
例如，访问[米扑](https://mimvp.com/)网站根目录:[https://mimvp.com](https://mimvp.com/)会列出根目录
![apache-setting-prohibit-access-to-the-site-directory](http://cdn-blog.mimvp.com/wp-content/uploads/2016/01/apache-setting-prohibit-access-to-the-site-directory.png)
本文将详细介绍如何操作禁止显示apache网站根目录
进入apache的配置文件 httpd.conf 找到：
vim /etc/httpd/conf/httpd.conf
Options Indexes FollowSymLinks
修改为：
Options FollowSymLinks
修改后结果如下：
<Directory “/var/www/html”>
\#    Options None
\#    Options Indexes FollowSymLinks
Options FollowSymLinks
AllowOverride All
Order allow,deny
Allow from all
</Directory>
其实就是将Indexes去掉，Indexes表示若当前目录没有index.html就会显示目录结构。
重启Apache服务器/etc/init.d/httpd restart
再访问米扑网站，就没问题了：[https://mimvp.com](https://mimvp.com/)
**1. 禁止访问某些文件/目录**
增加Files选项来控制，比如要不允许访问 .inc 扩展名的文件，保护php类库：
<Files ~ “.inc$”>
Order allow,deny
Deny from all
</Files>
禁止访问某些指定的目录：（可以用 <DirectoryMatch>   来进行正则匹配）
<Directory ~ “^/var/www/(.+/)*[0-9]{3}”>
Order allow,deny
Deny from all
</Directory>
通过文件匹配来进行禁止，比如禁止所有针对图片的访问：
<FilesMatch .(?i:gif|jpeg|png)$>
Order allow,deny
Deny from all
</FilesMatch>
针对URL相对路径的禁止访问：
<Location /dir/>
Order allow,deny
Deny from all
</Location>
**配置示例**
<Directory “E:/Program Files/Apache Software Foundation/Apache2.2/htdocs”>
\#
\# Possible values for the Options directive are “None”, “All”,
\# or any combination of:
\# Indexes Includes FollowSymLinks SymLinksifOwnerMatch ExecCGI MultiViews
\#
\# Note that “MultiViews” must be named *explicitly* — “Options All”
\# doesn’t give it to you.
\#
\# The Options directive is both complicated and important. Please see
\# http://httpd.apache.org/docs/2.2/mod/core.html\#options
\# for more information.
\# 就是这一行，只去掉indexes也可
\#Options Indexes FollowSymLinks
Options FollowSymLinks
\#
\# AllowOverride controls what directives may be placed in .htaccess files.
\# It can be “All”, “None”, or any combination of the keywords:
\# Options FileInfo AuthConfig Limit
\#
AllowOverride None
\#
\# Controls who can get stuff from this server.
\#
Order allow,deny
Allow from all
</Directory>
建议默认情况下，设置APACHE禁止用户浏览目录内容。


