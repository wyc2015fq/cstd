# nginx、php-fpm、mysql用户权限解析 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月09日 14:55:00[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：279
nginx本身不能处理PHP，它只是个web服务器。当接收到客户端请求后，如果是php请求，则转发给php解释器处理，并把结果返回给客户端。如果是静态页面的话，nginx自身处理，然后把结果返回给客户端。
Nginx下php解释器使用最多的就是fastcgi。一般情况nginx把php请求转发给fastcgi管理进程处理，fastcgi管理进程选择cgi子进程进行处理，然后把处理结果返回给nginx。
在这个过程中就牵涉到两个用户，一个是nginx运行的用户，一个是php-fpm运行的用户。如果访问的是一个静态文件的话，则只需要nginx运行的用户对文件具有读权限或者读写权限。
而如果访问的是一个php文件的话，则首先需要nginx运行的用户对文件有读取权限，读取到文件后发现是一个php文件，则转发给php-fpm，此时则需要php-fpm用户对文件具有有读权限或者读写权限。
查看nginx运行在什么用户下
ps aux|grep nginx
看到nginx的父进程是运行在root用户下的，而子进程时运行在nobody用户下，而且只有一个子进程。这个和我们在nginx的配置文件中，配置的一致。
注意：nginx如果没有配置运行用户的话，默认是使用nobody用户运行。使用nobody运行nginx安全性是比较高的。
ps aux |grep php-fpm
通过上图，我们可以看到php-fpm的父进程运行在root用户下，而子进程全部运行在apache用户下。
我们再来看看mysql数据库运行在哪个用户下面，使用ps命令查看。如下：
ps aux |grep mysql
在实际生产环境中，我们一般的配置是nginx与php-fpm都运行在nobody用户下，而且网站的根目录也要属于nobody用户，并且根目录对nobody用户具有所有权限。
这样配置是最安全的，因为nobody用户最安全。即使黑客攻破了网站，但是也不能登录系统。

这个客户端貌似相当不错！！！！
为什么带有图片的文章就不能发布呢？
其实这个问题很简单，文章中的图片需要先上传到网站的根目录下，然后才能正常访问。现在php-fpm运行在apache用户下，而问题是apache用户对虚拟主机a.ilanni.com根目录没有访问权限，更没有写入权限。
那么如何解决这个问题呢？
我们现在需要做的就是统一nginx与php-fpm运行用户为nobody，然后把nginx的虚拟主机a.ilanni.com网站根目录对nobody用户及nobody用户组具有所有权限。
调整php-fpm运行用户
php-fpm我们是通过yum方式进行安装的。而且使用的还是默认配置，该配置文件为/etc/php-fpm.d/www.conf。
编辑完毕后，重启php-fpm。如下：
/etc/init.d/php-fpm restart
ps aux|grep php-fpm
我们现在来修改虚拟主机的根目录用户及用户组。如下：
chown nobody:nobody -R a.ilanni.com/
参考：
[http://www.ilanni.com/?p=7438](http://www.ilanni.com/?p=7438)
