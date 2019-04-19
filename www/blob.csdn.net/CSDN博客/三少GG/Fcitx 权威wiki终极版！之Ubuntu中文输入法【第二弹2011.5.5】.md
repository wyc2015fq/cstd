# Fcitx 权威wiki终极版！之Ubuntu中文输入法【第二弹2011.5.5】 - 三少GG - CSDN博客
2010年12月17日 16:23:00[三少GG](https://me.csdn.net/scut1135)阅读数：1825标签：[输入法																[ubuntu																[module																[工具																[gtk																[input](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=gtk&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=输入法&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
**2011.5.5 搞了半天， 居然是在终端使用命令行输入： fcitx  来打开输入法！！！！！！！**
[http://wiki.ubuntu.org.cn/Fcitx](http://wiki.ubuntu.org.cn/Fcitx)
**Fcitx──小企鹅输入法：Free Chinese Input Toy for X**是一个以[GPL](http://www.gnu.org/copyleft/gpl.html)方式发布的、基于XIM的简体中文输入法集合（原为G五笔），包括**五笔**、**五笔拼音**、**二笔**、**仓颉**、**晚风**、**冰蟾全息**、**拼音(全拼和双拼)**、**区位**以及**码表输入模块**。
Fcitx 4是新版，各方面都比老版强很多。
## 目录
[隐藏]
- [1卸载ibus](http://wiki.ubuntu.org.cn/Fcitx#.E5.8D.B8.E8.BD.BDibus)
- [2fcitx4](http://wiki.ubuntu.org.cn/Fcitx#fcitx4)- [2.1配置工具](http://wiki.ubuntu.org.cn/Fcitx#.E9.85.8D.E7.BD.AE.E5.B7.A5.E5.85.B7)
- [2.2简繁切换](http://wiki.ubuntu.org.cn/Fcitx#.E7.AE.80.E7.B9.81.E5.88.87.E6.8D.A2)
- [2.3日文](http://wiki.ubuntu.org.cn/Fcitx#.E6.97.A5.E6.96.87)
- [2.4韩文](http://wiki.ubuntu.org.cn/Fcitx#.E9.9F.A9.E6.96.87)
- [3英文环境](http://wiki.ubuntu.org.cn/Fcitx#.E8.8B.B1.E6.96.87.E7.8E.AF.E5.A2.83)
- [4外部链接](http://wiki.ubuntu.org.cn/Fcitx#.E5.A4.96.E9.83.A8.E9.93.BE.E6.8E.A5)
## [[编辑](http://wiki.ubuntu.org.cn/index.php?title=Fcitx&action=edit&section=1)]卸载ibus 
有安装ibus的先卸载
sudo apt-get remove ibus
## [[编辑](http://wiki.ubuntu.org.cn/index.php?title=Fcitx&action=edit&section=2)]fcitx4
![](http://wiki.ubuntu.org.cn/images/d/d3/Fcitx_4.png)
![](http://wiki.ubuntu.org.cn/skins/common/images/magnify-clip.png)
Fcitx 4 默認輸入法主題
sudo add-apt-repository ppa:wengxt/fcitx-nightly
sudo apt-get update
sudo apt-get install fcitx fcitx-config fcitx-sunpinyinim-switch -s fcitx -z default  #设为默认输入法，一般不需要，除非系统有多个输入法
### [[编辑](http://wiki.ubuntu.org.cn/index.php?title=Fcitx&action=edit&section=3)]配置工具 
配置文件在
~/.config/fcitx/config 
如果没有，用下列命令生成
fcitx -c
如果安装了 fcitx-config，右击输入法-配置Fcitx。
### [[编辑](http://wiki.ubuntu.org.cn/index.php?title=Fcitx&action=edit&section=4)]简繁切换
常用的中文输入法简繁切换是快捷键ctrl+shift+f，fcitx不是这样。
![](http://wiki.ubuntu.org.cn/images/thumb/e/e0/Fcitx_4_zh_hant.png/512px-Fcitx_4_zh_hant.png)
![](http://wiki.ubuntu.org.cn/skins/common/images/magnify-clip.png)
Fcitx 4 繁体切换快捷键
### [[编辑](http://wiki.ubuntu.org.cn/index.php?title=Fcitx&action=edit&section=5)]日文
请看[这里](http://forum.ubuntu.org.cn/viewtopic.php?f=95&t=262914&hilit=fcitx+%E6%97%A5%E8%AF%AD)
### [[编辑](http://wiki.ubuntu.org.cn/index.php?title=Fcitx&action=edit&section=6)]韩文
参考 [这里](http://forum.ubuntu.org.cn/viewtopic.php?f=8&t=277120&start=0)
首先确保主题能显示韩文，参照上面步骤改成**微米黑**字体。
利用的是码表，所以确保开启了
使用码表=1
新建，编辑码表文件
gedit ~/.config/fcitx/tables.conf[码表]
名称=韩文
码表=hy.mb
调频=0
自动上屏=1
复制合适的韩语编码文档（.mb格式，如果是.txt格式，还需要用txt2mb工具转换）到fcitx-utf8目录下便可。
txt2mb hy.txt hy.mb
![](http://wiki.ubuntu.org.cn/images/thumb/f/fe/Fcitx-utf8han.png/300px-Fcitx-utf8han.png)
![](http://wiki.ubuntu.org.cn/skins/common/images/magnify-clip.png)
效果图
## [[编辑](http://wiki.ubuntu.org.cn/index.php?title=Fcitx&action=edit&section=7)]英文环境
若原本就是英文环境en_US.UTF-8下，安装fcitx，可正常启动。若是最初是中文环境zh_CN.UTF-8，中途改为英文环境en_US.UTF-8，会出问题，fcitx能启动，但无法输入。错误提示
Please set XMODIFIERS
解决方法：添加下列字段到
~/.bashrcexport XMODIFIERS="@im=fcitx"
export GTK_IM_MODULE=xim
export QT_IM_MODULE=xim
