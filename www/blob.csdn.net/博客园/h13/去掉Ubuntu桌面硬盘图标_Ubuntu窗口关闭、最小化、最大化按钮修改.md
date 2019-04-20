# 去掉Ubuntu桌面硬盘图标_Ubuntu窗口关闭、最小化、最大化按钮修改 - h13 - 博客园
去掉Ubuntu桌面硬盘图标：
(1)应用程序-附件-终端-输入gconf-editor
(相当于“Alt+F2“，弹出对话框输入gconf-editor，点击“运行“)
(2)打开后,在窗口左侧依次点开:apps->nautilus->desktop
(3)在右边的窗口中找到“volumes_visible”选项,去掉后面的勾
Ubuntu窗口关闭、最小化、最大化按钮修改：
PS：本人不建议修改，虽然与Windows不同，但感觉这个设计其实挺合理的，软件菜单都在左上角，为什么“关闭、最小化、最大化”这三个按钮非要放在右上角呢？而且如果像Windows一样放在了右上角，就会和Ubuntu关机按钮非常靠近，会带来麻烦。 
(1)应用程序-附件-终端-输入gconf-editor
(相当于“Alt+F2“，弹出对话框输入gconf-editor，点击“运行“)
(2)打开后,在窗口左侧依次点开:apps->metacity->general
(3)改变右边“button_layout”，右击-“编辑键”-将原来的 close,minimize,maximize: 改为 :minimize,maximize,close就变得和Windows一样放在右上角了。

