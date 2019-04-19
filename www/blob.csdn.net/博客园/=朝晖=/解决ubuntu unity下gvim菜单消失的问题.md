# 解决ubuntu unity下gvim菜单消失的问题 - =朝晖= - 博客园
# [解决ubuntu unity下gvim菜单消失的问题](https://www.cnblogs.com/dhcn/p/7100299.html)
#问题描述：
在终端下用gvim 指令打开 gvim就不显示菜单。在不启用unity的桌面环境下用终端打开gvim是有菜单的。
从程序菜单中打开gvim是显示菜单的。
用sudo打开gvim也可以显示菜单，但是不是unity统一桌面顶行的那种菜单，在顶行的下面。
#解决办法：我在ubuntu11.04 unity环境下，用mv $home/.gnome2/Vim的方法，不顶用，还有人提出的方法是修改vimrc配置文件，太麻烦，最后我从以下这个地址找到了一个简单的解决办法：
在gvim指令后面加上 -f选项即可解决问题。具体这个选项啥意思，自己man一下看吧。
参考地址：[http://superuser.com/questions/299548/updated-ubuntu-11-04-from-10-x-and-vims-menu-bar-disappeared](http://superuser.com/questions/299548/updated-ubuntu-11-04-from-10-x-and-vims-menu-bar-disappeared)

