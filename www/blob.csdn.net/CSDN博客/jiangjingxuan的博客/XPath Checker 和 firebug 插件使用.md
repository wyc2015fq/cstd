# XPath Checker 和 firebug 插件使用 - jiangjingxuan的博客 - CSDN博客





2017年02月27日 09:45:19[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：833









# 安装插件:

　　1.firebug

　　2.FirePath

　　3.xpath finder

　　4.XPath Checker



# XPath Checker 下载安装 

https://addons.mozilla.org/en-US/firefox/addon/xpath-checker/



 安装XPath Checker后，就可右键--view xpath，写Xpath路径



参考    [Firefox中firebug和xpath checker工具的使用](http://www.cnblogs.com/zhwl/p/3438997.html)



一直想把自己这段时间做的东西整理下，确迟迟没有动手，现在信息抽取工作已经做的差不多，把自己感觉很好用的两个工具介绍给大家吧！
 Firefox真是一个好东西，它许多插件。本人是很讨厌插件的，每次电脑里都会安装一大堆无用的插件，看着心里不爽。由于项目需要，要看网页的代码，并且找到有用信息，如果下载一个html页面，用记事本打开看，那无异于是一场灾难，幸好同事给我推荐了firebug这个看代码的工具，后来把html文件转化成xml，要提取信息，需要用到xpath。由于不知道xpath是否正确，这个工具就可以验证，呵呵。说了好多废话，开始介绍下我的使用历程吧。

 从firefox的官方网站上下载firebug和xpathchecker这两个插件，安装步骤安装就可以了。
 Firebug:
 安装好之后，可以在屏幕的下方看到一个小虫的标志，是暗色的，说明该插件没有开启，点小虫图标屏幕下方会出现一栏。
 在这一栏可以看到小虫变亮了，旁边还有查看，编辑的功能。
 点查看，然后移动鼠标到你想的内容，这时发现你鼠标在的地方在下面一栏的代码就出现了。



 简单吧，但对你要看代码，知道代码的具体位子可是很重要的。
 在代码处 点右键， 出现菜单栏， 选中复制XPath


 将XPath 复制到文件就可以看到选中内容的xpath了。  
 它还有很多有用的功能，因为不需要，也没有研究，呵呵。

 XPath Checker:
 在网上看了使用方法，开始没看明白，后来才知道是在页面上点鼠标右键，出现菜单，点击view xpath。


 将刚才的XPath复制到XPath checker中刚才的内容就显示出来。





 还可以把xml用firefox打开，然后点view xpath，在xpath checker里的xpath输入你要查找的xpath，结果就出来了，很有用的哦！



# [找xpath好用的工具（Firefox插件）](http://blog.csdn.net/qiyueqinglian/article/details/49280221)

#### 安装
- 打开firefox浏览器，进入网址[https://addons.mozilla.org/en-US/firefox/](https://addons.mozilla.org/en-US/firefox/)
- 在搜索框里输入WebDriver Element Locator
![这里写图片描述](https://img-blog.csdn.net/20151113154357517)
- 点击Add to firefox
![这里写图片描述](https://img-blog.csdn.net/20151113154847638)
- 会有个弹出框，点击install now
- 可以看从firefox浏览器的menu -> Tools -> Add ons -> Extension里看是否安装成功

#### 使用

1.在firefox里打开www.baidu.com

2. 将光标定位在搜索框里，鼠标右键，如下图所示，会提供多个语言(C#,[Java](http://lib.csdn.net/base/javase),[Python](http://lib.csdn.net/base/python),Ruby等)的xpath。

找到对应自己语言的xpath就可以使用了
![这里写图片描述](https://img-blog.csdn.net/20151113155856305)

### Firebug

这个插件大家一定不陌生，可以用了干很多事情，比如调试[JavaScript](http://lib.csdn.net/base/javascript)等等。

还有一个作用就是定位页面元素，这样你可以清晰的看到某个元素的id,class等等

- 从[https://addons.mozilla.org/en-US/firefox/addon/firebug/](https://addons.mozilla.org/en-US/firefox/addon/firebug/)里找到你的firefox浏览器对应的firebug的版本

-[https://getfirebug.com/downloads](https://getfirebug.com/downloads)，下载并安装

- 安装完后，你的firefox浏览器会有这个图标![这里写图片描述](https://img-blog.csdn.net/20151020172439321)

### firepath

#### 安装
- 下载安装：[https://addons.mozilla.org/en-us/firefox/addon/firepath/](https://addons.mozilla.org/en-us/firefox/addon/firepath/)
- firepath需要结合firebug插件一起使用。安装完并重启浏览器后，点击firebug，在打开的firebug 窗口就能看到firepath安装成功。
![这里写图片描述](https://img-blog.csdn.net/20151116115657060)

#### 使用

通过firebug+firepath可以得到元素的xpath和css path。

##### 1. 得到xpath

1.1 打开firebug窗口，选择firepath tab。

1.2 点击firebug窗口的inspector,然后选择要查看xpath的页面元素，xpath就会显示在firebug窗口，如下图
![这里写图片描述](https://img-blog.csdn.net/20151116120232033)

这种方法可以得到元素的相对xpath,如果想得到元素的绝对xpath，在FirePath旁边的小三角选择Generate Absolute xpath.
![这里写图片描述](https://img-blog.csdn.net/20151116120444413)

##### 2. 得到css path

方法和得到xpath一样，选择css就行，如下图所示
![这里写图片描述](https://img-blog.csdn.net/20151116120659178)

### Xpath Checker

这个插件用了检查你写的xpath是否正确。

- 下载安装：[https://addons.mozilla.org/zh-cn/firefox/addon/xpath-checker/](https://addons.mozilla.org/zh-cn/firefox/addon/xpath-checker/)

- 安装完后，右键 -> View Xpath， Xpath Checker就打开了




