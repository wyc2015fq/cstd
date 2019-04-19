# C++与Flash的交互 - 逍遥剑客 - CSDN博客
2008年10月22日 10:12:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：25730
研究Flash嵌入游戏中的可行性.......
- 渲染问题已解决
- 事件响应已解决
- 下面是C++与Flash AS的交互, 以MFC为例:
1. 新建一个MFC Dialog程序
2. 添加一个Flash控件
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081022/CppFlash_AddActiveX.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081022/CppFlash_AddFlash.JPG)
3. 把Flash控件添加一个变量
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081022/CppFlash_AddVariable.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081022/CppFlash_AddflashUI.JPG)
4. 在OnInitDialog()中添加载.swf文件
// CCppFlashDlg 消息处理程序  
BOOL CCppFlashDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动  
    //  执行此操作  
    SetIcon(m_hIcon, TRUE);         // 设置大图标  
    SetIcon(m_hIcon, FALSE);        // 设置小图标  
    // TODO: 在此添加额外的初始化代码  
    this->flashUI.LoadMovie(0, "C://Documents and Settings//Administrator//My Documents//My Flash//HelloWorld.swf");
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE  
}
5. 制作一个flash, 放一个Button上去, 导出一下就可以在MFC中看到了
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081022/CppFlash_FlashButton.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081022/CppFlash_MFCDialog.JPG)
6. flash调用C++. 
这个很简单, 在flash的那个Button组件的动作中添加脚本:
on(click)
{
    fscommand("MsgBox", "这是flash调用c++的响应");
}
然后在MFC中添加事件响应:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081022/CppFlash_AddEvent.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081022/CppFlash_AddEventMethod.JPG)
void CCppFlashDlg::FSCommandShockwaveflash1(LPCTSTR command, LPCTSTR args)
{
    // TODO: Add your message handler code here 
    if (0 == strcmp("MsgBox", command))
    {
        MessageBox(args);
    }
}
7. C++调用Flash. 
首先在Flash中注册回调函数:
import mx.controls.Alert;
import flash.external.*;
ExternalInterface.addCallback("MsgBox", this, MsgBox);
function MsgBox(msg:String)
{
    Alert.show(msg, "",Alert.OK);
}
然后在C++中添加调用:
void CCppFlashDlg::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class 
    this->flashUI.CallFunction("/
       <invoke name=/"MsgBox/">/
           <arguments>/
               <string>这是C++调用flash的响应</string>/
           </arguments>/
       </invoke>");
}
另外, 还可以通过GetVariable()和SetVariable()来设置flash中定义的变量
8. 导出一下flash, 编译一下C++, 就可以看到效果了:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081022/CppFlash_CallCpp.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081022/CppFlash_CallFlash.JPG)
