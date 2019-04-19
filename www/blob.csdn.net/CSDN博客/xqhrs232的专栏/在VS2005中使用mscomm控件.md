# 在VS2005中使用mscomm控件 - xqhrs232的专栏 - CSDN博客
2012年12月29日 22:06:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1233
原文地址::[http://163n.blog.163.com/blog/static/56035552201011257315995/](http://163n.blog.163.com/blog/static/56035552201011257315995/)
相关网帖
1.VS2005中如何注册使用mscomm控件----[http://xuduo791107.blog.163.com/blog/static/21255902420121012941286/](http://xuduo791107.blog.163.com/blog/static/21255902420121012941286/)
2.VS2005 MSCOMM 控件添加方法----[http://hi.baidu.com/wy494032874/item/8a8edf69c1257a92c4d24927?1356789818](http://hi.baidu.com/wy494032874/item/8a8edf69c1257a92c4d24927?1356789818)
3.**VS2005中串口通信的实现----[http://www.docin.com/p-113254200.html](http://www.docin.com/p-113254200.html)**
- 新建一个基于对话框的MFC应用程序工程SCom。
- **添加ActiveX控件**。在对话框的空白处点击右键，在弹出的菜单中选择“Insert ActiveX Control”，这时弹出一个对话框，列出了本机已注册的所有控件，选择“Microsoft Communications Control, version 6.0”，点击“ok”。
- **添加类和关联变量**。打开类浏览器，右击工程名，在弹出的弹菜中选择“Add-->Class”，然后选择“MFC Class From ActiveX Control”，在“Available ActiveX controls”中选择“Microsoft Communications Control, version 6.0”，将“IMSComm”加入到“Generated classes”中，点击“Finish”。然后右击对话框中新加的串口控件（电话图标），选择“Add-->Add
 Variable”，为该控件关系一个成员变量。
- **添加串口事件消息处理函数**。右击对话框中的串口控件图标，在弹出的菜单中选择“Add Event Handler”，保持默认的函数处理程序名称，点击“Add and Edit”，即可完成串口事件消息处理函数的添加。这个函数是用来处理串口消息事件的，例如，每当串口接收到数据，就会产生一个串口接收数据缓冲中有字符的消息事件，这个函数就会执行。
- **初始化串口**。假设串口控件的关联变量名为m_sComm，可通过以下代码进行初始化：
// Select com2
m_sComm.put_CommPort(2);
// Set size of input buffer
m_sComm.put_InBufferCount(1024);
// Set size of output buffer
m_sComm.put_OutBufferCount(1024);
if (! m_sComm.get_PortOpen())
{
        m_sComm.put_PortOpen(true);
}
else
{
        AfxMessageBox("Com2 can't be opened!");
}
// Set input mode as binary
m_sComm.put_InputMode(1);
// Set baud rate and other parameters
m_sComm.put_Settings("9600,n,8,1");
// Set threshold as 1, generate event after receiving 1 byte
m_sComm.put_RThreshold(1);
//===========================
备注::
1>上面的代码有点问题----应该把put_InBufferCount/put_OutBufferCount修改为put_InBufferSize/put_OutBufferSize
