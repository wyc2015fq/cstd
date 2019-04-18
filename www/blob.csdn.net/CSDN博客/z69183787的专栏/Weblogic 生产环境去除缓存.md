# Weblogic 生产环境去除缓存 - z69183787的专栏 - CSDN博客
2012年11月20日 11:46:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1766
weblogic 下的生产环境，默认修改文件，只能通过先删除缓存，后重启应用才能有效果。
缓存路径为：
Y:\portal.shmetro.com\portal.shmetro.com\servers\AdminServer\tmp\_WL_user
domain_name 下的 servers 下的 服务器名 下的tmp 下的 WL_user 下 会有该应用的名称，停止应用并删除该文件夹后，
启动应用，改变生效。
