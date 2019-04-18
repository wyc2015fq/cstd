# VC中Tooltip的实现 - ljx0305的专栏 - CSDN博客
2010年08月12日 16:27:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：3541
**正文：**
**我们经常在一些软件中看到类似的功能：鼠标移动到一个按钮（或其他控件）上的时候，自动弹出一些提示（不是弹出对话框），以提醒用户该控件的作用或注意事项。这种提示就是所说的****Tooltip****（如****图１****所示）。****关于****Tooltip****，网上已经有很多资料了，我在这里只是给大家提供一些链接，让没有做过的朋友能在最短的时间内实现这个功能。**
![](http://lingfengme.bokee.com/inc/ShowTooltip.jpg)
**图１**
**Codeproject****已经有许多现成的****Tooltip****类，并且有的功能非常之强，可以直接拿来用，这会为你节省许多时间！具体链接及说明如下：**
**（****1****）****XSuperTooltip - Office 2007 Super Tooltip class****地址****：**[http://www.codeproject.com/useritems/XSuperTooltip.asp](http://www.codeproject.com/useritems/XSuperTooltip.asp)
**说明：这是一个功能相当强大的****Tooltip****类，它实现了****Office 2007****风格的****Tooltip****，当然，初次接触它的话，你肯定要花更多的时间去学习怎么用，我没有用过这个类，因为它所实现的效果远远超过了我软件中的需求，所以，这个类适合那些想让自己的软件****Tooltip****非常炫的朋友。**
**　（２）****CPPToolTip****地址****：**[http://www.codeproject.com/miscctrl/pptooltip.asp](http://www.codeproject.com/miscctrl/pptooltip.asp)
**说明：这个类同样强大，你可以看一下上面链接中它的图例，我也没有去仔细看，因为它花哨的功能我也用不了这么多。**
**　（３）****CXInfoTip - Information Tooltip****地址****：**[http://www.codeproject.com/miscctrl/xinfotip.asp](http://www.codeproject.com/miscctrl/xinfotip.asp)
**说明：这个类是本文介绍的重点，它实现了我的要求，上面的****图１****就是我用它做出来的效果。**
**　说一下这个类的使用（非常简单）：**
**以****VC.NET 2003****为例，新建一个基于对话框的****MFC****应用程序，在该工程中添加****XInfoTip.cpp****及****XInfoTip.h****文件（这两个文件在上面的****Codeproject****链接中你可以找到）。然后在你需要实现****Tooltip****的对话框类中添加****CXInfoTip****成员变量（如果有多个****Tooltip****，请依次添加）****m_tip1****，然后在**OnInitDialog()**函数中（如果是子窗口，则需重载子对话框类的该函数，否则看不到这个函数****——****这是基础知识）添加以下两句代码：**
**     m_tip1.Create(this);**
**     m_tip1.AddTool(GetDlgItem(IDC_COMBO_COMPORT),******                 _T("******串口未正确连接的话****/n/n****将导致系统不能运作！****"),   ****//Tooltip****的内容******                ::LoadIcon(NULL, IDI_WARNING));****** //****加载一个警告图标**
**AddTool()****的第一个参数意思是：当鼠标移动到哪一个控件上的时候弹出****Tooltip****，这里用****GetDlgItem****来获取句柄就可以了，上面的代码写得非常明白，一看就懂吧？第二个参数是设置弹出的****Tooltip****中的内容，其中****/n****是换行符（地球人都知道）。关于第三个参数，可用****LoadIcon()****函数来加载一个图标显示在****Tooltip****中（如图****1****所示），可查****MSDN****，用法非常简单。**
**这样做之后，你就差最后一步就可以完成了，即：重载你用到****Tooltip****的对话框类的****PreTranslateMessage()****函数（别告诉我你不知道怎么重载哟～），然后将里面的代码改成如下所示：**
**{**
**      // TODO: ****在此添加专用代码和****/****或调用基类**
**m_tip1.RelayEvent(pMsg);   **** //****添加这一句**
**      return CDialog::PreTranslateMessage(pMsg);**
**}**
**好了，大功告成！编译一下试试看，再把鼠标移动到控件上，是不是弹出了很酷的****Tooltip****？**
**本文只涉及到了该类的很简单的功能使用，关于改变****Tooltip****中文字的字体、控制****Tooltip****显示时间等操作，请参阅****Codeproject****链接中作者的文章以及****Demo****。**
**还有几个类似的，在此一并给出链接：**
**　（****4****）****Message Balloons****地址****：**[http://www.codeproject.com/dialog/message_balloons.asp](http://www.codeproject.com/dialog/message_balloons.asp)
**　（****5****）****A multiline extended tooltip control****地址****：**[http://www.codeproject.com/miscctrl/tooltipex.asp](http://www.codeproject.com/miscctrl/tooltipex.asp)
**　（****6****）****Display ToolTip in any place on the screen****地址****：**[http://www.codeproject.com/miscctrl/tooltipzen.asp](http://www.codeproject.com/miscctrl/tooltipzen.asp)
引用：[http://lingfengme.bokee.com/5716026.html](http://lingfengme.bokee.com/5716026.html)
