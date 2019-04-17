# 设置Sublime Text 3的光标样式 - 在思索中前行！ - CSDN博客





2014年07月08日 21:31:42[_Tham](https://me.csdn.net/txl16211)阅读数：6891








升级了Sublime Text 3，结果光标变成了这个样子，非常不习惯：

![001](http://css88.b0.upaiyun.com/css88/2014/04/001.jpg)



     查了文档[http://www.sublimetext.com/3](http://www.sublimetext.com/3) ，Build
 3059中得描述：

> 
Added settings caret_extra_top, caret_extra_bottom and caret_extra_width to control the caret size


意思是增加了caret_extra_top caret_extra_bottom caret_extra_width 这三个设置来控制光标尺寸；
- caret_extra_top 超出光标上方的额外距离
- caret_extra_bottom 超出光标下方的额外距离
- caret_extra_width 超出光标宽度

设置的方法很简单 打开 sublime text–>refrences–>settings-user,在配置中加上这三个键/值对。

我自己测试了，基本算改回来了



Json

- {
- "caret_extra_top":1,
- "caret_extra_bottom":1,
- "caret_extra_width":1
- }










