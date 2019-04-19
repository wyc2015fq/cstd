# pads logic off-page新建及编辑符号 - xqhrs232的专栏 - CSDN博客
2013年07月17日 15:54:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4013
原文地址：：[http://lttian1068.blog.163.com/blog/static/132150608201161445446442/](http://lttian1068.blog.163.com/blog/static/132150608201161445446442/)
### pads logic off-page 新建及编辑符号
2011-07-14 16:59:30|  分类：[电子技术](http://lttian1068.blog.163.com/blog/#m=0&t=1&c=fks_084067085087081065083086084095085085083066081086080067092)|字号订阅
首先tools-->part editor，file-->open
弹出对话框如下：
![pads logic off-page 新建及编辑符号 - lttian - 心晴lttian](http://img.ph.126.net/0UMz4dekdD1nS8i4A9TgXw==/1169247053272866875.jpg)
选择右边的off-page，OK。
默认情况下，你可以看到两种off-page符号：refin和refout 。
我们再添加一个输入输出的双向符号。
edit-->part type editor
弹出对话框，左侧add，浏览lib，寻找合适的符号
![pads logic off-page 新建及编辑符号 - lttian - 心晴lttian](http://img.ph.126.net/ROMjADaLGAtw_ne2vy57Kg==/1547267946995277589.jpg)
系统自带的connect lib中有一个双向箭头，我们可以直接选择它。
（自己用2D LINE工具画一个也是可以的，注意连接点要放在原点处）
 完成后OK退出。可以看到多出来了一个叫CON_IO的符号。保存退出。
即可。
需要注意的是，这个更新不能在原有之前的sheet上反映出来，需要新建sheet才能够使用。
（执行Tools/Update off-page from library命令就可以在当前SHEET里面调用了  ）
祝你顺利！
//===========================================================
备注：：
1》padslogic_user.pdf里面也提到了Creating New Special Symbols
