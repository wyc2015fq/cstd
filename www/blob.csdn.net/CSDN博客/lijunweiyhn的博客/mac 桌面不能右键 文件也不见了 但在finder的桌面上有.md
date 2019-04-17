# mac 桌面不能右键 文件也不见了 但在finder的桌面上有 - lijunweiyhn的博客 - CSDN博客





2018年11月23日 16:48:27[_Junwei](https://me.csdn.net/lijunweiyhn)阅读数：70








# mac 桌面不能右键 文件也不见了 但在finder的桌面上有



出现该现象，可能是因为安装了带有隐藏桌面文件功能的软件，无意中操作引起的。可以利用终端轻松解决：

1、在Launchpad中找到终端并打开：

![](https://gss0.baidu.com/-vo3dSag_xI4khGko9WTAnF6hhy/zhidao/wh%3D600%2C800/sign=1e4484c19913b07ebde8580e3ce7bd1b/03087bf40ad162d9176561fc1bdfa9ec8b13cd43.jpg)

2、粘贴如下代码，回车即可：

defaults write com.apple.finder CreateDesktop -bool true; killall Finder

如此可以解决桌面不显示文件的问题。

ps.当我们需要截图发给别人，担心泄露桌面文件的时候，可以逆向操作，隐藏桌面文件。方法同上，代码如下：

defaults write com.apple.finder CreateDesktop -bool FALSE; killall Finder

其实就是修改默认设置，有点类似windows里面修改注册表。

还有一些app可以直观的显示/隐藏桌面，比如：Alfred Workflow，HiddenMe Free等，希望对你有用。



