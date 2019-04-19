# duilib使用注意 - xqhrs232的专栏 - CSDN博客
2012年09月22日 20:52:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1248
原文地址::[http://blog.csdn.net/mail_cm/article/details/7565140](http://blog.csdn.net/mail_cm/article/details/7565140)
1.VC6使用记得Project Settings -> C/C++ -> Preprocessor definitions添加_USRDLL,UILIB_EXPORTS。
2.mfc中新建duilib窗口退出时整个程序退出了，是因为duilib窗口退出时使用了PostQuitMessage(0)，使得整个程序退出，换成Close(); 但是不知道什么原因有的duilib窗口先退出了，mfc窗口出非法访问错误也退出了，有的duilib窗口就不会。
3.建duilib窗口要PostQuitMessage(0);   ::CoInitialize(NULL);
**①：框架程序基本流程**
-> WinMain(入口函数)
-> CPaintManagerUI::SetResourceInstance  (实例句柄与渲染类关联)
-> ::CoInitialize  (初始化COM库, 为加载COM库提供支持)
-> new C360SafeFrameWnd (创建窗口类)
-> pFrame->Create (注册窗口类与创建窗口)
->RegisterSuperclass (注册一个超类 即已有一个窗口类的基上再注册一个窗口类)
->RegisterWindowClass（注册窗口类）
-> ::CreateWindowEx (创建窗口，此时触发 WM_CREATE消息)
-> HandleMessage  ( WM_CREATE消息处理OnCreate)
-> pFrame->CenterWindow (窗口居中显示)
-> CPaintManagerUI::MessageLoop (处理消息循环)
-> ::CoUninitialize();   （退出程序并释放COM库）
**②：上面的流程图是一个基本的WIN32程序框架，相对于 DUILIB库，此时我们应关心的是窗口上的元素是如何建立起来的。这时我们应关心WM_CREATE的消息处理函数OnCreate的实现**
-> C360SafeFrameWnd:: OnCreate
  -> m_pm.Init(m_hWnd) (主窗口类与窗口句柄关联)
-> CControlUI* pRoot = builder.Create（加载XML并动态创建界面无素，与布局界面元素，**核心函数**单独分析 注意：CDialogBuilder并不是一个对话框类）
-> m_pm.AttachDialog （附加控件数据到HASH表中）
      -> InitControls （初始化控件）
             ->FindControl
->__FindControlFromNameHash
    ->pManager->m_mNameHash.Insert（把控件插入到Hash中）
-> m_pm.AddNotifier增加通知处理
**③：接下来我们应关心的是XML的加载过程，及XML中的数是如何与窗口类进行关联的**
**-> CDialogBuilder::Create （加载XML文件）**
->  m_xml.LoadFromFile
-> CMarkup::LoadFromFile （也可从资源文件中加载::LoadResource）
    ->  CMarkup::LoadFromMem（把外部XML数据加入到内存）
->  m_xml.GetRoot （获得XML 根结点， 开始解析XML数据）
->  "window"  （解析根结点window下的三个公共元素属性）
   ->  "image"  pManager->AddImage（加载设备无关位图并关联到图像HASH中m_mImageHash.Insert）
 ->  "font"  pManager->AddFont（增加字体）
->  CPaintManagerUI::AddFont（增加字体 并关联字体数组m_aCustomFonts.Add）
->  "default" （增加默认属性 如：垂直滚动条和水平滚动条）
   -> _Parse  （解析具体的控件元素 动态生成控件元素）
             -> node.GetName (跳过已经解析过的三个元素"image" "font" "default")
-> _tcscmp （根据元素的名称 动态生成控件"Edit" "List" "Text"  "Combo" "Label" "Button"  "Option" "Slider" "Control" "ActiveX" "Progress"  "Container" "TabLayout" "ListHeader" "TileLayout" "DialogLayout" "VerticalLayout" "ListHeaderItem"  "ListTextElement"  "HorizontalLayout" 
 "ListLabelElement" "ListExpandElement" "ListContainerElement"）
-> pContainer->Add(pControl)   （增加控件）
-> m_items.Add（关联于控件数组中）
           -> pControl->SetAttribute设置控件属性
**④上面的工作完成了XML数据的加载，并动态生成了控件，把控件加载到了控件列表，建立了与控件相关的HASH表。接下来我们应关心的是控件如何显示在界面上的。**
通过上面的源码，我们应关心几个核心类的关联：
->首先产生主窗口类：C360SafeFrameWnd，
->在主窗口类中放置成变量 CPaintManagerUIm_pm; 与 控件绘制和消息处理关联起来
->CPaintManagerUI::MessageHandler WM_PAINT
->双缓存绘图 
-> m_pRoot->DoPaint绘背景图 
-> CControlUI::DoPaint
-> CRenderEngine 真正的绘图类
-> pPostPaintControl->DoPostPaint在背景图上绘制控件
->::BitBlt 把离屏视图画到主屏上
**⑤当界面元素正常显示后，接下来我们应关心的是如何处理控件的事件**
-> C360SafeFrameWnd:: HandleMessage （应用需要处理的消息及处理函数）
-> m_pm.MessageHandler（DUILIB库帮我们处理的消息及相关的处理函数）
-> CWindowWnd::HandleMessage（应用层和 DUILIB都不处理的消息交由系统默认处理）
