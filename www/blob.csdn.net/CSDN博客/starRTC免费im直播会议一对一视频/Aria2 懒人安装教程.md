# Aria2 懒人安装教程 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年08月02日 15:49:23[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：3312
[https://aria2.github.io/](https://aria2.github.io/)
uI版：[https://github.com/ziahamza/webui-aria2](https://github.com/ziahamza/webui-aria2) web的
可以在线使用的
[https://ziahamza.github.io/webui-aria2/](https://ziahamza.github.io/webui-aria2/) SSL/TLS 加密 需要关闭，然后设置为localhost和6800
[http://aria2c.com/](http://aria2c.com/)
[http://binux.github.io/yaaw/demo/](http://binux.github.io/yaaw/demo/)
配置
[https://aria2c.com/usage.html](https://aria2c.com/usage.html)
[https://aria2.github.io/manual/en/html/aria2c.html](https://aria2.github.io/manual/en/html/aria2c.html)
最简便的方法是直接打开以上网站然后设置主机为localhost会自动连接
命令
aria2c [http://example.org/mylinux.iso](http://example.org/mylinux.iso)
aria2c [http://example.org/mylinux.torrent](http://example.org/mylinux.torrent)
aria2c 'magnet:?xt=urn:btih:248D0A1CD08284299DE78D5C1ED359BB46717D8C'
aria2c --conf-path="/etc/aria2/aria2.conf"
默认用户配置文件~/.aria2/aria2.conf
mkdir ~/.aria2 (该命令会在用户根目录下会生成一个.aria2的隐藏文件夹) open ~/.aria2 (该命令会打开.aria2隐藏文件夹)
将配置文件aria2.conf 拖入.aria2文件夹中（这一步是为了方便每次启动aria2c的时候不用每次手动输入配置文件的位置）
列出种子内容
aria2c -S target.torrent
下载种子内编号为 1、4、5、6、7 的文件
aria2c --select-file=1,4-7 target.torrent
默认启动是 6800 端口
windows上，新建几个文件
Aria2.log （日志，空文件就行） aria2.session （下载历史，空文件就行） aria2.conf （配置文件） HideRun.vbs （隐藏cmd窗口运行用到的）
log=D:\Program Files\aria2\Aria2.log （日志文件，如果不需要日志，这一行可去掉，
DHT全称叫分布式哈希表(Distributed Hash Table)
PT（Private Tracker）下载其实是原理类似 BT（BitTorrent，比特流） 下载的一种，但有两个明显的改进：
- 一是私密的小范围下载；
- 二是进行流量统计，根据下载量决定你的权限。
[https://www.appinn.com/aria2-in-windows-setup/](https://www.appinn.com/aria2-in-windows-setup/)
[https://sspai.com/post/34072](https://sspai.com/post/34072)
[https://www.jianshu.com/p/8437b2e152fa](https://www.jianshu.com/p/8437b2e152fa)
[https://www.jianshu.com/p/bb9490ea1c4d](https://www.jianshu.com/p/bb9490ea1c4d)
[https://medium.com/@Justin___Smith/aria2%E9%85%8D%E7%BD%AE%E6%95%99%E7%A8%8B-mac%E5%92%8Cwindows-b31d0f64bd4e](https://medium.com/@Justin___Smith/aria2%E9%85%8D%E7%BD%AE%E6%95%99%E7%A8%8B-mac%E5%92%8Cwindows-b31d0f64bd4e)
[https://zhuanlan.zhihu.com/p/30666881](https://zhuanlan.zhihu.com/p/30666881)
[https://zhuanlan.zhihu.com/p/21831960](https://zhuanlan.zhihu.com/p/21831960)
小结：
windows实践
1，下载
（[百度盘链接](https://pan.baidu.com/s/1nuRSFzz)，密码: gafp），最新的程序在[https://github.com/aria2/aria2/releases](https://github.com/aria2/aria2/releases)
双击运行 aria2启动.vbs 文件让 aria2 在后台运行（无任何提示）；使用 vbs 仅仅是为了隐藏一个 cmd 窗口
最后，双击运行 aria2关闭.bat 文件关闭在后台运行的 aria2。
默认下载路径：aria2 文件夹内的 downloads 文件夹
打开 [http://aria2c.com/](http://aria2c.com/) 添加删除任务，理论上，打开就能用，无需设置。如果看到右上角的 Aria2 版本号，那么就已经正常工作了。
[将百度盘文件发往 Aria2[Chrome]](https://github.com/acgotaku/BaiduExporter)
