# 解决listCtrl 无法显示列头的问题 - writeeee的专栏 - CSDN博客
2015年08月24日 10:38:07[writeeee](https://me.csdn.net/writeeee)阅读数：1264
学习 listCtrl 时发现显示不出列头，如图
![](https://img-blog.csdn.net/20150824104012939?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
后边找到了原因，上代码
```cpp
<span style="white-space:pre">	</span>// TODO: 在此添加额外的初始化代码
	CRect rect;   
	m_taskList.GetClientRect(&rect);  
	//
	//<span style="color:#ff6666;">通过本段代码，即可解决 显示不出列头的问题</span>
	//
	LONG lStyle;
	lStyle = GetWindowLong(m_taskList.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_taskList.m_hWnd, GWL_STYLE, lStyle);//设置style
	
	m_taskList.SetExtendedStyle (LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES );
	m_taskList.InsertColumn(0,_T("taskName"),LVCFMT_CENTER, rect.Width()/5);
	m_taskList.InsertColumn(1,_T("taskType"),LVCFMT_CENTER, rect.Width()/5);
	m_taskList.InsertColumn(2,_T("taskDes"),LVCFMT_CENTER, rect.Width()/5);
	m_taskList.InsertColumn(3,_T("CurExcuteTime"),LVCFMT_CENTER,rect.Width()/5);
	m_taskList.InsertColumn(4,_T("NextExcuteTime"),LVCFMT_CENTER, rect.Width()/5);
	//m_taskList.InsertColumn(5,"taskCycle",LVCFMT_LEFT, 40);
	m_taskList.InsertItem(0, _T("task1"));   
	m_taskList.SetItemText(0, 1, _T("手动"));
	m_taskList.SetItemText(0, 2, _T("描述--1"));   
	m_taskList.SetItemText(0, 3, _T("2010-1-1"));   
	m_taskList.SetItemText(0, 4, _T("2010-1-2"));
```
运行效果，如图
![](https://img-blog.csdn.net/20150824104226937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
