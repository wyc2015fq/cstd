# ubuntu11.04安装nginx+php+mysql - =朝晖= - 博客园
# [ubuntu11.04安装nginx+php+mysql](https://www.cnblogs.com/dhcn/p/7105356.html)
先列参考内容，后面我再补充点东西：
[http://www.4wei.cn/archives/1001436](http://www.4wei.cn/archives/1001436)
[http://www.gidot.net/blog/article.asp?id=322](http://www.gidot.net/blog/article.asp?id=322)
上面2个教程大概都差不多。
无非按顺序安装nginx,php5,php5-fpm,[MySQL](http://lib.csdn.net/base/mysql) Server。
我补充几点：
1、nginx安装就不说什么了。
2、关于php5安装，我说两点：
         2.1：在上面的教程里，在php5安装指令里面，很多人装了很多扩展，我估计是那个教程的源作者是他的具体应用环境下安装指令，你不必装那么多，比如有了后面的php5-fpm,就不用装[PHP](http://lib.csdn.net/base/php)-cgi了，把php5、pear主要的包装了就行，其他的根据自己的应用需要再裁剪吧。
         2.2：如果在新立得里，它会依赖安装apache2,当然如果前面你启动nginx后，没有关闭它，apache2是无法启动的，否则的话，apache2在安装完后会自动启动，所以在此之后，如果你要启动 nginx的话，提示：地址端口无法绑定，当然你可以用下面的netstat指令：
```
sudo netstat -tlnp
```
看看到底是那个程序占了www服务80端口,然后用前面stop nginx的方法关闭apache2即可，关于apache2，还有一个问题，它和nginx都会注册为开机启动服务,不过nginx的start 优先级比apache2的高，所以开机以后，nginx先占用了端口，apache2启动不了，可按以下指令参看如下：
```
username@hostname:~$ ls -l /etc/rc?.d/*apache2  
lrwxrwxrwx 1 root root 17 2011-10-12 16:07 /etc/rc0.d/K09apache2 -> ../init.d/apache2  
lrwxrwxrwx 1 root root 17 2011-10-12 16:07 /etc/rc1.d/K09apache2 -> ../init.d/apache2  
lrwxrwxrwx 1 root root 17 2011-10-12 16:07 /etc/rc2.d/S91apache2 -> ../init.d/apache2  
lrwxrwxrwx 1 root root 17 2011-10-12 16:07 /etc/rc3.d/S91apache2 -> ../init.d/apache2  
lrwxrwxrwx 1 root root 17 2011-10-12 16:07 /etc/rc4.d/S91apache2 -> ../init.d/apache2  
lrwxrwxrwx 1 root root 17 2011-10-12 16:07 /etc/rc5.d/S91apache2 -> ../init.d/apache2  
lrwxrwxrwx 1 root root 17 2011-10-12 16:07 /etc/rc6.d/K09apache2 -> ../init.d/apache2  
username@hostname:~$ ls -l /etc/rc?.d/*nginx  
lrwxrwxrwx 1 root root 15 2011-10-12 15:23 /etc/rc0.d/K20nginx -> ../init.d/nginx  
lrwxrwxrwx 1 root root 15 2011-10-12 15:23 /etc/rc1.d/K20nginx -> ../init.d/nginx  
lrwxrwxrwx 1 root root 15 2011-10-12 15:23 /etc/rc2.d/S20nginx -> ../init.d/nginx  
lrwxrwxrwx 1 root root 15 2011-10-12 15:23 /etc/rc3.d/S20nginx -> ../init.d/nginx  
lrwxrwxrwx 1 root root 15 2011-10-12 15:23 /etc/rc4.d/S20nginx -> ../init.d/nginx  
lrwxrwxrwx 1 root root 15 2011-10-12 15:23 /etc/rc5.d/S20nginx -> ../init.d/nginx  
lrwxrwxrwx 1 root root 15 2011-10-12 15:23 /etc/rc6.d/K20nginx -> ../init.d/nginx
```
如果你愿意原因，你可以把apache2开机启动取消了：
```
user@host:~$ sudo update-rc.d -f apache2 remove  
 Removing any system startup links for /etc/init.d/apache2 ...  
   /etc/rc0.d/K09apache2  
   /etc/rc1.d/K09apache2  
   /etc/rc2.d/S91apache2  
   /etc/rc3.d/S91apache2  
   /etc/rc4.d/S91apache2  
   /etc/rc5.d/S91apache2  
   /etc/rc6.d/K09apache2
```
如果你的机器是开发的用的机器，这些服务仅仅是为了测试程序用，不准备长时间运行，你可以用以上指令把nginx、php5-fpm的开机启动都取消了。不过mysql服务估计不能这样取消，因为用前面的ls指令看不到mysql link，不过有blog说用**rcconf工具**
3、关于php5-fpm的选择,可以参看：[http://www.mike.org.cn/articles/what-is-cgi-fastcgi-php-fpm-spawn-fcgi/](http://www.mike.org.cn/articles/what-is-cgi-fastcgi-php-fpm-spawn-fcgi/)
4、你在phpinfo页面也许会看到如下信息：
|_SERVER["USER"]|www-data|
|----|----|
|_SERVER["HOME"]|/var/www|
那个/var/www是apache2的发布目录，但在此处仅是指启动nginx的专指用户是www-data,而这个用户的主目录是/var/www,和发布目录没有关系，真正的发布目录还是：
|_SERVER["DOCUMENT_ROOT"]|/usr/share/nginx/www|
参考文章：
[http://www.debuntu.org/how-to-manage-services-with-update-rc.d](http://www.debuntu.org/how-to-manage-services-with-update-rc.d)
[http://www.mike.org.cn/articles/what-is-cgi-fastcgi-php-fpm-spawn-fcgi/](http://www.mike.org.cn/articles/what-is-cgi-fastcgi-php-fpm-spawn-fcgi/)

