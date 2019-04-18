# VMware，win7与linux centos6.4文件互传，linux下挂载windows共享文件夹，vmware tools安装方法 - z69183787的专栏 - CSDN博客
2018年02月07日 18:31:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：134
个人分类：[Linux-虚拟机](https://blog.csdn.net/z69183787/article/category/2281091)
[https://www.cnblogs.com/js-html/p/3464416.html](https://www.cnblogs.com/js-html/p/3464416.html)
本方法是以win7，VMware9.0.1 ，centos6.4为基础实验的。
对于linux的初级使用阶段，都会Windows中使用linux虚拟机VMWare或者其它的。在Windows与linux之间互传文件是一个问题。本方法介绍的是在linux下挂载Windows共享文件夹的方法来实现的。
1.首先安装VMware Tools（如果已经安装过了就不用安装了）
（1）挂载VMware Tools
先点击虚拟机中菜单栏的VM，下拉菜单中的settings，然后弹出下面这张图，按顺序选择。
![](https://images0.cnblogs.com/blog/516233/201312/08225141-89c1dd3e985c489297e6e51d0e8451ce.png)
 在第3步Browse中会选择vmware tools，可以在你的安装文件的VMware Workstation下面找到5个iso的文件。虚拟机中装的什么系统你就选择相应的iso，本方法中选择的是linux.iso。如果你安装的是精简版或者绿色版的话没有那5个iso文件，需要自己下载，下载的通用地址为：http://softwareupdate.vmware.com/cds/vmw-desktop/ws（代表workstation版本）/7.1.2（版本号）/301548（版本号）/windows（or  linux）（宿主操作系统类型）/packages/tools-windows-8.4.4.exe.tar(安装文件包名)。你可以直接浏览器输入http://softwareupdate.vmware.com/cds/vmw-desktop/ws/在里面找跟自己版本对应的工具。查看自己安装的版本在虚拟机的Help->About
 VMware Workstation->Porduct Information。然后把自己下载的工具通过第3步加载进去。
![](https://images0.cnblogs.com/blog/516233/201312/09082720-1fbd6eda466e4777830d9ca989f03baf.png)
 完成第5步ok后，桌面上就会出现一个VMware Tools的光盘，然后你可以在桌面出现的光盘中（双击）或者/media/VMware Tools下面找到你需要的安装文件。
（2）复制安装文件，解压，安装
复制桌面光盘中的文件（鼠标操作）或者cp   /media/VMware Tools下的文件到任何地方（本文复制到桌面），只要你安装的时候能找到就可以了。然后解压，安装，见下图![](https://images0.cnblogs.com/blog/516233/201312/09090953-d41db1b6ffb5434baa5a1573a9ac3b11.png)
![](https://images0.cnblogs.com/blog/516233/201312/09091009-503fbf6668d8435697c98857e0090180.png)
![](https://images0.cnblogs.com/blog/516233/201312/09091025-c22462b3b767464584f074eb11796dda.png)
安装的只是直接cd进vmware-tools-distrib/执行./wmware-install.pl即可，中间出现的选择yes or no的选项直接enter即可，安装需要一定时间。该目录下的INSTALL文件里有详细的安装，卸载方法。
到此VMware Tools安装完成。
有些时候运行vmware-install.pl会出错。解决方法：
自己搭建了一个vmware虚拟机，当更新vmware tool的时候，发现安装文件是一个 wmware-install.pl的文件，不是rpm安装文件，上网找资料，发现PL文件安装的时候需要使用chmod命令。
1、使用cd命令切换到安装的目录下
2、使用 chmod + wmware-install.pl文件(+的意义还没有完全弄明白)
3、使用 ./ wmware-install.pl 安装就可。
2. 共享文件夹设置
（1）设置Windows下的共享文件夹
对你要共享的文件夹右键共享，特定用户，选择用户，完成即可。
![](https://images0.cnblogs.com/blog/516233/201312/09092241-fec2b07e57e34c3fa1a4a5c75d6472a6.png)
（2）linux共享设置
虚拟机菜单栏VM->settings，打开下图的中的框。选择Options->Shared Folders->Always enabled->Add->Ok即可
![](https://images0.cnblogs.com/blog/516233/201312/09092705-4c36dabd020c4364bbc7f66abc09dd6c.png)
3.完成设置
然后执行下面的命令，data文件夹就是共享的文件夹
![](https://images0.cnblogs.com/blog/516233/201312/09093049-464f0775cba84caea5dce7346562fc9a.png)
到此完成所有设置
如有错误的地方请指导留言，谢谢
参考博文：
[http://blog.chinaunix.net/uid-28690609-id-3683573.html](http://blog.chinaunix.net/uid-28690609-id-3683573.html)
[http://blog.csdn.net/tianxiehuiyuan123/article/details/9167139](http://blog.csdn.net/tianxiehuiyuan123/article/details/9167139)
[http://kumu1988.blog.51cto.com/4075018/776165](http://kumu1988.blog.51cto.com/4075018/776165)
