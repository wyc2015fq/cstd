# Python屏幕取词(整理) - xqhrs232的专栏 - CSDN博客
2017年01月12日 11:07:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2228
原文地址::[http://blog.sina.com.cn/s/blog_54ce569c01008q01.html](http://blog.sina.com.cn/s/blog_54ce569c01008q01.html)
相关文章
1、[屏幕取词技术实现原理与关键源码](http://blog.csdn.net/chuan014/article/details/1569955)----[http://blog.csdn.net/chuan014/article/details/1569955](http://blog.csdn.net/chuan014/article/details/1569955)
Ubuntu 下可以监视 gtk.Clipboard 来获取当前选中文字和位置。
我以前尝试过定时抓取鼠标附近图像做 OCR 来取词，
改成快捷键取词会省一点。
直接获得文字的悬停取词比较麻烦。
网页、XUL 应用程序可以有鼠标悬停事件。
X11 自己没有， 不过以前流行中文平台、中文外挂时候 TurboLinux 的中文 X-window 被修改为集成鼠标悬停取词翻译。
Gtk 程序也许你可以在 ATK 层入手，自己改 ATK 和用 LD_LIBRARY_PATH。
windows下很不好做。
普遍用的是HOOK API的方法。
可以参考stardict的取词模块。
不过我感觉stardict的取词模块也不是太好用（没金山词霸的好用），感觉有bug。
似乎以前某版本的金山词霸可以翻译图片中的文字，就是用 OCR
再，金山似乎出过 Linux Qt3 版本，找 Zoomq 几位在金山的老大索取源码看看吧
嗯， 金山词霸确实出过 Linux 版本，
是基于 wine 的，不是原生的 Linux 版本...
linux下就不知道，windows下，应该是做一个api hook，钩住TextOut,DrawText和DrawTextEx。
要取词的时候给鼠标所在的窗口发一个WM_PAINT消息，让窗口重绘。
当窗口调用TextOut, DrawText或是DrawTextEx进行重绘的时候，你就可以根据传入的参数知道
窗口想在鼠标下画什么东西了。
