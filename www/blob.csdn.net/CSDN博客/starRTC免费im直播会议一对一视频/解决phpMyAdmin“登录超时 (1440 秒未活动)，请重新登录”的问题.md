# 解决phpMyAdmin“登录超时 (1440 秒未活动)，请重新登录”的问题 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年10月10日 15:53:29[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1975

## 问题描述
phpMyAdmin打开一段时间后会出现“登录超时 (1440 秒未活动)，请重新登录”的问题：
## 解决方法
永久有效：
vim config.inc.php
然后在里面加上下面这一行：
$cfg['LoginCookieValidity'] = 1440000 ;
## 查找php.ini文件路径
一般在/etc目录下。
新建一个test.php页面，输入
**<?php**phpinfo();**?>**
使用浏览器访问该页面，会看到一个PHP配置信息的相关页面
找到Configuration File (php.ini) Path这一行
右边的值 /etc 就是php.ini文件所在目录
打开后在php.ini中修改session.gc_maxlifetime
修改以后记得重启相关程序方能生效。
service php-fpm restart
临时解决方法：
登录进去后，点击“服务器:localhost”->“设置”->“功能”
将“登录cookie有效期”设置为一个比较大的值即可（但正如上图所示，此修改仅作用于本次会话，下次重新登录会失效）
同时在首页底部会出现如下提示，不用管它：
## 艺搜参考
[http://www.qttc.net/201208179.html](http://www.qttc.net/201208179.html)
[](http://www.qttc.net/201208179.html)
## 问题描述
phpMyAdmin打开一段时间后会出现“登录超时 (1440 秒未活动)，请重新登录”的问题：
## 解决方法
永久有效：
vim config.inc.php
然后在里面加上下面这一行：
$cfg['LoginCookieValidity'] = 1440000 ;
## 查找php.ini文件路径
一般在/etc目录下。
新建一个test.php页面，输入
<?phpphpinfo();?>
使用浏览器访问该页面，会看到一个PHP配置信息的相关页面
找到Configuration File (php.ini) Path这一行
右边的值 /etc 就是php.ini文件所在目录
打开后在php.ini中修改session.gc_maxlifetime
修改以后记得重启相关程序方能生效。
service php-fpm restart
临时解决方法：
登录进去后，点击“服务器:localhost”->“设置”->“功能”
将“登录cookie有效期”设置为一个比较大的值即可（但正如上图所示，此修改仅作用于本次会话，下次重新登录会失效）
同时在首页底部会出现如下提示，不用管它：
## 艺搜参考
[http://www.qttc.net/201208179.html](http://www.qttc.net/201208179.html)
