# Ubuntu 11.10 快捷键小记 ||gFTP&服务器vsftpd - 三少GG - CSDN博客
2011年11月07日 15:30:33[三少GG](https://me.csdn.net/scut1135)阅读数：4221标签：[ubuntu																[服务器																[终端																[mozilla																[firefox																[gtk](https://so.csdn.net/so/search/s.do?q=gtk&t=blog)](https://so.csdn.net/so/search/s.do?q=firefox&t=blog)](https://so.csdn.net/so/search/s.do?q=mozilla&t=blog)](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
**评论：**
**11.10用了unity代替gnome**，只有侧边栏了。如果你的显卡支持3d，会自动选择3D，否则就会使用unity 2d.
可以安装 ccsm配置3D，sudo apt-get install compizconfig-settings-manager 
......
我感觉Unity比gnome3容易适应，现在感觉挺好的
ubuntu在易用性上确实下了很多功夫
**[http://www.liveqz.cn.sixxs.org/archives/725](http://www.liveqz.cn.sixxs.org/archives/725)**
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://miyulano.iteye.com/blog/1196457](http://miyulano.iteye.com/blog/1196457)
记忆力不好的人伤比起啊伤不起，只好用笔记辅助记忆之。
**启动终端： Ctrl + Alt + T**
切换正在运行中的程序：Alt + Tab
**切换桌面： Ctrl + Alt + 上/下/左/右**
**快速启动：微标键**
**弹出侧边栏：长按微标键**
快速启动侧边栏程序：微标键 + 对应数字
剩下的：
![](https://lh3.googleusercontent.com/-QnPsC63XWbQ/TpgbAhgocbI/AAAAAAAAAOg/-jNlb29hw8Y/s800/Screenshot-2011-10-14%25252014%25253A07%25253A10.png)
++++++++++++++++++++++++++++++++
# ubuntu 11.10字体
Posted on [2011年10月15日by](http://www.liveqz.cn.sixxs.org/archives/750)[admin](http://www.liveqz.cn.sixxs.org/archives/author/admin)
在安装完ubuntu11.10后发现系统字体不好看，找遍了系统也没有找到修改的，上google问下，其实很简单。
在终端打入一下命令：
sudo apt-get install gnome-tweak-tool
安装后，在终端打入一下命令 gnome-tweak-tool即可开打该软件。
## ++++++++++++
[http://yp.oss.org.cn/software/show_resource.php?resource_id=1027](http://yp.oss.org.cn/software/show_resource.php?resource_id=1027)
11.10 包括了最新版的Compiz 和 Unity。用户可以发现新版界面支持Alt+Tab切换功能，并提供更好的快速启动和任务面板性能。原来的
 “Places” 按钮改为了“Lenses”，它在面板中集成多种系统资源和高级过滤器，如评级，范围，类别等。目前也完全支持中日韩语言和其它脚本语言。
不幸的是，传统的经典菜单已经被去除了。用户可以以Unity 2D或3D的模式登录桌面，除此之外，用户也可以从Ubuntu软件仓库中选择安装Gnome 3（Gnome Shell）来使用传统的经典界面。
输入以下命令：
sudo apt-get install gnome-shell
**新加入的软件和一些缺省软件的变动**
  Ubuntu 11.10 的软件中心已经迁移到GTK 3平台上了，因此，界面新颖而美观，并带有新样式的图标。Ubuntu软件中心现在在主要分类和子分类页面中加入了一个“高评价”视图列表，来推荐流行的软件。它现在也允许你编辑和删除自己的评价。且.deb文件的安装（gdebi 功能）也得到加速。
Mozilla Thunderbird 7已经取代了原来的Evolution邮件客户端。随着进来不断的更新，Firefox 7现在是Ocelot的缺省浏览器。系统还包含了Déjà Dup软件作为系统备份工具，而Gwibber是一个微博工具。Oneric开始使用Nautilus 3，它提供了一个可分页的界面，通过选项支持在同一窗口中使用多个标签页显示多个打开的目录。
## [如何使ubuntu Unity 的程序面板加载到桌面底部](http://imcn.me.sixxs.org/html/y2011/8544.html)
这是一个非官方的Compiz的插件，可以移动Unity面板（适用于Unity 3D）到桌面底部。如图所示:
![unity-launcher-bottom](http://imcn.me.sixxs.org/wp-content/uploads/2011/10/unity-launcher-bottom-608x456.png)
unity-launcher-bottom
+++++++++++++++++++++++++++++++++++++++++++++++++++
### [Ubuntu 10.10  恢复顶部/底部面板到默认位置](http://huizhenbing.blog.163.com/blog/static/92294282010611103725395/)
打开终端，终端窗口打开之后，在提示符后面输入下列命令：
**gconftool --recursive-unset/apps/panel**
（注意：每个斜杠 “/” 后面没有空格）
接下来输入下列命令：
**rm -rf~/.gconf/apps/panel**
最后还需要执行：
**pkillgnome-panel**
Ubuntu默认的上下两栏面板就会恢复默认布局,图标也会恢复。

