# VC中ComboBox下拉列表不显示数据或只显示一行问题 - xqhrs232的专栏 - CSDN博客
2013年12月21日 16:55:54[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：939
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::http://blog.sina.com.cn/s/blog_879a9e660100tduq.html
相关网帖
1、VC中Combo Box的使用小结----http://blog.csdn.net/bagboy_taobao_com/article/details/5414333
这是转某人的，说的算是具体，我再补充个图上去。
今天在CSDN里看到有不少人问一个关于VC中Combo Box下拉列表不显示数据的问题，他们正确添加了数据，但是在运行时点击Combo Box却不显示数据，其实原因很简单，在使用Combo Box控件时，有一个显示列表区域大小的设置，默认的显示区域太小，故无法显示数据。只要将鼠标在Combo Box的下三角上单击后，出现下拉的图标，向下拉下一个显示列表的区域即可。
下三角的位置
   另外，有人问道如何向Combo Box中添加数据，有两种方法：一种，直接鼠标右键，选择“属性”-“数据”，输入即可，一行表示一个数据，Ctrl+Enter换行。另一种方法，在OnInitDialog()中添加ComboBox.AddString("数据名")。添加项的值从大到小排列，最后添加的数据项，值最小，默认为0。ComboBox.SetCurSel(0); 可以设置Combo Box默认显示第0项数据。
//=========================================================================================
备注::
1》我在VS2005下按这样的方法试了还是不行，还得去设置其属性Disable No Scroll=TRUE才可以！！！挺奇怪的！
2》我在程序里面用SetWindowsPos改变了一下Como box控件的位置，就又只能显示一行数据，下拉框又不显示了，好奇怪！！！
