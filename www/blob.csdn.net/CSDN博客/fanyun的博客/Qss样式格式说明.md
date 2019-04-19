# Qss样式格式说明 - fanyun的博客 - CSDN博客
2017年12月26日 22:30:52[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：198标签：[qt](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
个人分类：[Qt](https://blog.csdn.net/fanyun_01/article/category/6429937)

Qss 其实就是一段文本，当然得按一定格式来写。下面请看Qss的语法格式：
选择器 { 属性1：值；属性2：值；……属性n：值；}
对应上篇文章的Qss样式：
![1](http://images.cnitblog.com/blog/712842/201502/081004390166277.png)
现在我们可以来解释这段话的意思了：
对应用程序中的QPushButton类的背景色、文本颜色（background-color、color)使用指定的rgb值（rgb(120,120,120)、rgb(0,230,230))。
看来写Qss样式，得先确定选择器符号。那么选择器符号有哪些呢？
先看表1，可以充当选择器的符号
|选择器|实例|可以匹配的窗口部件|
|----|----|----|
|全局对象|*|任意窗口部件|
|类型|QDial|给定类的实例，包括子类|
|类|.QDial|给定类的实例，不包括子类|
|标识|QDial#ageDial|给定对象名称的窗口部件|
|Qt属性|QDial[y="0"]|为某些属性赋值的窗口部件|
|子对象|QFrame>QDial|给定窗口部件的直接子窗口部件|
|子孙对象|QFrame QDial|给定窗口部件的子孙窗口部件|
再来看表2，和上表中选择器配合使用的辅助控制器。
|辅助控制器|说明|
|----|----|
|::indicator|复选框、单选钮、可选菜单项或可选群组框的指示器|
|::menu-indicator|按钮的菜单指示器|
|::item|菜单、菜单栏或状态栏项|
|::up-button|微调框或滚动条的向上按钮|
|::down-button|微调框或滚动条的向下按钮|
|::up-arrow|微调框、滚动条或标题视图的向上箭头|
|::down-arrow|微调框、滚动条、标题视图或组合框的向下箭头|
|::drop-down|组合框的下拉箭头|
|::title|群组框的标题|
再来看表3，和表1、表2配合使用的状态。
|状态|说明|
|----|----|
|:disabled|禁用窗口部件|
|:enabled|启用窗口部件|
|:focus|窗口部件有输入焦点|
|:hover|鼠标在窗口部件上悬停|
|:pressed|鼠标按键点击窗口部件|
|:checked|按钮已被选中|
|:unchecked|按钮未被选中|
|:indeteminate|按钮被部分选中|
|:open|窗口部件位于打开或扩展状态|
|:closed|窗口部件位于关闭或销毁状态|
|:on|窗口部件的状态是“on”|
|:off|窗口部件的状态是“off”|
