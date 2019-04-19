# Allegro从.brd文件中导出器件封装 - xqhrs232的专栏 - CSDN博客
2018年01月15日 17:24:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：647
原文地址::[http://blog.sina.com.cn/s/blog_7177add50100vdwt.html](http://blog.sina.com.cn/s/blog_7177add50100vdwt.html)
相关文章
1、使用Allegro从brd文件中导出封装及焊盘的方法----[https://wenku.baidu.com/view/dddb8a4c0029bd64783e2cfd.html](https://wenku.baidu.com/view/dddb8a4c0029bd64783e2cfd.html)
2、10Allegro从.brd文件中导出器件封装----[https://wenku.baidu.com/view/115daccf710abb68a98271fe910ef12d2af9a9c2.html](https://wenku.baidu.com/view/115daccf710abb68a98271fe910ef12d2af9a9c2.html)
打开.brd文件，**File→Export→Libraries**，除了No libraries dependencies之外，所有选项都勾选上，设定好存放路径之后，Export！！
注意事项：
1. 一般的，**将.dra, .pad, .psm, .fsm, .fsm文件存入path\symbols文件夹，而将.txt文件存入path\devices文件夹**，然后再在PCB
 Editor中设置路径：Setup→User Preferences→Design Paths中加载path\symbols路径到padpath和psmpath。
2. 若出现某些Package Symbol不能调用的情况，有以下两种原因：
情形一：该.pad文件需要重新修复。
Tools→Padstack→Modify Design
 Padstack中save as一下即可（没有实际操作过，感觉貌似用DB Doctor组件也可以吧，O(∩_∩)O~）
情形二：某些padstack包含Thermal relief pad，而导出封装时未能导出.fsm文件。
只需保证导出封装时勾选所有文件类型即可。

