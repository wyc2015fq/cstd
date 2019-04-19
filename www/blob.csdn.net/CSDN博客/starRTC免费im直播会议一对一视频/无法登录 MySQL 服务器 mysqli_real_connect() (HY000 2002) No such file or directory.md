# 无法登录 MySQL 服务器 mysqli_real_connect()  (HY000 2002) No such file or directory - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月19日 17:04:40[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：2349
mysqli_real_connect(): (HY000/2002): No such file or directory
change localhost to 127.0.0.1 in config.inc.php
$cfg['Servers'][$i]['host'] = '127.0.0.1';
