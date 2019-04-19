# dconf-editor||gconf-editor - 三少GG - CSDN博客
2011年10月20日 11:35:35[三少GG](https://me.csdn.net/scut1135)阅读数：13975
- [https://www.deleak.com/blog/2011/05/03/gnome-3/](https://www.deleak.com/blog/2011/05/03/gnome-3/)
- 
### **修改配置**
![](https://www.deleak.com/blog/wp-content/uploads//2011/05/images.jpeg)gnome2
 的 “注册表编辑器” gconf-editor 正在被  dconf-editor 所取代，更多的隐藏设置也可以通过 dconf-editor 进行修改。不过目前您修改 gconf-editor 同样生效。
> 
使用 dconf 是GNOME 3的若干改进之一（倒退？！），由于dconf 的配置文件是二进制的，只能通过dconf-editor来编辑。
++++++++++++++++++++++
托盘的问题是源自于unity面板的白名单制，即unity面板的托盘只允许它规定的几个程序显示出来～
安装dconf-tools
sudo apt-get install dconf-tools
然后在终端输入
dconf-editor
就可以启动图形化的修改界面
desktop->unity->panel
修改键值为['all']
然后注销一下～再次登录～其他程序的托盘图标也可以显示出来了～
- 
# [秀一下ubuntu 11.04](http://tieba.baidu.com/f?kz=1064253309)
**+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**
**gconf-editor**
- 
gconf-editor为[GNOME](http://baike.soso.com/ShowTitle.e?sp=SGNOME)桌面环境下的一个配置编辑软件,它可以看做是[gconftool](http://baike.soso.com/ShowTitle.e?sp=Sgconftool)的GUI版。
**它可以管理GNOME配置的注册表项，界面功能类似[WINDOWS](http://baike.soso.com/ShowTitle.e?sp=SWINDOWS)下的[注册表](http://baike.soso.com/ShowTitle.e?sp=S%E6%B3%A8%E5%86%8C%E8%A1%A8)**。
gconf-editor能够让操作者方便的存取基于[XML](http://baike.soso.com/ShowTitle.e?sp=SXML)的[GConf](http://baike.soso.com/ShowTitle.e?sp=SGConf)配置数据库或数据表项。在gconf-editor中设置修改相关参数值，就能修改整个GNOME的界面及行为，如：修改菜单按钮的图标，窗口关闭按钮的左右位置，登录界面背景等等。
![](http://hi.csdn.net/attachment/201110/20/0_1319081750fu2C.gif)
### [gconf-editor的使用](http://blog.163.com/bqj123@126/blog/static/283513342009101152553793/)
2009-11-11 17:25:53|  分类：[Debian](http://blog.163.com/bqj123@126/blog/#m=0&t=1&c=fks_084066085087089070086085083095086094082070085085085071)|字号订阅
gconf-editor是一款类似于注册表的软件，不过远远比改注册表简单，
更改里面的设置实际上是修改了硬盘上的文本文件。
主要是为了方便对桌面环境及一些软件做一些较多
修改──如果一个一个改文本，就太麻烦了。
1、去掉桌面上的已挂载盘符和加上“计算机”等图标 
定位到“/apps/nautilus/desktop”，勾上“Computer_icon_visible”
“Home_icon_visible”“Network_icon_visible”，这样桌面上就有
“计算机”“主目录”“网络服务器”三个图标了，
再把“Volumes_visible”的勾去掉，这样桌面上的已挂载的图标就会消失，
以后需要再进“计算机”里找就OK了！
2、去掉“Bug报告” 
有时候在Gnome下难免会碰到应用程序崩溃，这时Bug报告就会出来。
假如你觉得比较讨厌，就可以通过gconf-editor去掉它。
定位到“/apps/bug-buddy”，把右侧“run_on_crash”勾掉，
就再也不会跑出来烦人了。
3、让截图有阴影效果 
这招能让你用Alt+PrintScreen键截图后，让图片有各种效果。
定位到“/apps/gnome-screenshot”，在右侧，boder_effect的值改一下，
默认是none，改成“shadow”就是有阴影效果了，black-line的话，
截图周围就有黑线了。
4、去掉窗口右侧的关闭等按钮。 
apps/metacity/general
button_layout原来为menu:minimize,maximize,close。
把button_layout内容清空就可以了。
conf-editor 总是被人用来和 Windows 的注册表编辑器打比方，实际上我觉得它比注册表编辑器强多了。因为它在每个项目下方都告诉你了作用。换 字体当然也是小菜一叠。
在 Shell 中输入：
`gconf-editor`
打开编辑器后找到**desktop>gnome>interface**选项。在”font_name”写上你喜欢的字体和大小。
用此方法可以改变 Gnome 环境下的所有默认字体。
使用gnome时我的个人喜好是不要桌面的所有图标，除非是自己保存的文件。
设置起来如果改写xml文件比较麻，好在gnome有gconf-editor程序来方便修改。
修改/app/nautilus/desktop下面几个值就可以了。
如图：
From [Eric's Blog](http://picasaweb.google.com/eric.zq.ma/EricSBlog?feat=embedwebsite)
under: [gnome](http://blog.ust.hk/ericma/category/linux/gnome/),[linux](http://blog.ust.hk/ericma/category/linux/)
发现gnome下无法使用win的组合键,在键盘快捷键出无法设置
解决方法是在本用户下运行gconf-editor这个配置工具相当于windows下的注册表修改器
(注意必须是相同用户，如果用超级用户下运行则修改的是超级用户的)
找到/apps/metacity/目录
其中的global_keybindings和window_keybindings是设置快捷键的
例如:
我要设置<Win>+D的快捷键为显示桌面,则
将global_keybindings目录下show_desktop的键值设为<Mod4>D
(<Mod4>是win左键做组合键)
如果要设<Win>+E为打开资源管理器，则
在keybinding_command目录下将某个command的值设为nautilus
然后在global_keybindings目录下将对应的command的值设为<Mod4>E就可以了
(nautilus默认情况下是进入当前用户的home目录，也可以直接在后面加路径如:nautilus /mnt/c 这样就直接到该目录了)
如果仅仅使用win左键作为单键则其标识为Super_L
还有就是我想用<Win>+R调出Run application的对话框，不过不清楚其命令是什么
win右键视乎设置，其标识也不清楚，如果谁知道怎么让win右键有效，麻烦指点一二 谢谢
----------------------------------------
习惯了Windows下的Win组合键，想在FC5下使用（老是用鼠标点来点去不舒服）。经历了一点小波折，最终还是成功了，分享一下：
先是用FC5自带的设置键盘快捷键功能，在“桌面”，“首选项”里可以找到。但是里面不可以设置“Win”键，“ALT”“CTRL”倒是可以，折腾了半天，很是郁闷。
网上有人说“gconf-editor”可以设置，从安装光盘里找到RPM包安装，装好后在终端里输入“gconf-editor”或者从“应用程序 ”，“系统工具”里打开“配置编辑器”，定位到apps--matacity，下面的global_keybindings和 keybindgs_commands两项就是控制键盘快捷键的。
先设置Win+D来显示桌面（其实FC5默认已经可以用Ctrl+Alt+D来显示桌面），找到global_keybindings下的 show_desktop项，修改为“<mod4>d”就可以了，FC5默认把mod4映射为WIN键（刚开始的时候按网上的方法填 上"<Mod4>d"，结果一按D键就显示桌面，害得我一打带d的单词就显示桌面，看来Linux里注意大小写是有点过了）
然后设置用Win+E来打开文件管理器，先定位到keybindgs_commands，在下面打到一个不用的command项，我用的是 command_2，在键值里填上nautilus（如果习惯打开文件管理器到指定文件夹，可以在nautilus后加路径，如nautilus /home），设置好了要运行的程序还要指定按键，同样，找到global_keybindings下的run_command_2项，在键值里填 上"<mod4>e"，这样就设置好了，可以在Linux下按Win+E键打开文件管理器了
