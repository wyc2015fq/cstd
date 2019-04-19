# sublime text3安装以及插件配置教程 - HJ - CSDN博客
2017年01月25日 15:34:56[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：4328
本文是安装的Sublime Text3是portable version（可移动版本） ，安装的环境是基于window 7 或者window 8.1。主要介绍sublime text3 的安装，汉化，常用插件的安装和使用！
一、下载安装sublime Text3
到官网[http://www.sublimetext.com/3](http://www.sublimetext.com/3)下载安装包（根据你的电脑系统下载对应的portable version）,下载好st3后，然后不断的点击next，然后blablabla……就可以安装好了（我的系统的window8.1 64位，安装的目录D盘）
二、汉化sbulime Text3
安装完sublime text3 后，发现都是英文，四级没过的同学怎么办，当然是汉化！
2.1下载汉化安装包
地址：[http://pan.baidu.com/s/1qWnBNvI](http://pan.baidu.com/s/1qWnBNvI)
2.2安装汉化包(将汉化包拷到 Installed Packages 文件夹中)
进入：D:\Sublime Text3\Data\Installed Packages（根据你的安装目录去寻找Installed Packages，本文是安装在D盘），将刚刚下载的汉化包解压，得到的文件Default.sublime-package 拷贝到 Installed Packages 文件夹中，这个时候就会发现汉化成功啦！
三、安装Package Control
Package Control非常重要，因为这关系到后面的st3 安装包的使用，而Package Control 的安装有两种方法：
3.1通过控制面板安装
①点击工具栏的“查看”->”显示面板”(快捷键 是”ctrl+`”)，
![这里写图片描述](http://images.cnitblog.com/blog2015/631758/201505/091642005631557.png)
②到[https://packagecontrol.io/installation](https://packagecontrol.io/installation)官网，将对应的代码拷贝到显示面板中，然后回车。如果安装成功，那么就显示Package Settings 和Package Control 这两个项， 
![这里写图片描述](http://images.cnitblog.com/blog2015/631758/201505/091649311102979.png)
如果没有安装成功，那么建议用手动安装
3.2通过手动安装
①到[https://packagecontrol.io/installation](https://packagecontrol.io/installation)官网，右边有一个“Package Control.sublime-package”，点击下载，然后将文件拷贝到 Installed Packages文件夹中
四、安装st3常用的插件
对于st3,它之所以轻巧，是因为它设计就是为了让你按需加载，根据自己的需要，去下载对应的插件，所以才不会像其他的IDE那样笨重。安装插件步骤是：“Preferences -> Package Control”(快捷键是：shift+ctrl+p),然后执行“Package Control: Install Package”,接在就是输入你要安装的插件就可以，下面就说一下你个人常用的插件：
1、Emmet
作用：快速编写HTML，CSS代码
示例：html:5+tab,div#box+tab  
![这里写图片描述](http://images0.cnblogs.com/blog2015/631758/201505/151540360795722.gif)
2、SublimeCodeIntel
作用：代码提示插件，可以帮你补全代码，快速方便书写！大大提升效率！
示例： 
![这里写图片描述](http://images0.cnblogs.com/blog2015/631758/201505/151551427202226.gif)
3、jQuery
作用：在使用jQuery的时候，给出对应的方法和API的提示！
示例： 
![这里写图片描述](http://images0.cnblogs.com/blog2015/631758/201505/151605025951760.gif)
4、Alignment
作用：该插件是用来对齐的，当你在写js的时候，为了代码的美观，就需要用到这个插件！
快捷键：默认的是：ctrl+alt+a（与qq截图刚好冲突，建议修改）
示例：  
![这里写图片描述](http://images0.cnblogs.com/blog2015/631758/201505/151610175327011.gif)
5、DocBlocker
作用：快熟书写注释！
示例：
![这里写图片描述](http://images0.cnblogs.com/blog2015/631758/201505/151611508452970.gif)
    6、Jsformat
作用：js压缩文件的还原，但是遗憾的是变量名称是
快捷键：默认的是：ctrl+alt+f
示例：
![这里写图片描述](http://images0.cnblogs.com/blog2015/631758/201505/151618097988730.gif)
   7、Bracket Highlighter
作用：当你鼠标选中标签或者是某个片段代码的时候，对应的开头和闭合标签会高亮！
示例：  
![这里写图片描述](http://images0.cnblogs.com/blog2015/631758/201505/151619285176915.gif)
8、sublimeLinter
作用：提供js和css的语法检测！检测的标准是可以根据用户自定义的方式来执行！
9、View in Browser
作用：在浏览器中可以像dreamweaver 那样按下F12的时候在浏览器中浏览。
10、AutoFileName
作用：可以在你输入引用文件路径时给出提示，最典型的就是图片的引用
示例：  
![这里写图片描述](http://images0.cnblogs.com/blog2015/631758/201505/162238054396226.gif)
