# MFC的消息映射有什么作用 - maopig的专栏 - CSDN博客
2012年02月11日 18:38:57[maopig](https://me.csdn.net/maopig)阅读数：1587
绝对以下这三个解释的比较简洁，特此做个记录！以感谢回答的这些人！
MFC的消息映射有什么作用: 
Windows操作系统主要是有消息来处理的，每个程序都有自己的消息队列，并且这些消息是有优先级的，也就是谁会先执行的。你定义个消息，先发给操作系统，操作系统再传到你的执行函数
比如下面的消息映射：
BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
 ON_CBN_SELCHANGE(IDC_COMBO1, &CMyDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()
函数OnCbnSelchangeCombo1处理唯一的ID——IDC_COMBO1
CBN_SELCHANGE是消息名，
IDC_COMBO1是触发这个消息的控件、
CMyDlg::OnCbnSelchangeCombo1是处理这个消息的函数，
这些相关联的东西用ON_CBN_SELCHANGE(IDC_COMBO1, &CMyDlg::OnCbnSelchangeCombo1)
这句话关联起来
Windows程序是消息驱动的，那就有谁能处理消息，接收到消息后应当怎么做的问题。消息映射就是规定了一种比较方便的模式来把消息、消息处理者、消息处理函数关联起来，即建立起对应关系。比如下面的消息映射：BEGIN_MESSAGE_MAP(CMyDlg, CDialog) ON_CBN_SELCHANGE(IDC_COMBO1, &CMyDlg::OnCbnSelchangeCombo1)END_MESSAGE_MAP()CBN_SELCHANGE是消息名，IDC_COMBO1是触发这个消息的控件、CMyDlg::OnCbnSelchangeCombo1是处理这个消息的函数，这些相关联的东西用ON_CBN_SELCHANGE(IDC_COMBO1, &CMyDlg::OnCbnSelchangeCombo1)这句话关联起来。消息系统对于一个win32程序来说十分重要，它是一个程序运行的动力源泉。一个消息，是系统定义的一个32位的值，他唯一的定义了一个事件，向Windows发出一个通知，告诉应用程序某个事情发生了。例如，单击鼠标、改变窗口尺寸、按下键盘上的一个键都会使Windows发送一个消息给应用程序。消息可以由系统或者应用程序产生。系统在发生输入事件时产生消息。举个例子, 当用户敲键, 移动鼠标或者单击控件。系统也产生消息以响应由应用程序带来的变化, 比如应用程序改变系统字体改变窗体大小。应用程序可以产生消息使窗体执行任务，或者与其他应用程序中的窗口通讯。 
