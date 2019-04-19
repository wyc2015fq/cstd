# easyui使用介绍 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年09月07日 14:49:39[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：141
[http://www.jeasyui.com/index.php](http://www.jeasyui.com/index.php)
based on jQuery, Angular and Vue.
don't need to write many javascript code, you usually define user-interface by writing some HTML markups.
Freeware edition does not include source code (code is "compressed").
Each component of easyui has properties, methods and events. Users can extend them easily.
properties is defined in jQuery.fn.{plugin}.defaults
events(callback functions) is defined in jQuery.fn.{plugin}.defaults also. 一样
Methods defined in jQuery.fn.{plugin}.methods.
 Each method has two parameters: jq and param. 
调用：$('selector').plugin('method', parameter);
引入
- <link rel="stylesheet" type="text/css" href="easyui/themes/default/easyui.css">
- <link rel="stylesheet" type="text/css" href="easyui/themes/icon.css">
- <script type="text/javascript" src="easyui/jquery.min.js"></script>
- <script type="text/javascript" src="easyui/jquery.easyui.min.js"></script>
或可选的
//引入语言包 <script type="text/javascript" src="easyui/locale/easyui-lang-zh_CN.js" ></script>
[https://www.kancloud.cn/aktiajyx/jquery-easyui/250048](https://www.kancloud.cn/aktiajyx/jquery-easyui/250048)
窗口
Load window content via ajax.
- $('#win').window('refresh', 'get_content.php');
动态添加tabs
[http://www.jeasyui.com/tutorial/layout/tabs2.php](http://www.jeasyui.com/tutorial/layout/tabs2.php)
[http://www.jeasyui.net/tutorial/17.html](http://www.jeasyui.net/tutorial/17.html)
分页
折叠面板：个人感觉可以用作左侧导航， 布局也可以作左侧导航（region="west"和center）
布局：5部分，可以嵌套, 如果放在整个页面上，需要加在body上<body class="easyui-layout">，而不是<div id="cc" class="easyui-layout"
fit属性：尺寸适应它的父容器
有时<!DOCTYPE html> 这行代码会出现
![](http://note.youdao.com/yws/res/103449/3B6571500B6A486DB311A7EE4D644193)
菜单
![](http://note.youdao.com/yws/res/103452/BDFF3FA520D441E19C199C1BE71EB317)
sidemenu
![](http://note.youdao.com/yws/res/103456/760505506A5A4763843A35E3F67832CD)
按钮有链接按钮，
![](http://note.youdao.com/yws/res/103460/3E8A7B21B2CA42199499A6B489B6241B)
菜单按钮：可实现下拉式菜单，
![](http://note.youdao.com/yws/res/103464/C38162E79E2E40C1BFA791002C29A262)
SplitButton跟菜单按钮差不多，区别是， splitbutton is split into two parts.When moving mouse over the splitbutton, a 'split' line will display. The menu only display when moving mouse over the right part of splitbutton.
![](http://note.youdao.com/yws/res/103469/CCB3A5F7D63C48F2AB831F4FBAEA4AF0)
SwitchButton
![](http://note.youdao.com/yws/res/103473/A6BE60FE5CB34EE58DC034651F29062C)
MaskedBox：结构化输入
![](http://note.youdao.com/yws/res/103482/C2BC9BDDCB0D4BB3947C9229BF2DE78A)
Combo：display a editable text box and drop-down panel
![](http://note.youdao.com/yws/res/103486/78A6C6DB6C014DC9A45BB41CC4CB0F01)
DataGrid
![](http://note.youdao.com/yws/res/103518/76E315F3E95F4073B0800342402EDEBC)
DataList：a special datagrid that displays data in one column.
![](http://note.youdao.com/yws/res/103522/F5123BB8735243CE8FE3932045C8EA9F)
ComboBox：display an editable text box and drop-down list，user can select one or multiple values. 
![](http://note.youdao.com/yws/res/103492/7EB1726BAA5A449D9A18DB909F20BAD9)
ComboTree： similar to combobox but replace the list with tree component.
![](http://note.youdao.com/yws/res/103496/C9E83F1E49424649BC0645BCC4C76379)
ComboGrid：an editable text box with drop-down datagrid panel
![](http://note.youdao.com/yws/res/103500/DCC7A2569701408A899B695DBEFC292A)
TreeGrid
![](http://note.youdao.com/yws/res/103534/1DA4216266DD4E2CB32538F5FBA69B2C)
ComboTreeGrid
![](http://note.youdao.com/yws/res/103502/C18B52E10D2B4F74A70CAF385288F222)
TagBox
![](http://note.youdao.com/yws/res/103506/3CB429CCA9494D2B923DA157433239E7)
Spinner：
![](http://note.youdao.com/yws/res/103511/F5C2C47129FD4033B7FDEDD15195E9CF)
Messager
![](http://note.youdao.com/yws/res/103515/E1AF0FCA927A4054A3450BC21F376149)
PropertyGrid
![](http://note.youdao.com/yws/res/103527/C4385DD0C377454BBC0985E068E53F09)
Tree
![](http://note.youdao.com/yws/res/103530/4BDBCD8BEB0B4B14BC083BE3FF16A86B)
RTL (Right to Left) 
示例程序源码
[http://www.jeasyui.com/demo/main/index.php](http://www.jeasyui.com/demo/main/index.php)
[http://www.jeasyui.com/tutorial/index.php](http://www.jeasyui.com/tutorial/index.php)
中文文档[http://www.jeasyui.net/plugins/](http://www.jeasyui.net/plugins/)
其它：rtl解释：[https://www.theme-junkie.com/what-is-rtl-wordpress-theme/](https://www.theme-junkie.com/what-is-rtl-wordpress-theme/)
