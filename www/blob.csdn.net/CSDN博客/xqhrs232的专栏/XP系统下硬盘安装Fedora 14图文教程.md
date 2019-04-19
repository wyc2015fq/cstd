# XP系统下硬盘安装Fedora 14图文教程 - xqhrs232的专栏 - CSDN博客
2016年04月19日 15:26:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：385
原文地址::[http://server.zol.com.cn/215/2152612.html](http://server.zol.com.cn/215/2152612.html)
相关文章
1、Fedora14安装图解教程----[http://wenku.baidu.com/link?url=hGv9OrAqmQRlu9dRuyype7a336HJDUAYC2cdz8dUHNpmGPnAMygp46H9wlivArkqpmk0Bdr9jjxZKiYT5KE2Sz9sy1z9XFYQsUBe7QBM4t3](http://wenku.baidu.com/link?url=hGv9OrAqmQRlu9dRuyype7a336HJDUAYC2cdz8dUHNpmGPnAMygp46H9wlivArkqpmk0Bdr9jjxZKiYT5KE2Sz9sy1z9XFYQsUBe7QBM4t3)
2、Fedora 14安装全面体验[图文]----[http://www.linuxidc.com/Linux/2010-11/30158.htm](http://www.linuxidc.com/Linux/2010-11/30158.htm)
3、整体与部分的奥秘 Fedora 14的安装初体验----[http://os.51cto.com/art/201011/235506.htm](http://os.51cto.com/art/201011/235506.htm)
4、Fedora14系统安装----[http://bbs.csdn.net/topics/360000041](http://bbs.csdn.net/topics/360000041)
# Fedora14 每一年Ubuntu和Fedora都会发布新版本，Ubuntu和Fedora的开发周期大致相同，基本上都是时隔6个月就要发布一个重要版本。随着Fedora 14与Ubuntu 10.10在2010年的相继发布，用户可以在第一时间体验这两款[产品](http://detail.zol.com.cn/)的变化。
![XP下硬盘安装Fedora/Ubuntu图文教程](http://2e.zol-img.com.cn/product/59_450x337/986/cecVW5VBlPlA.jpg)
Fedora 14系统
　　Fedora 14正式版新增了硬件加速2D图形、加密和硬件游标。Fedora 14是100%免费并且只包含自由和开源软件，提供了1万多个应用程序，号称“无懈可击软件”，因此不再有[杀毒软件](http://xiazai.zol.com.cn/antivirus_soft_index/antivirus_page_1.html)和间谍软件的烦恼。
　　据悉，这款产品采用Linux kernel 2.6.35，GNOME 2.32桌面，主要组件包括X.org Server 1.9、Python 2.7、Sugar 0.90等。Fedora 14还新增了libjpeg-turbo，将JPEG加密图片的加载时间缩短了一半。对于开发人员来说，Fedora 14提供了对D编程语言的完整支持，Python库和工具也同时进行了升级。
　　Fedora是非常优秀的Linux发行版，它改进后有众多特点，下面通过图文介绍让采用Window XP系统用户在[硬盘](http://memory.zol.com.cn/)上进行安装体验。
系统安装
一直以来很多用户都是利用虚拟机来安装体验linux系统，没有更加亲密的接触，想脱离虚拟机安装，但是想破坏原有Windows XP，怕一旦安装错误，数据会造成无法避免的损失，对于Fedora 14系统的体验，希望体验一下真实的感觉的用户通过[硬盘](http://memory.zol.com.cn/)安装双系统。
　　安装的时候，window预留一个空白分区，最好提供两个硬盘分区，根据下面的图文介绍开始安装，安装的时候一步步根据提示，很快可以体验新系统的功能，在这里只是简单介绍Fedora 14系统安装。
　　首先准备2个软件：
　　1 grub4dos：[http://sourceforge.net/projects/grub4dos/](http://sourceforge.net/projects/grub4dos/)
　　2 Fedora [DVD](http://detail.zol.com.cn/dvdrom_index/subcate66_list_1.html) ISO  [http://www.linuxsight.com/blog/988](http://www.linuxsight.com/blog/988)
　　安装开始：
![XP下硬盘安装Fedora/Ubuntu图文教程](http://2a.zol-img.com.cn/product/59_450x337/348/ceZ5hhLRUaX2.jpg)
grldr放在XP所在的C盘
![XP下硬盘安装Fedora/Ubuntu图文教程](http://2b.zol-img.com.cn/product/59_450x337/349/ceXvHkssr3p2A.jpg)
相关文件解压到D盘
　　首先将grub4dos的grldr放在XP所在的C盘；解压Fedora DVD ISO isolinux目录下的initrd.img和vmlinuz到D盘，同时把images文件夹也解压到D盘。
　　4、把grub4dos的menu.lst也放在D盘，修改menu.lst,在最后加上如下代码：(注：(hd0,4)是我的D盘，(hd0,0)是C盘)
　　title Install Fedora  14
　　kernel (hd0,4)/vmlinuz
　　initrd (hd0,4)/initrd.img
![XP下硬盘安装Fedora/Ubuntu图文教程](http://2c.zol-img.com.cn/product/59_450x337/380/ceHjeBNBVy63.jpg)
修改boot.ini
![XP下硬盘安装Fedora/Ubuntu图文教程](http://2d.zol-img.com.cn/product/59_450x337/381/cezW6hJQwBXjs.jpg)
选择启动系统
![XP下硬盘安装Fedora/Ubuntu图文教程](http://2d.zol-img.com.cn/product/59_450x337/399/ce3DjbcA2IU.jpg)
选择语言类型
　  修改boot.ini 加上：c:grldr="fedra"，然后重启[电脑](http://pc.zol.com.cn/)，准备进入安装，进入grub4dos引导，成功加载系统安装，选择语言：中文。
![XP下硬盘安装Fedora/Ubuntu图文教程](http://2e.zol-img.com.cn/product/59_450x337/400/cevTDWz5QuB8k.jpg)
选择[硬盘](http://memory.zol.com.cn/)[](http://detail.zol.com.cn/picture_index_591/index5901397.shtml)
[](http://detail.zol.com.cn/picture_index_591/index5901397.shtml)
![XP下硬盘安装Fedora/Ubuntu图文教程](http://2b.zol-img.com.cn/product/59_450x337/397/ce8ixypKsxfEw.jpg)
通过命令进行有效安装
　　确定选择Hard drive，然后再/dev/sda5输入/images/install.img，注意分区换成FAT32，之后进入了正式安装了，根据相关的提示可以进行新版安装过程。
![XP下硬盘安装Fedora/Ubuntu图文教程](http://2b.zol-img.com.cn/product/59_450x337/421/cengtIsX2pHg.jpg)
自定义分区安装
![XP下硬盘安装Fedora/Ubuntu图文教程](http://2c.zol-img.com.cn/product/59_450x337/422/ce8rOrMSukgxU.jpg)
选择好所需要分区的文件类型
![XP下硬盘安装Fedora/Ubuntu图文教程](http://2f.zol-img.com.cn/product/59_450x337/419/cePk79ZQaUwcg.jpg)
完成安装
　　最后通过自定义分区，选择好所需要分区的文件类型，适当修改一下引导，等待安装完成。
　　进入Fedora 14在系统可以体验GNOME 2.32桌面，而且在不破坏window xp系统的情况下，这下双系统安装完毕，对于不想利用虚拟机来体验linux新系统平台的用户，这种方法可以让用户有一种最直观操作感受。
