# ubuntu 技巧----添加右键【打开终端】【以管理员身份打开】 - 三少GG - CSDN博客
2011年09月26日 16:00:14[三少GG](https://me.csdn.net/scut1135)阅读数：4695标签：[终端																[ubuntu																[terminal																[file																[脚本](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=terminal&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=终端&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
                
1.添加右键菜单以管理员身份打开：
`sudo apt-get install nautilus-gksu      这个挺好用的 直接修改文件`
`用鼠标右键单击任一文件夹或文件，选择“以管理员打开”，就可以进行你想要做的操作了。`
```
重启X后就能在右键中看到这个选项了
```
2. 添加右键菜单打开终端：
`sudo apt-get install nautilus-open-terminal`
`重启`.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++[](http://apps.hi.baidu.com/share/detail/20488336)
[ubuntu右键添加打开终端的快捷菜单](http://apps.hi.baidu.com/share/detail/20488336)
ubuntu上面来遇到的第一个最常用的问题就是，右键单击桌面，发现没有“打开终端”这个选项，下面介 绍两种方法来在右键添加“打开终端”这个选项。
第一种方法：
sudo apt-get install nautilus-open-terminal
安装一个包，即可在右键里面添加一个“打开终端”的菜单。
第二种方法：
进入主目录的.gnome2/nautilus- scripts目录。新建一个文件，文件名任意（这个文件名会显示在右键菜单里，最好是通俗易懂的，比如“打开终端”或“open- terminal”），文件内容如下:
#!/bin/bash
#
# This script opens a gnome-terminal in the directory you select.
#
# Distributed under the terms of GNU GPL version 2 or later
#
# Install in ~/.gnome2/nautilus-scripts or ~/Nautilus/scripts
# You need to be running Nautilus 1.0.3+ to use scripts.
# When a directory is selected, go there. Otherwise go to current
# directory. If more than one directory is selected, show error.
if [ -n "$NAUTILUS_SCRIPT_SELECTED_FILE_PATHS" ]; then
set $NAUTILUS_SCRIPT_SELECTED_FILE_PATHS
if [ $# -eq 1 ]; then
destination="$1"
# Go to file's directory if it's a file
if [ ! -d "$destination" ]; then
destination="`dirname "$destination"`"
fi
else
zenity --error --title="Error - Open terminal here" \
--text="You can only select one directory."
exit 1
fi
else
destination="`echo "$NAUTILUS_SCRIPT_CURRENT_URI" | sed 's/^file:\/\///'`"
fi
# It's only possible to go to local directories
if [ -n "`echo "$destination" | grep '^[a-zA-Z0-9]\+:'`" ]; then
zenity --error --title="Error - Open terminal here" \
--text="Only local directories can be used."
exit 1
fi
cd "$destination"
exec x-terminal-emulator
第一种方法会在右键菜单添加一个“在终端中打开”的选项，第二种方法会在右键菜单添加一个“脚本”的选项，在其子菜单中就会出现你命名的脚本名称。
[1. 第二种方法建出来的是一个二级菜单，它先是在右键菜单中添加一个名叫scripts的菜单，
在这个菜单下才是我们需要的“打开终端”或 “open- terminal”（这个地方的名字是你新建的
脚本的文件名），所以显得有些不方便。第一种方法是直接在右键菜单中添加菜单。
2. 两种方法都是基于nautilus的，在任何nautilus的界面中，都可以启用此功能，更特别的
是第二种方法，它会识别当前目录，打开的终端 默认就是当前目录，这点比较人性化，
而第一种方法，无论你在什么目录，它打开后默认的目录都是主目录。](http://blog.sina.com.cn/s/blog_67fb00610100i8lx.html)
[](http://blog.sina.com.cn/s/blog_67fb00610100i8lx.html)
[第一种方法测试一切正常。。。](http://blog.sina.com.cn/s/blog_67fb00610100i8lx.html)
[第二种未测试，感兴趣的可以是下](http://blog.sina.com.cn/s/blog_67fb00610100i8lx.html)
[有一个发现，第一种的方法，也是在哪个目录，打开就是这个目录](http://blog.sina.com.cn/s/blog_67fb00610100i8lx.html)
