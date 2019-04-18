# duilib控件消息类型 - 深之JohnChen的专栏 - CSDN博客

2019年04月14日 23:42:38[byxdaz](https://me.csdn.net/byxdaz)阅读数：23


**duilib中各控件响应的消息类型**

消息类型
|消息|说明|Sender|
|----|----|----|
|click|鼠标点击|CButtonUI|
|dropdown|下拉显示|CComboUI|
|headerclick|点击列标题|CListHeaderItemUI|
|itemactivate||CListElementUI、CListContainerElementUI|
|itemclick|单击选项|CListLabelElementUI、CListContainerElementUI|
|itemselect|选择选项|CComboUI、CListUI|
|killfocus|失去焦点|CControlUI|
|link||CTextUI、CListTextElementUI|
|menu||CButtonUI、CControlUI|
|return|回车|CEditWnd、CRichEditUI|
|scroll|滚动|CScrollBarUI|
|selectchanged|变更选项|COptionUI|
|setfocus|获得焦点|CControlUI|
|showactivex||CActiveXUI|
|textchanged|文本被改变|CEditWnd|
|tabselect|标签页被选中|CTabLayoutUI|
|timer||CControlUI|
|valuechanged|值发生变化|CSliderUI|
|windowinit|窗体初始化||

**[扩展duilib的消息](/)**

duilib并没有提供双击和右键消息，所以需要我们自行扩展，这里以添加双击消息为例，

　　在UIDefine.h里，我们只看到了DUI_MSGTYPE_CLICK消息，却没有看到双击消息，因此需要在这里添加一个双击消息：

　　#define DUI_MSGTYPE_DBCLICK                (_T("dbclick"))
　　在UIManager.h里可以看到 UIEVENT_DBLCLICK、UIEVENT_RBUTTONDOWN等消息，这是因为它的右键和双击等消息只给了内部的控件使用，这时我们需要让他传递出来，所以需要在CControlUI::DoEvent函数里传递双击消息，如下：

    if( event.Type == UIEVENT_DBLCLICK )
    {
        m_pManager->SendNotify(this, DUI_MSGTYPE_DBCLICK, event.wParam, event.lParam);
        //return;
    }
　　这里记住不要return，因为双击消息UIEVENT_DBLCLICK 还需要传递给List等控件。

　　然后重新编译一下duilib，就可以响应双击消息啦，

　　我们在Notify函数里添加以下代码：

if( msg.sType == _T("dbclick"))  
{    
     // 这里会传进来很多次双击消息，所以只获取祖先控件的消息
    if (! msg.pSender->GetParent())
    {
         FullScreen(! m_bFullScreenMode);
    }
}
　　即可响应双击消息，和click一样简单哦~

　　需要注意的是：这个消息会从子控件一直传递给父控件，我们如果加了很多Layout控件的话，可以发现会进来很多次dbclick消息，如果我们不想要父控件收到这个消息，可以处理之后直接return。

