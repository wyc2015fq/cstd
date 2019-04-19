# Discuz系列1：安装 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月11日 09:36:07[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：427
[http://www.discuz.net/forum.php](http://www.discuz.net/forum.php)     官网，点击“[Discuz!
 程序发布](http://www.discuz.net/forum-10-1.html)”
代码库：
[https://git.oschina.net/ComsenzDiscuz/DiscuzX](https://git.oschina.net/ComsenzDiscuz/DiscuzX)
下载简体UTF-8 [http://www.comsenz.com/downloads/install/discuzx](http://www.comsenz.com/downloads/install/discuzx)
解压并上传 Discuz! X3 程序到服务器且修改相应目录权限
- upload 这个目录下面的所有文件是我们需要上传到服务器上的可用程序文件；
- readme 目录为产品介绍、授权、安装、升级、转换以及版本更新日志说明；
- utility 目录为论坛附带工具，包括升级程序。
将其中 upload 目录下的所有文件使用 FTP 软件或winscp以二进制方式上传到空间。
设置相关目录的文件属性，以便数据文件可以被程序正确读写
将服务器上以下目录、以及该目录下面的所有文件的属性设置为 777，
chmod -R 777 config/ data/ uc_client/ uc_server/
关于目录权限修改可以参考：点击查看详细说明 [http://faq.comsenz.com/viewnews-183](http://faq.comsenz.com/viewnews-183)
在浏览器中运行 [http://www.domain.com/install/](http://www.domain.com/install/) 开始全新安装
[](http://www.domain.com/%E4%B8%BA%E4%BD%A0%E7%9A%84%E7%AB%99%E7%82%B9%E8%AE%BF%E9%97%AE%E5%9C%B0%E5%9D%80%EF%BC%89)
选择“全新安装 Discuz! X (含 UCenter Server)”
安装完后,删除install/index.php
问题：
如何实现的：

## 艺搜参考
[http://www.discuz.net/thread-3258186-1-1.html](http://www.discuz.net/thread-3258186-1-1.html)
[http://wangjing.cn/](http://wangjing.cn/)

