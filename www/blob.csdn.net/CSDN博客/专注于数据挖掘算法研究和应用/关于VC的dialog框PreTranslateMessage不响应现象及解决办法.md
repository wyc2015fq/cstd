# 关于VC的dialog框PreTranslateMessage不响应现象及解决办法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年07月01日 15:57:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：9156








1.案例情况：

1)dialog动态生成按钮

 CButton* pBtn = new CButton();// 定义按钮
   CRect rcBtn(10,10,40, 30); //按钮显示位置长30，高20
   pBtn->Create("动态按钮", BS_FLAT, rcBtn, this, nBtnID);//nBtnID自定义，如10000
   pBtn->ShowWindow(SW_SHOW);

2)重载DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)响应动态生成按钮

  LRESULT C**Dlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
 // TODO: Add your specialized code here and/or call the base class
 if(WM_COMMAND==message)
 {
  this->SetFocus();//设置父窗体焦点，响应按钮
  WORD wID = LOWORD(wParam);
  if ( nBtnID==wID)
  {
   //按键处理代码

   GetDlgItem(wID)->EnableWindow(FALSE);//使动态按钮灰色

  }
 }
 return CDialog::DefWindowProc(message, wParam, lParam);
}

3)重载PreTranslateMessage(MSG* pMsg) ，实现对数字按键的反应：

BOOL C**Dlg::PreTranslateMessage(MSG* pMsg) 
{
 // TODO: Add your specialized code here and/or call the base class
 if(pMsg-> message   ==   WM_KEYDOWN) 
    {
  switch(pMsg-> wParam) 
  { 
  case   VK_RETURN://屏蔽回车
   {
    OnOK();
    return   TRUE; 
   }
  case   VK_ESCAPE://屏蔽Esc 
   {
    OnCancel();
    return   TRUE; 
   }
  case VK_NUMPAD1://小键盘上数字
   {
    AfxMessageBox("1");

    return   TRUE; 
   }
   } 

} 
    return CDialog::PreTranslateMessage(pMsg);
}

2.现象：在点击动态生成按钮前，按数字1可以响应，但点击动态生成的按钮后就无法响应，按"esc"也不响应退出；

3.分析：可能是由于dialog窗体内控件获得焦点，而使dialog父窗口失去焦点，导致无法截取到按键信息；

4.处理：在点击动态生成的按钮后，通过this->SetFocus();使dialog父窗口获取焦点，这样就可以响应键盘按键，本案例在DefWindowProc中使dialog父窗口重新获取由于点击内部控件而失去的焦点。





