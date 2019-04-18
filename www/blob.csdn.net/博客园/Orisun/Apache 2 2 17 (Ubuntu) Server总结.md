# Apache/2.2.17 (Ubuntu) Server总结 - Orisun - 博客园







# [Apache/2.2.17 (Ubuntu) Server总结](https://www.cnblogs.com/zhangchaoyang/articles/2101589.html)





卸载apache
首先停止服务
sudo /etc/init.d/apache2 stop
再卸载
sudo apt-get  --purge remove apache2
sudo apt-get --purge remove apache2.2-common
sudo apt-get autoremove (此命令会自动卸载PHP)

安装apache
sudo apt-get install apache2

启动、停止和重启
因为apache2ctl命令在/usr/sbin下面，所以可以直接这样：
sudo apache2ctl -k start
sudo apache2ctl -k stop
sudo apache2ctl -k restart
也可以使用/etc/init.d/下的apache2命令：
sudo /etc/init.d/apache2 start
sudo /etc/init.d/apache2 stop
sudo /etc/init.d/apache2 restart
注：最新版本的apache已经默认是开机自动启动了

安装后的原始文件结构

主目录是/etc/apache2

conf.d下有个charset，把文件里“AddDefaultCharset UTF-8”前的注释去掉，当然你也可以把编码改为其他格式

打开mods-available，这些都是可以使用的模块，Apache要动态加载的模块在/usr/lib/apache2/modules/下，最新版本的apache已经默认load了dav_svn_module和authz_svn_module

打开sites-avalable，有个很重要的文件default，其全部内容如下：

<VirtualHost *>
	ServerAdmin webmaster@localhost

	DocumentRoot /var/www
	<Directory />
		Options FollowSymLinks
		AllowOverride None
	</Directory>
	<Directory /var/www/>
		Options Indexes FollowSymLinks MultiViews
		AllowOverride None
		Order allow,deny
		allow from all
	</Directory>

	ScriptAlias /cgi-bin/ /usr/lib/cgi-bin/
	<Directory "/usr/lib/cgi-bin">
		AllowOverride None
		Options +ExecCGI -MultiViews +SymLinksIfOwnerMatch
		Order allow,deny
		Allow from all
	</Directory>

	ErrorLog ${APACHE_LOG_DIR}/error.log

	# Possible values include: debug, info, notice, warn, error, crit,
	# alert, emerg.
	LogLevel warn

	CustomLog ${APACHE_LOG_DIR}/access.log combined

    Alias /doc/ "/usr/share/doc/"
    <Directory "/usr/share/doc/">
        Options Indexes MultiViews FollowSymLinks
        AllowOverride None
        Order deny,allow
        Deny from all
        Allow from 127.0.0.0/255.0.0.0 ::1/128
    </Directory>

</VirtualHost>



这里还有三个配置文件：apache2.conf、ports.conf和httpd.conf，httpd.conf是空的，当你要把不同的虚拟主机布置在不同的端口上时，需要配置ports.conf。apache2.conf是主配置文件，它里面有这第几句话：

Include httpd.conf

Include ports.conf

Include conf.d/

Include sites-enabled/

启动apache时提示：apache2: Could not reliably determine the server's fully qualified domain name, using 127.0.1.1 for ServerName

在ports.conf中“Listen 80”的上面加入这第一句：ServerName 127.0.0.1

上面ports.conf里面配置的DocumentRoot是/var/www，可我在

/var/www下面放一个index,html后访问127.0.0.1/idex.html得到404 Not Found，查看日志文件/var/log/apache2/error.log，发现：/etc/apache2/htdocs，哪里指定
DocumentRoot是
/etc/apache2/htdocs了呢？无奈我只好建立

/etc/apache2/htdocs并把index.html放入其中，再次访问

127.0.0.1/idex.html成功，但apache只是把它当成一个普通的文件全部显示出来，并没有当成html文件来展现。查看日志文件，发现：Attempt to serve directory: /etc/apache2/htdocs/

把default的<VirtualHost *>改为<VirtualHost 127.0.0.1:80>

日志错误： File does not exist: /etc/apache2/htdocs/favicon.ico

创建一个图标文件就可以了，有很多在线生成icon的网站

这了让apache和svn搭配使用，我已建好了仓库/opt/svnroot，并使用htpasswd命令创建了密码文件/etc/apache2/svnuser，在httpd.conf中添加

如下内容：



<Location "/svn_repos">
	DAV svn
	SVNPath "/opt/svnroot"
	AuthType Basic
	AuthName "Log to SVN"
	AuthUserFile "/etc/apache2/svnuser"
	#允许匿名用户只读访问
	#<LimitExcept GET REPORT OPTIONS PROPFIND>
		Require valid-user
	#</LimitExcept>
</Location>


发生错误：Invalid command 'AuthUserFile', perhaps misspelled or defined by a module not included in the server configuration

在httpd.conf中加载三个模块：

LoadModule auth_basic_module /usr/lib/apache2/modules/mod_auth_basic.so

LoadModule authz_owner_module /usr/lib/apache2/modules/mod_authz_owner.so

LoadModule authn_file_module /usr/lib/apache2/modules/mod_authn_file.so

但是提交代码（svn import -m "import a.c" ~/a.c http://127.0.0.1/svn_repos/）时发生错误：svn: Server sent unexpected return value (500 Internal Server Error) in response to OPTIONS request for 'http://127.0.0.1/svn_repos'

这是验证用户功能出了问题

如果允许匿名用户只读访问

<LimitExcept GET REPORT OPTIONS PROPFIND>
		Require valid-user</LimitExcept>则检出时是没有问题的：svn co http://zcypc/svn_repos ~/web成功，在浏览器中访问127.0.0.1/svn_repos也顺利。
既然用户验证的问题暂时提不到解决，那我就为了让匿名用户也具有写权限--可以提交代码算了：把Require valid-user一句注释掉~~安装apxs为Apache编译安装扩展模块时需要apxs工具。ubuntu采用apt-get自动安装的apache后并没有安装apxs，需要再apt-get install apache2-dev把apxs装上，默认装在了/usr/lib/apache2/modules/apxs2.












