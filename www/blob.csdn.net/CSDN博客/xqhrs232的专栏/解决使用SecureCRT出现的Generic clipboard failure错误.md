# 解决使用SecureCRT出现的Generic clipboard failure错误 - xqhrs232的专栏 - CSDN博客
2016年10月20日 14:46:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4086
原文地址::[http://www.cnblogs.com/william-lee/archive/2011/01/24/1942827.html](http://www.cnblogs.com/william-lee/archive/2011/01/24/1942827.html)
相关文章
1、RecureCRT could not get data from the Clipboard 和SAP快捷剪切和粘贴（Q）功能不能用----[http://blog.163.com/huangbao_007/blog/static/3563653520160190455241/](http://blog.163.com/huangbao_007/blog/static/3563653520160190455241/)
我使用securecrt连接hp-ux进行操作，使用的版本是securecrt 6.6 64bit。
不过，选定文本，进行右击时，发现如下的错误：
Could not get data from the Clipboard:Generic clipboard failure.
![复制错误](http://images.cnblogs.com/cnblogs_com/william-lee/201101/201101240837109840.png)
不能从选定的文本进行复制。总是弹这个错误，不能弹出复制相关的上下文菜单。
找到“options”---“global options”，选择terminal:
![选中时复制](http://images.cnblogs.com/cnblogs_com/william-lee/201101/201101240837111858.png)
如上图，找到"mouse”，选定“选择时复制”，然后你再操作试试：选定文本，然后光标定位到要复制的位置，右击，便粘贴了刚才选定的文本，是不是很方便！
//====================================================================================
备注::
1>我遇到的情况是因为跟金山词霸的功能起冲突了
2>请检查你的系统内是否安装了其它类似的软件：如金山词霸、有道翻译的划词翻译功能，SAP的快捷剪切和粘贴（Q）功能等。这些功能都会有冲突。
3>经过验证：SecureCRT 7.2和SAP Gui7.4同时开启右键复制和粘贴功能相到不冲突，但是这两款系统与金山词霸、有道翻译的划词翻译功能相冲突。
