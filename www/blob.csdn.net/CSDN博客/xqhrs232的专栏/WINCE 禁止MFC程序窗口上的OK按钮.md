# WINCE 禁止MFC程序窗口上的OK按钮 - xqhrs232的专栏 - CSDN博客
2014年11月05日 14:54:03[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：687
原文地址::[http://blog.csdn.net/brantyou/article/details/7177864](http://blog.csdn.net/brantyou/article/details/7177864)
相关文章
1、
# [wince 禁止程序标题栏上的退出按钮X](http://blog.csdn.net/brantyou/article/details/7177947)----[http://blog.csdn.net/brantyou/article/details/7177947](http://blog.csdn.net/brantyou/article/details/7177947)
# 2、MFC对话框程序屏蔽回车与ESC键----[http://blog.csdn.net/oemoon/article/details/6332682](http://blog.csdn.net/oemoon/article/details/6332682)
3、
## VC/MFC 一个拦截系统消息的钩子(HOOK)程序事例
 ----[http://blog.sina.com.cn/s/blog_4b44e1c00100c6dt.html](http://blog.sina.com.cn/s/blog_4b44e1c00100c6dt.html)
打开资源文件(*.rc),找到这行:EXSTYLE WS_EX_APPWINDOW | 0x80000000L.只要将这一句删除或者注释,就可以去掉"OK"按钮.同理,如果要添加"OK"键,在其后添上EXSTYLE WS_EX_APPWINDOW | 0x80000000L即可.
