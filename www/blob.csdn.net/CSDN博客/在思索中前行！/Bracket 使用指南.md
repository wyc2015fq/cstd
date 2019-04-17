# Bracket 使用指南 - 在思索中前行！ - CSDN博客





2014年07月06日 18:07:44[_Tham](https://me.csdn.net/txl16211)阅读数：5606








**Brackets** 是一个免费、开源且跨平台的 HTML/CSS/JavaScript 前端 WEB 集成[开发](http://www.iplaysoft.com/tag/%E5%BC%80%E5%8F%91)环境 (IDE工具)。该项目由[Adobe](http://www.iplaysoft.com/tag/adobe) 创建和维护，根据MIT许可证发布，支持
 Windows、Linux 以及 OS X 平台。

[Brackets](http://www.iplaysoft.com/brackets.html) 的特点是简约、优雅、快捷！它没有很多的视图或者面板，也没太多花哨的功能，它的核心目标是减少在开发过程中那些[效率](http://www.iplaysoft.com/tag/%E6%95%88%E7%8E%87)低下的重复性工作，例如浏览器刷新，修改元素的样式，搜索功能等等。和 [Sublime
 Text](http://www.iplaysoft.com/sublimetext.html)、[Everedit](http://www.iplaysoft.com/everedit.html) 等通用代码编辑器不一样，Brackets 是专门针对 **WEB 前端开发**而生……

### Brackets - 优秀免费的开源前端开发工具演示视频：



### 一、项目设置


1、打开 Brackets，整个界面很简洁，顶部菜单栏只提供file > exit退出[编辑器](http://www.iplaysoft.com/tag/%E7%BC%96%E8%BE%91%E5%99%A8)功能。左侧为项目组织结构的文件树，使用Ctrl/Cmd+Shift+H可以呼出与关闭文件树。右侧为编辑区，上部为工具栏，中部为文档区域，下部为提示区域。

![brackets](http://ips.chotee.com/wp-content/uploads/2013/brackets/1.jpg)

2、打开项目 使用File > Open Folder命令打开项目文件夹，左侧文件树项目名更新为项目文件夹名，文件树更新为当前项目的文件树。

![brackets](http://ips.chotee.com/wp-content/uploads/2013/brackets/2.jpg)


在项目名上单击左键，弹出项目编辑菜单，编辑菜单会显示历史项目，以及项目编辑命令。

![brackets](http://ips.chotee.com/wp-content/uploads/2013/brackets/3.jpg)


Open Folder命令：打开新的项目。

Project Settings命令：设置当前项目的Web地址，在页面调试预览时会用到。

设置要求：必须是http://开头的web地址。

![brackets](http://ips.chotee.com/wp-content/uploads/2013/brackets/4.jpg)


如上图，设置为http://127.0.0.1/demo/slide时，在浏览器预览时，会通过web地址打开相应的页面。

![brackets](http://ips.chotee.com/wp-content/uploads/2013/brackets/5.jpg)


如果没有设置，会通过文件的盘符地址打开页面。

![brackets](http://ips.chotee.com/wp-content/uploads/2013/brackets/6.jpg)

### 二、文件编辑


在文件树中点击index.html，主区域打开index.html的文档。

![brackets](http://ips.chotee.com/wp-content/uploads/2013/brackets/7.jpg)


1、Brackest会检测文档是否符合html规范，如下图提示，在20行有一个style样式块需要放在head节点里。

![brackets](http://ips.chotee.com/wp-content/uploads/2013/brackets/8.jpg)


2、把光标放在一个class或id属性的标签名称上，按下Ctrl/Cmd + E（“编辑”）或退出编辑。[Brackets](http://www.iplaysoft.com/brackets.html)将搜索项目下所有CSS文件，然后打开一个内嵌的编辑器嵌入在[HTML](http://www.iplaysoft.com/tag/html)文件中，可以让你迅速修改CSS代码。

![brackets](http://ips.chotee.com/wp-content/uploads/2013/brackets/9.jpg)


当前class/id标签有多处样式定义时，编辑窗口提供切换按钮来切换显示样式，也可以使用Alt + Up/Down箭头键切换。

需要注意的是，Brackets会检测当前html文档以及项目下所有[CSS](http://www.iplaysoft.com/tag/css)文件来查找class/id样式，即使某些CSS文件在当前html文档中未被引用到。


3、Brackets同样支持对JS对象定义的快速预览/编辑，把光标放在一个js函数名称上，按下Ctrl/Cmd + E（“编辑”）或退出编辑。

![brackets](http://ips.chotee.com/wp-content/uploads/2013/brackets/10.jpg)


4、Brackets内建取色器，提供RGBa、HEX、HSLa的颜色编码形式。把光标放在一个颜色编码上，按下Ctrl/Cmd + E（“编辑”），退出取色器窗口需要使用Esc键。

![brackets](http://ips.chotee.com/wp-content/uploads/2013/brackets/11.jpg)


5、根据读者的反映，新版本中的 Brackets 已经支持 HTML、HTML5、CSS 以及 Javascript 的[代码](http://www.iplaysoft.com/tag/%E4%BB%A3%E7%A0%81)提示（代码补全）了。

### 三、即时预览


Brackets提供网页即时预览功能。使用该功能时，Brackets调用 Chrome [浏览器](http://www.iplaysoft.com/tag/%E6%B5%8F%E8%A7%88%E5%99%A8)打开当前页面，此后修改html、css、javascript并保存后，所修改的内容会即时响应到浏览器中的页面，无须手动刷新页面。这是 [Brackets](http://www.iplaysoft.com/brackets.html) 最大的一个亮点，有两个显示器的coder有福了，可以分屏显示Brackets和chrome，即时修改即时预览，无需切换编辑器/浏览器和刷新页面。


目前即时预览功能的一些限制：

它仅适用于 Chrome 浏览器为目标浏览器，你必须安装 [Google Chrome](http://www.iplaysoft.com/google-chrome.html)。

它依赖于在Chrome浏览器中的远程调试功能，这是一个命令行标志启用。在Mac上，如果你已经在使用Chrome浏览器，这时启动“即时预览”，Brackets将询问你是否要重新启动Chrome浏览器启用远程调试功能。

只能同时对一个HTML文件进行预览 - 如果切换另一个HTML文件，Brackets将关闭原来的预览。

### 四、部分快捷键


Ctrl/Cmd+Shift+H 可以呼出与关闭文件树

Ctrl/Cmd + E 快速预览/编辑 css样式/javascript函数

Ctrl/Cmd + +/- 放大缩小编辑区字体大小

Ctrl/Cmd + 0 重置编辑区字体大小

Ctrl/Cmd + Alt + P 打开即时预览功能

Ctrl/Cmd + / 行注释

Ctrl/Cmd + Alt + / 块注释

注：css代码、html代码注释时只能使用块注释快捷键

### 有待改进之处：


1、撤销修改功能支持的步数较少

2、未保存的修改在撤销时会一次撤销到最后一次保存的状态

3、取色器不支持Ctrl/Cmd + E关闭

4、Javascript只支持对函数的快速预览/编辑

5、对于css样式、javascript函数的快速预览：Brackets会检测当前html文档以及项目下所有css、javascript文件来查找目标，即使某些css、javascript文件在当前html文档中未被引用到。对于作为JSON对象属性的函数的查找结果，其他对象中的同名函数也会被索引到

6、在使用网页即时预览功能时，打开Chrome控制台，Brackets会退出即时预览

7、不支持双击选中中文字符串

8、不支持代码折叠


总的来说，Brackets 这个项目还很新，潜力巨大，而且现在 WEB 前端开发需求极其旺盛，真正好用强大的免费工具也不是特别多，加上 Brackets 开源，支持多平台，所以还是非常值得尝试与[学习](http://www.iplaysoft.com/tag/%E5%AD%A6%E4%B9%A0)的，希望这款开发工具可以对你有所帮助吧



### Brackets插件推荐



1.代码格式化插件：Beautify


直接打开Brackets 插件管理器，搜索安装beautify。

安装完毕后，选择菜单上的编辑→beautify（或使用快捷键ctrl+alt+L）完成代码格式化

2.HTML代码编辑：Emmet


最强大的HTML和CSS快速编辑插件

3.CSS和JS压缩插件：Minify


很强大的插件，集成了CSS和JS的压缩，会在同个目录下自动生成*.min.*格式的文件。

4.CSS3代码自动补全插件：auto prefix


使用CanIUse上的[css3](http://caibaojian.com/t/css3)数据，自动补全css3前缀和浏览器兼容，不过可能有些新了。有一些旧版本的浏览器会被抛弃掉。

5.代码折叠插件：brackets-code-folding


安装URL：https://github.com/thehogfather/brackets-code-folding


默认不支持代码的可折叠功能。通过此扩展的安装，对代码添加可折叠功能。

6.当关闭编辑器时自动保存所有打开未保存的文件：Autosave Files on Window Blur


类似Sublime Text2、phpstorm / webstorm的风格，之前提到的不足全部解决了。

7.Brackets主题：theme


安装URL：https://github.com/MiguelCastillo/Brackets-Themes


该扩展为一个用于使用基于CodeMirror的主题集的扩展。安装该扩展后应用程序菜单中出现“Themes”菜单，开发者可以为应用程序界面从中选用一个主题。





### 相关文件下载地址：


官方网站：[访问](http://brackets.io/)

软件性质：免费开源

[下载 Brackets](http://dl.iplaysoft.com/files/2526.html) (Win)  |  [Mac](http://dl.iplaysoft.com/files/2527.html)  |
  [Linux64位](http://dl.iplaysoft.com/files/2528.html)、[32位](http://dl.iplaysoft.com/files/2529.html)  |
  [来自异次元](http://www.iplaysoft.com/brackets.html)  |  [开发相关](http://www.iplaysoft.com/tag/%E5%BC%80%E5%8F%91)  |
  [WEB相关](http://www.iplaysoft.com/tag/web)  |  [更多编辑器](http://www.iplaysoft.com/tag/%E7%BC%96%E8%BE%91%E5%99%A8)



