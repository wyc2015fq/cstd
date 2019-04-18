# MongoDB Linux 安装日志 - weixin_33985507的博客 - CSDN博客
2015年09月15日 18:58:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
首先说是安装日志，其实这一次并没有安装，
到官网上http://www.mongodb.org/downloads下载对应版本的安装包，解压后，运行服务即可。
1.下载解压
[](https://link.jianshu.com?t=http://photo.blog.sina.com.cn/showpic.html#blogid=5fe385900101h1y6&url=http://album.sina.com.cn/pic/001KS8qkgy6JrnLjNsh55)
![882388-04e4d4e9965aa0a5](https://upload-images.jianshu.io/upload_images/882388-04e4d4e9965aa0a5)
2.把解压后的mongodb-linux-i686-1.8.2目录里面的内容拷贝到/usr/local/mongodb内。【方便管理】
3.在/data/目录下创建db目录，即/data/db;【存放库所在文件夹，可以修改】
在vim /usr/local/mongodb/bin/mongodb.conf 配置文件（没有就新增）
![882388-cab5a4bf7f069a7e.png](https://upload-images.jianshu.io/upload_images/882388-cab5a4bf7f069a7e.png)
4.进入/usr/local/mongodb/bin 找到mongod，注意这里才是mongodb的服务，如下图：
[](https://link.jianshu.com?t=http://photo.blog.sina.com.cn/showpic.html#blogid=5fe385900101h1y6&url=http://album.sina.com.cn/pic/001KS8qkgy6JroaWMpvfd)
![882388-ef6025a820e08fcf](https://upload-images.jianshu.io/upload_images/882388-ef6025a820e08fcf)
使用配置文件启动mongo
/usr/local/mongodb/bin/mongod --config /usr/local/mongodb/bin/mongodb.conf
5.另外开启命令行界面，我们通过命令行控制一下装好的mongoDB，注意区别mongod与mongo两个文件
前者是服务，后者是控制终端，如下图：
[](https://link.jianshu.com?t=http://photo.blog.sina.com.cn/showpic.html#blogid=5fe385900101h1y6&url=http://album.sina.com.cn/pic/001KS8qkgy6Jrossc7204)
![882388-9ce2f8106f9993f3](https://upload-images.jianshu.io/upload_images/882388-9ce2f8106f9993f3)
安装mongodb php扩展
1、下载扩展包，地址：https://github.com/mongodb/mongo-php-driver/archive/master.zip
[](https://link.jianshu.com?t=http://photo.blog.sina.com.cn/showpic.html#blogid=5fe385900101h1y6&url=http://album.sina.com.cn/pic/001KS8qkgy6JroUHwC21e)
![882388-38117a79ac9de16e](https://upload-images.jianshu.io/upload_images/882388-38117a79ac9de16e)
2、进入解压后的目录编译
phpize
./configure
make install
3、修改php.ini
extension =/usr/local/src/mongo-php-driver-master/modules/mongo.so【这是mongo.so的目录地址】
最后注意一下，如果有开启防火墙，记得开启mongo访问端口
vim /etc/sysconfig/iptables
-A INPUT -m state --state NEW -m tcp -p tcp --dport 27017 -j ACCEPT
service iptables restart 重启防火墙
