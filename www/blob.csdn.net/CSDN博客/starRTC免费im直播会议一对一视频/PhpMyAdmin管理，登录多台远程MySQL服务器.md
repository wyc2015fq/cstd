# PhpMyAdmin管理，登录多台远程MySQL服务器 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年10月08日 16:35:07[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：296
法一：
可直接在config.inc.php里添加数据库连接信息即可
先$i++， 然后复制原来的配置信息后修改
不过这种方式需要将连接信息写在配置文件中，有点麻烦。
这种后面省事，不用填信息，选择一下即可。
法二。
可以选择下面这种方式。
$cfg['AllowArbitraryServer'] = 1;
$cfg['Servers'][$i]['extension'] = 'mysql';//修改mysqli为mysql
//$cfg['Servers'][$i]['controluser'] = 'root';//下面2行可以注释
//$cfg['Servers'][$i]['controlpass'] = 'pwd';
登录时本地可填localhost
其它远程服务器可填IP:Port
