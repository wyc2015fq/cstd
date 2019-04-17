# web前端不可错过的开发工具–Adobe Brackets - 在思索中前行！ - CSDN博客





2014年07月05日 18:43:13[_Tham](https://me.csdn.net/txl16211)阅读数：1308








Adobe Brackets是一个开源的基于HTML/CSS/JavaScript开发，运行在native shell上的集成开发环境。该项目由Adobe创建和维护，根据MIT许可证发布。提供Windows和OS X平台支持。

Brackets的特点是简约、快捷，没有很多的视图或者面板，它的核心目标是减少在开发过程中那些效率低下的重复性工作，例如浏览器刷新，修改元素的样式，搜索功能等等。

前去官网下载：[http://brackets.io/](http://brackets.io/)

### 功能及使用介绍

官方提供简体中文语言包，在菜单栏处选择debug->language->simple chinese就可更换为简体中文版本

![1](http://www.zjgsq.com/wp-content/uploads/2013/11/12.jpg)

左侧目录树顶部会显示你最近打开的正在工作的几个文档，非常的方便。

Brackest会检测文档是否符合html规范，并且还有JS错误提示，点击黄色三角感叹号就会弹出错误提示，如下图
![2](http://www.zjgsq.com/wp-content/uploads/2013/11/21.jpg)

把光标放在一个class或id属性的标签名称上，按下Ctrl/Cmd + E（“编辑”）或退出编辑。Brackets将搜索项目下所有CSS文件，然后打开一个内嵌的编辑器嵌入在HTML文件中，可以让你迅速修改CSS代码。唯一美中不足的是：搜索的是项目下所有的CSS文件，如果能修改成所搜当前文件使用的CSS文件就更好了。
![3](http://www.zjgsq.com/wp-content/uploads/2013/11/31.jpg)

Brackest同样也支持JS的快速预览和编辑。

Brackets内建取色器，提供RGBa、HEX、HSLa的颜色编码形式。把光标放在一个颜色编码上，按下Ctrl/Cmd + E（“编辑”），退出取色器窗口需要使用Esc键。
![4](http://www.zjgsq.com/wp-content/uploads/2013/11/41.jpg)

Brackets提供网页即时预览功能。使用该功能时，Brackets调用Chrome浏览器打开当前页面，此后修改html、css、javascript并保存后，所修改的内容会即时响应到浏览器中的页面，无须手动刷新页面。这是Brackets最大的一个亮点，有两个显示器的coder有福了，可以分屏显示Brackets和chrome，即时修改即时预览，无需切换编辑器/浏览器和刷新页面。

目前即时预览功能的一些限制：

它仅适用于Chrome浏览器为目标浏览器，你必须安装Chrome。

它依赖于在Chrome浏览器中的远程调试功能，这是一个命令行标志启用。在Mac上，如果你已经在使用Chrome浏览器，这时启动“即时预览”，Brackets将询问你是否要重新启动Chrome浏览器启用远程调试功能。

只能同时对一个HTML文件进行预览 – 如果切换另一个HTML文件，Brackets将关闭原来的预览。

### 部分快捷键

Ctrl/Cmd+Shift+H 可以呼出与关闭文件树

Ctrl/Cmd + E 快速预览/编辑 css样式/javascript函数

Ctrl/Cmd + +/- 放大缩小编辑区字体大小

Ctrl/Cmd + 0 重置编辑区字体大小

Ctrl/Cmd + Alt + P 打开即时预览功能

Ctrl/Cmd + / 行注释

Ctrl/Cmd + Alt + / 块注释

不足：不支持代码折叠。没有代码格式化。

### 插件推荐

Brackets 代码格式化的插件–**beautify**。

直接打开Brackets 插件管理器，搜索安装beautify。
![5](http://www.zjgsq.com/wp-content/uploads/2013/11/51.jpg)

安装完毕后，选择菜单上的编辑→beautify（或使用快捷键ctrl+alt+L）完成代码格式化（或者也可选择beautify on load保存时自动格式化）。

如果不能在线安装，可直接下载[https://github.com/drewhjava/brackets-beautify](https://github.com/drewhjava/brackets-beautify)后，解压放到Brackets 插件目录下（插件目录Help 帮助>Show Extensions Folder 显示扩展目录），记得要放在user目录里，然后重启Brackets
 ，就能看到beautify了。

注：新版brackets用以上方法安装不了,必须从[https://s3.amazonaws.com/extend.brackets/brackets-beautify/brackets-beautify-1.1.3.zip](https://s3.amazonaws.com/extend.brackets/brackets-beautify/brackets-beautify-1.1.3.zip)下载，安装方法同上，解压出来直接把brackets-beautify-1.1.3文件夹放到扩展目录的user目录下，重启即可。

快速编写HTML和CSS：**Emmet**

新发现的代码格式化的插件–**jsbeautifier**：这款插件支持更多的代码格式化，下载地址：[https://s3.amazonaws.com/extend.brackets/brackets-jsbeautifier/brackets-jsbeautifier-0.0.2.zip](https://s3.amazonaws.com/extend.brackets/brackets-jsbeautifier/brackets-jsbeautifier-0.0.2.zip)
**Emmet演示请看：**[http://www.zjgsq.com/1062.html](http://www.zjgsq.com/1062.html)

插件安装方法同beautify

或从GITHUB手动下载：[https://github.com/emmetio/brackets-emmet](https://github.com/emmetio/brackets-emmet)



