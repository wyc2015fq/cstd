# 开启mysql远程登录 - weixin_33985507的博客 - CSDN博客
2017年02月12日 12:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
开发过程中经常遇到远程访问mysql的问题，每次都需要搜索，感觉太麻烦，这里记录下，也方便我以后查阅。 
首先访问本机的mysql(用ssh登录终端，输入如下命令)：
```
mysql -uroot -p
```
输入密码登陆进去后，输入如下的语句：
```
use mysql;
grant all privileges on *.* to 'user'@'%' identified by 'password' with grant option;
```
其中:
user是用户名
mysql默认的是root
password是密码自己设置
%表示任意主机,也可以指定ip地址
这样就允许远程任意一台主机访问了。
示例：
```
grant all privileges on *.* to 'root'@'192.168.1.100' identified by '' with grant option;
```
即：允许192.168.1.100以root用户无密码登录。
好，就这些了，希望对你有帮助。 
本文github地址：
[https://github.com/mike-zhang/mikeBlogEssays/blob/master/2017/20170212_开启mysql远程登录.md](https://github.com/mike-zhang/mikeBlogEssays/blob/master/2017/20170212_%E5%BC%80%E5%90%AFmysql%E8%BF%9C%E7%A8%8B%E7%99%BB%E5%BD%95.md)
欢迎补充 
