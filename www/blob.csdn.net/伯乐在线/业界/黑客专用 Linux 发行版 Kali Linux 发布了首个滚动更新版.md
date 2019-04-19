# 黑客专用 Linux 发行版 Kali Linux 发布了首个滚动更新版 - 文章 - 伯乐在线
原文出处： [Linux中国](https://linux.cn/article-6926-1.html)
著名的黑客专用 Linux 发行版 Kali Linux 公开发布了其第一个滚动更新版 2016.1。Kali Linux 在 2.0 版本时改为了滚动更新模式，不过仅有部分尝鲜的人可以从 2.0 升级为滚动更新版。经过五个月的测试后，Kali Linux 给黑客们送上了这份最好的礼物 —— 继承了 Debian 的稳定性，集成了各种渗透测试工具，滚动更新的 Kali-Rolling (2016.1)。
![](http://jbcdn2.b0.upaiyun.com/2016/01/1bdf5123ce2a40a4b2d9d09ed7bcb230.jpg)
### Kali 滚动更新版与标准版本的比较
Kali 滚动更新版是基于 Kali 标准版的，而不是基于标准的 Debian 发行版（比如说 Debian 7、8、9等），也不用经历“新版本、维护、过期”的阶段。Kali 滚动版会不断从 [Debian testing](https://www.debian.org/devel/testing) 进行更新，确保源源不断的得到软件的新版本。
### 持续更新的渗透测试工具
在测试滚动更新版的这五个月里，渗透测试工具的更新自动提示系统工作得很好，总是可以及时得到这些关注的工具的及时更新。一般来说，从得到这些新工具的更新提示，然后经过打包、测试和发布到仓库的过程，要经过24-48小时的时间差。我们的新的 **[Kali Linux Package Tracker](http://pkg.kali.org/)**可以让你通过邮件和更全面的 Web 界面了解到 Kali Linux 的变化过程。这个跟踪器可以让你了解到各个工具和软件包的什么版本是什么时候进入到仓库中的。举个例子，下面的截屏显示了 nmap 软件包在 Kali 中的版本变化：
![pkg-kali](http://jbcdn2.b0.upaiyun.com/2016/01/2a37ecc8077a117570c4bc8fd896bb7c.png)
### VMware Tools 与 Open-VM-Tools 的对比
这个版本也有一个 VMware 客户端工具方面的戏剧性变化。在 2015年9月， VMware [推荐](http://kb.vmware.com/kb/2073803) 使用发行版特定的 **open-vm-tools**，而不是使用原来的针对客户机的 VMware Tools 软件包。最新的 Kali Linux 滚动更新版包含了一个可以完美工作的 open-vm-tools 软件包，实现了所有需要的功能，比如文件复制、剪贴板复制/粘贴和自动屏幕缩放等。要安装你的 Kali Linux 滚动更新版中的 open-vm-tools 软件包，使用如下命令：


```
apt-get update
apt-get install open-vm-tools-desktop fuse
reboot
```
### 从 Kali 2.0 升级到 Kali 滚动更新版
要从已经安装好的 Kali 2.0（sana）升级到 Kali 滚动更新版很简单，以 root 身份，运行如下命令：


```
cat  /etc/apt/sources.list
deb http://http.kali.org/kali kali-rolling main non-free contrib
EOF
apt-get update
apt-get dist-upgrade # 喝杯咖啡，可能需要10分钟
reboot
```
**请注意，Kali 2.0（sana）的仓库不再更新了，并将于2016年4月15日结束产品周期。**
### 下载 Kali Linux 滚动更新版
Kali Linux 2.0 包括两种 ISO 镜像，一个使用 Gnome 的完整的 ISO 镜像，以及一个“轻量级” ISO 镜像——只包括 [top 10](http://tools.kali.org/kali-metapackages) 的元包和 XFCE。
下载地址： [https://www.kali.org/downloads/](https://www.kali.org/downloads/)
|**镜像名**|**类型**|**Torrent**|**大小**|**版本**|**SHA1Sum**|
|----|----|----|----|----|----|
|**Kali Linux 64 bit**|[ISO](http://cdimage.kali.org/kali-2016.1/kali-linux-2016.1-amd64.iso)|[Torrent](http://images.kali.org/kali-linux-2016.1-amd64.torrent)|2.6G|**2016.1**|deaa41c5c8f26b7854cafb34b6f1b567871c4875|
|**Kali Linux 32 bit**|[ISO](http://cdimage.kali.org/kali-2016.1/kali-linux-2016.1-i386.iso)|[Torrent](http://images.kali.org/kali-linux-2016.1-i386.torrent)|2.6G|**2016.1**|23dadf9c6d3fcd190e345ee070aa57155e93b745|
|**Kali Linux 64 bit Light**|[ISO](http://cdimage.kali.org/kali-2016.1/kali-linux-light-2016.1-amd64.iso)|[Torrent](http://images.kali.org/kali-linux-light-2016.1-amd64.torrent)|0.8G|**2016.1**|4132238042deba9e3bc1702afbdb1b4672b64bcb|
|**Kali Linux 32 bit Light**|[ISO](http://cdimage.kali.org/kali-2016.1/kali-linux-light-2016.1-i386.iso)|[Torrent](http://images.kali.org/kali-linux-light-2016.1-i386.torrent)|0.8G|**2016.1**|addd89b750e31030e96c6cbd5a3da4f0f17287a8|
|**Kali Linux 64 bit mini**|[ISO](http://cdimage.kali.org/kali-2016.1/kali-linux-mini-2016.1-amd64.iso)|[Torrent](http://images.kali.org/kali-linux-mini-2016.1-amd64.torrent)|30M|**2016.1**|0b37df151ba4a78c940d19af8beafd380326f196|
|**Kali Linux 32 bit mini**|[ISO](http://cdimage.kali.org/kali-2016.1/kali-linux-mini-2016.1-i386.iso)|[Torrent](http://images.kali.org/kali-linux-mini-2016.1-i386.torrent)|30M|**2016.1**|bb5bb9cf94ef626aac420de019669862861c013d|
|**Kali Linux armel**|[Image](http://cdimage.kali.org/kali-2016.1/kali-linux-light-2016.1-armel.img.xz)|[Torrent](http://images.kali.org/kali-linux-light-2016.1-armel.torrent)|0.7G|**2016.1**|b40781f0427924256ce8c7d1c3cbbb5cc824b5fa|
|**Kali Linux armhf**|[Image](http://cdimage.kali.org/kali-2016.1/kali-linux-light-2016.1-armhf.img.xz)|[Torrent](http://images.kali.org/kali-linux-light-2016.1-armhf.torrent)|0.7G|**2016.1**|cd750dde538eaed9f8e4efea011a9b9dc1e75143|
