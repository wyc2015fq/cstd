# 如何在一个 U 盘上安装多个 Linux 发行版 - 文章 - 伯乐在线
原文出处： [Ambarish Kumar](https://itsfoss.com/multiple-linux-one-usb/)   译文出处：[Linux中国/geekpi](https://linux.cn/article-8957-1.html)
> 
本教程介绍如何在一个 U 盘上安装多个 Linux 发行版。这样，你可以在单个 U 盘上享受多个现场版live Linux 发行版了。
我喜欢通过 U 盘尝试不同的 Linux 发行版。它让我可以在真实的硬件上测试操作系统，而不是虚拟化的环境中。此外，我可以将 USB 插入任何系统（比如 Windows 系统），做任何我想要的事情，以及享受相同的 Linux 体验。而且，如果我的系统出现问题，我可以使用 U 盘恢复！
创建单个[可启动的现场版 Linux USB](https://itsfoss.com/create-live-usb-of-ubuntu-in-windows/) 很简单，你只需下载一个 ISO 文件并将其刻录到 U 盘。但是，如果你想尝试多个 Linux 发行版呢？你可以使用多个 U 盘，也可以覆盖同一个 U 盘以尝试其他 Linux 发行版。但这两种方法都不是很方便。
那么，有没有在单个 U 盘上安装多个 Linux 发行版的方式呢？我们将在本教程中看到如何做到这一点。
### 如何创建有多个 Linux 发行版的可启动 USB
我们有一个工具正好可以做到*在单个 U 盘上保留多个 Linux 发行版*。你所需要做的只是选择要安装的发行版。在本教程中，我们将介绍*如何在 U 盘中安装多个 Linux 发行版*用于现场会话live session。
要确保你有一个足够大的 U 盘，以便在它上面安装多个 Linux 发行版，一个 8 GB 的 U 盘应该足够用于三四个 Linux 发行版。
#### 步骤 1
[MultiBootUSB](http://multibootusb.org/) 是一个自由、开源的跨平台应用程序，允许你创建具有多个 Linux 发行版的 U 盘。它还支持在任何时候卸载任何发行版，以便你回收驱动器上的空间用于另一个发行版。
下载 .deb 包并双击安装。
[下载 MultiBootUSB](https://github.com/mbusb/multibootusb/releases/download/v8.8.0/python3-multibootusb_8.8.0-1_all.deb)
#### 步骤 2
推荐的文件系统是 FAT32，因此在创建多引导 U 盘之前，请确保格式化 U 盘。
#### 步骤 3
下载要安装的 Linux 发行版的 ISO 镜像。
#### 步骤 4
完成这些后，启动 MultiBootUSB。
![MultiBootUSB](http://jbcdn2.b0.upaiyun.com/2017/10/59bb31afe67fed96bf8544aa03d90b62.png)
主屏幕要求你选择 U 盘和你打算放到 U 盘上的 Linux 发行版镜像文件。
MultiBootUSB 支持 Ubuntu、Fedora 和 Debian 发行版的持久化，这意味着对 Linux 发行版的现场版本所做的更改将保存到 USB 上。
你可以通过拖动 MultiBootUSB 选项卡下的滑块来选择持久化大小。持久化为你提供了在运行时将更改保存到 U 盘的选项。
![MultiBootUSB persistence storage](http://jbcdn2.b0.upaiyun.com/2017/10/1445361a61e458b7f7c0bee4ee389da3.png)
#### 步骤 5
单击“安装发行版”选项并继续安装。在显示成功的安装消息之前，需要一些时间才能完成。
你现在可以在已安装部分中看到发行版了。对于另外的操作系统，重复该过程。这是我安装 Ubuntu 16.10 和 Fedora 24 后的样子。
![MultiBootSystem with Ubuntu and Fedora](http://jbcdn2.b0.upaiyun.com/2017/10/04299cb6ee0226e024c35f1a0445bc49.png)
#### 步骤 6
下次通过 USB 启动时，我可以选择任何一个发行版。
![Boot Menu](http://jbcdn2.b0.upaiyun.com/2017/10/67d16917c3bce3f95bc67691edcb45c7.png)
只要你的 U 盘允许，你可以添加任意数量的发行版。要删除发行版，请从列表中选择它，然后单击卸载发行版。
### 最后的话
MultiBootUSB 真的很便于在 U 盘上安装多个 Linux 发行版。只需点击几下，我就有两个我最喜欢的操作系统的工作盘了，我可以在任何系统上启动它们。
如果你在安装或使用 MultiBootUSB 时遇到任何问题，请在评论中告诉我们。
