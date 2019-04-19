# vmdk/vdi互转（VirtualBox-WMvare） - 三少GG - CSDN博客
2013年01月21日 01:04:38[三少GG](https://me.csdn.net/scut1135)阅读数：2614
个人分类：[流媒体基础](https://blog.csdn.net/scut1135/article/category/934830)
### [vmdk/vhd 转 vdi（vmdk/vhd to vdi）、vdi 转 vmdk/vhd（vdi to vmdk/vhd）](http://caihorse.iteye.com/blog/871433)
vmdk/vhd 转 vdi（vmdk/vhd to vdi）、vdi 转 vmdk/vhd（vdi to vmdk/vhd）一步完成
最近由于要把 vmdk 转成 vdi 格式（本来想继续用 VMware 的，但由于 VMware 的无缝模式没有 VirtualBox 好，且内存、CPU 占有太高，所以又换回了 VirtualBox。换来换去，换了好几次了，我是不折腾会死星人(￣▽￣")），想上网找一下有没有比上一篇文章提到的更简单的方法。搜索了一下，除了上一篇写的要用到 qemu 软件进行两次转换外，还真让我找到了一个**一步转换完成**的方法。
      原来新版的 VirtualBox 附带的 VBoxManage 已经可以实现转换虚拟磁盘格式的功能,不再需要借助 qemu 等第三方软件（[將vmdk/vhd轉換為VDI格式](http://blog.tenyi.com/2009/12/convert-from-vmdkvhd-to-vdi-format.html)）。
      把 vmdk 转成 vdi 的命令是：
VBoxManageclonehdsource.vmdktarget.vdi--formatVDI
**   --format**
    是指target的格式，source的格式似乎会自动判断
Your are no longer needed to use vditool, the tool "VBoxManage" comes with recent revision VirtualBox.
Just type the command:
> 
VBoxManage clonehd source.vmdk target.vdi --format VDI
**- format**
refers to the target format, source format seems to be automatically determined.
VirtualBox Command Line Management Interface Version 3.1.0
(C) 2005-2009 Sun Microsystems, Inc.
All rights reserved.
Usage:
VBoxManage clonehd          <uuid>|<filename> <outputfile>
                            [--format VDI|VMDK|VHD|RAW|<other>]
                            [--variant Standard,Fixed,Split2G,Stream,ESX]
                            [--type normal|writethrough|immutable]
                            [--remember] [--existing]</other></outputfile></filename></uuid>
        VBoxManage 的其他参数可以通过在命令提示符中输入 VBoxManage 或 "E:\Program Files\VirtualBox\VBoxManage.exe"(VBoxManage.exe 文件路径) 来获取。
Allrightsreserved.Usage:VBoxManage [-v|--version]    printversionnumberandexitVBoxManage [-q|--nologo] ... suppressthelogoVBoxManagelist [--long|-l] vms|runningvms|ostypes|hostdvds|hostfloppies|bridgedifs|hostonlyifs|dhcpservers|hostinfo|hostcpuids|hddbackends|hdds|dvds|floppies|usbhost|usbfilters|systempropertiesVBoxManageshowvminfo       <uuid>|<name> [--details] [--statistics][--machinereadable]
VBoxManageshowvminfo       <uuid>|<name> --log <idx>
………………
实例操作（图）
      命令：
"E:\Program Files\VirtualBox\VBoxManage.exe" clonehd "D:\Backup\我的文档\Virtual Machines\Ubuntu\Ubuntu.vmdk" "D:\Backup\我的文档\Virtual Machines\Ubuntu\Ubuntu.vdi" --format VDI
或 "E:\Program Files\VirtualBox\VBoxManage.exe" clonehd Ubuntu.vmdk Ubuntu.vdi --format VDI
![[Image]](http://farm5.static.flickr.com/4006/4634358066_71182ce630_o.png)
     好了，转换完了。
     同理，把 vdi 转成 vmdk 的命令是：
VBoxManageclonehdsource.vditarget.vmdk--formatVMDK
     vhd 格式的转换也是同一个道理，只需改一下相关的参数即可。
