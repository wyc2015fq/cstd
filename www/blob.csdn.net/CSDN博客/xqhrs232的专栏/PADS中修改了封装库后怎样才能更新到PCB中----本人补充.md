# PADS中修改了封装库后怎样才能更新到PCB中----本人补充 - xqhrs232的专栏 - CSDN博客
2013年10月29日 14:21:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：7669
原文地址::[http://qianbwh.blog.163.com/blog/static/1860924072012112510442594/](http://qianbwh.blog.163.com/blog/static/1860924072012112510442594/)
http://zhidao.baidu.com/question/168586033.html
2种方法：
1：在PCB中进入ECO模式-选择要更新的元件-点击change component图标-（选中元件后）右键点击元件-在弹出的菜单中选择libraby browse-勾选update part type from library （已实践）
2. 从logic原理图来更新PCB
打开原理图，选中要变更的元件，分配好要变更的PCB封装
tools-PADS Layout...点击preference,勾选compare PCB Decal Assignments-design-ECO to PCB
//================================================================================================================================
备注::
1》第一种方法的具体实现
 1111----点ECO  Toolbar进入ECO模式 
2222----选中要更改的元器件 
3333----点ECO Toolbar工具栏上的Change Component 
4444----回到绘图区，按鼠标右键弹出菜单，选择Library Browse...功能 
5555----Library::定位你的Library,Items::定位你的元器件名称 
6666----点击Replace 
7777----产生ECO文件，然后反标注到SCH原理图 
8888----打开对应的SCH原理图，用File/Import...菜单命令导入在PCB中产生的ECO文件
9999----这个也只限于修改Part Type----难道每一个封装都的对应一个Part Type
2》第二种方法的说明
    1111----Tools/PADS Layout.../弹出PADS Layout Link对话框
    2222----选中Preferences页面的Compare PCB Decal Assignments
    3333----选中Design页面的ECO To PCB----在PCB就会执行更新了
    4444----PCB中就会自动更新了-----PCB要也同时打开了

