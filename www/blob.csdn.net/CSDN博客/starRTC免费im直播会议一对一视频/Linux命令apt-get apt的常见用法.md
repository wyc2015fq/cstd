# Linux命令apt-get  apt的常见用法 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年06月30日 16:03:59[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1128
高级包装工具（英语：Advanced Packaging Tools，缩写为APT）
apt-cache search foo //搜索和"foo"匹配的包。
apt-cache show foo //显示"foo"包的相关信息，例如描述、版本、大小、依赖以及冲突。
apt-get install package //安装包
apt-get remove package //删除包，保留配置文件。
apt-get remove --purge package  //连配置文件一块删除.
apt-get update  **Update the Package Index**
在你更改了/etc/apt/sources.list 或 /etc/apt/preferences 后，需要运行这个命令以令改动生效。同时也要定期运行该命令，以确保你的源列表是最新的。该命令等价于新立得软件包管理器中的“刷新”，或者是 Windows和OS X 下的 Adept 软件包管理器的“check
 for updates”。
apt-get upgrade
更新所有已安装的软件包。类似一条命令完成了新立得软件包管理器中的“标记所有软件包以便升级”并且“应用”。注意在运行该命令前应先运行 apt-get update更新数据库。

## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Linux%E5%91%BD%E4%BB%A4apt-get%E7%9A%84%E5%B8%B8%E8%A7%81%E7%94%A8%E6%B3%95&action=edit&section=1)]艺搜参考
[http://wiki.ubuntu.org.cn/UbuntuHelp:AptGet/Howto/zh](http://wiki.ubuntu.org.cn/UbuntuHelp:AptGet/Howto/zh)
[Ubuntu Linux系统下apt-get命令详解](http://os.51cto.com/art/200802/65583.htm)
[http://baike.baidu.com/view/1580236.htm](http://baike.baidu.com/view/1580236.htm)
[http://www.blogjava.net/void241/archive/2008/10/05/223840.html](http://www.blogjava.net/void241/archive/2008/10/05/223840.html)
