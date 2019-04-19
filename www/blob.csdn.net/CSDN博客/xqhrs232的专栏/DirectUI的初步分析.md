# DirectUI的初步分析 - xqhrs232的专栏 - CSDN博客
2012年08月22日 21:19:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：872标签：[脚本																[wince																[button																[图形																[dropdown																[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=dropdown&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址::[http://www.cppblog.com/sleepwom/archive/2011/02/15/122554.html](http://www.cppblog.com/sleepwom/archive/2011/02/15/122554.html)
相关网帖
1.**directUI库 皮肤功能 动画功能----[http://biyou.blog.51cto.com/957202/288938](http://biyou.blog.51cto.com/957202/288938)**
2.最近DirectUI技术挺火的，分享心得，共享点资料给大家！----[http://topic.csdn.net/u/20100304/20/e4c1c35f-9ccb-4d21-805d-30a94e4670e3.html?seed=2054461849&r=79479103#r_79479103](http://topic.csdn.net/u/20100304/20/e4c1c35f-9ccb-4d21-805d-30a94e4670e3.html?seed=2054461849&r=79479103#r_79479103)
3.请教DirectUI的问题----[http://topic.csdn.net/u/20100212/17/ef9b6247-e3c9-434b-ad6a-96402e41bb58.html](http://topic.csdn.net/u/20100212/17/ef9b6247-e3c9-434b-ad6a-96402e41bb58.html)
4.DirectUI官方网站----[http://www.directui.com/](http://www.directui.com/)
5.MFC和DirectUI各有什么优势？做界面用哪个好些？求解答。----[http://www.icodeguru.com/1/1625.html](http://www.icodeguru.com/1/1625.html)
6.DirectUI开发心得----[http://www.360doc.com/content/11/0513/10/3406069_116381780.shtml](http://www.360doc.com/content/11/0513/10/3406069_116381780.shtml)
# 最近DirectUI技术挺火的，分享心得，共享点资料给大家
最近由于项目的需要学习了一下DirectUI方面的东西,主要借鉴的是一个国外程序员写的代码(见引用一),看了后发现它更多的是探讨一种实现的可能性和思路,和实际应用还是有距离的,不过其实现还是很有意思的。在写此小结的时候又发现国内一个程序员将这个代码部分移植到WINCE下的代码(见引用二),因为平台的差异性要完全开发一个WINCE下的实际代码还是需要时间的。
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)由于本人GUI开发做得少,工作中有关这方面的东西主要是提供思路和方法,学习DirectUI的主要目的是为了更新知识学习思路,文章中难免出现错误。
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)一、核心
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)1、CWindowWnd: 窗口对象类(窗口实例对象父类)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)2、CDialogBuilder: 创建控件类,分析脚本并用递归方式(_Parse函数)创建所有控件实例
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)3、CPaintManagerUI: 窗口消息及图形绘制管理器类
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)4、CGUIRenderEngineUI: 图形渲染引擎类,在离屏DC中生成最终显示的图形,可根据需要扩展多种图形效果显示。
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)5、INotifyUI: 事件通知抽象类
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)6、IMessageFilterUI: 消息过滤抽象类
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)二、控件
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CControlUI: 控件管理抽象父类,父类INotifyUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)1、button
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CButtonUI: 按钮控件
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)COptionUI: 选择按钮控件
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)2、combox
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CSingleLinePickUI: 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CDropDownUI: 下拉控件,父类另有CContainerUI和IListOwnerUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)3、decoration
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CTitleShadowUI: 阴影效果
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CListHeaderShadowUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CSeparatorLineUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CFadedLineUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)4、edit
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CSingleLineEditUI: 单行编辑框控件
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CMultiLineEditUI: 多行编辑框控件
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)5、label
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CLabelPanelUI: 可设置背景色和文字色的静态标签控件
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CGreyTextHeaderUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)6、list
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)第一种:
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CListUI: 列表控件,包含以下几个子控件
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)(1)CListHeaderItemUI: 列表头
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)(2)CListExpandElementUI: 列表项
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)第二种:用法不明
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CListHeaderUI: 列表头
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CListElementUI: 列表项,父类另有IListItemUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CListLabelElementUI: 列表项,父类CListElementUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CListTextElementUI: 列表项
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CListFooterUI: 列表尾
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)7、panel
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CTextPanelUI: 父类CLabelPanelUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CTaskPanelUI: 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CNavigatorPanelUI: 导航面板,父类另有IListOwnerUI,包含CNavigatorButtonUI子控件
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CSearchTitlePanelUI: 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CImagePanelUI: 图片显示
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CWarningPanelUI: 警告提示,父类CTextPanelUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CPaddingPanelUI: 填充栏
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)8、tab
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CTabFolderUI: 父类另有CContainerUI和IListOwnerUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CTabPageUI: 父类另有CContainerUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)9、toolbar
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CToolbarUI: 工具栏,包含以下几个子控件
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)(1)CToolButtonUI: 图形按钮
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)(2)CToolSeparatorUI: 分隔符
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)(3)CToolGripperUI:  gripper
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)10、title
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CToolbarTitlePanelUI: 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)11、statusbar
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CStatusbarUI: 状态栏,父类另有CContainerUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)12、anim
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CAnimJobUI: 动画显示类
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)13、ActiveX
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CActiveXUI: 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)三、容器：
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CContainerUI: 容器类,父类CControlUI和IContainerUI。可以认为容器是特殊的控件(见上面控件类关于父类的说明),其目的之一是具有容器特性的控件可以容纳其它控件,这样可以方便的实现控件的叠加;目的之二实际的窗口只有一个,对于叠加的控件必须要进行层次管理才能正确绘图和事件分发。另外可参见引用三
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)1、画布: CCanvasUI(父类CContainerUI),可绘制背景色、画线、贴图
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CWindowCanvasUI: 父类CCanvasUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CControlCanvasUI: 父类CCanvasUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CWhiteCanvasUI: 父类CCanvasUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CDialogCanvasUI: 父类CCanvasUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CTabFolderCanvasUI: 父类CCanvasUI 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)2、布局: 管理不同层次的控件
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CDialogLayoutUI: 父类CContainerUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CVerticalLayoutUI: 父类CContainerUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CHorizontalLayoutUI: 父类CContainerUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CTileLayoutUI: 父类CContainerUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)四、通用
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)1、script
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CMarkup
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CMarkupNode
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)2、language
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CUIUtility
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)3、multi-thread
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CriticalSection
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)AutoCriticalSection
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CMutex
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CAutoMutex
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CEvent
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CAutoEvent
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CManualEvent
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)五、主要数据成员
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)1、CPaintManagerUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CControlUI* m_pRoot: 如果控件是叠加的则存放最下层的控件对象,否则存放第一个创建的控件对象
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CControlUI* m_pFocus: 存放获得焦点的控件对象指针
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CControlUI* m_pEventHover: 存放当前有鼠标移进移出事件的控件对象指针
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CControlUI* m_pEventClick: 存放当前有点击事件的控件对象指针
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CControlUI* m_pEventKey: 存放当前有按键事件的控件对象指针
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CStdPtrArray m_aNotifiers: 记录所有需要事件通知的窗口,根据窗口名称调用相应的消息处理函数
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CStdPtrArray m_aNameHash: 保存控件对象指针hash表(用控件名称生成hash值)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CStdPtrArray m_aPostPaint: panel的fade效果
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CStdPtrArray m_aMessageFilters: 保存需要进行消息过滤的控件或功能(如动画类)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CStdPtrArray m_aDelayedCleanup: 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CStdPtrArray m_aPreMessages: 预处理消息
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)HWND m_hWndPaint: 控件布局窗口句柄
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)HDC m_hDcPaint: 控件布局窗口设备DC
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)HDC m_hDcOffscreen: 离屏内存DC
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)HBITMAP m_hbmpOffscreen: 离屏内存DC相关联HBITMAP 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)2、CControlUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CPaintManagerUI* m_pManager: 窗口或控件绘图及消息管理器
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CControlUI* m_pParent: 逻辑上的父窗口(控件)对象指针
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CStdString m_sName: 控件标识
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CStdString m_sText: 控件显示标题或显示脚本字符串
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CStdString m_sToolTip: 控制的Tip信息
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)3、CContainerUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CStdPtrArray m_items: 同一层的控件对象或控件对象的子对象,例如canvas上放置的按钮、combox由edit和list两个子对象组成,其它还有tab等。具体见CDropDownUI、CTabFolderUI、CNavigatorPanelUI三个类定义
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)4、CDialogLayoutUI
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)CStdValArray m_aModes: 用于存放在Layout上绝对坐标转成相对坐标(CDialogLayoutUI::RecalcArea)的控件对象(指针、大小、模式),目的是否为了让布局上的控件随布局变化而变化,能够正确绘图？？？
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)六、控件属性
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)待完成
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)七、脚本例子
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)<Dialog>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)<WindowCanvas pos=\"0,0,600,800\">
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)<DialogLayout pos=\"0,0,600,800\">
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)<Button pos=\"390, 30, 490, 58\" text=\"OK\" name=\"ok\"/>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)</DialogLayout>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)</WindowCanvas>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)</Dialog>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)八、绘图及事件处理
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)1、绘图
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP01. CWindowWnd::__WndProc: 主窗口程序
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP02. pThis->HandleMessage: pThis是布局窗口对象指针,并与布局窗口绑定(SetWindowLongPtr)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP03. m_pm.MessageHandler: m_pm为CPaintManagerUI唯一实例对象
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP04. CPaintManagerUI::MessageHandler: 处理WM_PAINT
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP05. m_pRoot->DoPaint: m_pRoot为最下层的控件对象,在本例中为CWindowCanvasUI控件(对应脚本中的WindowCanvas)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP06. CCanvasUI::DoPaint: 往画布上绘制背景色、边角弧形、水印等。
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP07. CContainerUI::DoPaint: 在布局窗口(对应脚本中DialogLayout)画所有控件(控件实例对象保存在m_items中)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP08. pControl->DoPaint: pControl为控件对象实例之一,利用多态性来调用不同控件的绘图方法
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP09. CButtonUI::DoPaint: 按钮(对应脚本中Button)绘图方法,有下面两种方法
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)i)文字方法: CGUIRenderEngineUI::DPaintButton
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)ii)图片方法: CGUIRenderEngineUI::DoPaintBitmap
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP10. 新一轮消息循环
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)2、事件
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP01. CWindowWnd::__WndProc: 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP02. pThis->HandleMessage: 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP03. m_pm.MessageHandler: 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP04. CPaintManagerUI::MessageHandler: 处理WM_LBUTTONDOWN
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP05. CPaintManagerUI::FindControl: 根据鼠标坐标查找相应控件对象
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP06. m_pRoot->FindControl: 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP07. CContainerUI::FindControl: 在布局窗口上查找相应控件对象
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP08. CControlUI::FindControl: 在m_items中查找相对应的控件对象
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP09. pControl->Event: pControl为控件对象实例之一,利用多态性来调用不同控件的事件方法
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP10. CPaintManagerUI::MessageHandler: 处理WM_LBUTTONUP
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP11. m_pEventClick->Event: 利用多态性来调用不同控件的事件方法(m_pEventClick说明见"主要数据成员")
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP12. CButtonUI::Event: 按钮(对应脚本中Button)事件方法
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP13. CButtonUI::Activate: 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP14. m_pManager->SendNotify: 传递控件对象指针和触发事件(文本方式)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP15. CPaintManagerUI::SendNotify: 注意以下两点实现是完成控制和业务分离的关键
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)i)利用重载特性调用注册的监听对象(窗口)的消息处理函数Notify(监听对象保存在m_aNotifiers中)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)for( int i =0; i < m_aNotifiers.GetSize(); ++i )
{
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    static_cast < INotifyUI *> (m_aNotifiers[i]) -> Notify(Msg);
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)} 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)ii)布局窗口CStartPageWnd的消息处理,宏定义展开后实际就是重载的Notify函数
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)DIRECT_BEGIN_NOTIFYMAP(CStartPageWnd)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)    PROCESS_BUTTON_CLICK(_T("ok"),OnOk)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)    。。。
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)DIRECT_END_NOTIFYMAP(CStandardPageWnd)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP16. CStartPageWnd::OnOk: 控件消息处理函数,此处可以加入具体的事务逻辑处理
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)STEP17. 新一轮消息循环
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)3、消息定义(文本)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)"click"、"changed"、"link"、"browse"、"itemclick"、"itemselect"、"dropdown"、"itemactivate"、"headerdragging"、"headerclick"、"headerdragged"、"itemexpand"、"itemcollapse"、"windowinit"、"killfocus"、"setfocus"、"timer"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)九、疑问
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)1、Edit、Combox的下拉列表部分、ScrollBar、Tooltip控件是创建的实际窗口,这个与DirectUI思路还是有差别的
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)2、实例中有创建一个不进行消息处理的窗口(CFrameWindowWnd),然后又创建了一个窗口(CStandardPageWnd)用于具体的控件布局。但是我用一个窗口也能实现,原作者为什么这样还不清楚
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)3、控件是用文本形式来做标识的,消息类型是文本形式,是否改成数值型比较好
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)十、引用
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)引用一: http://www.viksoe.dk/code/windowless1.htm
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)引用二: http://directui.googlecode.com/
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)引用三: http://www.cnblogs.com/cutepig/archive/2010/06/14/1758204.html
