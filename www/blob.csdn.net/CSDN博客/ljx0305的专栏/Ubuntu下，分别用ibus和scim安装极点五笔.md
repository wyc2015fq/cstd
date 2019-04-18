# Ubuntu下，分别用ibus和scim安装极点五笔 - ljx0305的专栏 - CSDN博客
2012年07月25日 15:44:43[ljx0305](https://me.csdn.net/ljx0305)阅读数：1204
刚刚安装好了 Ubuntu 11.04，但是没有五笔的日子实在不好过，WINDOWS下用习惯了极点五笔，本以为极点的作者不出Linux版的，结果在网上一搜，居然有高手想到引用极点五笔的字典实现可以同时打拼音和五笔，废话少说，下面看如何操作：
1. ibus下安装极点五笔
在终端中执行如下操作
sudo wget [http://www.unicom-china.com/download/vissible-ibus.tar.gz](http://www.unicom-china.com/download/vissible-ibus.tar.gz)
tar -zxvf vissible-ibus.tar.gz
sudo cp vissible.db /usr/share/ibus-table/tables
sudo cp vissible.gif /usr/share/ibus-table/icons
完成后，在点任务栏中那个键盘小图标，在下拉菜单选“重新重启” 重启ibus输入法，然后再点一下键盘小图标并在下拉菜单中选“首选项”，然后在弹出的窗口中点击“输入法”选项卡，接着点“选择输入法”>"汉语"，然后点"添加" 极点五笔，OK！
原载于：[http://www.jzxue.com/fuwuqi/linux/201106/22-7891.html](http://www.jzxue.com/fuwuqi/linux/201106/22-7891.html)
2. SCIM下安装极点五笔
一、安装
解压、复制 极点五笔.bin 到主目录下
sudo mv 极点五笔.bin /usr/share/scim/tables
sudo pkill scim && scim -d
退出终端再执行下面命令
scim-setup
二、设置
输入法引擎：全局设置：简体中文：极点五笔 6.1（勾选）：确定
sudo pkill scim && scim -d
退出终端
下载地址：[http://forum.ubuntu.org.cn/viewtopic.php?f=8&t=237713](http://forum.ubuntu.org.cn/viewtopic.php?f=8&t=237713)
P.S.：其实这只是用上了极点五笔的码表，极点五笔的很多增强功能是没有的，但至少比原来的五笔输入法强了。
原载于：[http://www.linuxdiyf.com/viewarticle.php?id=103953](http://www.linuxdiyf.com/viewarticle.php?id=103953)
