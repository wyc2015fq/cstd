# vc6.0制作窗体可嵌入IE内的OCX - 专注于数据挖掘算法研究和应用 - CSDN博客





2012年03月24日 16:09:29[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3997








1.vc6.0环境，新建：MFC ActiveX ControlWizard工程；



2.在建立的ActiveX工程中，插入dialog，在**ctrl类内增加关联该dialog的变量；



3.处理**ctrl类的wm_create函数，如：

=========================================================================

int C**Ctrl::OnCreate(LPCREATESTRUCT lpCreateStruct)

{

 if (COleControl::OnCreate(lpCreateStruct) == -1)

  return -1;

 // TODO:  Add your specialized creation code here

 m_MainDialog.Create(IDD_MAINDIALOG, this);

 return 0;

}

=============================================================================

其中IDD_MAINDIALOG为插入dialog的ID标识，m_MainDialog为关联dialog类的**Ctrl类成员函数；该函数主要完成创建dialog；

4.处理**ctrl类的OnDraw函数，如：

  ===================================================================

void CMyActiveXCtrl::OnDraw(

   CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)

{

 if (!pdc)

  return;

 DoSuperclassPaint(pdc, rcBounds);

 m_MainDialog.MoveWindow(rcBounds, TRUE);

 CBrush brBackGnd(TranslateColor(AmbientBackColor()));

 pdc->FillRect(rcBounds, &brBackGnd);

}

============================================================================

这里完成将dialog嵌入到空间本身绘制区域内，ocx通过web调用，即嵌入到IE内；

参考地址：[http://www.codeproject.com/Articles/14533/A-Complete-ActiveX-Web-Control-Tutorial/](http://www.codeproject.com/Articles/14533/A-Complete-ActiveX-Web-Control-Tutorial/)



5.在ActiveX控件中引入窗体技术，可参考：[http://blog.sina.com.cn/s/blog_620782850100ikkh.html](http://blog.sina.com.cn/s/blog_620782850100ikkh.html)

  可实现在IE内嵌入OCX完成的窗体，包括console窗口。



