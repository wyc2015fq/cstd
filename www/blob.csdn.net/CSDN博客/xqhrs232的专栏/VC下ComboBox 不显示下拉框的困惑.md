# VC下ComboBox 不显示下拉框的困惑 - xqhrs232的专栏 - CSDN博客
2014年05月22日 16:18:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3623
原文地址::[http://blog.csdn.net/linuxandroidwince/article/details/6863203](http://blog.csdn.net/linuxandroidwince/article/details/6863203)
相关文章
1、[VC中Combo Box控件使用大全](http://www.cppblog.com/Lee7/archive/2008/02/21/43066.html)----[http://www.cppblog.com/Lee7/archive/2008/02/21/43066.html](http://www.cppblog.com/Lee7/archive/2008/02/21/43066.html)
2、MFC组合框ComboBox总结 ----[http://bbs.csdn.net/topics/390129282?page=1](http://bbs.csdn.net/topics/390129282?page=1)
CreateWindow(TEXT("ComboBox"), TEXT(""), CBS_DROPDOWNLIST | WS_VSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP,0, 120, 100, 20, hWnd, (HMENU)IDC_COMBOBOX, g_hInst, NULL);
 //Add items
 for (int i=0; i<5; i++)
 {
  SendMessage(GetDlgItem(hWnd, IDC_COMBOBOX), CB_ADDSTRING, 0, (LPARAM)comboxitem[i]);
 }
 SendMessage(GetDlgItem(hWnd, IDC_COMBOBOX), CB_SETCURSEL, 0, 0);
创建COMBOBOX没有问题，可就是不显示下拉框,为什么呢？
0, 120, 100, 20 这四个数据分别代表COMBOBOX左上角坐标和宽度高度，最后一个是高度，这里设置太低了，如果改为200，效果就不一样了。
//===============================================================================================================
备注::
1》一般Combobox控件的下拉列表的控制方法是点中那个下拉箭头，此时出现的调整框就是Combo Box的下拉调整框
2》但如果用了SetWindowPos函数来调整了Combobox控件的位置与大小，就必须把Combobox控件的高度调整到足够大，以便可以容下
Combo
 Box的下拉调整框，这样才可以把Combo Box的下拉调整框显示的出来
3》属性里有个
 No integral height 钩选项，表示最大长度为设计长度，如果实际内容比设计长度多，就出现滚动条，少就以实际长度显示
4》Combox控件有3种类型属性可以选择----simple/Dropdown/Drop
 list,其中Drop list的内容是不能修改的。
