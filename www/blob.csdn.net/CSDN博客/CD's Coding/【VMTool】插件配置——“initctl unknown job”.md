# 【VMTool】插件配置——“initctl unknown job” - CD's Coding - CSDN博客





2017年03月21日 11:56:20[糖果天王](https://me.csdn.net/okcd00)阅读数：833








最近老师布置作业，给了个VMWare10的Ubuntu14.04镜像，想跟以前的Windows虚拟机一样开启VMTools来使用剪切板和文件拖放，结果怎么编译总会出错，调了半天终于装好了，记录一下过程。

![VMTools](https://img-blog.csdn.net/20170321113641542?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

简单的来说大概是如下流程：
- 先获取root权限 
- 修改root密码 `sudo passwd root`
- 临时root（即仅当前指令使用root权限） `sudo blablabla`，会提示输入root密码
- 当前Terminal的永久root（终端关闭则权限收回），`sudo su``su -`

- 获得安装包 
- 一般在虚拟机上右键，“重新安装VMTools”可以自动在虚拟机中装载vmtools镜像
- 然后将镜像中的压缩包 `xxx.tar.gz`拖动到桌面或解压缩到桌面（HINT：我拖到HOME目录下所以半天装不好，可能是这一步的问题，嗯嗯）
- 解压缩获得一个文件夹，以`-disturb`结尾

- 安装 
- 使用Root好的Terminal进入到这个文件夹里 `cd xxx/Desktop/xxx-disturb/`
- 安装 `sudo ./vmware-install.pl`
- 安装过程中会有好多好多问题，比如装在哪里，功能是否开启，如果懒得自定义可以加`-d`后缀全都按照默认选择，`sudo ./vmware-install.pl -d`


参考资料 

[1] [kb.vmware.com](https://kb.vmware.com/selfservice/microsites/search.do?language=en_US&cmd=displayKC&externalId=1022525)



