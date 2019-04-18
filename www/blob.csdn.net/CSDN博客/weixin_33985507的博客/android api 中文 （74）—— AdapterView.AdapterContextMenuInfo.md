# android api 中文 （74）—— AdapterView.AdapterContextMenuInfo - weixin_33985507的博客 - CSDN博客
2010年12月23日 13:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

**前言**
　　本章内容是android.widget.AdapterView.AdapterContextMenuInfo，版本为Android 2.3 r1，翻译来自"cnmahj"，欢迎大家访问他的博客：[http://android.toolib.net/blog/](http://android.toolib.net/blog/)，再次感谢"cnmahj"！欢迎你一起参与Android API 的中文翻译，联系我over140@gmail.com。 
**声明**
　　欢迎转载，但请保留文章原始出处:) 
　　　　农民伯伯：[http://over140.blog.51cto.com/](http://over140.blog.51cto.com/)
　　　　Android中文翻译组：[http://code.taobao.org/project/view/404/](http://code.taobao.org/project/view/404/)
**正文**
　　一、结构
***public static class [AdapterContextMenuInfo](http://android.toolib.net/reference/android/widget/AdapterView.AdapterContextMenuInfo.html) extends Object***
***implements [ContextMenu.ContextMenuInfo](http://android.toolib.net/reference/android/view/ContextMenu.ContextMenuInfo.html)***
java.lang.Object
android.widget.AdapterView.AdapterContextMenuInfo
　　二、概述
当显示 AdapterView的上下文菜单时，为 `[onCreateContextMenu(ContextMenu, View, ContextMenuInfo)](http://android.toolib.net/reference/android/view/View.OnCreateContextMenuListener.html#onCreateContextMenu%28android.view.ContextMenu,%20android.view.View,%20android.view.ContextMenu.ContextMenuInfo%29)`回调函数提供的额外的菜单信息。
　　三、字段
public long **id**
用于显示上下文菜单的子视图的行 ID。 
public int **position**
用于显示上下文菜单的子视图在适配器中的位置。 
public [View](http://android.toolib.net/reference/android/view/View.html)**targetView**
用于显示上下文菜单的子视图。也是 AdapterView的子视图之一。 
　　四、构造函数
public AdapterView.AdapterContextMenuInfo ([View](http://android.toolib.net/reference/android/view/View.html) targetView, int position, long id) 
构造函数
　　五、相关章节
[android api 中文 （73）—— AdapterView](http://over140.blog.51cto.com/2543800/582515)
　　六、补充
本文由[Android中文在线文档](http://android.toolib.net/reference/android/widget/AdapterView.AdapterContextMenuInfo.html)转换而成。
